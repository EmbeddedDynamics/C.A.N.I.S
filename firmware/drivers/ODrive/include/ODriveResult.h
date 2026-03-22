/***************************************************************************//**
* \file ODriveResult.h
* \version 1.0.0
*
* \brief
* ODrive operation result codes and status enumeration.
*
* \details
* Provides standardized result codes for all ODrive operations. Success is
* indicated by ODRIVE_RESULT_OK. Error codes are organized by category
* (operation, communication, parameter, device) for easier debugging and
* error handling.
*
********************************************************************************
* \copyright
* Copyright 2025, Embedded Dynamics. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef ODRIVE_RESULT_H
#define ODRIVE_RESULT_H

//========================================================
//      Standard Includes
//========================================================

#include <stdint.h>

//========================================================
//      Result Code Enumeration
//========================================================

/**
 * @brief ODrive operation result codes
 *
 * Result code layout:
 *
 *  0x0000                : Success
 *  0x2000 - 0x2FFF       : Operation-level errors
 *  0x3000 - 0x3FFF       : Communication errors
 *  0x4000 - 0x4FFF       : Parameter / validation errors
 *  0x5000 - 0x5FFF       : ODrive device errors
 *  0x6000 - 0x6FFF       : Warnings (non-fatal)
 *
 * Category can be extracted using ODRIVE_RESULT_CATEGORY_MASK.
 */
typedef enum
{
    //========================================================
    //      Result masks & bases
    //========================================================

    ODRIVE_RESULT_CATEGORY_MASK      = 0xF000,

    ODRIVE_RESULT_SUCCESS_BASE       = 0x0000,
    ODRIVE_RESULT_OPERATION_BASE     = 0x2000,
    ODRIVE_RESULT_COMM_BASE          = 0x3000,
    ODRIVE_RESULT_PARAM_BASE         = 0x4000,
    ODRIVE_RESULT_DEVICE_BASE        = 0x5000,
    ODRIVE_RESULT_WARNING_BASE       = 0x6000,

    //========================================================
    //      Success
    //========================================================

    /**
     * @brief Operation succeeded
     */
    ODRIVE_RESULT_OK                 = ODRIVE_RESULT_SUCCESS_BASE,

    //========================================================
    //      Operation-level errors (0x2000)
    //========================================================

    ODRIVE_ERROR_GENERAL             = ODRIVE_RESULT_OPERATION_BASE + 0x000,
    ODRIVE_ERROR_TIMEOUT             = ODRIVE_RESULT_OPERATION_BASE + 0x001,
    ODRIVE_ERROR_NOT_INITIALIZED     = ODRIVE_RESULT_OPERATION_BASE + 0x002,
    ODRIVE_ERROR_OPERATION_FAILED    = ODRIVE_RESULT_OPERATION_BASE + 0x003,
    ODRIVE_ERROR_UNSUPPORTED         = ODRIVE_RESULT_OPERATION_BASE + 0x004,
  
    /**
     * @brief No available axis slot
     */
    ODRIVE_ERROR_NO_AXIS_SLOT        = ODRIVE_RESULT_OPERATION_BASE + 0x005,

    /**
     * @brief The ODrive driver is in a invalid state
     */
    ODRIVE_ERROR_INVALID_STATE       = ODRIVE_RESULT_OPERATION_BASE + 0x006,

    //========================================================
    //      Communication errors (0x3000)
    //========================================================

    ODRIVE_ERROR_INVALID_CAN_ID       = ODRIVE_RESULT_COMM_BASE + 0x000,
    ODRIVE_ERROR_CAN_TX_FAILED        = ODRIVE_RESULT_COMM_BASE + 0x001,
    ODRIVE_ERROR_CAN_RX_FAILED        = ODRIVE_RESULT_COMM_BASE + 0x002,
    ODRIVE_ERROR_CAN_NO_RESPONSE      = ODRIVE_RESULT_COMM_BASE + 0x003,
    ODRIVE_ERROR_INVALID_RESPONSE    = ODRIVE_RESULT_COMM_BASE + 0x004,
    ODRIVE_ERROR_CHECKSUM_MISMATCH   = ODRIVE_RESULT_COMM_BASE + 0x005,
    ODRIVE_ERROR_CAN_BUS_OFF          = ODRIVE_RESULT_COMM_BASE + 0x006,
    ODRIVE_ERROR_COMM_INIT_FAILED    = ODRIVE_RESULT_COMM_BASE + 0x007,
    ODRIVE_ERROR_INVALID_BACKEND     = ODRIVE_RESULT_COMM_BASE + 0x008,

    //========================================================
    //      Parameter / validation errors (0x4000)
    //========================================================

    ODRIVE_ERROR_INVALID_PARAM       = ODRIVE_RESULT_PARAM_BASE + 0x000,
    ODRIVE_ERROR_NULL_POINTER        = ODRIVE_RESULT_PARAM_BASE + 0x001,
    ODRIVE_ERROR_BUFFER_TOO_SMALL    = ODRIVE_RESULT_PARAM_BASE + 0x002,
    ODRIVE_ERROR_OUT_OF_RANGE        = ODRIVE_RESULT_PARAM_BASE + 0x003,
    ODRIVE_ERROR_INVALID_MODE        = ODRIVE_RESULT_PARAM_BASE + 0x004,
    ODRIVE_ERROR_NO_MEMORY           = ODRIVE_RESULT_PARAM_BASE + 0x005,

    //========================================================
    //      ODrive device errors (0x5000)
    //========================================================

    ODRIVE_ERROR_DEVICE_ERROR        = ODRIVE_RESULT_DEVICE_BASE + 0x000,
    ODRIVE_ERROR_MOTOR_DISARMED      = ODRIVE_RESULT_DEVICE_BASE + 0x001,
    ODRIVE_ERROR_CALIBRATION_REQUIRED= ODRIVE_RESULT_DEVICE_BASE + 0x002,
    ODRIVE_ERROR_ESTOP_ACTIVE        = ODRIVE_RESULT_DEVICE_BASE + 0x003,

    //========================================================
    //      Warnings (0x6000) – non-fatal
    //========================================================

    ODRIVE_WARNING_GENERAL           = ODRIVE_RESULT_WARNING_BASE + 0x000,
    ODRIVE_WARNING_NO_RX_MESSAGES    = ODRIVE_RESULT_WARNING_BASE + 0x001,

} odrive_result_t;

//========================================================
//      Helper Macro's
//========================================================


/**
 * @brief Check if result indicates success
 * 
 * @param result - ODriveResult value
 * @return true if operation succeeded, false otherwise
 */
#define ODRIVE_RESULT_IS_OK(result) \
    ((result) == ODRIVE_RESULT_OK)

/**
 * @brief Check if result is an operation-level error
 * 
 * @param result - ODriveResult value
 * @return true if error is in operation category, false otherwise
 */
#define ODRIVE_RESULT_IS_OPERATION_ERROR(result) \
    (((result) & 0xF000) == 0x2000)

/**
 * @brief Check if result is a communication error
 * 
 * @param result - ODriveResult value
 * @return true if error is in communication category, false otherwise
 */
#define ODRIVE_RESULT_IS_COMM_ERROR(result) \
    (((result) & 0xF000) == 0x3000)

/**
 * @brief Check if result is a parameter/validation error
 * 
 * @param result - ODriveResult value
 * @return true if error is in parameter category, false otherwise
 */
#define ODRIVE_RESULT_IS_PARAM_ERROR(result) \
    (((result) & 0xF000) == 0x4000)

/**
 * @brief Check if result is a device error
 * 
 * @param result - ODriveResult value
 * @return true if error is in device category, false otherwise
 */
#define ODRIVE_RESULT_IS_DEVICE_ERROR(result) \
    (((result) & 0xF000) == 0x5000)

//========================================================
//      Error Checking Macro
//========================================================

/**
 * @brief Check result and return early on error
 * 
 * @param expr - Expression returning ODriveResult
 * @return ODriveResult from expression, or early return on error
 * 
 * @example
 *   ODriveResult result = ODRIVE_CHECK(odrive_init(&driver));
 *   // If odrive_init fails, returns immediately with error code
 */
#define ODRIVE_CHECK(expr) do { \
    odrive_result_t _result = (expr); \
    if (_result != ODRIVE_RESULT_OK) { \
        return _result; \
    } \
} while (0)

//========================================================
//      End of File
//========================================================

#endif /* !ODRIVE_RESULT_H */