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
 
/* Internal headers */
#include <stdint.h>
#include <stdbool.h>
    
/* Project headers */
#include "ODriveResult.h"
#include "ODriveCore.h"
#include "ODriveAxis.h"
    
// ========================================================
// Public Typpedefs
// ========================================================

/**
 * @brief CAN receive callback function type
 *
 * Called when a CAN message is received from an ODrive axis.
 *
 * @param[in] can_id - CAN message ID (encodes node_id and command_id)
 * @param[in] data - Pointer to 8-byte CAN data payload
 * @param[in] dlc - Data length (0-8 bytes)
 * @param[in] user_data - User context pointer from registration
 *
 * @note Callback may be invoked from ISR context. Keep execution time minimal.
 */
typedef void (*ODriveCANRxCallback)(uint32_t can_id, uint8_t* data, uint8_t dlc, void* user_data);

    
// ========================================================
// Public Structs
// ========================================================

/**
 * @brief ODrive driver configuration
 */
typedef struct {
    /**< CAN bus bitrate (e.g., 250000, 500000, 1000000) */
    uint32_t can_bitrate;           
    
    /**< CAN transmit timeout [ms] */
    uint16_t tx_timeout_ms;         
    
    /**< CAN receive timeout [ms] */
    uint16_t rx_timeout_ms;         
    
    /**< Maximum number of axes to support */
    uint8_t max_axes;               
    
    /**< Monitor axis heartbeat messages */
    bool enable_heartbeat_monitor;  
    
    /**< Heartbeat timeout [ms] (if monitoring enabled) */
    uint16_t heartbeat_timeout_ms;  
} ODriveDriverConfig;

// ========================================================
// Public Methods
// ========================================================

/**
 * @brief Creates ODrive CAN driver instance
 *
 * Initializes the main CAN driver that manages communication with all
 * ODrive axes on the bus. Must be called before creating any axis instances.
 *
 * @param[in] config - Driver configuration
 * @param[out] driver - Pointer to receive ODriveDriver handle
 *
 * @return Operation result:
 *         - #ODRIVE_RESULT_OK - Driver created successfully
 *         - #ODRIVE_ERROR_NULL_POINTER - NULL parameter
 *         - #ODRIVE_ERROR_OUT_OF_MEMORY - Allocation failed
 *         - #ODRIVE_ERROR_CAN_INIT_FAILED - CAN peripheral initialization failed
 */
ODriveResult ODriveDriver_Create(const ODriveDriverConfig* config, ODriveDriver* driver);

/**
 * @brief Destroys ODrive driver instance
 *
 * Shuts down CAN communication and frees all resources. All associated
 * axis instances should be destroyed before destroying the driver.
 *
 * @param[in] driver - ODriveDriver handle
 *
 * @return Operation result:
 *         - #ODRIVE_RESULT_OK - Driver destroyed successfully
 *         - #ODRIVE_ERROR_NULL_POINTER - NULL driver handle
 */
ODriveResult ODriveDriver_Destroy(ODriveDriver driver);

/**
 * @brief Starts CAN communication
 *
 * Enables CAN peripheral and begins processing messages.
 *
 * @param[in] driver - ODriveDriver handle
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_Start(ODriveDriver driver);

/**
 * @brief Stops CAN communication
 *
 * Disables CAN peripheral and halts message processing.
 *
 * @param[in] driver - ODriveDriver handle
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_Stop(ODriveDriver driver);

// ========================================================
// Function Prototypes - CAN Communication (Low-Level)
// ========================================================

/**
 * @brief Sends raw CAN message
 *
 * Low-level function to transmit CAN frame. Used internally by axis
 * control functions.
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Target axis CAN node ID (0-63)
 * @param[in] cmd_id - ODrive CAN command ID
 * @param[in] data - Pointer to data buffer (max 8 bytes)
 * @param[in] dlc - Data length code (0-8)
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_SendCANMessage(ODriveDriver driver,
                                         uint8_t node_id,
                                         uint8_t cmd_id,
                                         const uint8_t* data,
                                         uint8_t dlc);

/**
 * @brief Receives CAN message with timeout
 *
 * Waits for CAN message matching specified node_id and cmd_id.
 * Blocking call with timeout.
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Expected axis CAN node ID (0-63)
 * @param[in] cmd_id - Expected ODrive CAN command ID
 * @param[out] data - Buffer to receive data (min 8 bytes)
 * @param[out] dlc - Pointer to receive data length
 * @param[in] timeout_ms - Receive timeout [ms]
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_ReceiveCANMessage(ODriveDriver driver,
                                            uint8_t node_id,
                                            uint8_t cmd_id,
                                            uint8_t* data,
                                            uint8_t* dlc,
                                            uint16_t timeout_ms);

/**
 * @brief Processes incoming CAN messages
 *
 * Should be called periodically (from main loop or ISR) to handle
 * received CAN messages and update internal state.
 *
 * @param[in] driver - ODriveDriver handle
 *
 * @return Number of messages processed
 */
uint16_t ODriveDriver_ProcessMessages(ODriveDriver driver);

// ========================================================
// Function Prototypes - Axis Management
// ========================================================

/**
 * @brief Registers an axis with the driver
 *
 * Associates an ODriveAxis instance with this driver. Called internally
 * by ODriveAxis_Create(). Allows driver to route CAN messages to
 * correct axis.
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Axis CAN node ID
 * @param[in] axis_context - Opaque pointer to axis instance
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_RegisterAxis(ODriveDriver driver,
                                       uint8_t node_id,
                                       void* axis_context);

/**
 * @brief Unregisters an axis from the driver
 *
 * Removes axis association. Called internally by ODriveAxis_Destroy().
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Axis CAN node ID
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_UnregisterAxis(ODriveDriver driver, uint8_t node_id);

// ========================================================
// Function Prototypes - Protocol-Level Commands
// ========================================================

/**
 * @brief Sends axis state command
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Target axis CAN node ID
 * @param[in] state - Requested axis state
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_SendAxisState(ODriveDriver driver,
                                        uint8_t node_id,
                                        ODriveAxisState state);

/**
 * @brief Sends position setpoint
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Target axis CAN node ID
 * @param[in] position - Position setpoint [turns]
 * @param[in] vel_ff - Velocity feedforward [turns/s]
 * @param[in] torque_ff - Torque feedforward [Nm]
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_SendInputPos(ODriveDriver driver,
                                       uint8_t node_id,
                                       float position,
                                       float vel_ff,
                                       float torque_ff);

/**
 * @brief Sends velocity setpoint
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Target axis CAN node ID
 * @param[in] velocity - Velocity setpoint [turns/s]
 * @param[in] torque_ff - Torque feedforward [Nm]
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_SendInputVel(ODriveDriver driver,
                                       uint8_t node_id,
                                       float velocity,
                                       float torque_ff);

/**
 * @brief Sends torque setpoint
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Target axis CAN node ID
 * @param[in] torque - Torque setpoint [Nm]
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_SendInputTorque(ODriveDriver driver,
                                          uint8_t node_id,
                                          float torque);

/**
 * @brief Requests encoder estimates
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Target axis CAN node ID
 * @param[out] position - Pointer to receive position [turns]
 * @param[out] velocity - Pointer to receive velocity [turns/s]
 * @param[in] timeout_ms - Response timeout [ms]
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_RequestEncoderEstimates(ODriveDriver driver,
                                                  uint8_t node_id,
                                                  float* position,
                                                  float* velocity,
                                                  uint16_t timeout_ms);

/**
 * @brief Sets control and input modes
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Target axis CAN node ID
 * @param[in] control_mode - Control mode (position/velocity/torque)
 * @param[in] input_mode - Input mode (passthrough/trap_traj/etc)
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_SetControllerModes(ODriveDriver driver,
                                             uint8_t node_id,
                                             ODriveControlMode control_mode,
                                             ODriveInputMode input_mode);

/**
 * @brief Clears all errors on specified axis
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Target axis CAN node ID
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_ClearErrors(ODriveDriver driver, uint8_t node_id);

/**
 * @brief Reboots ODrive controller
 *
 * Note: Reboots entire ODrive board (both axes).
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Any axis CAN node ID on target ODrive
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_Reboot(ODriveDriver driver, uint8_t node_id);

// ========================================================
// Function Prototypes - Heartbeat Monitoring
// ========================================================

/**
 * @brief Gets last heartbeat data for an axis
 *
 * Retrieves cached heartbeat data from most recent heartbeat message.
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Axis CAN node ID
 * @param[out] error - Pointer to receive axis error flags
 * @param[out] state - Pointer to receive current axis state
 * @param[out] is_armed - Pointer to receive armed status
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_GetHeartbeat(ODriveDriver driver,
                                       uint8_t node_id,
                                       uint32_t* error,
                                       ODriveAxisState* state,
                                       bool* is_armed);

/**
 * @brief Checks if axis heartbeat is alive
 *
 * Returns true if heartbeat received within timeout period.
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Axis CAN node ID
 *
 * @return true if heartbeat alive, false if timeout or error
 */
bool ODriveDriver_IsHeartbeatAlive(ODriveDriver driver, uint8_t node_id);

// ========================================================

#endif // !ODRIVE_H

// ========================================================

/* [] END OF FILE */
