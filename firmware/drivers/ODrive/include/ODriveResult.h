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
 * @details
 * Standard result type for all ODrive operations. Success indicated by
 * ODRIVE_RESULT_OK (0x0000). Error codes organized by category:
 * 
 * - 0x0000: Success
 * - 0x2000-0x2FFF: Operation-level errors
 * - 0x3000-0x3FFF: Communication errors
 * - 0x4000-0x4FFF: Parameter/validation errors
 * - 0x5000-0x5FFF: ODrive device errors
 * 
 * Use bitwise AND with category mask to identify error type:
 * @code
 *   if (result & 0x2000) { // Operation error }
 *   if (result & 0x3000) { // Communication error }
 * @endcode
 */
typedef enum {
    //========================================================
    //      Succes
    //========================================================


    /**< Operation completed successfully */
    ODRIVE_RESULT_OK = 0x0000,

    //========================================================
    //      Operation-Level Errors (0x2000-0x2FFF)
    //========================================================


    /**< Unspecified general error */
    ODRIVE_ERROR_GENERAL = 0x2000,

    /**< Operation timed out waiting for result */
    ODRIVE_ERROR_TIMEOUT = 0x2001,

    /**< ODrive subsystem not initialized */
    ODRIVE_ERROR_NOT_INITIALIZED = 0x2002,

    /**< ODrive busy with another operation */
    ODRIVE_ERROR_BUSY = 0x2003,

    /**< Operation failed to complete */
    ODRIVE_ERROR_OPERATION_FAILED = 0x2004,

    /**< Operation not supported by this firmware version */
    ODRIVE_ERROR_UNSUPPORTED = 0x2005,

    /**< ODrive in incorrect state for requested operation */
    ODRIVE_ERROR_WRONG_STATE = 0x2006,


    //========================================================
    //      Communication Errors (0x3000-0x3FFF)
    //========================================================


    /**< CAN node ID out of valid range (0-63) */
    ODRIVE_ERROR_INVALID_CAN_ID = 0x3000,

    /**< CAN message transmission failed */
    ODRIVE_ERROR_CAN_TX_FAILED = 0x3001,

    /**< CAN message reception failed */
    ODRIVE_ERROR_RX_FAILED = 0x3002,

    /**< No response received from ODrive within timeout */
    ODRIVE_ERROR_NO_RESPONSE = 0x3003,

    /**< Response message format invalid or corrupted */
    ODRIVE_ERROR_INVALID_RESPONSE = 0x3004,

    /**< Data checksum or CRC verification failed */
    ODRIVE_ERROR_CHECKSUM_MISMATCH = 0x3005,

    /**< CAN bus entered bus-off error state */
    ODRIVE_ERROR_BUS_OFF = 0x3006,

    /**< CAN bus hardware initialization failed */
    ODRIVE_ERROR_PERIPHERAL_INIT_FAILED = 0x3007,


    ODRIVE_ERROR_INVALID_BACKEND,


    //========================================================
    //      Parameter/Validation Errors (0x4000-0x4FFF)
    //========================================================


    /**< Invalid parameter value provided */
    ODRIVE_ERROR_INVALID_PARAM = 0x4000,

    /**< Null pointer passed to function expecting valid pointer */
    ODRIVE_ERROR_NULL_POINTER = 0x4001,

    /**< Buffer size insufficient for operation */
    ODRIVE_ERROR_BUFFER_OVERFLOW = 0x4002,

    /**< Value outside acceptable range */
    ODRIVE_ERROR_OUT_OF_RANGE = 0x4003,

    /**< Axis number invalid (must be 0 or 1) */
    ODRIVE_ERROR_INVALID_AXIS = 0x4004,

    /**< Control mode or input mode invalid */
    ODRIVE_ERROR_INVALID_MODE = 0x4005,

    /**< Memory allocation failed (out of heap) */
    ODRIVE_ERROR_NO_MEMORY = 0x4006,


    //========================================================
    //      ODrive Device Errors (0x5000-0x5FFF)
    //========================================================


    /**< ODrive reported internal device error */
    ODRIVE_ERROR_DEVICE_ERROR = 0x5000,

    /**< Motor is disarmed (must arm before operation) */
    ODRIVE_ERROR_MOTOR_DISARMED = 0x5001,

    /**< Calibration must be performed before operation */
    ODRIVE_ERROR_CALIBRATION_REQUIRED = 0x5002,

    /**< Emergency stop is active (must reset to continue) */
    ODRIVE_ERROR_ESTOP_ACTIVE = 0x5003,

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