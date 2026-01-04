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

#include <stdint.h>
#include <stdio.h>
#include <math.h>


#include "project.h"
#include "cyapicallbacks.h"

#include "CanStack.h"
#include "ODrive.h"

void CORDIC_done_callback(void)
{
    
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    
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
        
        //uint32_t amr, acr;
        //CANSTACK_ODRIVE_FILTER_CMD(CAN_CMD_GET_TEMPERATURE, amr, acr);
        //canstack_mb_filter_t rx1_filter = {.amr = amr, .acr = acr};
        
        //CAN_STACK_ERROR_CHECK(canstack_configure_rx_filter(can_driver, 1u, &rx1_filter));
        
        pending = CORDIC_has_pending();
        if (pending)
        {
            if (CORDIC_get_data(&res))
            {
                
            }
        }
        
    }
}

/* [] END OF FILE */
