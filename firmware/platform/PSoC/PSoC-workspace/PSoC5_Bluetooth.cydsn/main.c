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
#include "project.h"
#include <stdint.h>
#include <string.h>

static uint8_t I2C_RX_Buffer[64];

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

} RobotControllerData;


static RobotControllerData Frame;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    
   
    I2C_SlaveInitWriteBuf(I2C_RX_Buffer, 64);
    I2C_Start(); 
    
     

    for(;;)
    {
        /* Place your application code here. */
        volatile uint8_t I2C_status = I2C_SlaveStatus();
        if(I2C_status & I2C_SSTAT_WR_CMPT) {
            I2C_SlaveClearWriteStatus();
            volatile uint8_t WrittenBytes = I2C_SlaveGetWriteBufSize();
            if(WrittenBytes == 16) {
               memcpy(&Frame, I2C_RX_Buffer, WrittenBytes);
            }
            
            I2C_SlaveClearWriteBuf(); 
        }
        
        
        if(Frame.rightY <= -450) {
            volatile uint8_t forward = 0x50;
        }
        else if (Frame.rightY >= 450) {
            volatile uint8_t reverse = 0x51;
        }
 
        CyDelay(50);

    }
}

/* [] END OF FILE */
