//Project Title: Express ELRS PWM Receiver with FreeRTOS
//Author: Perijn Huijser, Ruben van der Veen, Daan Smit
//Date: 29-3-2026
//Version: 1

 /* Description:
 * Code for robotdog
 * 
 * Hardware:
 *   - PSoC5
 *   - ER5A
 *   - Radiomaster pocket
 *   - XBOX controller
 *
 * Software/Libraries:
 *  - CabStack.h
 *  - ODrive.h
 *  - ik.h
 *  - accelerators/ik_cordic.h
 *  - models/ik_4bar_diamond.h
 *  - driver/CanStack/ODriveCanStack.h
 *
 * Credits/References:
 *
 * License:
 *   - non
 */

/* [] END OF FILE */


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



int main(void)
{
    
    for(;;)
    {
        
    }
    
}
