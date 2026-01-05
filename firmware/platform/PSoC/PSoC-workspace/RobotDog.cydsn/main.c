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

#include "driver/CanStack/ODriveCanStack.h"

//========================================================
//      Global Variables
//========================================================

static canstack_driver canstack = NULL;

static odrive_com odrive_communication = NULL;
static odrive_driver odrive = NULL;
    
void CORDIC_done_callback(void)
{
    
}

//========================================================
//      Main Function
//========================================================

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    /* Create CanStack driver */
    const canstack_config_t canstack_cfg = {
        .bitrate = 1000000u,
        .ctx = NULL,
    };
    
    CANSTACK_ERROR_CHECK(canstack_create_driver(&canstack_cfg, &canstack));
    
    /* Create ODrive-CanStack driver */
    const odrive_canstack_config_t odrive_canstack_cfg = {
        .driver = canstack,
    };
    
    if (odrive_create_canstack_driver(&odrive_canstack_cfg , &odrive_communication) != ODRIVE_RESULT_OK)
    {
        return 1;
    }
    
    /* Create ODrive interface */
    const odrive_driver_config_t odrive_cfg = {
        .com = odrive_communication,
    };
    
    if (odrive_create_driver(&odrive_cfg , &odrive) != ODRIVE_RESULT_OK)
    {
        return 1;
    }
    
    uint32_t amr, acr;
    CANSTACK_ODRIVE_FILTER_CMD(CAN_CMD_HEARTBEAT, amr, acr);
    canstack_mb_filter_t rx1_filter = {.amr = amr, .acr = acr};
    
    CANSTACK_ERROR_CHECK(canstack_configure_rx_filter(canstack, 0u, &rx1_filter));
    
    CORDIC_init();
    CORDIC_start();
    
    CORDIC_enable_interrupt();
    
    CORDIC_vector_t vec1 = {
        .x = 300.0f,
        .y = 500.0f,
    };
    
    CORDIC_vector_t vec2 = {
        .x = 350.0f,
        .y = 700.0f,
    };
    
    CORDIC_vector_t vec3 = {
        .x = 200.0f,
        .y = 200.0f,
    };
    
    CORDIC_vector_t vec4 = {
        .x = 800.0f,
        .y = 700.0f,
    };
    
    CORDIC_result_t res;
    
    CORDIC_queue_data(&vec1);
    CORDIC_queue_data(&vec2);
    CORDIC_queue_data(&vec3);
    CORDIC_queue_data(&vec4);
    
    //CY_SET_REG16(CORDIC_X_ENGINE_F0_PTR, (int16_t)(0.3 * 32768.0f)); //(int16_t) 0xF000);
    //CY_SET_REG16(CORDIC_Y_ENGINE_F0_PTR, (int16_t)(0.953939f * 32768.0f));
    
    bool pending;
    
    for(;;)
    {
        /* Place your application code here. */
        //canstack_config_t can_cfg;
        
        //canstack_driver_t can_driver;
        //canstack_create_driver(&can_driver, &can_cfg);
        
        pending = CORDIC_has_pending();
        if (pending)
        {
            if (CORDIC_get_data(&res))
            {
                
            }
        }
        
    }
}

//========================================================
//      End of File
//========================================================

/* [] END OF FILE */
