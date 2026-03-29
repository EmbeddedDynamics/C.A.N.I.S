/*******************************************************************************
* File Name: `$INSTANCE_NAME`.c
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file contains the setup, control and status commands for the RFMS_CORDIC
*  component.
*
* Note:
*
*******************************************************************************
* Copyright 2011-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

//========================================================
//      Standard Includes
//========================================================

#include "`$INSTANCE_NAME`.h"
#include "`$INSTANCE_NAME`_ISR.h"
#include "cyapicallbacks.h"

//========================================================
//      CORDIC Globals
//========================================================

static uint8_t init_var = 0u;

//========================================================
//      CORDIC Enable functions
//========================================================

void `$INSTANCE_NAME`_enable(void) `=ReentrantKeil($INSTANCE_NAME . "_enable")`
{
    uint8 interrupt_state = CyEnterCriticalSection();
  
    // Stage the enable bit high
    `$INSTANCE_NAME`_CONTROL_REG |= `$INSTANCE_NAME`_EN;
    
    `$INSTANCE_NAME`_COMMIT_CTRL;
    
    CyExitCriticalSection( interrupt_state );
}

void `$INSTANCE_NAME`_disable(void) `=ReentrantKeil($INSTANCE_NAME . "_disable")`
{
    uint8 interrupt_state = CyEnterCriticalSection();
  
    // Mask the control register with the intervese value of the enable bit mask
    `$INSTANCE_NAME`_CONTROL_REG &= ((uint8_t) ~`$INSTANCE_NAME`_EN);
    
    `$INSTANCE_NAME`_COMMIT_CTRL;
    
    CyExitCriticalSection( interrupt_state );
}

bool `$INSTANCE_NAME`_has_pending(void) `=ReentrantKeil($INSTANCE_NAME . "_has_pending")`
{
    return (`$INSTANCE_NAME`_STATUS_REG & `$INSTANCE_NAME`_STS_PENDING) == `$INSTANCE_NAME`_STS_PENDING;
}

//========================================================
//      CORDIC Enable functions
//========================================================

void `$INSTANCE_NAME`_init(void) `=ReentrantKeil($INSTANCE_NAME . "_init")`
{
    if (init_var)
        return;
    
    /* For all we know the interrupt is active. */
    `$INSTANCE_NAME`_ISR_Disable();

    /* Set the ISR to point to the CORDIC_ISR Interrupt. */
    `$INSTANCE_NAME`_ISR_SetVector(`$INSTANCE_NAME`_ISR_Handler);

    /* Set the priority. */
    `$INSTANCE_NAME`_ISR_SetPriority((uint8)`$INSTANCE_NAME`_ISR_INTC_PRIOR_NUMBER);
    
    init_var = 1u;
}

uint8_t `$INSTANCE_NAME`_start(void) `=ReentrantKeil($INSTANCE_NAME . "_start")`
{
    if (!init_var)
        `$INSTANCE_NAME`_init();
    
    uint16_t timeout = 1000u;
    
    /* Disable the CORDIC engine temporarly */
    `$INSTANCE_NAME`_disable();
    
    `$INSTANCE_NAME`_CONTROL_REG |= `$INSTANCE_NAME`_RST;
    
    `$INSTANCE_NAME`_COMMIT_CTRL;
    
    while((timeout--) && (timeout != 0u) && !(`$INSTANCE_NAME`_CONTROL_REG & `$INSTANCE_NAME`_RST))
    {
        CyDelayUs(1);
    }
    if (timeout == 0u) return CYRET_TIMEOUT;
    
    timeout = 1000u;
    `$INSTANCE_NAME`_CONTROL_REG &= ((uint8_t) ~`$INSTANCE_NAME`_RST);
    
    `$INSTANCE_NAME`_COMMIT_CTRL;
    
    while((timeout--) && (timeout != 0u) && (`$INSTANCE_NAME`_CONTROL_REG & `$INSTANCE_NAME`_RST))
    {
        CyDelayUs(1);
    }
    if (timeout == 0u) return CYRET_TIMEOUT;
    
    `$INSTANCE_NAME`_enable();
    
    return CYRET_SUCCESS;
}

uint8_t `$INSTANCE_NAME`_stop(void) `=ReentrantKeil($INSTANCE_NAME . "_stop")`
{
    uint16_t timeout = 1000u;
    
    `$INSTANCE_NAME`_disable();
    
    `$INSTANCE_NAME`_CONTROL_REG |= `$INSTANCE_NAME`_RST;
    
    `$INSTANCE_NAME`_COMMIT_CTRL;
    
    while((timeout--) && !(`$INSTANCE_NAME`_STATUS_REG & `$INSTANCE_NAME`_RST))
    {
        CyDelayUs(1);
    }
    if (timeout == 0u) return CYRET_TIMEOUT;
    
    timeout = 1000u;
    `$INSTANCE_NAME`_CONTROL_REG &= ((uint8_t) ~`$INSTANCE_NAME`_RST);
    
    `$INSTANCE_NAME`_COMMIT_CTRL;
    
    while((timeout--) && (`$INSTANCE_NAME`_STATUS_REG & `$INSTANCE_NAME`_RST))
    {
        CyDelayUs(1);
    }
    if (timeout == 0u) return CYRET_TIMEOUT;
    
    return CYRET_SUCCESS;
}

uint8_t `$INSTANCE_NAME`_get_data(`$INSTANCE_NAME`_vector_t* result) `=ReentrantKeil($INSTANCE_NAME . "_get_data")`
{
    if (!result)
        return CYRET_BAD_PARAM;
    
    if (!`$INSTANCE_NAME`_has_pending())
        return CYRET_CANCELED;
    
    result->x = `$INSTANCE_NAME`_X_ENGINE_F1_REG;
    result->y = `$INSTANCE_NAME`_Y_ENGINE_F1_REG;
    result->z = `$INSTANCE_NAME`_Z_ENGINE_F1_REG;
    
    return CYRET_SUCCESS;
}

uint8_t `$INSTANCE_NAME`_queue_data(const `$INSTANCE_NAME`_vector_t* vector) `=ReentrantKeil($INSTANCE_NAME . "_queue_data")`
{
    if (!vector)
        return CYRET_BAD_PARAM;

    uint8 interrupt_state = CyEnterCriticalSection();
    
    CY_SET_REG16(`$INSTANCE_NAME`_X_ENGINE_F0_PTR, (int16_t) vector->x); //(int16_t) 0xF000);
    CY_SET_REG16(`$INSTANCE_NAME`_Y_ENGINE_F0_PTR, (int16_t) vector->y);
    CY_SET_REG16(`$INSTANCE_NAME`_Z_ENGINE_F0_PTR, (int16_t) vector->z);
    
    CyExitCriticalSection( interrupt_state );
    
    return CYRET_SUCCESS;
}

void  `$INSTANCE_NAME`_enable_interrupt(void) `=ReentrantKeil($INSTANCE_NAME . "_EnableInterrupt")`
{
    uint8_t interruptState;
    interruptState = CyEnterCriticalSection();
    `$INSTANCE_NAME`_INT_STATUS_AUX_CTRL |= `$INSTANCE_NAME`_INTR_ENBL;
    CyExitCriticalSection(interruptState);
    `$INSTANCE_NAME`_ISR_Enable();
}

void  `$INSTANCE_NAME`_disable_interrupt(void) `=ReentrantKeil($INSTANCE_NAME . "_DisableInterrupt")`
{
    uint8_t interruptState;
    interruptState = CyEnterCriticalSection();
    `$INSTANCE_NAME`_INT_STATUS_AUX_CTRL &= (uint8_t)(~`$INSTANCE_NAME`_INTR_ENBL);
    CyExitCriticalSection(interruptState);
    `$INSTANCE_NAME`_ISR_Disable();
}

CY_ISR(`$INSTANCE_NAME`_ISR_Handler)
{   
    uint8_t int_status = `$INSTANCE_NAME`_INT_STATUS_REG;
    
    if (int_status & `$INSTANCE_NAME`_STS_DONE)
    {
        #if defined(`$INSTANCE_NAME`_DONE_CALLBACK)
            
        `$INSTANCE_NAME`_done_callback();
        
        `$INSTANCE_NAME`_CONTROL_REG ^= `$INSTANCE_NAME`_CLR_DONE;
    
        `$INSTANCE_NAME`_COMMIT_CTRL;
        
        #endif
    }
}

/* [] END OF FILE */
