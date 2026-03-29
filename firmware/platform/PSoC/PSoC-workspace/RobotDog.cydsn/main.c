/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

//========================================================
//      Standard Includes
//========================================================

#include <stdint.h>
#include <stdio.h>
#include <math.h>


#include "project.h"
#include "cyapicallbacks.h"

#include "CanStack.h"
#include "ODrive.h"

#include "ik.h"
#include "accelerators/ik_cordic.h"
#include "models/ik_4bar_diamond.h"

#include "driver/CanStack/ODriveCanStack.h"

#define IK_L1 76.8f
#define IK_L2 120.0f

#define IK_L1_POW 5898.24f
#define IK_L2_POW 14400.0f

/* Optional: make these compile-time constants (best) */
#ifndef IK_K_CONST
/* k = 1 / (-2*L1)  */
#define IK_K_CONST   (1.0f / (-2.0f * (float)IK_L1))
#endif

#ifndef IK_PHI_SCALE
/* -7 / (2*pi) */
#define IK_PHI_SCALE (-7.0f / (2.0f * (float)M_PI))
#endif

//#define M_PI 3.14159f

#define M_2PI 6.283185307f

#define SYSTICK_MAXVAL 0x00FFFFFF
static volatile uint32 SysCntVal;

static uint8_t i2c_rx_buffer[16u];

//========================================================
//      Global Variables
//========================================================

static uint32_t sys_tick;

static canstack_driver canstack = NULL;

static odrive_backend backend = NULL;
static odrive_driver odrive = NULL;

static ik_model_h ik_model;

static ik_cordic_h ik_cordic;

static odrive_gpio_driver_t gpio_driver;

//========================================================
//      System Tick Functions
//========================================================

void sys_tick_update(void)
{
    sys_tick++;
}

uint32_t get_tick(void)
{
    return sys_tick;   
}

//========================================================
//      Main Function
//========================================================

void write_home_pin(uint8_t pin, uint8_t value)
{
    switch (pin)
    {
    case 1u:
        {
            Motor0_Endstop_Write(value);
        }
        break;
    case 2u:
        {
            Motor1_Endstop_Write(value);
        }
        break;
    }
}

// Prolate cycloid (trochoid): x = r*t - h*sin(t), y = r - h*cos(t)
// Assumes t is in radians.
/*void cycloidal_function(float t, ik_vector2f_t* out)
{
    const float r = 20.0f;
    const float h = 24.0f;

    // Rotate vector (h, 0) by angle t -> (h*cos(t), h*sin(t))
    ik_vector3f_t in  = { .v = { h, 0.0f, t } };
    ik_vector3f_t res = { 0 };

    ik_cordic_rot_sync(
        ik_cordic,
        IK_CORDIC_COORD_CIRCULAR,
        IK_CORDIC_FORMAT_VECTOR,
        &in,
        &res
    );

    const float h_cos = IK_VEC3_X(res); // h*cos(t)
    const float h_sin = IK_VEC3_Y(res); // h*sin(t)

    IK_VEC3_XP(out) = (r * t) - h_sin;
    IK_VEC3_YP(out) = r - h_cos;
}*/

// Wrap angle to [0, 2*pi)

void cycloidal_sagittal(float t, ik_vector2f_t* out)
{
    const float r = 15.0f;
    const float h = 30.0f;

    // Compute base prolate cycloid at phase t
    /*ik_vector3f_t in  = { .v = { h, 0.0f, t } };
    ik_vector3f_t res = { 0 };

    ik_cordic_rot_sync(ik_cordic, IK_CORDIC_COORD_CIRCULAR, IK_CORDIC_FORMAT_VECTOR, &in, &res);

    float x = (r * t) - IK_VEC3_Y(res);   // r*t - h*sin(t)
    float y = r - IK_VEC3_X(res);         // r - h*cos(t)

    // Center x around mid-stride so it spans +/- instead of only +
    // Over [0, 2pi], x increases ~ linearly by 2pi*r, so subtract pi*r.
    x -= (M_PI * r);

    // Mirror the second half into the left side (optional, if you want symmetric reuse)
    if (t > pi) x = -x;*/
    
    float h_cos = h * cosf(t);
    float h_sin = h * sinf(t);
    
    float x = (r*t) - h_sin;
    float y = r - h_cos;
    
    y = 180.0f - y;
    x = x - (M_PI*r);

    IK_VEC2_XP(out) = x;
    IK_VEC2_YP(out) = y;
} 

static inline float clampf(float x, float lo, float hi) {
    return (x < lo) ? lo : (x > hi) ? hi : x;
}

static inline float smoothstep(float a, float b, float x) {
    float t = clampf((x - a) / (b - a), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

static inline float lerpf(float a, float b, float t) {
    return a + (b - a) * t;
}

// A "stance-aware" wrapper that uses your cycloidal_sagittal() for swing,
// and inserts a straight-line segment around pi.
void gait_sagittal(float n, ik_vector2f_t* out)
{
    const float two_pi = 2.0f * (float)M_PI;
    const float pi     = (float)M_PI;

    // Your cusp-skip range:
    const float t_start = 1.1f;
    const float t_end   = two_pi - 1.1f;

    // Deadzone around pi (in radians) that will become straight line.
    // Bigger = more straight-line samples + softer reversal.
    const float dz = 0.5f;              // ~14 degrees

    // How much of n to spend on stance straight line.
    // Bigger = more samples on straight line.
    const float stance_ratio = 0.30f;    // 30% of samples on stance

    // Define the “swing” t ranges excluding the deadzone around pi
    float t1 = t_start;
    float t2 = pi - dz;
    float t3 = pi + dz;
    float t4 = t_end;

    // Precompute endpoints for the straight line using your cycloid
    ik_vector2f_t p2, p3;
    cycloidal_sagittal(t2, &p2);
    cycloidal_sagittal(t3, &p3);

    // You probably want the stance line to be at (or near) ground height.
    // Option A: keep it exactly at the lower of the two endpoints (safer against penetration).
    float y_stance = fmaxf(IK_VEC2_YP(&p2), IK_VEC2_YP(&p3)); // depends on your axis; swap if inverted
    // Option B: average:
    // float y_stance = 0.5f * (IK_VEC2_YP(&p2) + IK_VEC2_YP(&p3));

    float n_stance0 = 0.5f - 0.5f * stance_ratio;
    float n_stance1 = 0.5f + 0.5f * stance_ratio;

    if (n < n_stance0)
    {
        // Swing 1: map n -> [t1..t2]
        float u = n / n_stance0;
        // Optional: ease to avoid jerk at boundaries
        u = smoothstep(0.0f, 1.0f, u);
        float t = lerpf(t1, t2, u);
        cycloidal_sagittal(t, out);
    }
    else if (n > n_stance1)
    {
        // Swing 2: map n -> [t3..t4]
        float u = (n - n_stance1) / (1.0f - n_stance1);
        u = smoothstep(0.0f, 1.0f, u);
        float t = lerpf(t3, t4, u);
        cycloidal_sagittal(t, out);
    }
    else
    {
        // Stance straight line: lots of samples here
        float u = (n - n_stance0) / (n_stance1 - n_stance0);
        u = smoothstep(0.0f, 1.0f, u);

        float x = lerpf(IK_VEC2_XP(&p2), IK_VEC2_XP(&p3), u);
        float y = y_stance;

        IK_VEC2_XP(out) = x;
        IK_VEC2_YP(out) = y;
    }
}

void test_function(ik_vector2f_t* res)
{ 
    odrive_setpoint_t pos1 = {
        .position = IK_VEC2_XP(res),
        .velocity_ff = 1,
        .torque_ff = 1,
    };
    odrive_axis_set_position(odrive, 0x21, &pos1);
    
    odrive_setpoint_t pos2 = {
        .position = IK_VEC2_YP(res),
        .velocity_ff = 1,
        .torque_ff = 1,
    };
    odrive_axis_set_position(odrive, 0x20, &pos2);
}

typedef struct {
    // Joysticks (8 bytes)
    int16_t leftX;       // Left stick X-as (-511 tot +512)
    int16_t leftY;       // Left stick Y-as (-511 tot +512)
    int16_t rightX;      // Right stick X-as (-511 tot +512)
    int16_t rightY;      // Right stick Y-as (-511 tot +512)
    
    // Triggers (4 bytes)
    int16_t brake;       // Left trigger L2 (0-1023)
    int16_t throttle;    // Right trigger R2 (0-1023)
    
    // Buttons & D-pad (3 bytes)
    uint16_t buttons;    // Button bitmask (A/B/X/Y/LB/RB/start/select/etc)
    uint8_t dpad;        // D-pad (up/down/left/right als bits)
    
    // Extra (1 byte)
    uint8_t misc;

} controller_data_t;

static controller_data_t frame;



float ik_cordic_acos_test(float c)
{
    /* Clamp input */
    if (c >  1.0f) c =  1.0f;
    if (c < -1.0f) c = -1.0f;

    /* Compute sin = +sqrt(1 - c^2) */
    float s = 1.0f - c * c;
    if (s < 0.0f) s = 0.0f;
    s = sqrtf(s);

    /* Build vectoring job:
       atan2(y=s, x=c) */
    ik_vector3f_t in = {
        .v = { c, s, 0.0f }   /* z=0, offset handled via submit */
    };

    ik_vector3f_t out = {0};

    ik_cordic_vec_sync(
        ik_cordic,
        IK_CORDIC_COORD_CIRCULAR,
        IK_CORDIC_FORMAT_UNIT,
        &in,
        &out
    );

    /* IMPORTANT:
       submit() has already stored quadrant correction in job->z_offset_rad
       vec_sync() should apply it when forming out.z
    */
    return IK_VEC3_Z(out);
}



int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    // Starting system tick timer
    CySysTickInit();
    CySysTickClear();
    CySysTickStart();
    
    CySysTickSetCallback(0u, sys_tick_update);
    
    USBUART_Start(USBUART_COM_PORT1,USBUART_5V_OPERATION);
    
    //while(USBUART_GetConfiguration()==0) {}
    
    Motor0_Endstop_Write(0x00);
    Motor1_Endstop_Write(0x00);
    
    gpio_driver.write = write_home_pin;
    
    I2C_SlaveInitWriteBuf(i2c_rx_buffer, 16);
    I2C_Start(); 
    
    /* Create CanStack driver */
    const canstack_config_t canstack_cfg = {
        .bitrate = 1000000u,
        .ctx = NULL,
    };
    
    CANSTACK_ERROR_CHECK(canstack_create_driver(&canstack_cfg, &canstack));
    
    odrive_canstack_config_t backend_cfg = {
        .driver = canstack,
        .gpio = &gpio_driver
    };
    
    if (odrive_create_canstack_backend(&backend_cfg, &backend) != ODRIVE_RESULT_OK)
    {
        while (1) {}
    }
    
    odrive_driver_cfg_t odrive_cfg = {
        .backend = backend,
        .enable_heartbeat_monitor = true,
        .enable_force_feedback_homing = true,
        .heartbeat_timeout_ms = 200u, 
    };
    
    if (odrive_create_driver(&odrive_cfg, &odrive) != ODRIVE_RESULT_OK)
    {
        while (1) {}
    }
    
    // Register CANSTACK filters
    CANSTACK_ODRIVE_REGISTER_FILTER(canstack, CAN_CMD_HEARTBEAT, 0u);
    CANSTACK_ODRIVE_REGISTER_FILTER(canstack, CAN_CMD_HEARTBEAT, 1u);
    CANSTACK_ODRIVE_REGISTER_FILTER(canstack, CAN_CMD_GET_IQ, 2u);
    CANSTACK_ODRIVE_REGISTER_FILTER(canstack, CAN_CMD_GET_BUS_VOLTAGE_CURRENT, 3u);
    CANSTACK_ODRIVE_REGISTER_FILTER(canstack, CAN_CMD_GET_ENCODER_ESTIMATES, 4u);
   
    // Start CORDIC hardware
    CORDIC_start();
    CORDIC_enable_interrupt();
    
    odrive_force_homing_cfg_t home_cfg = {
        .enable_force_feedback_homing = true,
        .is_force_homing = false,
        .homing_pin = 1u,
        .iq_threshold = 3.0f,
        .position_delta = 0.0f,
    };
    
    odrive_force_homing_cfg_t home_cfg2 = {
        .enable_force_feedback_homing = true,
        .is_force_homing = false,
        .homing_pin = 2u,
        .iq_threshold = 4.0f,
        .position_delta = 0.0f,
    };
    
    odrive_start_driver(odrive);
    odrive_register_axis(odrive, &home_cfg2, 0x20);
    odrive_register_axis(odrive, &home_cfg, 0x21);
    
    CyDelayUs(1000);
    
    odrive_force_feedback_homing(odrive, 0x20);
    odrive_force_feedback_homing(odrive, 0x21);
    
    uint8_t is_ready = 0;
    
    uint16_t samples = 300;
    uint16_t current_sample = 0;
    
    uint32_t sample_tick = 0;
    uint32_t ic2_tick = 0;
    uint32_t sample_delay = 0u;
    
    ik_4bar_diamond_params_t model_params = {
        .l1 = IK_L1,
        .l2 = IK_L2,
        .scale = IK_PHI_SCALE
    };
    
    ik_model_cfg_t model_config = {
        .solve_mode = IK_SOLVE_MODE_NUMERIC,
        .numerical_solver = IK_NUMERICAL_SOLVER_NONE,
        .max_iters = 1u,
        .features = IK_FEAT_CORDIC_SUPPORT,
        .tick = get_tick,
        .ctx = (void*) &model_params
    };
    
    IK_CHECK(ik_4bar_create(&model_config, &ik_model));
    
    ik_cordic = ik_4bar_diamond_get_cordic_handle();
    
    /*float test[] = { -1.0f, -0.5f, 0.0f, 0.5f, 1.0f };

    for (int i = 0; i < 5; i++)
    {
        float c = test[i];
        float hw = ik_cordic_acos_test(c);
        float sw = acosf(c);

        printf("c=%+.2f  hw=%f  sw=%f\n", c, hw, sw);
    }*/
    
    for(;;)
    {
        if (sys_tick >= (ic2_tick + 40u))
        {
            uint8_t I2C_status = I2C_SlaveStatus();
            if(I2C_status & I2C_SSTAT_WR_CMPT) {
                I2C_SlaveClearWriteStatus();

                volatile uint8_t WrittenBytes = I2C_SlaveGetWriteBufSize();
                if(WrittenBytes >= 16) {
                   memcpy(&frame, i2c_rx_buffer, 16u);
                }
                
                I2C_SlaveClearWriteBuf(); 
            }
            ic2_tick = sys_tick;
        }
        
        //float power = odrive_max_power(odrive);
        
        odrive_poll_driver(odrive, sys_tick);
        
        if (odrive_axis_is_homed(odrive, 0x20) && odrive_axis_is_homed(odrive, 0x21))
            is_ready = 1;
       
        if (is_ready == 1)
        {
           
            if (sys_tick >= (sample_tick + sample_delay))
            {
                current_sample++;
                if (current_sample >= samples)   // wrap at samples-1
                    current_sample = 0;

                float n = (float)current_sample / (float)(samples - 1); // n goes 0..1 across samples points
                
                //float n = ((float) current_sample / (float) samples);
                float t = 1.1f + (n * (M_2PI - 1.9f));
                
                ik_vector2f_t coords = {0u, 0u};
                
                gait_sagittal(n, &coords);
               
                ik_vector2f_t output;
                
                volatile ik_result_t res = ik_4bar_diamond_ik(ik_model, &coords, &output);
                
                test_function(&output);
                
                uint16_t max = 100;
                uint16_t min = 4;
                volatile float map = 1.0f - (1.0f / (512.0f - (float) frame.rightY));
                float delta = ((float) max - (float) min) * map;
                uint16_t dly = delta + min;
                
                sample_delay = 45;
                sample_tick = sys_tick;
            }
            
            /*if (frame.rightY >= 40)
            {
                
            }
            else if (frame.rightY <= -40)
            {
                if (sys_tick >= (sample_tick + sample_delay))
                {
                    current_sample--;
                    if(current_sample <= 0)
                        current_sample = samples;
                    
                    volatile float n = ((float) current_sample / (float) samples);
                    volatile float t = 1.1f + (n * (M_2PI - 1.1f));
                    
                    ik_vector2f_t coords = {0u, 0u};
                    
                    cycloidal_sagittal(t, &coords);
                   
                    ik_vector2f_t output;
                    
                    ik_4bar_diamond_ik(ik_model, &coords, &output);
                    
                    test_function(&output);
                    
                    uint16_t max = 8000;
                    uint16_t min = 100;
                    volatile float map = 1.0f - (1.0f / (-512.0f + (float) frame.rightY));
                    float delta = ((float) max - (float) min) * map;
                    uint16_t dly = delta + min;
                    
                    sample_delay = dly;
                    sample_tick = sys_tick;
                }
            }*/
            
        }
    }
    
}

//========================================================
//      End of File
//========================================================

/* [] END OF FILE */
