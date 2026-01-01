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

//========================================================
//      CORDIC Enable functions
//========================================================

void `$INSTANCE_NAME`_enable(void) `=ReentrantKeil($INSTANCE_NAME . "_Enable")`
{
    `$INSTANCE_NAME`_CONTROL_REG |= `$INSTANCE_NAME`_EN;
}

void `$INSTANCE_NAME`_disable(void) `=ReentrantKeil($INSTANCE_NAME . "_Enable")`
{
    // Mask the control register with the intervese value of the enable bit mask
    `$INSTANCE_NAME`_CONTROL_REG &= ((uint8_t) ~`$INSTANCE_NAME`_EN);
}

//========================================================
//      CORDIC Enable functions
//========================================================

uint8_t `$INSTANCE_NAME`_start(void) `=ReentrantKeil($INSTANCE_NAME . "_start")`
{
    uint16_t timeout = 1000u;
    
    /* Disable the CORDIC engine temporarly */
    `$INSTANCE_NAME`_disable();
    
    `$INSTANCE_NAME`_CONTROL_REG |= `$INSTANCE_NAME`_RST;
    while((timeout--) && !(`$INSTANCE_NAME`_STATUS_REG & `$INSTANCE_NAME`_RST))
    {
        CY_NOP;
    }
    if (timeout == 0u) return CYRET_TIMEOUT;
    
    timeout = 1000u;
    `$INSTANCE_NAME`_CONTROL_REG &= ((uint8_t) ~`$INSTANCE_NAME`_RST);
    while((timeout--) && (`$INSTANCE_NAME`_STATUS_REG & `$INSTANCE_NAME`_RST))
    {
        CY_NOP;
    }
    if (timeout == 0u) return CYRET_TIMEOUT;
    
    `$INSTANCE_NAME`_enable();
    
    return CYRET_SUCCESS;
}

uint8_t `$INSTANCE_NAME`_stop(void) `=ReentrantKeil($INSTANCE_NAME . "_Stop")`
{
    uint16_t timeout = 1000u;
    
    `$INSTANCE_NAME`_disable();
    
    `$INSTANCE_NAME`_CONTROL_REG |= `$INSTANCE_NAME`_RST;
    while((timeout--) && !(`$INSTANCE_NAME`_STATUS_REG & `$INSTANCE_NAME`_RST))
    {
        CY_NOP;
    }
    if (timeout == 0u) return CYRET_TIMEOUT;
    
    timeout = 1000u;
    `$INSTANCE_NAME`_CONTROL_REG &= ((uint8_t) ~`$INSTANCE_NAME`_RST);
    while((timeout--) && (`$INSTANCE_NAME`_STATUS_REG & `$INSTANCE_NAME`_RST))
    {
        CY_NOP;
    }
    if (timeout == 0u) return CYRET_TIMEOUT;
    
    return CYRET_SUCCESS;
}

uint8_t `$INSTANCE_NAME`_get_data(`$INSTANCE_NAME`_result_t* result) `=ReentrantKeil($INSTANCE_NAME . "_GetData")`
{
    if (!result)
        return CYRET_BAD_PARAM;
        
    return CYRET_SUCCESS;
}

void  `$INSTANCE_NAME`_enable_interrupt(void) `=ReentrantKeil($INSTANCE_NAME . "_EnableInterrupt")`
{
    uint8_t interruptState;
    interruptState = CyEnterCriticalSection();
    `$INSTANCE_NAME`_INT_STATUS_AUX_CTRL |= `$INSTANCE_NAME`_INTR_ENBL;
    CyExitCriticalSection(interruptState);
}

void  `$INSTANCE_NAME`_disable_interrupt(void) `=ReentrantKeil($INSTANCE_NAME . "_DisableInterrupt")`
{
    uint8_t interruptState;
    interruptState = CyEnterCriticalSection();
    `$INSTANCE_NAME`_INT_STATUS_AUX_CTRL &= (uint8_t)(~`$INSTANCE_NAME`_INTR_ENBL);
    CyExitCriticalSection(interruptState);
}

/* [] END OF FILE */
