/***************************************************************************//**
* \file ODriveResult.h
* \version 1.0.0
*
*  \brief
*  Public header for the ODrive results.
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

#ifndef ODRIVE_RESULT_H
#define ODRIVE_RESULT_H

// ========================================================
// Public defines
// ========================================================
 
/* Internal headers */
#include <stdint.h>
    

// ========================================================
// Public Enums
// ========================================================

/**
 * @brief ODrive operation result codes
 *
 * These result codes provide insight into the status of each operation.
 * Success is indicated by ODRIVE_RESULT_OK (0x0000).
 * Error codes are organized by category for easier debugging.
 *
 * @note Error codes in range 0x2000-0x2FFF indicate operation-level errors
 * @note Error codes in range 0x3000-0x3FFF indicate communication errors
 * @note Error codes in range 0x4000-0x4FFF indicate parameter/validation errors
 */
typedef enum {
    /**< Operation completed successfully */
    ODRIVE_RESULT_OK                    = 0x0000,
    
    
    // General operation errors (0x2000-0x2FFF)
    /**< Unspecified general error */
    ODRIVE_GENERAL_ERROR                = 0x2000,
    
    /**< Operation timed out */
    ODRIVE_ERROR_TIMEOUT                = 0x2001,
    
    /**< ODrive not initialized */
    ODRIVE_ERROR_NOT_INITIALIZED        = 0x2002,
    
    /**< ODrive busy with another operation */
    ODRIVE_ERROR_BUSY                   = 0x2003,
    
    /**< Operation failed to complete */
    ODRIVE_ERROR_OPERATION_FAILED       = 0x2004,
    
    /**< Operation not supported */
    ODRIVE_ERROR_UNSUPPORTED            = 0x2005,
    
    /**< ODrive in incorrect state for operation */
    ODRIVE_ERROR_WRONG_STATE            = 0x2006,
    
    
    // Communication errors (0x3000-0x3FFF)
    /**< CAN ID out of valid range (0-0x3F) */
    ODRIVE_ERROR_INVALID_CAN_ID         = 0x3000,
    
    /**< CAN transmission failed */
    ODRIVE_ERROR_CAN_TX_FAILED          = 0x3001,
    
    /**< CAN reception failed */
    ODRIVE_ERROR_CAN_RX_FAILED          = 0x3002,
    
    /**< No response received from ODrive */
    ODRIVE_ERROR_NO_RESPONSE            = 0x3003,
    
    /**< Response format invalid or corrupted */
    ODRIVE_ERROR_INVALID_RESPONSE       = 0x3004,
    
    /**< Data checksum verification failed */
    ODRIVE_ERROR_CHECKSUM_MISMATCH      = 0x3005,
    
    /**< CAN bus entered bus-off state */
    ODRIVE_ERROR_BUS_OFF                = 0x3006,
    
    /**< CAN bus initialization failed */
    ODRIVE_ERROR_CAN_INIT_FAILED        = 0x3007,
    
    
    
    // Parameter/validation errors (0x4000-0x4FFF)
    
    /**< Invalid parameter value provided */
    ODRIVE_ERROR_INVALID_PARAMETER      = 0x4000,
    
    /**< Null pointer passed to function */
    ODRIVE_ERROR_NULL_POINTER           = 0x4001,
    
    /**< Buffer size insufficient */
    ODRIVE_ERROR_BUFFER_OVERFLOW        = 0x4002,
    
    /**< Value outside acceptable range */
    ODRIVE_ERROR_OUT_OF_RANGE           = 0x4003,
    
    /**< Axis number invalid */
    ODRIVE_ERROR_INVALID_AXIS           = 0x4004,
    
    /**< Control or input mode invalid */
    ODRIVE_ERROR_INVALID_MODE           = 0x4005,
    
    /**< Memory allocation failed */
    ODRIVE_ERROR_OUT_OF_MEMORY          = 0x4006,
    
    // ODrive device errors (0x5000-0x5FFF)
    
    /**< ODrive reported internal error */
    ODRIVE_ERROR_DEVICE_ERROR           = 0x5000,
    
    /**< Motor is disarmed */
    ODRIVE_ERROR_MOTOR_DISARMED         = 0x5001,
    
    /**< Calibration must be performed first */
    ODRIVE_ERROR_CALIBRATION_REQUIRED   = 0x5002,
    
    /**< Emergency stop is active */
    ODRIVE_ERROR_ESTOP_ACTIVE           = 0x5003,
} ODriveResult;


// ========================================================

#endif // !ODRIVE_H

// ========================================================

/* [] END OF FILE */