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

// Component: CyStatusReg_v1_90
`ifdef CY_BLK_DIR
`undef CY_BLK_DIR
`endif

`ifdef WARP
`define CY_BLK_DIR "$CYPRESS_DIR\..\psoc\content\CyPrimitives\cyprimitives.cylib\CyStatusReg_v1_90"
`include "$CYPRESS_DIR\..\psoc\content\CyPrimitives\cyprimitives.cylib\CyStatusReg_v1_90\CyStatusReg_v1_90.v"
`else
`define CY_BLK_DIR "C:\Program Files (x86)\Cypress\PSoC Creator\4.4\PSoC Creator\psoc\content\CyPrimitives\cyprimitives.cylib\CyStatusReg_v1_90"
`include "C:\Program Files (x86)\Cypress\PSoC Creator\4.4\PSoC Creator\psoc\content\CyPrimitives\cyprimitives.cylib\CyStatusReg_v1_90\CyStatusReg_v1_90.v"
`endif
//`#end` -- edit above this line, do not edit this line


module bCORDIC (
	input   clock,
	input   en,
	input   rst,
   
    output  isr
);
	parameter CY_CONFIG_TITLE = "CORDIC_N";
    
//`#start body` -- edit after this line, do not edit this line

    //======================================================================================================
    //      REGISTERS & WIRES
    //======================================================================================================
    
    wire F0_x_lsb_full;
    wire F0_x_lsb_empty;
    wire F0_x_msb_full;
    wire F0_x_msb_empty;
    
    wire F0_y_lsb_full;
    wire F0_y_lsb_empty;
    wire F0_y_msb_full;
    wire F0_y_msb_empty;
    
    wire fifo_ready = (!F0_x_lsb_empty & !F0_x_msb_empty & !F0_y_lsb_empty & !F0_y_msb_empty);
    
    //wire F0_z_lsb_full;
    // wire F0_z_lsb_empty;
    //wire F0_z_msb_full;
    //wire F0_z_msb_empty; 
    
    //reg        cordic_complete; /* Denotes the completion of CORDIC           */
	//reg        cordic_complete_buf; /* Buffer to give Core A0 and A1 loading
    
    /* State machine states */
    reg [2:0] cordic_state; // State machine state
    reg [3:0] iter; // Loop counter
    
    reg cordic_done;
    
    wire cordic_enable;
    wire cordic_reset;
    reg  cordic_enable_buf;
    
    wire core_init;
    wire hold_reset = 0;
    wire core_reset = 0;
    
    /* Datapath control opcodes */
    reg [2:0] core_state;
    
    /* Datapath chain connects */
    wire [14:0] chain_x;
    wire [14:0] chain_y;
    wire [14:0] chain_z;
    
    /* Datapath outputs (16-bit) */
    wire [7:0] x_pi_lsb;
    wire [7:0] x_pi_msb;
    wire [7:0] x_po_lsb;
    wire [7:0] x_po_msb;
    
    wire [7:0] y_pi_lsb;
    wire [7:0] y_pi_msb;
    wire [7:0] y_po_lsb;
    wire [7:0] y_po_msb;
    
    wire [7:0] z_pi_lsb;
    wire [7:0] z_pi_msb;
    
    wire load_out_fifo;
    reg load_out;
    
    assign load_out_fifo = load_out;
    
    /* Datapath output assignments*/
    
    /* Link z_pi to wires */
    reg[15:0] z_pi;
    
    assign z_pi_lsb = z_pi[7:0];
    assign z_pi_msb = z_pi[15:8];
    
    /* Temp shift registers (store shifted deltas during S_SHIFT phase) */
    reg [15:0] dX_in_temp;  /* Shifted X delta (for Y calculation) */
    reg [15:0] dY_in_temp;  /* Shifted Y delta (for X calculation) */
    
    /* Register for determining the sign of Y */
    reg sign;
    
    reg [15:0] dX_out_temp;  /* Shifted X delta (for Y calculation) */
    reg [15:0] dY_out_temp;  /* Shifted Y delta (for X calculation) */
    
    assign y_pi_lsb = dY_in_temp[7:0];
    assign y_pi_msb = dY_in_temp[15:8];
    
    assign x_pi_lsb = dX_in_temp[7:0];
    assign x_pi_msb = dX_in_temp[15:8];
    
    //======================================================================================================
    //      STATE MACHINE STATES
    //======================================================================================================
    
    localparam [2:0] STATE_IDLE     = 3'b000;
    localparam [2:0] STATE_LOAD     = 3'b001;
    localparam [2:0] STATE_SHIFT0   = 3'b010;
    localparam [2:0] STATE_SHIFT1   = 3'b011;
    localparam [2:0] STATE_CALC0    = 3'b100;
    localparam [2:0] STATE_CALC1    = 3'b101;
    localparam [2:0] STATE_DONE     = 3'b110;
    
    localparam [2:0] OP_IDLE        = 3'b000;
    localparam [2:0] OP_FIFO        = 3'b001;
    localparam [2:0] OP_SR_A0       = 3'b010;
    localparam [2:0] OP_SR_A1       = 3'b011;
    localparam [2:0] OP_INST0_D0    = 3'b100;
    localparam [2:0] OP_INST1_D0    = 3'b101;
    localparam [2:0] OP_INST0_D1    = 3'b110;
    localparam [2:0] OP_INST1_D1    = 3'b111;
    
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
    //      Interrupt status register
    //======================================================================================================
    
    /* ==================== Wire and Register Declarations ==================== */
    wire StatusIntReg_1_status0;
    wire StatusIntReg_1_status1;
    wire StatusIntReg_1_status2;
    wire StatusIntReg_1_status3;
    wire StatusIntReg_1_status4;
    wire StatusIntReg_1_status5;
    wire StatusIntReg_1_status6;
    //wire StatusIntReg_1_int;
    
    /* ==================== Assignment of Combinatorial Variables ==================== */
    assign StatusIntReg_1_status0 = (cordic_done);
    assign StatusIntReg_1_status1 = (1'b0);
    assign StatusIntReg_1_status2 = (1'b0);
    assign StatusIntReg_1_status3 = (1'b0);
    assign StatusIntReg_1_status4 = (1'b0);
    assign StatusIntReg_1_status5 = (1'b0);
    assign StatusIntReg_1_status6 = (1'b0);

    /* ==================== StatusIntReg_1 ==================== */
        CyStatusReg_v1_90 StatusIntReg_1 (
            .clock(clock),
            .intr(isr),
            .status_0(StatusIntReg_1_status0),
            .status_1(StatusIntReg_1_status1),
            .status_2(StatusIntReg_1_status2),
            .status_3(StatusIntReg_1_status3),
            .status_4(StatusIntReg_1_status4),
            .status_5(StatusIntReg_1_status5),
            .status_6(StatusIntReg_1_status6),
            .status_7(1'b0),
            .status_bus(8'b0));
        defparam StatusIntReg_1.Bit0Mode = 1;
        defparam StatusIntReg_1.Bit1Mode = 1;
        defparam StatusIntReg_1.Bit2Mode = 1;
        defparam StatusIntReg_1.Bit3Mode = 1;
        defparam StatusIntReg_1.Bit4Mode = 1;
        defparam StatusIntReg_1.Bit5Mode = 1;
        defparam StatusIntReg_1.Bit6Mode = 1;
        defparam StatusIntReg_1.Bit7Mode = 0;
        defparam StatusIntReg_1.BusDisplay = 0;
        defparam StatusIntReg_1.Interrupt = 1;
        defparam StatusIntReg_1.MaskValue = 127;
        defparam StatusIntReg_1.NumInputs = 7;

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
    localparam  MDIO_CTRL_ENABLE = 3'b001;
    localparam  MDIO_CTRL_RESET  = 3'b010;
    
    cy_psoc3_control #(.cy_force_order(1)) CtlReg
    (
        .control(ctrl)
    );        
    
    assign cordic_enable = ctrl[MDIO_CTRL_ENABLE] | en;
    assign cordic_reset = ctrl[MDIO_CTRL_RESET] | rst;
    
    always @(posedge op_clock)
    begin
        cordic_enable_buf <= cordic_enable;
    end
    
    assign core_init = cordic_enable & ~cordic_enable_buf;

    //======================================================================================================
    //      STATUS REGISTER
    //======================================================================================================
    
    wire[7:0] mystatus;
    
    cy_psoc3_status #(.cy_force_order(`TRUE), .cy_md_select(8'b00000000)) StsReg (
        .status(mystatus),
        .reset(cordic_reset),
        .clock(op_clock)
    );
    
    assign mystatus[7:5] = 7'b0;
    assign mystatus[4] = !F0_x_lsb_empty & !F0_x_msb_empty;  // X input ready
    assign mystatus[3] = !F0_y_lsb_empty & !F0_y_msb_empty;  // Y input ready
    assign mystatus[2] = cordic_done;                         // Done (non-sticky)
    assign mystatus[1] = (cordic_state == STATE_IDLE);        // Idle
    assign mystatus[0] = hold_reset | core_reset;
    
    //======================================================================================================
    //      STATE MACHINE
    //======================================================================================================
    
    always @(posedge op_clock) begin
    if(cordic_reset) 
    begin
        iter <= 4'b0;
        
        cordic_state <= STATE_IDLE;
        core_state <= OP_IDLE;
        
        cordic_done <= 1'b0;
        load_out <= 1'b0;
    end 
    else if (cordic_enable)
    begin
        case(cordic_state)
            STATE_IDLE: 
            begin
                load_out <= 1'b0;
                if (fifo_ready)
                begin
                    cordic_state <= STATE_LOAD;
                    core_state <= OP_FIFO;
                    
                    // Reset loop counter
                    iter <= 4'b0;
                    //cordic_done <= 1'b0;
                end
            end
            STATE_LOAD: 
            begin
                cordic_state <= STATE_SHIFT0;
                core_state <= OP_FIFO;
            end
            STATE_SHIFT0: 
            begin
                cordic_state <= STATE_CALC0;
                core_state <= OP_SR_A0;
                
                dY_out_temp[7:0] <= y_po_lsb;
                dY_out_temp[15:8] <= y_po_msb;
                
                dX_out_temp[7:0] <= x_po_lsb;
                dX_out_temp[15:8] <= x_po_msb;
                
                sign <= y_po_msb[7];
            end
            STATE_SHIFT1: 
            begin
                cordic_state <= STATE_CALC0;
                core_state <= OP_SR_A1;
                
                dY_out_temp[7:0] <= y_po_lsb;
                dY_out_temp[15:8] <= y_po_msb;
                
                dX_out_temp[7:0] <= x_po_lsb;
                dX_out_temp[15:8] <= x_po_msb;
                
                sign <= y_po_msb[7];
            end
            STATE_CALC0: 
            begin
                dY_in_temp[15:0] <= dX_out_temp[15:0];
                dX_in_temp[15:0] <= dY_out_temp[15:0];
                
                cordic_state <= STATE_CALC1;
                if (sign)
                begin
                    core_state <= OP_INST0_D1;
                end
                else
                begin
                    core_state <= OP_INST0_D0;
                end
            end
            STATE_CALC1: 
            begin
                core_state <= core_state + 1;
                
                if (iter == 4'd15)
                begin
                    cordic_state <= STATE_DONE;
                end
                else
                begin
                    iter <= iter + 1;
                    cordic_state <= STATE_SHIFT1;
                end
            end
            STATE_DONE: 
            begin
                cordic_state <= STATE_IDLE;
                core_state <= OP_IDLE;
                //iter <= 4'b0;
                load_out <= 1'b1;
                cordic_done <= 1'b1;
            end
            default: 
            begin
                cordic_state <= STATE_IDLE;
                core_state <= OP_IDLE;
            end
        endcase
    end
end

//======================================================================================================
//      X-ENGINE (16-bit: 2x 8-bit datapaths)
//======================================================================================================

cy_psoc3_dp #(.cy_dpconfig(
{
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM0:     Idle*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC___F0, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM1:     Load FIFO F0 -> A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP___SR, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM2:     Rs A0 (A1 = A0 >> 1)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A1, `CS_SRCB_D0,
    `CS_SHFT_OP___SR, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM3:     Rs A1 (A1 = A1 >> 1)*/
    `CS_ALU_OP__ADD, `CS_SRCA_A0, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM4:     Add A0 (A0 = PI + A0)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM5:     NOP*/
    `CS_ALU_OP__SUB, `CS_SRCA_A0, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM6:     Sub A0 (A0 = PI - A0)*/
    `CS_ALU_OP__XOR, `CS_SRCA_A0, `CS_SRCB_D1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM7:     Negate A0 (A0 = A0 ^ D1)*/
    8'hFF, 8'h00,  /*CFG9:       ALU Mask unused*/
    8'hFF, 8'hFF,  /*CFG11-10:       CMP Mask unused*/
    `SC_CMPB_A0_D1, `SC_CMPA_A0_D1, `SC_CI_B_ARITH,
    `SC_CI_A_ARITH, `SC_C1_MASK_DSBL, `SC_C0_MASK_DSBL,
    `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_DEFSI,
    `SC_SI_A_DEFSI, /*CFG13-12:             */
    `SC_A0_SRC_ACC, `SC_SHIFT_SR, `SC_PI_DYN_EN,
    1'h0, `SC_FIFO1__A0, `SC_FIFO0_BUS,
    `SC_MSB_DSBL, `SC_MSB_BIT0, `SC_MSB_NOCHN,
    `SC_FB_NOCHN, `SC_CMP1_NOCHN,
    `SC_CMP0_NOCHN, /*CFG15-14:       Enable DYN PI*/
    10'h00, `SC_FIFO_CLK__DP,`SC_FIFO_CAP_AX,
    `SC_FIFO__EDGE,`SC_FIFO__SYNC,`SC_EXTCRC_DSBL,
    `SC_WRK16CAT_DSBL /*CFG17-16:             */
}),
.d0_init(8'b00000000),
.d1_init(8'b11111111)
) dp_x_engine_LSB(
        /*  input                   */  .reset(cordic_reset),
        /*  input                   */  .clk(op_clock),
        /*  input   [02:00]         */  .cs_addr(core_state),
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
        /*  output                  */  .f1_bus_stat(),
        /*  output                  */  .f1_blk_stat(),
        
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
cy_psoc3_dp #(.cy_dpconfig(
{
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM0:     Idle*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC___F0, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM1:     Load FIFO F0 -> A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP___SR, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM2:     Rs A0 (A1 = A0 >> 1)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A1, `CS_SRCB_D0,
    `CS_SHFT_OP___SR, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM3:     Rs A1 (A1 = A1 >> 1)*/
    `CS_ALU_OP__ADD, `CS_SRCA_A0, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM4:     Add A0 (A0 = PI + A0)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM5:     NOP*/
    `CS_ALU_OP__SUB, `CS_SRCA_A0, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM6:     Sub A0 (A0 = PI - A0)*/
    `CS_ALU_OP__XOR, `CS_SRCA_A0, `CS_SRCB_D1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM7:     Negate A0 (A0 = A0 ^ D1)*/
    8'hFF, 8'h00,  /*CFG9:       ALU Mask unused*/
    8'hFF, 8'hFF,  /*CFG11-10:       CMP Mask unused*/
    `SC_CMPB_A0_D1, `SC_CMPA_A0_D1, `SC_CI_B_ARITH,
    `SC_CI_A_ARITH, `SC_C1_MASK_DSBL, `SC_C0_MASK_DSBL,
    `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_DEFSI,
    `SC_SI_A_DEFSI, /*CFG13-12:             */
    `SC_A0_SRC_ACC, `SC_SHIFT_SR, `SC_PI_DYN_EN,
    `SC_SR_SRC_MSB, `SC_FIFO1__A0, `SC_FIFO0_BUS,
    `SC_MSB_DSBL, `SC_MSB_BIT0, `SC_MSB_NOCHN,
    `SC_FB_NOCHN, `SC_CMP1_NOCHN,
    `SC_CMP0_NOCHN, /*CFG15-14:       Enable DYN PI*/
    10'h00, `SC_FIFO_CLK__DP,`SC_FIFO_CAP_AX,
    `SC_FIFO__EDGE,`SC_FIFO__SYNC,`SC_EXTCRC_DSBL,
    `SC_WRK16CAT_DSBL /*CFG17-16:             */
}),
.d0_init(8'b00000000),
.d1_init(8'b11111111)
) dp_x_engine_MSB(
        /*  input                   */  .reset(cordic_reset),
        /*  input                   */  .clk(op_clock),
        /*  input   [02:00]         */  .cs_addr(core_state),
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
        /*  output                  */  .f1_bus_stat(),
        /*  output                  */  .f1_blk_stat(),
        
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

cy_psoc3_dp #(.cy_dpconfig(
{
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM0:     Idle*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC___F0, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM1:     Load FIFO F0 -> A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP___SR, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM2:     Rs A0 (A1 = A0 >> 1)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A1, `CS_SRCB_D0,
    `CS_SHFT_OP___SR, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM3:     Rs A1 (A1 = A1 >> 1)*/
    `CS_ALU_OP__SUB, `CS_SRCA_A0, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM4:     Sub A0 (A0 = PI - A0)*/
    `CS_ALU_OP__XOR, `CS_SRCA_A0, `CS_SRCB_D1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM5:     Negate A0 (A0 = A0 ^ D1)*/
    `CS_ALU_OP__ADD, `CS_SRCA_A0, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM6:    Add A0 (A0 = PI + A0)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM7:    NOP*/
    8'hFF, 8'h00,  /*CFG9:       ALU Mask unused*/
    8'hFF, 8'hFF,  /*CFG11-10:       CMP Mask unused*/
    `SC_CMPB_A0_D1, `SC_CMPA_A0_D1, `SC_CI_B_ARITH,
    `SC_CI_A_ARITH, `SC_C1_MASK_DSBL, `SC_C0_MASK_DSBL,
    `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_DEFSI,
    `SC_SI_A_DEFSI, /*CFG13-12:             */
    `SC_A0_SRC_ACC, `SC_SHIFT_SR, `SC_PI_DYN_EN,
    1'h0, `SC_FIFO1_BUS, `SC_FIFO0_BUS,
    `SC_MSB_DSBL, `SC_MSB_BIT0, `SC_MSB_NOCHN,
    `SC_FB_NOCHN, `SC_CMP1_NOCHN,
    `SC_CMP0_NOCHN, /*CFG15-14:       Enable DYN PI*/
    10'h00, `SC_FIFO_CLK__DP,`SC_FIFO_CAP_AX,
    `SC_FIFO__EDGE,`SC_FIFO__SYNC,`SC_EXTCRC_DSBL,
    `SC_WRK16CAT_DSBL /*CFG17-16:             */
}),
.d0_init(8'b00000000),
.d1_init(8'b11111111)
) dp_y_engine_LSB(
        /*  input                   */  .reset(cordic_reset),
        /*  input                   */  .clk(op_clock),
        /*  input   [02:00]         */  .cs_addr(core_state),
        /*  input                   */  .route_si(1'b0),
        /*  input                   */  .route_ci(1'b0),
        /*  input                   */  .f0_load(1'b0),
        /*  input                   */  .f1_load(1'b0),
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
cy_psoc3_dp #(.cy_dpconfig(
{
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM0:     Idle*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC___F0, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM1:     Load FIFO F0 -> A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP___SR, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM2:     Rs A0 (A1 = A0 >> 1)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A1, `CS_SRCB_D0,
    `CS_SHFT_OP___SR, `CS_A0_SRC_NONE, `CS_A1_SRC__ALU,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM3:     Rs A1 (A1 = A1 >> 1)*/
    `CS_ALU_OP__SUB, `CS_SRCA_A0, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM4:     Sub A0 (A0 = PI - A0)*/
    `CS_ALU_OP__XOR, `CS_SRCA_A0, `CS_SRCB_D1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM5:     Negate A0 (A0 = A0 ^ D1)*/
    `CS_ALU_OP__ADD, `CS_SRCA_A0, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM6:    Add A0 (A0 = PI + A0)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM7:    NOP*/
    8'hFF, 8'h00,  /*CFG9:       ALU Mask unused*/
    8'hFF, 8'hFF,  /*CFG11-10:       CMP Mask unused*/
    `SC_CMPB_A0_D1, `SC_CMPA_A0_D1, `SC_CI_B_ARITH,
    `SC_CI_A_ARITH, `SC_C1_MASK_DSBL, `SC_C0_MASK_DSBL,
    `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_DEFSI,
    `SC_SI_A_DEFSI, /*CFG13-12:             */
    `SC_A0_SRC_ACC, `SC_SHIFT_SR, `SC_PI_DYN_EN,
    `SC_SR_SRC_MSB, `SC_FIFO1_BUS, `SC_FIFO0_BUS,
    `SC_MSB_DSBL, `SC_MSB_BIT0, `SC_MSB_NOCHN,
    `SC_FB_NOCHN, `SC_CMP1_NOCHN,
    `SC_CMP0_NOCHN, /*CFG15-14:       Enable DYN PI*/
    10'h00, `SC_FIFO_CLK__DP,`SC_FIFO_CAP_AX,
    `SC_FIFO__EDGE,`SC_FIFO__SYNC,`SC_EXTCRC_DSBL,
    `SC_WRK16CAT_DSBL /*CFG17-16:             */
}),
.d0_init(8'b00000000),
.d1_init(8'b11111111)
) dp_y_engine_MSB(
        /*  input                   */  .reset(cordic_reset),
        /*  input                   */  .clk(op_clock),
        /*  input   [02:00]         */  .cs_addr(core_state),
        /*  input                   */  .route_si(1'b0),
        /*  input                   */  .route_ci(1'b0),
        /*  input                   */  .f0_load(1'b0),
        /*  input                   */  .f1_load(1'b0),
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

cy_psoc3_dp #(.cy_dpconfig(
{
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM0:     Idle*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC___F0, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM1:     Load FIFO F0 -> A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM2:   NOP*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM3:   NOP*/
    `CS_ALU_OP__ADD, `CS_SRCA_A0, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM4:     Add A0 (A0 = PI + A0)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM5:     NOP*/
    `CS_ALU_OP__SUB, `CS_SRCA_A0, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM6:     Sub A0 (A0 = PI - A0)*/
    `CS_ALU_OP__XOR, `CS_SRCA_A0, `CS_SRCB_D1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM7:     Negate A0 (A0 = A0 ^ D1)*/
    8'hFF, 8'h00,  /*CFG9:       ALU Mask unused*/
    8'hFF, 8'hFF,  /*CFG11-10:       CMP Mask unused*/
    `SC_CMPB_A0_D1, `SC_CMPA_A0_D1, `SC_CI_B_ARITH,
    `SC_CI_A_ARITH, `SC_C1_MASK_DSBL, `SC_C0_MASK_DSBL,
    `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_DEFSI,
    `SC_SI_A_DEFSI, /*CFG13-12:             */
    `SC_A0_SRC_ACC, `SC_SHIFT_SR, `SC_PI_DYN_EN,
    1'h0, `SC_FIFO1__A0, `SC_FIFO0_BUS,
    `SC_MSB_DSBL, `SC_MSB_BIT0, `SC_MSB_NOCHN,
    `SC_FB_NOCHN, `SC_CMP1_NOCHN,
    `SC_CMP0_NOCHN, /*CFG15-14:       Enable DYN PI*/
    10'h00, `SC_FIFO_CLK__DP,`SC_FIFO_CAP_AX,
    `SC_FIFO__EDGE,`SC_FIFO__SYNC,`SC_EXTCRC_DSBL,
    `SC_WRK16CAT_DSBL /*CFG17-16:             */
}),
.d0_init(8'b00000000),
.d1_init(8'b11111111)
) dp_z_engine_LSB(
        /*  input                   */  .reset(cordic_reset),
        /*  input                   */  .clk(op_clock),
        /*  input   [02:00]         */  .cs_addr(core_state),
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
        /*  output                  */  .f1_bus_stat(),
        /*  output                  */  .f1_blk_stat(),
        
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
cy_psoc3_dp #(.cy_dpconfig(
{
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM0:     Idle*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC___F0, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM1:     Load FIFO F0 -> A0*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM2:   NOP*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM3:   NOP*/
    `CS_ALU_OP__ADD, `CS_SRCA_A0, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM4:     Add A0 (A0 = PI + A0)*/
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM5:     NOP*/
    `CS_ALU_OP__SUB, `CS_SRCA_A0, `CS_SRCB_A0,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_ENBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM6:     Sub A0 (A0 = PI - A0)*/
    `CS_ALU_OP__XOR, `CS_SRCA_A0, `CS_SRCB_D1,
    `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE,
    `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA,
    `CS_CMP_SEL_CFGA, /*CFGRAM7:     Negate A0 (A0 = A0 ^ D1)*/
    8'hFF, 8'h00,  /*CFG9:       ALU Mask unused*/
    8'hFF, 8'hFF,  /*CFG11-10:       CMP Mask unused*/
    `SC_CMPB_A0_D1, `SC_CMPA_A0_D1, `SC_CI_B_ARITH,
    `SC_CI_A_ARITH, `SC_C1_MASK_DSBL, `SC_C0_MASK_DSBL,
    `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_DEFSI,
    `SC_SI_A_DEFSI, /*CFG13-12:             */
    `SC_A0_SRC_ACC, `SC_SHIFT_SR, `SC_PI_DYN_EN,
    `SC_SR_SRC_MSB, `SC_FIFO1__A0, `SC_FIFO0_BUS,
    `SC_MSB_DSBL, `SC_MSB_BIT0, `SC_MSB_NOCHN,
    `SC_FB_NOCHN, `SC_CMP1_NOCHN,
    `SC_CMP0_NOCHN, /*CFG15-14:       Enable DYN PI*/
    10'h00, `SC_FIFO_CLK__DP,`SC_FIFO_CAP_AX,
    `SC_FIFO__EDGE,`SC_FIFO__SYNC,`SC_EXTCRC_DSBL,
    `SC_WRK16CAT_DSBL /*CFG17-16:             */
}),
.d0_init(8'b00000000),
.d1_init(8'b11111111)
) dp_z_engine_MSB(
        /*  input                   */  .reset(cordic_reset),
        /*  input                   */  .clk(op_clock),
        /*  input   [02:00]         */  .cs_addr(core_state),
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
        /*  output                  */  .f1_bus_stat(),
        /*  output                  */  .f1_blk_stat(),
        
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
        /* output [07:00]           */  .po()       // Parallel data port
);

//`#end` -- edit above this line, do not edit this line
endmodule

//`#start footer` -- edit after this line, do not edit this line

`endif /* bCORDIC_V_ALREADY_INCLUDED */

//`#end` -- edit above this line, do not edit this line




