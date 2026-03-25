/* ========================================
 * main.c
 * Simple implementation - Arduino style
 * ======================================== */

//========================================================================================================================//
//                                                 Includes                                                               //                                                                 
//========================================================================================================================//

#include "project.h"
#include "MPU6050_ComplentaryFilter.h"
#include <math.h>

//========================================================================================================================//
//                                                 Global Variabeles                                                      //                                                                 
//========================================================================================================================//

volatile uint8 LoopFlag = 0;
float LoopTimer;

// Timer ISR
CY_ISR(Timer_ISR) {
    LoopFlag = 1;
    Timer_ReadStatusRegister();
}

int main(void) {
    
    //=========================================================================================================================//
    //                                                 Setup                                                                   //                                                                 
    //======================================================================================================================== //
    
    CyGlobalIntEnable;
    
    CyDelay(2000);
    
    Init_Gyro();
    Gyro_Calibration();
    
    // Start Timer for 250 Hz loop (4ms = 4000 with 1MHz clock)
    Timer_Start();
    isr_Timer_StartEx(Timer_ISR);
    
    //=========================================================================================================================//
    //                                                 Main Loop                                                               //                                                                 
    //======================================================================================================================== //
    
    for(;;) {
        
        // If loop flag = 1 than 4 ms has passed and the ISR fires and the main loops runs
        if(LoopFlag) {
            LoopFlag = 0;
            
            Read_Angle();
            
            
            if (Gyro_Angle_Pitch >= 45) {
                LED_Write(1);
            }
            else if (Gyro_Angle_Pitch <= -45) {
                LED_Write(1);
            }
            else if (Gyro_Angle_Roll >= 45) {
                LED_Write(1);
            }
            else if (Gyro_Angle_Roll <= -45) {
                LED_Write(1);
            }
            
            else
                LED_Write(0);
            // Use angles here (Angle_Roll, Angle_Pitch, Angle_Yaw)
            // Example: Send to PWM, display, control loop, etc.
        }
    }
}