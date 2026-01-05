/***************************************************************************//**
* \file CanStackResult.h
* \version 1.0.0
*
*  \brief
*  Public header for the CanStack library provinding status codes
*
********************************************************************************
* \copyright
* Copyright 2025, Embedded Dynamics. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

// ========================================================
// Include guard
// ========================================================

#ifndef CAN_STACK_RESULT_H
#define CAN_STACK_RESULT_H

// ========================================================
// Includes
// ========================================================
 
/* Internal headers */
#include <stdint.h> // Standard integer types

#define CANSTACK_ERROR_CHECK(expr) do { \
    canstack_result_t _res = (expr);     \
    if (_res != CAN_RESULT_OK)           \
        return _res;                     \
} while(0)

// ========================================================
// CAN Status enums
// ========================================================

typedef enum {
    /**< Operation completed successfully */
    CAN_RESULT_OK           = 0x0000,
    
    CAN_RESULT_WARNING      = 0x1000,

    CAN_RESULT_ERROR        = 0x2000,
    CAN_RESULT_INVALID_ARG,
    CAN_RESULT_NULL_POINTER,
    CAN_RESULT_NO_MEMORY,
    CAN_RESULT_NOT_CONFIGURED,
    CAN_RESULT_NO_MAILBOX,
    CAN_RESULT_INVALID_CMD,
    CAN_RESULT_PAYLOAD_OVERFLOW,
    CAN_RESULT_INVALID_STATE,
    CAN_RESULT_NO_PLATFORM_DRIVER,

} canstack_result_t;


// ========================================================

#endif // !CAN_STACK_RESULT_H

// ========================================================

/* [] END OF FILE */