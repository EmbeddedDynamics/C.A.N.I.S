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

#define CAN_RECEIVE_MSG_0_CALLBACK
#define CAN_RECEIVE_MSG_1_CALLBACK

#include <stdint.h>

#include "project.h"

#include "CanStack.h"
#include "ODrive.h"

CY_ISR_PROTO(CORDIC_ISR);

// Our callback function
void CAN_ReceiveMsg_0_Callback(void)
{

}


CY_ISR(CORDIC_ISR)
{   
	
}

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    CORDIC_enable();
    
    uint8_t cState = CORDIC_STATUS_REG;

    for(;;)
    {
        /* Place your application code here. */
        canstack_config_t can_cfg;
        
        canstack_driver_t can_driver;
        canstack_create_driver(&can_driver, &can_cfg);
        
        uint32_t amr, acr;
        CANSTACK_ODRIVE_FILTER_CMD(CAN_CMD_GET_TEMPERATURE, amr, acr);
        canstack_mb_filter_t rx1_filter = {.amr = amr, .acr = acr};
        
        CAN_STACK_ERROR_CHECK(canstack_configure_rx_filter(can_driver, 1u, &rx1_filter));
    }
}

/* [] END OF FILE */
