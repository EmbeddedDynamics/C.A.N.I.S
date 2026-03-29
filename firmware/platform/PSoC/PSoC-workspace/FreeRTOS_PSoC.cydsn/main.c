//Project Title: Express ELRS PWM Receiver with FreeRTOS
//Author: Daan Smit
//Date: 25-3-2026
//Version: 2

 /* Description:
 * PWM receiver using FreeRTOS for task management
 * 
 * Hardware:
 *   - PSoC5
 *   - ER5A
 *   - Radiomaster pocket
 *
 * Software/Libraries:
 *  - stdbool.h
 *  - FreeRTOS
 *
 * Credits/References:
 *
 * License:
 *   - non
 */


//========================================================================================================================//
//                                                 Includes                                                               //                                                                 
//========================================================================================================================//

#include "project.h"
#include <stdbool.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#define I2C_SLAVE_ADDR      0x20  // FIX: Match ESP32's Wire.begin(0x20)
#define CONTROLLER_DATA_SIZE sizeof(controller_data_t)

static uint8_t i2c_rx_buffer[16u];

extern void RTOS_Start(void);

//========================================================================================================================//
//                                                 Global Variables                                                       //                                                                 
//========================================================================================================================//

volatile uint32_t CH1_rising_edge_start = 0, CH2_rising_edge_start = 0, CH3_rising_edge_start = 0, CH4_rising_edge_start = 0;
volatile uint32_t CH1_Width = 0, CH2_Width = 0, CH3_Width = 0, CH4_Width = 0;

CY_ISR_PROTO(CH1_Handler);
CY_ISR_PROTO(CH2_Handler);
CY_ISR_PROTO(CH3_Handler);
CY_ISR_PROTO(CH4_Handler);

uint32_t getRadioPWM(int CH_Number);


typedef struct __attribute__((packed)) {
    int16_t  leftX;
    int16_t  leftY;
    int16_t  rightX;
    int16_t  rightY;
    int16_t  brake;
    int16_t  throttle;
    uint16_t buttons;
    uint8_t  dpad;
    uint8_t  misc;
} controller_data_t;


static controller_data_t frame;

//========================================================================================================================//
//                                                 Function Prototypes                                                    //                                                                 
//========================================================================================================================//

void PWM_Read_Task(void *arg);
void PWM_Read_Task(void *arg);
void Controller_Read_Task(void *arg); 

//========================================================================================================================//
//                                                 MAIN                                                                   //                                                                 
//========================================================================================================================//

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    
    // Start all timers (do this BEFORE starting RTOS)
    CH1_Timer_Start();
    CH2_Timer_Start();
    CH3_Timer_Start();
    CH4_Timer_Start();
    
    // Start all interrupt handlers
    isr_1_StartEx(CH1_Handler);
    isr_2_StartEx(CH2_Handler);
    isr_3_StartEx(CH3_Handler);
    isr_4_StartEx(CH4_Handler);
    
    I2C_1_Start();

    
    // Initialize FreeRTOS
    RTOS_Start();
    
    
    // Create tasks
    xTaskCreate(PWM_Read_Task, "PWM Read", configMINIMAL_STACK_SIZE, 0, 2, 0);
    xTaskCreate(Controller_Read_Task, "Controller Read", configMINIMAL_STACK_SIZE, 0, 2, 0);
    
    // Start scheduler
    vTaskStartScheduler();
    
    // Should never reach here
    for(;;)
    {
        // If scheduler fails, loop here
    }
}

//========================================================================================================================//
//                                                 RTOS TASKS                                                             //                                                                 
//========================================================================================================================//

void PWM_Read_Task(void *arg)
{
    (void)arg;
    
    while(1)
    {
        // Read all channels
        uint32_t ch1 = getRadioPWM(1);
        uint32_t ch2 = getRadioPWM(2);
        uint32_t ch3 = getRadioPWM(3);
        uint32_t ch4 = getRadioPWM(4);
        
        // You can add processing here, like filtering or scaling
        // Or store to global variables for other tasks to use
        if (ch2 > 1600) {
            //LED_BUILDIN_Write(1);
        }
        else if (ch2 < 1400) {
            //LED_BUILDIN_Write(1);
        }
        else {
            //LED_BUILDIN_Write(0);
        }
        
        vTaskDelay(pdMS_TO_TICKS(10));  // 10ms delay
    }
}


void Controller_Read_Task(void *arg)
{
    (void)arg;
    const TickType_t delay = pdMS_TO_TICKS(20); // 50 Hz

    while(1)
    {
        // Wait until I2C master is free (with timeout)
        uint32_t timeout = 100;
        while (I2C_1_MasterStatus() & I2C_1_MSTAT_XFER_INP) {
            vTaskDelay(pdMS_TO_TICKS(1));
            if (--timeout == 0) break;
        }

        // Start read transaction
        if (I2C_1_MasterReadBuf(I2C_SLAVE_ADDR, i2c_rx_buffer, CONTROLLER_DATA_SIZE,
                                I2C_1_MODE_COMPLETE_XFER) == I2C_1_MSTR_NO_ERROR)
        {
            // Wait for transfer to complete (with timeout)
            timeout = 100;
            while (!(I2C_1_MasterStatus() & I2C_1_MSTAT_RD_CMPLT)) {
                vTaskDelay(pdMS_TO_TICKS(1));
                if (--timeout == 0) break;
            }

            // Only copy if transfer actually completed
            if (I2C_1_MasterStatus() & I2C_1_MSTAT_RD_CMPLT) {
                memcpy(&frame, i2c_rx_buffer, CONTROLLER_DATA_SIZE);
            }

            // Clear status for next transfer
            I2C_1_MasterClearStatus();
        }

        // LED test: right joystick forward
        if (frame.rightY < -400) {
            LED_BUILDIN_Write(1);
        } else {
            LED_BUILDIN_Write(0);
        }

        vTaskDelay(delay);
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
        CH1_rising_edge_start = current_time;
    } 
    else {
        CH1_Width = CH1_rising_edge_start - current_time;
    }
    
    CH1_Pin_ClearInterrupt();
}

CY_ISR(CH2_Handler) {
    int state = CH2_Pin_Read();
    uint32_t current_time = CH2_Timer_ReadCounter();
    
    if(state == 1) {
        CH2_rising_edge_start = current_time;
    } 
    else {
        CH2_Width = CH2_rising_edge_start - current_time;
    }
    
    CH2_Pin_ClearInterrupt();
}

CY_ISR(CH3_Handler) {
    int state = CH3_Pin_Read();
    uint32_t current_time = CH3_Timer_ReadCounter();
    
    if(state == 1) {
        CH3_rising_edge_start = current_time;
    } 
    else {
        CH3_Width = CH3_rising_edge_start - current_time;
    }
    
    CH3_Pin_ClearInterrupt();
}

CY_ISR(CH4_Handler) {
    int state = CH4_Pin_Read();
    uint32_t current_time = CH4_Timer_ReadCounter();
    
    if(state == 1) {
        CH4_rising_edge_start = current_time;
    } 
    else {
        CH4_Width = CH4_rising_edge_start - current_time;
    }
    
    CH4_Pin_ClearInterrupt();
}

/* [] END OF FILE */