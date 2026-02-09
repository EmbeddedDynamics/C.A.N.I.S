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

//========================================================================================================================//
//                                                 Global Variabeles                                                      //                                                                 
//========================================================================================================================//

volatile uint32_t CH1_rising_edge_start = 0, CH2_rising_edge_start = 0, CH3_rising_edge_start = 0, CH4_rising_edge_start = 0;
volatile uint32_t CH1_Width = 0, CH2_Width = 0, CH3_Width = 0, CH4_Width = 0;

//=========================================================================================================================//
//                                                 Function Prototypes                                                     //                                                                 
//======================================================================================================================== //

uint32_t getRadioPWM(int CH_number);

//========================================================================================================================//
//                                                 INTERRUPT HANDLERS                                                     //                                                                 
//========================================================================================================================//

CY_ISR(isr_1_Handler) {
    uint32_t state = CH1_Pin_Read();
    if(state == 1) {
        CH1_rising_edge_start = CH1_Timer_ReadCapture();
    }
    else {
        CH1_Width = CH1_Timer_ReadCapture() - CH1_rising_edge_start;
        CH1_Width = CH1_Width * 1000/24; // 24 MHz clock to microseconds
    }
}

CY_ISR(isr_2_Handler) {
    uint32_t state = CH2_Pin_Read();
    if(state == 1) {
        CH2_rising_edge_start = CH2_Timer_ReadCapture();
    }
    else {
        CH2_Width = CH2_Timer_ReadCapture() - CH2_rising_edge_start;
        CH2_Width = CH2_Width * 1000/2; // 24 MHz clock to microseconds
    }
}

CY_ISR(isr_3_Handler) {
    uint32_t state = CH3_Pin_Read();
    if(state == 1) {
        CH3_rising_edge_start = CH3_Timer_ReadCapture();
    }
    else {
        CH3_Width = CH3_Timer_ReadCapture() - CH3_rising_edge_start;
        CH3_Width = CH3_Width * 1000/24; // 24 MHz clock to microseconds
    }
}

CY_ISR(isr_4_Handler) {
    uint32_t state = CH4_Pin_Read();
    if(state == 1) {
        CH4_rising_edge_start = CH4_Timer_ReadCapture();
    }
    else {
        CH4_Width = CH4_Timer_ReadCapture() - CH4_rising_edge_start;
        CH4_Width = CH4_Width * 1000/24; // 24 MHz clock to microseconds
    }
}

//=========================================================================================================================//
//                                                 For loop                                                                //                                                                 
//======================================================================================================================== //

int main(void)
{
    
    //=========================================================================================================================//
    //                                                 Setup                                                                   //                                                                 
    //======================================================================================================================== //
    
    
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    CH1_Timer_Start();
    isr_1_StartEx(isr_1_Handler);
    
    CH2_Timer_Start();
    isr_2_StartEx(isr_2_Handler);
    
    CH3_Timer_Start();
    isr_3_StartEx(isr_3_Handler);
    
    CH4_Timer_Start();
    isr_4_StartEx(isr_4_Handler);

    //=========================================================================================================================//
    //                                                 Main Loop                                                               //                                                                 
    //======================================================================================================================== //
    
    for(;;)
    {
        /* Place your application code here. */
        
        uint32_t CH1 = getRadioPWM(1);
        uint32_t CH2 = getRadioPWM(2);
        uint32_t CH3 = getRadioPWM(3);
        uint32_t CH4 = getRadioPWM(4);
    }
}


uint32_t getRadioPWM (int CH_Number) {
    if(CH_Number == 1) {
        return CH1_Width;
    }
    else if (CH_Number == 2) {
        return CH2_Width;
    }
    else if (CH_Number == 3) {
        return CH3_Width;
    }
    else if (CH_Number == 4) {
        return CH4_Width;
    }
}


/* [] END OF FILE */


