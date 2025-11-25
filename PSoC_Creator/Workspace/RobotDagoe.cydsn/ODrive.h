/***************************************************************************//**
* \file ODrive.h
* \version 1.0.0
*
*  \brief
*  Public header for the ODrive library.
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

#ifndef ODRIVE_H
#define ODRIVE_H

// ========================================================
// Public defines
// ========================================================
 
#include <stdint.h>
    
/*!
 * @brief Bit position to bit mask conversion macro
 *
 * Converts a bit position (0-31) to a bitmask value.
 * Useful for setting individual bits in registers or flags.
 *
 * @param[in] n Bit position (0-31)
 * @return Bitmask with bit n set (value: 2^n)
 */
#define ODRIVE_BIT(n) (1 << n)

/*!
 * @brief Define an opaque handle type
 *
 * Creates a typedef for an opaque pointer handle.
 * The resulting type is an incomplete pointer that hides implementation details.
 *
 * @param[in] name - Name of the handle type to create
 *
 */ 
#define ODRIVE_DEFINE_HANDLE(name) typedef struct name##_T* name

/**
  * @brief Node id for the CAN bus controller
  */
typedef uint8_t CanNodeId;
// ========================================================
// Public Enums
// ========================================================

/**
 * @brief ODrive CAN Protocol Command IDs
 *
 * Defines the CAN message command identifiers for the ODrive CANSimple protocol.
 * The CAN message ID is constructed as: (node_id << 5) | cmd_id
 * 
 * Messages prefixed with "Get_" can be requested by the host using RTR=1,
 * or sent periodically by the ODrive based on configured intervals.
 * Messages prefixed with "Set_" are sent by the host to configure the ODrive.
 *
 * @note All values are encoded in little endian
 * @note Floats use IEEE 754 standard format
 * @see ODrive CAN Protocol Documentation: https://docs.odriverobotics.com/v/latest/manual/can-protocol.html
 */
typedef enum {
    /**< Get firmware and hardware version info (ODrive -> Host) */
    CAN_CMD_GET_VERSION                 = 0x000,
    
    /**< Periodic heartbeat with axis state and errors (ODrive -> Host) */
    CAN_CMD_HEARTBEAT                   = 0x001,
    
    /**< Emergency stop, disarms axis immediately (Host -> ODrive) */
    CAN_CMD_ESTOP                       = 0x002,
    
    /**< Get active errors and disarm reason (ODrive -> Host) */
    CAN_CMD_GET_ERROR                   = 0x003,
    
    /**< SDO receive - read/write arbitrary parameters (Host -> ODrive) */
    CAN_CMD_RXSDO                       = 0x004,
    
    /**< SDO transmit - response to RxSdo (ODrive -> Host) */
    CAN_CMD_TXSDO                       = 0x005,
    
    /**< Node discovery and addressing (Bidirectional) */
    CAN_CMD_ADDRESS                     = 0x006,
    
    /**< Set axis operational state (Host -> ODrive) */
    CAN_CMD_SET_AXIS_STATE              = 0x007,
    
    /**< Get position and velocity estimates (ODrive -> Host) */
    CAN_CMD_GET_ENCODER_ESTIMATES       = 0x009,
    
    /**< Set control and input mode (Host -> ODrive) */
    CAN_CMD_SET_CONTROLLER_MODE         = 0x00B,
    
    /**< Set position setpoint with feedforward (Host -> ODrive) */
    CAN_CMD_SET_INPUT_POS               = 0x00C,
    
    /**< Set velocity setpoint with torque feedforward (Host -> ODrive) */
    CAN_CMD_SET_INPUT_VEL               = 0x00D,
    
    /**< Set torque setpoint (Host -> ODrive) */
    CAN_CMD_SET_INPUT_TORQUE            = 0x00E,
    
    /**< Set velocity and current limits (Host -> ODrive) */
    CAN_CMD_SET_LIMITS                  = 0x00F,
    
    /**< Set trajectory velocity limit (Host -> ODrive) */
    CAN_CMD_SET_TRAJ_VEL_LIMIT          = 0x011,
    
    /**< Set trajectory acceleration/deceleration limits (Host -> ODrive) */
    CAN_CMD_SET_TRAJ_ACCEL_LIMITS       = 0x012,
    
    /**< Set trajectory inertia for feed-forward (Host -> ODrive) */
    CAN_CMD_SET_TRAJ_INERTIA            = 0x013,
    
    /**< Get q-axis current setpoint and measured (ODrive -> Host) */
    CAN_CMD_GET_IQ                      = 0x014,
    
    /**< Get FET and motor temperature (ODrive -> Host) */
    CAN_CMD_GET_TEMPERATURE             = 0x015,
    
    /**< Reboot ODrive with specified action (Host -> ODrive) */
    CAN_CMD_REBOOT                      = 0x016,
    
    /**< Get DC bus voltage and current (ODrive -> Host) */
    CAN_CMD_GET_BUS_VOLTAGE_CURRENT     = 0x017,
    
    /**< Clear errors and optionally identify (Host -> ODrive) */
    CAN_CMD_CLEAR_ERRORS                = 0x018,
    
    /**< Set absolute encoder position (Host -> ODrive) */
    CAN_CMD_SET_ABSOLUTE_POSITION       = 0x019,
    
    /**< Set position controller gain (Host -> ODrive) */
    CAN_CMD_SET_POS_GAIN                = 0x01A,
    
    /**< Set velocity controller gains (Host -> ODrive) */
    CAN_CMD_SET_VEL_GAINS               = 0x01B,
    
    /**< Get torque target and estimate (ODrive -> Host) */
    CAN_CMD_GET_TORQUES                 = 0x01C,
    
    /**< Get electrical and mechanical power (ODrive -> Host) */
    CAN_CMD_GET_POWERS                  = 0x01D,
    
    /**< Enter DFU bootloader mode (Host -> ODrive) */
    CAN_CMD_ENTER_DFU_MODE              = 0x01F,
} ODriveCanCommandId;

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
// Public Structs
// ========================================================

/**
 * @brief Opaque handle to an ODrive driver instance
 * 
 * This is an opaque pointer type used to reference an ODrive controller.
 * Users obtain valid handles through createODriveDriver() and pass them
 * to other ODrive functions.
 * 
 * @note Implementation details are hidden. Do not dereference directly.
 * @see #createODriveDriver
 */
ODRIVE_DEFINE_HANDLE(ODriveDriver);

// ========================================================
// Public Methods
// ========================================================

/**
 * @brief Creates a ODrive driver
 * 
 * @param[in] id - The #CanNodeId of the driver.
 * @param[out] driver - Pointer to the #ODriveDriver handle address.
 *
 * @return Operation result code:
 *         - #ODRIVE_RESULT_OK - Driver created successfully
 *         - #ODRIVE_ERROR_INVALID_CAN_ID - CAN ID out of valid range
 *         - #ODRIVE_ERROR_NULL_POINTER - driver parameter is NULL
*/
ODriveResult createODriveDriver(CanNodeId id, ODriveDriver* driver);

/**
 * @brief Clear all errors on the ODrive.
 *
 * This function returns immediately and does not check if the ODrive
 * received the CAN message.
 *
 * @param[in] driver - #ODriveDriver handle.
 *
 * @return Operation result code:
 *         - #ODRIVE_RESULT_OK - Errors cleared
 *         - #ODRIVE_ERROR_NULL_POINTER - driver parameter is NULL
 */
ODriveResult clearErrors(ODriveDriver driver);

// ========================================================

#endif // !ODRIVE_H

// ========================================================

/* [] END OF FILE */
