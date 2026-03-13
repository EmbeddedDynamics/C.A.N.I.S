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

//========================================================================================================================//
//                                                 Includes                                                               //                                                                 
//========================================================================================================================//

#include "project.h"
#include <stdbool.h>

//========================================================================================================================//
//                                                 Global Variabeles                                                      //                                                                 
//========================================================================================================================//

volatile uint32_t CH1_rising_edge_start = 0, CH2_rising_edge_start = 0, CH3_rising_edge_start = 0, CH4_rising_edge_start = 0;
volatile uint32_t CH1_Width = 0, CH2_Width = 0, CH3_Width = 0, CH4_Width = 0;


CY_ISR_PROTO(CH1_Handler);
CY_ISR_PROTO(CH2_Handler);
CY_ISR_PROTO(CH3_Handler);
CY_ISR_PROTO(CH4_Handler);

uint32_t getRadioPWM(int CH_Number);

int main(void)
{
    
    //=========================================================================================================================//
    //                                                 Setup                                                                   //                                                                 
    //======================================================================================================================== //
    
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    // Start all timers
    CH1_Timer_Start();
    CH2_Timer_Start();
    CH3_Timer_Start();
    CH4_Timer_Start();
    
    // Start all interrupt handlers with custom names
    isr_1_StartEx(CH1_Handler);
    isr_2_StartEx(CH2_Handler);
    isr_3_StartEx(CH3_Handler);
    isr_4_StartEx(CH4_Handler);
    

    //=========================================================================================================================//
    //                                                 Main Loop                                                               //                                                                 
    //======================================================================================================================== //
    
    for(;;)
    {
        int32_t ch1 = getRadioPWM(1);
        int32_t ch2 = getRadioPWM(2);
        int32_t ch3 = getRadioPWM(3);
        int32_t ch4 = getRadioPWM(4);
        
        CyDelay(10);  // Small delay
        if (ch3 > 1600) {
            LED_BUILDIN_Write(1);
        }
        else if (ch3 < 1400) {
            LED_BUILDIN_Write(1);
        }
        else {
            LED_BUILDIN_Write(0);
        }
    }
}

//=========================================================================================================================//
//                                                 getRadioPWM function                                                    //                                                                 
//======================================================================================================================== //

uint32_t getRadioPWM(int CH_Number) {
    uint32_t returnPWM = 0;
    if(CH_Number == 1) {
        returnPWM = CH1_Width;
    }
    else if(CH_Number == 2) {
        returnPWM = CH2_Width;
    }
    else if(CH_Number == 3) {
        returnPWM = CH3_Width;
    }
    else if(CH_Number == 4) {
        returnPWM = CH4_Width;
    }
    return returnPWM;
}

//========================================================================================================================//
//                                                 INTERRUPT HANDLERS                                                     //                                                                 
//========================================================================================================================//


CY_ISR(CH1_Handler) {
    int state = CH1_Pin_Read();
    uint32_t current_time = CH1_Timer_ReadCounter();
    
    if(state == 1) {
        // Rising edge
        CH1_rising_edge_start = current_time;
    } 
    else {
        // Falling edge - DOWN counting timer
        CH1_Width = CH1_rising_edge_start - current_time;
    }
    
    CH1_Pin_ClearInterrupt();
}

CY_ISR(CH2_Handler) {
    int state = CH2_Pin_Read();
    uint32_t current_time = CH2_Timer_ReadCounter();
    
    if(state == 1) {
        // Rising edge
        CH2_rising_edge_start = current_time;
    } 
    else {
        // Falling edge - DOWN counting timer
        CH2_Width = CH2_rising_edge_start - current_time;
    }
    
    CH2_Pin_ClearInterrupt();
}

CY_ISR(CH3_Handler) {
    int state = CH3_Pin_Read();
    uint32_t current_time = CH3_Timer_ReadCounter();
    
    if(state == 1) {
        // Rising edge
        CH3_rising_edge_start = current_time;
    } 
    else {
        // Falling edge - DOWN counting timer
        CH3_Width = CH3_rising_edge_start - current_time;
    }
    
    CH3_Pin_ClearInterrupt();
}

CY_ISR(CH4_Handler) {
    int state = CH4_Pin_Read();
    uint32_t current_time = CH4_Timer_ReadCounter();
    
    if(state == 1) {
        // Rising edge
        CH4_rising_edge_start = current_time;
    } 
    else {
        // Falling edge - DOWN counting timer
        CH4_Width = CH4_rising_edge_start - current_time;
    }
    
    CH4_Pin_ClearInterrupt();
}



/* [] END OF FILE */


