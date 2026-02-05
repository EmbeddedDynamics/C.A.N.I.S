//`#start header` -- edit after this line, do not edit this line
/*
 * ================================================================================================
 * CORDIC ACCELERATOR - FIXED POINT FORMAT SPECIFICATION
 * ================================================================================================
 * 
 * 1. COORDINATE SYSTEM (X/Y Registers)
 *    Format: Q12.4 Signed Fixed-Point
 *    ------------------------------------------------------------
 *    - Bit Allocation: [15] Sign | [14:4] Integer (mm) | [3:0] Fraction
 *    - Resolution:     2^-4 = 0.0625 mm (62.5 micrometers)
 *    - Range:          +/- 2047.9375 mm (~2 meters)
 *    
 *    Why this choice?
 *    - 5-Bar Linkage Scale: Supports typical arm lengths (e.g. 300-500mm) without overflow.
 *    - Precision Match: 0.06mm resolution exceeds typical mechanical rigidity limits of printed/hobby arms.
 *    - Implementation: Allows simple bit-shifts for conversion. (float_mm * 16 = int_input).
 *
 * 2. ANGLE SYSTEM (Z Register)
 *    Format: 16-bit Binary Angle Measurement System (BAMS)
 *    ------------------------------------------------------------
 *    - Mapping:        -180.0 deg = 0x8000 (-32768)
 *                      +179.9 deg = 0x7FFF (+32767)
 *    - Resolution:     360 deg / 65536 = 0.00549 degrees
 *    - Arc Error:      ~0.048 mm error at 500mm radius.
 
 *
 *    Why this choice?
 *    - Full Circle:    Inherently handles 4-quadrant wrapping (179 deg + 2 deg = -179 deg) automatically via integer overflow.
 *    - Efficiency:     Multiplication/Division by 180 is replaced by bitwise scaling.
 *    - LUT Generation: ATAN table values are derived using this 32768-base scale.
 *
 * ================================================================================================
 */


`include "cypress.v"
`ifdef  bCORDIC_V_ALREADY_INCLUDED
`else
`define bCORDIC_V_ALREADY_INCLUDED


//`#end` -- edit above this line, do not edit this line
module bCORDIC (
	input   clock,
	input   en,
	input   rst,
   
    output  isr
);
	parameter CY_CONFIG_TITLE = "CORDIC_N";
    
    localparam SYSTEM_LINEAR = 1;
	localparam SYSTEM_CIRCULAR = 2;
    localparam SYSTEM_HYPERBOLIC = 3;
    
    localparam OPERATION_ROTATING = 0;
	localparam OPERATION_VECTORING = 1;
    
    parameter CORDICSystem = SYSTEM_CIRCULAR;
    parameter CORDICOperation = OPERATION_VECTORING;
    
//`#start body` -- edit after this line, do not edit this line
    
    /* CORDIC states */
    reg  cordic_enable_buf;
    
    wire cordic_enable;
    wire cordic_reset;
    wire cordic_operation;
    
    wire core_init;
    wire hold_reset = 0;
    wire core_reset = 0;

    //======================================================================================================
    //      REGISTERS & WIRES
    //======================================================================================================

    /* FIFO status wires */
    wire F0_x_lsb_empty, F0_x_msb_empty;
    wire F0_y_lsb_empty, F0_y_msb_empty;

    wire fifo_empty = |{F0_x_lsb_empty, F0_x_msb_empty, F0_y_lsb_empty, F0_y_msb_empty};
    
    wire F1_x_lsb_filled, F1_x_msb_filled;
    wire F1_z_lsb_filled, F1_z_msb_filled;
    
    wire F1_x_lsb_full, F1_x_msb_full;
    wire F1_z_lsb_full, F1_z_msb_full;
    
    wire out_fifo_full = &{F1_x_lsb_full, F1_x_msb_full, F1_z_lsb_full, F1_z_msb_full};

    /* Datapath PI/PO */
    wire [7:0] x_pi_lsb, x_pi_msb;
    wire [7:0] x_po_lsb, x_po_msb;

    wire [7:0] y_pi_lsb, y_pi_msb;
    wire [7:0] y_po_lsb, y_po_msb;

    wire [7:0] z_pi_lsb, z_pi_msb;
    wire [7:0] z_po;

    /* Chaining */
    wire [14:0] chain_x, chain_y, chain_z;

    /* CORDIC control/state */
    reg  [3:0] state;
    reg  [2:0] dp_op;
    reg  [3:0] iter;
    reg  [3:0] shiftn;

    reg  cordic_done;
    reg  load_out;

    wire load_out_fifo = load_out;

    /* PI registers */
    reg  [15:0] x_pi;
    reg  [15:0] y_pi;
    reg  [15:0] z_pi;
    reg         sign;

    assign {x_pi_msb, x_pi_lsb} = x_pi;
    assign {y_pi_msb, y_pi_lsb} = y_pi;
    assign {z_pi_msb, z_pi_lsb} = z_pi;
    
    //======================================================================================================
    //      STATE / OPCODE ENUMS
    //======================================================================================================

    localparam [3:0] S_IDLE      = 4'd0;
    localparam [3:0] S_LOAD_A0   = 4'd1;
    localparam [3:0] S_LOAD_PO   = 4'd2;
    localparam [3:0] S_SAMPLE_PO = 4'd3;
    localparam [3:0] S_LOAD_A1   = 4'd4;
    localparam [3:0] S_SHIFT     = 4'd5;
    localparam [3:0] S_CALC      = 4'd6;
    localparam [3:0] S_DONE      = 4'd8;

    localparam [2:0] OP_IDLE     = 3'd0;
    localparam [2:0] OP_LOAD_A0  = 3'd1;
    localparam [2:0] OP_LOAD_PO  = 3'd2;
    localparam [2:0] OP_LOAD_A1  = 3'd3;
    localparam [2:0] OP_SHIFT    = 3'd4;
    localparam [2:0] OP_ADD      = 3'd5;
    localparam [2:0] OP_SUB      = 3'd6;

    localparam [3:0] ITER_MAX    = 4'd15;
    
    //======================================================================================================
    //      CLOCK SETUP
    //======================================================================================================
    
    wire op_clock;

    cy_psoc3_udb_clock_enable_v1_0 #(.sync_mode(`TRUE)) ClkSync
    (
        .clock_in(clock),
        .enable(1'b1),
        .clock_out(op_clock)
    );

    //======================================================================================================
    //      ATAN LOOKUP TABLE (16-bit BAMS format)
    //======================================================================================================
    
    reg [15:0] current_atan;
    
    always @(posedge op_clock)
    begin
        case(iter)
            4'd0:  current_atan <= 16'h2000; // 45.000 deg
            4'd1:  current_atan <= 16'h12E4; // 26.565 deg
            4'd2:  current_atan <= 16'h09FB; // 14.036 deg
            4'd3:  current_atan <= 16'h0511; // 7.125 deg
            4'd4:  current_atan <= 16'h028B; // 3.576 deg
            4'd5:  current_atan <= 16'h0146; // 1.790 deg
            4'd6:  current_atan <= 16'h00A3; // 0.895 deg
            4'd7:  current_atan <= 16'h0051; // 0.448 deg
            4'd8:  current_atan <= 16'h0029; // 0.224 deg
            4'd9:  current_atan <= 16'h0014; // 0.112 deg
            4'd10: current_atan <= 16'h000A; // 0.056 deg
            4'd11: current_atan <= 16'h0005; // 0.028 deg
            4'd12: current_atan <= 16'h0003; // 0.014 deg
            4'd13: current_atan <= 16'h0001; // 0.007 deg
            4'd14: current_atan <= 16'h0001; // 0.003 deg
            4'd15: current_atan <= 16'h0000; // < 0.002 deg
        endcase
    end
    
    always @(posedge op_clock) begin
        z_pi <= current_atan; 
    end
    
    //======================================================================================================
    //      CONTROL REGISTER
    //======================================================================================================
    
    wire [7:0] ctrl;
    localparam  MDIO_CTRL_ENABLE = 3'd0;
    localparam  MDIO_CTRL_RESET  = 3'd1;
    localparam  MDIO_CTRL_OPERATION  = 3'd2;
    
    cy_psoc3_control #(.cy_force_order(`TRUE)) CtlReg
    (
        .control(ctrl),
        .clock(op_clock)
    );        
    
    assign cordic_enable = ctrl[MDIO_CTRL_ENABLE] & en;
    assign cordic_reset = ctrl[MDIO_CTRL_RESET] | rst;
    assign cordic_operation = ctrl[MDIO_CTRL_OPERATION];
    
    //======================================================================================================
    //      STATUS REGISTER
    //======================================================================================================
    
    wire [7:0] cordic_status;

    wire x_ready = ~(F0_x_lsb_empty | F0_x_msb_empty);
    wire y_ready = ~(F0_y_lsb_empty | F0_y_msb_empty);
    
    wire x_filled = (F1_x_lsb_filled | F1_x_msb_filled);
    wire z_filled = (F1_z_lsb_filled | F1_z_msb_filled);

    // If these are always 0, just hardwire status[0] to 0.
    // If you later add a real core reset/hold, replace this.
    wire core_fault = 1'b0;

    cy_psoc3_status #(
        .cy_force_order(`TRUE),
        .cy_md_select(8'b01000000)
    ) StsReg (
        .status(cordic_status),   // <-- drive the bus you assign below
        .reset(cordic_reset),
        .clock(op_clock)
    );

    assign cordic_status[7]     = out_fifo_full;
    assign cordic_status[6]     = cordic_done;
    assign cordic_status[5]     = x_filled;
    assign cordic_status[4]     = z_filled;
    assign cordic_status[3]     = x_ready;          // X FIFO has data (both bytes)
    assign cordic_status[2]     = y_ready;          // Y FIFO has data (both bytes)
    assign cordic_status[1]     = (state == S_IDLE);
    assign cordic_status[0]     = core_fault;
    
    //======================================================================================================
    //      Interrupt status register
    //======================================================================================================
        
    //wire [6:0] int_sts;
    
    cy_psoc3_statusi #(.cy_force_order(`TRUE), .cy_md_select(7'b1000000),
    .cy_int_mask(7'b1000000)) InterruptRegister (
        .status(cordic_status[6:0]),
        .reset(cordic_reset),
        .clock(op_clock),
        .interrupt(isr)
   );

    //assign int_sts[6:1] = 6'b0;
    //assign int_sts[0] = cordic_done;

    always @(posedge op_clock) begin
      if (cordic_reset) cordic_done <= 1'b0;
      else if (state == S_DONE) cordic_done <= 1'b1;
      else if (state == S_IDLE) cordic_done <= 1'b0; // auto-clear for now
    end
    
    //======================================================================================================
    //      STATE MACHINE
    //======================================================================================================

    always @(posedge op_clock) begin
        if (cordic_reset) begin
            state    <= S_IDLE;
            dp_op    <= OP_IDLE;

            iter     <= 4'd0;
            shiftn   <= 4'd0;

            load_out <= 1'b0;
        end
        else if (cordic_enable) begin
            case (state)

                S_IDLE: begin
                    dp_op    <= OP_IDLE;
                    load_out <= 1'b0;

                    if (!fifo_empty & !out_fifo_full) begin
                        iter   <= 4'd0;
                        state  <= S_LOAD_A0;
                    end
                end

                S_LOAD_A0: begin
                    dp_op <= OP_LOAD_A0;
                    state <= S_LOAD_PO;
                end

                S_LOAD_PO: begin
                    dp_op <= OP_LOAD_PO;
                    state <= S_SAMPLE_PO;
                end

                // Capture current x/y from PO, decide direction from sign(y)
                // Also prepare cross-fed PI values for the upcoming A1 load:
                //   X-engine A1 <= Y,  Y-engine A1 <= X
                S_SAMPLE_PO: begin
                    dp_op <= OP_IDLE;

                    x_pi <= {y_po_msb, y_po_lsb};
                    y_pi <= {x_po_msb, x_po_lsb};
                    
                    sign <= (cordic_operation==OPERATION_VECTORING) ? y_po_msb[7] : ~z_po[7];

                    state <= S_LOAD_A1;
                end

                S_LOAD_A1: begin
                    dp_op  <= OP_LOAD_A1;
                    shiftn <= 4'd0;      // always reset here
                    state  <= S_SHIFT;
                end

                // Multi-cycle shift: perform exactly "iter" shifts of A1
                S_SHIFT: begin
                    if (shiftn < iter) begin
                        dp_op  <= OP_SHIFT;
                        shiftn <= shiftn + 1'b1;
                        state  <= S_SHIFT;
                    end else begin
                        dp_op  <= OP_IDLE;
                        state  <= S_CALC;
                    end
                end

                // Perform add/sub update in datapaths
                S_CALC: begin
                    dp_op <= sign ? OP_SUB : OP_ADD;

                    if (iter == ITER_MAX) begin
                        state <= S_DONE;
                    end else begin
                        iter  <= iter + 1'b1;
                        state <= S_LOAD_PO;   // resample updated A0 next iter
                    end
                end

                S_DONE: begin
                    dp_op    <= OP_IDLE;
                    load_out <= 1'b1;     // pulse output FIFO load
                    state    <= S_IDLE;
                end

                default: begin
                    state <= S_IDLE;
                    dp_op <= OP_IDLE;
                end
            endcase
        end
    end

//======================================================================================================
//      X-ENGINE (16-bit: 2x 8-bit datapaths)
//======================================================================================================

cy_psoc3_dp #(.d0_init(8'b00000000), 
.cy_dpconfig(
{
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM0:         Idle*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC___F0, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM1:                    Load FIFO F0 -> A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM2:         Load PO (PO = SRCA = A0)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM3:         Load A1 (A1 = PI = dY)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A1, `CS_SRCB_D0,
    `CS_SHFT_OP___SR, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM4:         Shift A1 (A1 = A1 >> 1)*/
    `CS_ALU_OP__ADD, `CS_SRCA_A0, `CS_SRCB_A1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM5:         Add A0 (A0 = A0 + A1)*/
    `CS_ALU_OP__SUB, `CS_SRCA_A0, `CS_SRCB_A1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM6:         Sub A0 (A0 = A0 - A1)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM7:         Idle*/
    8'hFF, 8'h00,  /*CFG9:                      ALU Mask unused*/
    8'hFF, 8'hFF,  /*CFG11-10:                      CMP Mask unused*/
    `SC_CMPB_A0_D1, `SC_CMPA_A0_D1, `SC_CI_B_ARITH,
    `SC_CI_A_ARITH, `SC_C1_MASK_DSBL, `SC_C0_MASK_DSBL,
    `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_CHAIN,
    `SC_SI_A_CHAIN, /*CFG13-12:                            */
    `SC_A0_SRC_ACC, `SC_SHIFT_SR, `SC_PI_DYN_EN,
    1'h0, `SC_FIFO1__A0, `SC_FIFO0_BUS,
    `SC_MSB_DSBL, `SC_MSB_BIT7, `SC_MSB_NOCHN,
    `SC_FB_NOCHN, `SC_CMP1_NOCHN,
    `SC_CMP0_NOCHN, /*CFG15-14:                      Enable DYN PI*/
    10'h00, `SC_FIFO_CLK__DP,`SC_FIFO_CAP_AX,
    `SC_FIFO__EDGE,`SC_FIFO__SYNC,`SC_EXTCRC_DSBL,
    `SC_WRK16CAT_DSBL /*CFG17-16:                            */
}),
.d1_init(8'b11111111)
) dp_x_engine_LSB(
        /*  input                   */  .reset(cordic_reset),
        /*  input                   */  .clk(op_clock),
        /*  input   [02:00]         */  .cs_addr(dp_op),
        /*  input                   */  .route_si(1'b0),
        /*  input                   */  .route_ci(1'b0),
        /*  input                   */  .f0_load(1'b0),
        /*  input                   */  .f1_load(load_out_fifo),
        /*  input                   */  .d0_load(1'b0),
        /*  input                   */  .d1_load(1'b0),
        /*  output                  */  .ce0(),
        /*  output                  */  .cl0(),
        /*  output                  */  .z0(),
        /*  output                  */  .ff0(),
        /*  output                  */  .ce1(),
        /*  output                  */  .cl1(),
        /*  output                  */  .z1(),
        /*  output                  */  .ff1(),
        /*  output                  */  .ov_msb(),
        /*  output                  */  .co_msb(),
        /*  output                  */  .cmsb(),
        /*  output                  */  .so(),
        /*  output                  */  .f0_bus_stat(F0_x_lsb_full),
        /*  output                  */  .f0_blk_stat(F0_x_lsb_empty),
        /*  output                  */  .f1_bus_stat(F1_x_lsb_filled),
        /*  output                  */  .f1_blk_stat(F1_x_lsb_full),
        
        /* input                    */  .ci(1'b0),          // Carry in from previous stage
        /* output                   */  .co(chain_x[12]),   // Carry out to engine_x_MSB
        /* input                    */  .sir(1'b0),         // Shift in from right side
        /* output                   */  .sor(),             // Shift out to right side
        /* input                    */  .sil(chain_x[10]),         // Shift in from left side
        /* output                   */  .sol(chain_x[11]),             // Shift out to left side
        /* input                    */  .msbi(chain_x[9]),        // MSB chain in
        /* output                   */  .msbo(),            // MSB chain out
        /* input [01:00]            */  .cei(2'b0),         // Compare equal in from prev stage
        /* output [01:00]           */  .ceo(chain_x[1:0]),             // Compare equal out to next stage
        /* input [01:00]            */  .cli(2'b0),         // Compare less than in from prv stage
        /* output [01:00]           */  .clo(chain_x[3:2]),             // Compare less than out to next stage
        /* input [01:00]            */  .zi(2'b0),          // Zero detect in from previous stage
        /* output [01:00]           */  .zo(chain_x[5:4]),              // Zero detect out to next stage
        /* input [01:00]            */  .fi(2'b0),          // 0xFF detect in from previous stage
        /* output [01:00]           */  .fo(chain_x[7:6]),              // 0xFF detect out to next stage
        /* input [01:00]            */  .capi(2'b0),        // Software capture from previous stage
        /* output [01:00]           */  .capo(chain_x[14:13]),            // Software capture to next stage
        /* input                    */  .cfbi(1'b0),        // CRC Feedback in from previous stage
        /* output                   */  .cfbo(chain_x[8]),            // CRC Feedback out to next stage
        /* input [07:00]            */  .pi(x_pi_lsb),  // Parallel data port
        /* output [07:00]           */  .po(x_po_lsb)       // Parallel data port
);
cy_psoc3_dp #(.d0_init(8'b00000000), 
.cy_dpconfig(
{
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM0:         Idle*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC___F0, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM1:                    Load FIFO F0 -> A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM2:         Load PO (PO = SRCA = A0)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM3:         Load A1 (A1 = PI = dY)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A1, `CS_SRCB_D0,
    `CS_SHFT_OP___SR, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM4:         Shift A1 (A1 = A1 >> 1)*/
    `CS_ALU_OP__ADD, `CS_SRCA_A0, `CS_SRCB_A1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM5:         Add A0 (A0 = A0 + A1)*/
    `CS_ALU_OP__SUB, `CS_SRCA_A0, `CS_SRCB_A1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM6:         Sub A0 (A0 = A0 - A1)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM7:         Idle*/
    8'hFF, 8'h00,  /*CFG9:                      ALU Mask unused*/
    8'hFF, 8'hFF,  /*CFG11-10:                      CMP Mask unused*/
    `SC_CMPB_A0_D1, `SC_CMPA_A0_D1, `SC_CI_B_CHAIN,
    `SC_CI_A_CHAIN, `SC_C1_MASK_DSBL, `SC_C0_MASK_DSBL,
    `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_DEFSI,
    `SC_SI_A_DEFSI, /*CFG13-12:                            */
    `SC_A0_SRC_ACC, `SC_SHIFT_SR, `SC_PI_DYN_EN,
    `SC_SR_SRC_MSB, `SC_FIFO1__A0, `SC_FIFO0_BUS,
    `SC_MSB_DSBL, `SC_MSB_BIT7, `SC_MSB_NOCHN,
    `SC_FB_NOCHN, `SC_CMP1_NOCHN,
    `SC_CMP0_NOCHN, /*CFG15-14:                      Enable DYN PI*/
    10'h00, `SC_FIFO_CLK__DP,`SC_FIFO_CAP_AX,
    `SC_FIFO__EDGE,`SC_FIFO__SYNC,`SC_EXTCRC_DSBL,
    `SC_WRK16CAT_DSBL /*CFG17-16:                            */
}),
.d1_init(8'b11111111)
) dp_x_engine_MSB(
        /*  input                   */  .reset(cordic_reset),
        /*  input                   */  .clk(op_clock),
        /*  input   [02:00]         */  .cs_addr(dp_op),
        /*  input                   */  .route_si(1'b0),
        /*  input                   */  .route_ci(1'b0),
        /*  input                   */  .f0_load(1'b0),
        /*  input                   */  .f1_load(load_out_fifo),
        /*  input                   */  .d0_load(1'b0),
        /*  input                   */  .d1_load(1'b0),
        /*  output                  */  .ce0(),
        /*  output                  */  .cl0(),
        /*  output                  */  .z0(),
        /*  output                  */  .ff0(),
        /*  output                  */  .ce1(),
        /*  output                  */  .cl1(),
        /*  output                  */  .z1(),
        /*  output                  */  .ff1(),
        /*  output                  */  .ov_msb(),
        /*  output                  */  .co_msb(),
        /*  output                  */  .cmsb(),
        /*  output                  */  .so(),
        /*  output                  */  .f0_bus_stat(F0_x_msb_full),
        /*  output                  */  .f0_blk_stat(F0_x_msb_empty),
        /*  output                  */  .f1_bus_stat(F1_x_msb_filled),
        /*  output                  */  .f1_blk_stat(F1_x_msb_full),
        
        /* input                    */  .ci(chain_x[12]),          // Carry in from previous stage
        /* output                   */  .co(),              // Carry out to next stage
        /* input                    */  .sir(chain_x[11]),         // Shift in from right side
        /* output                   */  .sor(chain_x[10]),             // Shift out to right side
        /* input                    */  .sil(),         // Shift in from left side
        /* output                   */  .sol(),             // Shift out to left side
        /* input                    */  .msbi(),        // MSB chain in
        /* output                   */  .msbo(chain_x[9]),            // MSB chain out
        /* input [01:00]            */  .cei(chain_x[1:0]),         // Compare equal in from prev stage
        /* output [01:00]           */  .ceo(),             // Compare equal out to next stage
        /* input [01:00]            */  .cli(chain_x[3:2]),         // Compare less than in from prv stage
        /* output [01:00]           */  .clo(),             // Compare less than out to next stage
        /* input [01:00]            */  .zi(chain_x[5:4]),          // Zero detect in from previous stage
        /* output [01:00]           */  .zo(),              // Zero detect out to next stage
        /* input [01:00]            */  .fi(chain_x[7:6]),          // 0xFF detect in from previous stage
        /* output [01:00]           */  .fo(),              // 0xFF detect out to next stage
        /* input [01:00]            */  .capi(chain_x[14:13]),        // Software capture from previous stage
        /* output [01:00]           */  .capo(),            // Software capture to next stage
        /* input                    */  .cfbi(chain_x[8]),        // CRC Feedback in from previous stage
        /* output                   */  .cfbo(),            // CRC Feedback out to next stage
        /* input [07:00]            */  .pi(x_pi_msb), // Parallel data port
        /* output [07:00]           */  .po(x_po_msb)       // Parallel data port
);

//======================================================================================================
//      Y-ENGINE (16-bit: 2x 8-bit datapaths)
//======================================================================================================

cy_psoc3_dp #(.d0_init(8'b00000000), 
.cy_dpconfig(
{
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM0:         Idle*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC___F0, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM1:                    Load FIFO F0 -> A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM2:         Load PO (PO = SRCA = A0)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM3:         Load A1 (A1 = PI = dY)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A1, `CS_SRCB_D0,
    `CS_SHFT_OP___SR, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM4:         Shift A1 (A1 = A1 >> 1)*/
    `CS_ALU_OP__SUB, `CS_SRCA_A0, `CS_SRCB_A1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM5:         Sub A0 (A0 = A0 - A1)*/
    `CS_ALU_OP__ADD, `CS_SRCA_A0, `CS_SRCB_A1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM6:         Add A0 (A0 = A0 + A1)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM7:         Idle*/
    8'hFF, 8'h00,  /*CFG9:                      ALU Mask unused*/
    8'hFF, 8'hFF,  /*CFG11-10:                      CMP Mask unused*/
    `SC_CMPB_A0_D1, `SC_CMPA_A0_D1, `SC_CI_B_ARITH,
    `SC_CI_A_ARITH, `SC_C1_MASK_DSBL, `SC_C0_MASK_DSBL,
    `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_CHAIN,
    `SC_SI_A_CHAIN, /*CFG13-12:                            */
    `SC_A0_SRC_ACC, `SC_SHIFT_SR, `SC_PI_DYN_EN,
    1'h0, `SC_FIFO1__A0, `SC_FIFO0_BUS,
    `SC_MSB_DSBL, `SC_MSB_BIT7, `SC_MSB_NOCHN,
    `SC_FB_NOCHN, `SC_CMP1_NOCHN,
    `SC_CMP0_NOCHN, /*CFG15-14:                      Enable DYN PI*/
    10'h00, `SC_FIFO_CLK__DP,`SC_FIFO_CAP_AX,
    `SC_FIFO__EDGE,`SC_FIFO__SYNC,`SC_EXTCRC_DSBL,
    `SC_WRK16CAT_DSBL /*CFG17-16:                            */
}),
.d1_init(8'b11111111)
) dp_y_engine_LSB(
        /*  input                   */  .reset(cordic_reset),
        /*  input                   */  .clk(op_clock),
        /*  input   [02:00]         */  .cs_addr(dp_op),
        /*  input                   */  .route_si(1'b0),
        /*  input                   */  .route_ci(1'b0),
        /*  input                   */  .f0_load(1'b0),
        /*  input                   */  .f1_load(load_out_fifo),
        /*  input                   */  .d0_load(1'b0),
        /*  input                   */  .d1_load(1'b0),
        /*  output                  */  .ce0(),
        /*  output                  */  .cl0(),
        /*  output                  */  .z0(),
        /*  output                  */  .ff0(),
        /*  output                  */  .ce1(),
        /*  output                  */  .cl1(),
        /*  output                  */  .z1(),
        /*  output                  */  .ff1(),
        /*  output                  */  .ov_msb(),
        /*  output                  */  .co_msb(),
        /*  output                  */  .cmsb(),
        /*  output                  */  .so(),
        /*  output                  */  .f0_bus_stat(F0_y_lsb_full),
        /*  output                  */  .f0_blk_stat(F0_y_lsb_empty),
        /*  output                  */  .f1_bus_stat(),
        /*  output                  */  .f1_blk_stat(),
        
        /* input                    */  .ci(1'b0),          // Carry in from previous stage
        /* output                   */  .co(chain_y[12]),   // Carry out to engine_x_MSB
        /* input                    */  .sir(1'b0),         // Shift in from right side
        /* output                   */  .sor(),             // Shift out to right side
        /* input                    */  .sil(chain_y[10]),         // Shift in from left side
        /* output                   */  .sol(chain_y[11]),             // Shift out to left side
        /* input                    */  .msbi(chain_y[9]),        // MSB chain in
        /* output                   */  .msbo(),            // MSB chain out
        /* input [01:00]            */  .cei(2'b0),         // Compare equal in from prev stage
        /* output [01:00]           */  .ceo(chain_y[1:0]),             // Compare equal out to next stage
        /* input [01:00]            */  .cli(2'b0),         // Compare less than in from prv stage
        /* output [01:00]           */  .clo(chain_y[3:2]),             // Compare less than out to next stage
        /* input [01:00]            */  .zi(2'b0),          // Zero detect in from previous stage
        /* output [01:00]           */  .zo(chain_y[5:4]),              // Zero detect out to next stage
        /* input [01:00]            */  .fi(2'b0),          // 0xFF detect in from previous stage
        /* output [01:00]           */  .fo(chain_y[7:6]),              // 0xFF detect out to next stage
        /* input [01:00]            */  .capi(2'b0),        // Software capture from previous stage
        /* output [01:00]           */  .capo(chain_y[14:13]),            // Software capture to next stage
        /* input                    */  .cfbi(1'b0),        // CRC Feedback in from previous stage
        /* output                   */  .cfbo(chain_y[8]),            // CRC Feedback out to next stage
        /* input [07:00]            */  .pi(y_pi_lsb),  // Parallel data port
        /* output [07:00]           */  .po(y_po_lsb)       // Parallel data port
);
cy_psoc3_dp #(.d0_init(8'b00000000), 
.cy_dpconfig(
{
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM0:         Idle*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC___F0, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM1:                    Load FIFO F0 -> A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM2:         Load PO (PO = SRCA = A0)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM3:         Load A1 (A1 = PI = dY)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A1, `CS_SRCB_D0,
    `CS_SHFT_OP___SR, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM4:         Shift A1 (A1 = A1 >> 1)*/
    `CS_ALU_OP__SUB, `CS_SRCA_A0, `CS_SRCB_A1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM5:         Sub A0 (A0 = A0 - A1)*/
    `CS_ALU_OP__ADD, `CS_SRCA_A0, `CS_SRCB_A1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM6:         Add A0 (A0 = A0 + A1)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM7:         Idle*/
    8'hFF, 8'h00,  /*CFG9:                      ALU Mask unused*/
    8'hFF, 8'hFF,  /*CFG11-10:                      CMP Mask unused*/
    `SC_CMPB_A0_D1, `SC_CMPA_A0_D1, `SC_CI_B_CHAIN,
    `SC_CI_A_CHAIN, `SC_C1_MASK_DSBL, `SC_C0_MASK_DSBL,
    `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_DEFSI,
    `SC_SI_A_DEFSI, /*CFG13-12:                            */
    `SC_A0_SRC_ACC, `SC_SHIFT_SR, `SC_PI_DYN_EN,
    `SC_SR_SRC_MSB, `SC_FIFO1__A0, `SC_FIFO0_BUS,
    `SC_MSB_DSBL, `SC_MSB_BIT7, `SC_MSB_NOCHN,
    `SC_FB_NOCHN, `SC_CMP1_NOCHN,
    `SC_CMP0_NOCHN, /*CFG15-14:                      Enable DYN PI*/
    10'h00, `SC_FIFO_CLK__DP,`SC_FIFO_CAP_AX,
    `SC_FIFO__EDGE,`SC_FIFO__SYNC,`SC_EXTCRC_DSBL,
    `SC_WRK16CAT_DSBL /*CFG17-16:                            */
}),
.d1_init(8'b11111111)
) dp_y_engine_MSB(
        /*  input                   */  .reset(cordic_reset),
        /*  input                   */  .clk(op_clock),
        /*  input   [02:00]         */  .cs_addr(dp_op),
        /*  input                   */  .route_si(1'b0),
        /*  input                   */  .route_ci(1'b0),
        /*  input                   */  .f0_load(1'b0),
        /*  input                   */  .f1_load(load_out_fifo),
        /*  input                   */  .d0_load(1'b0),
        /*  input                   */  .d1_load(1'b0),
        /*  output                  */  .ce0(),
        /*  output                  */  .cl0(),
        /*  output                  */  .z0(),
        /*  output                  */  .ff0(),
        /*  output                  */  .ce1(),
        /*  output                  */  .cl1(),
        /*  output                  */  .z1(),
        /*  output                  */  .ff1(),
        /*  output                  */  .ov_msb(),
        /*  output                  */  .co_msb(),
        /*  output                  */  .cmsb(),
        /*  output                  */  .so(),
        /*  output                  */  .f0_bus_stat(F0_y_msb_full),
        /*  output                  */  .f0_blk_stat(F0_y_msb_empty),
        /*  output                  */  .f1_bus_stat(),
        /*  output                  */  .f1_blk_stat(),
        
        /* input                    */  .ci(chain_y[12]),          // Carry in from previous stage
        /* output                   */  .co(),              // Carry out to next stage
        /* input                    */  .sir(chain_y[11]),         // Shift in from right side
        /* output                   */  .sor(chain_y[10]),             // Shift out to right side
        /* input                    */  .sil(),         // Shift in from left side
        /* output                   */  .sol(),             // Shift out to left side
        /* input                    */  .msbi(),        // MSB chain in
        /* output                   */  .msbo(chain_y[9]),            // MSB chain out
        /* input [01:00]            */  .cei(chain_y[1:0]),         // Compare equal in from prev stage
        /* output [01:00]           */  .ceo(),             // Compare equal out to next stage
        /* input [01:00]            */  .cli(chain_y[3:2]),         // Compare less than in from prv stage
        /* output [01:00]           */  .clo(),             // Compare less than out to next stage
        /* input [01:00]            */  .zi(chain_y[5:4]),          // Zero detect in from previous stage
        /* output [01:00]           */  .zo(),              // Zero detect out to next stage
        /* input [01:00]            */  .fi(chain_y[7:6]),          // 0xFF detect in from previous stage
        /* output [01:00]           */  .fo(),              // 0xFF detect out to next stage
        /* input [01:00]            */  .capi(chain_y[14:13]),        // Software capture from previous stage
        /* output [01:00]           */  .capo(),            // Software capture to next stage
        /* input                    */  .cfbi(chain_y[8]),        // CRC Feedback in from previous stage
        /* output                   */  .cfbo(),            // CRC Feedback out to next stage
        /* input [07:00]            */  .pi(y_pi_msb), // Parallel data port
        /* output [07:00]           */  .po(y_po_msb)       // Parallel data port
);

//======================================================================================================
//      Z-ENGINE (16-bit: 2x 8-bit datapaths)
//======================================================================================================

cy_psoc3_dp #(.d0_init(8'b00000000), 
.cy_dpconfig(
{
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM0:         Idle*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC___F0, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM1:                    Load FIFO F0 -> A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM2:         Idle*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM3:         Load LUT (A1 = PI = LUT[i])*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM4:         Idle*/
    `CS_ALU_OP__ADD, `CS_SRCA_A0, `CS_SRCB_A1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM5:         Add A0 (A0 = A0 + A1)*/
    `CS_ALU_OP__SUB, `CS_SRCA_A0, `CS_SRCB_A1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM6:         Sub A0 (A0 = A0 - A1)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM7:         Idle*/
    8'hFF, 8'h00,  /*CFG9:                      ALU Mask unused*/
    8'hFF, 8'hFF,  /*CFG11-10:                      CMP Mask unused*/
    `SC_CMPB_A0_D1, `SC_CMPA_A0_D1, `SC_CI_B_ARITH,
    `SC_CI_A_ARITH, `SC_C1_MASK_DSBL, `SC_C0_MASK_DSBL,
    `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_CHAIN,
    `SC_SI_A_CHAIN, /*CFG13-12:                            */
    `SC_A0_SRC_ACC, `SC_SHIFT_SR, `SC_PI_DYN_EN,
    1'h0, `SC_FIFO1__A0, `SC_FIFO0_BUS,
    `SC_MSB_DSBL, `SC_MSB_BIT7, `SC_MSB_NOCHN,
    `SC_FB_NOCHN, `SC_CMP1_NOCHN,
    `SC_CMP0_NOCHN, /*CFG15-14:                      Enable DYN PI*/
    10'h00, `SC_FIFO_CLK__DP,`SC_FIFO_CAP_AX,
    `SC_FIFO__EDGE,`SC_FIFO__SYNC,`SC_EXTCRC_DSBL,
    `SC_WRK16CAT_DSBL /*CFG17-16:                            */
}),
.d1_init(8'b11111111)
) dp_z_engine_LSB(
        /*  input                   */  .reset(cordic_reset),
        /*  input                   */  .clk(op_clock),
        /*  input   [02:00]         */  .cs_addr(dp_op),
        /*  input                   */  .route_si(1'b0),
        /*  input                   */  .route_ci(1'b0),
        /*  input                   */  .f0_load(1'b0),
        /*  input                   */  .f1_load(load_out_fifo),
        /*  input                   */  .d0_load(1'b0),
        /*  input                   */  .d1_load(1'b0),
        /*  output                  */  .ce0(),
        /*  output                  */  .cl0(),
        /*  output                  */  .z0(),
        /*  output                  */  .ff0(),
        /*  output                  */  .ce1(),
        /*  output                  */  .cl1(),
        /*  output                  */  .z1(),
        /*  output                  */  .ff1(),
        /*  output                  */  .ov_msb(),
        /*  output                  */  .co_msb(),
        /*  output                  */  .cmsb(),
        /*  output                  */  .so(),
        /*  output                  */  .f0_bus_stat(), //.f0_bus_stat(F0_z_msb_full),
        /*  output                  */  .f0_blk_stat(), //.f0_blk_stat(F0_z_msb_empty),
        /*  output                  */  .f1_bus_stat(F1_z_lsb_filled),
        /*  output                  */  .f1_blk_stat(F1_z_lsb_full),
        
        /* input                    */  .ci(1'b0),          // Carry in from previous stage
        /* output                   */  .co(chain_z[12]),   // Carry out to engine_x_MSB
        /* input                    */  .sir(1'b0),         // Shift in from right side
        /* output                   */  .sor(),             // Shift out to right side
        /* input                    */  .sil(chain_z[10]),         // Shift in from left side
        /* output                   */  .sol(chain_z[11]),             // Shift out to left side
        /* input                    */  .msbi(chain_z[9]),        // MSB chain in
        /* output                   */  .msbo(),            // MSB chain out
        /* input [01:00]            */  .cei(2'b0),         // Compare equal in from prev stage
        /* output [01:00]           */  .ceo(chain_z[1:0]),             // Compare equal out to next stage
        /* input [01:00]            */  .cli(2'b0),         // Compare less than in from prv stage
        /* output [01:00]           */  .clo(chain_z[3:2]),             // Compare less than out to next stage
        /* input [01:00]            */  .zi(2'b0),          // Zero detect in from previous stage
        /* output [01:00]           */  .zo(chain_z[5:4]),              // Zero detect out to next stage
        /* input [01:00]            */  .fi(2'b0),          // 0xFF detect in from previous stage
        /* output [01:00]           */  .fo(chain_z[7:6]),              // 0xFF detect out to next stage
        /* input [01:00]            */  .capi(2'b0),        // Software capture from previous stage
        /* output [01:00]           */  .capo(chain_z[14:13]),            // Software capture to next stage
        /* input                    */  .cfbi(1'b0),        // CRC Feedback in from previous stage
        /* output                   */  .cfbo(chain_z[8]),            // CRC Feedback out to next stage
        /* input [07:00]            */  .pi(z_pi_lsb),  // Parallel data port
        /* output [07:00]           */  .po()       // Parallel data port
);
cy_psoc3_dp #(.d0_init(8'b00000000), 
.cy_dpconfig(
{
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM0:         Idle*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC___F0, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM1:                    Load FIFO F0 -> A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM2:         Idle*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM3:         Load LUT (A1 = PI = LUT[i])*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM4:         Idle*/
    `CS_ALU_OP__ADD, `CS_SRCA_A0, `CS_SRCB_A1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM5:         Add A0 (A0 = A0 + A1)*/
    `CS_ALU_OP__SUB, `CS_SRCA_A0, `CS_SRCB_A1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM6:         Sub A0 (A0 = A0 - A1)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM7:         Idle*/
    8'hFF, 8'h00,  /*CFG9:                      ALU Mask unused*/
    8'hFF, 8'hFF,  /*CFG11-10:                      CMP Mask unused*/
    `SC_CMPB_A0_D1, `SC_CMPA_A0_D1, `SC_CI_B_CHAIN,
    `SC_CI_A_CHAIN, `SC_C1_MASK_DSBL, `SC_C0_MASK_DSBL,
    `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_DEFSI,
    `SC_SI_A_DEFSI, /*CFG13-12:                            */
    `SC_A0_SRC_ACC, `SC_SHIFT_SR, `SC_PI_DYN_EN,
    `SC_SR_SRC_MSB, `SC_FIFO1__A0, `SC_FIFO0_BUS,
    `SC_MSB_DSBL, `SC_MSB_BIT7, `SC_MSB_NOCHN,
    `SC_FB_NOCHN, `SC_CMP1_NOCHN,
    `SC_CMP0_NOCHN, /*CFG15-14:                      Enable DYN PI*/
    10'h00, `SC_FIFO_CLK__DP,`SC_FIFO_CAP_AX,
    `SC_FIFO__EDGE,`SC_FIFO__SYNC,`SC_EXTCRC_DSBL,
    `SC_WRK16CAT_DSBL /*CFG17-16:                            */
}),
.d1_init(8'b11111111)
) dp_z_engine_MSB(
        /*  input                   */  .reset(cordic_reset),
        /*  input                   */  .clk(op_clock),
        /*  input   [02:00]         */  .cs_addr(dp_op),
        /*  input                   */  .route_si(1'b0),
        /*  input                   */  .route_ci(1'b0),
        /*  input                   */  .f0_load(1'b0),
        /*  input                   */  .f1_load(load_out_fifo),
        /*  input                   */  .d0_load(1'b0),
        /*  input                   */  .d1_load(1'b0),
        /*  output                  */  .ce0(),
        /*  output                  */  .cl0(),
        /*  output                  */  .z0(),
        /*  output                  */  .ff0(),
        /*  output                  */  .ce1(),
        /*  output                  */  .cl1(),
        /*  output                  */  .z1(),
        /*  output                  */  .ff1(),
        /*  output                  */  .ov_msb(),
        /*  output                  */  .co_msb(),
        /*  output                  */  .cmsb(),
        /*  output                  */  .so(),
        /*  output                  */  .f0_bus_stat(), //.f0_bus_stat(F0_z_msb_full),
        /*  output                  */  .f0_blk_stat(), //.f0_blk_stat(F0_z_msb_empty),
        /*  output                  */  .f1_bus_stat(F1_z_msb_filled),
        /*  output                  */  .f1_blk_stat(F1_z_msb_full),
        
        /* input                    */  .ci(chain_z[12]),          // Carry in from previous stage
        /* output                   */  .co(),              // Carry out to next stage
        /* input                    */  .sir(chain_z[11]),         // Shift in from right side
        /* output                   */  .sor(chain_z[10]),             // Shift out to right side
        /* input                    */  .sil(),         // Shift in from left side
        /* output                   */  .sol(),             // Shift out to left side
        /* input                    */  .msbi(),        // MSB chain in
        /* output                   */  .msbo(chain_z[9]),            // MSB chain out
        /* input [01:00]            */  .cei(chain_z[1:0]),         // Compare equal in from prev stage
        /* output [01:00]           */  .ceo(),             // Compare equal out to next stage
        /* input [01:00]            */  .cli(chain_z[3:2]),         // Compare less than in from prv stage
        /* output [01:00]           */  .clo(),             // Compare less than out to next stage
        /* input [01:00]            */  .zi(chain_z[5:4]),          // Zero detect in from previous stage
        /* output [01:00]           */  .zo(),              // Zero detect out to next stage
        /* input [01:00]            */  .fi(chain_z[7:6]),          // 0xFF detect in from previous stage
        /* output [01:00]           */  .fo(),              // 0xFF detect out to next stage
        /* input [01:00]            */  .capi(chain_z[14:13]),        // Software capture from previous stage
        /* output [01:00]           */  .capo(),            // Software capture to next stage
        /* input                    */  .cfbi(chain_z[8]),        // CRC Feedback in from previous stage
        /* output                   */  .cfbo(),            // CRC Feedback out to next stage
        /* input [07:00]            */  .pi(z_pi_msb), // Parallel data port
        /* output [07:00]           */  .po(z_po)       // Parallel data port
);

//`#end` -- edit above this line, do not edit this line
endmodule

//`#start footer` -- edit after this line, do not edit this line

`endif /* bCORDIC_V_ALREADY_INCLUDED */

//`#end` -- edit above this line, do not edit this line


