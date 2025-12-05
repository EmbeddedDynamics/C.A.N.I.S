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

#define ODRIVE_MAX_AXES 12u
    
// ========================================================
// Public Typedefs
// ========================================================



    
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
 * @brief Reboots all ODrive controllers
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Any axis CanNodeId on target ODrive
 *
 * @return Operation result
 * 
 * @note Reboots all ODrive controllers connected to the CAN bus.
 */
ODriveResult ODriveDriver_RebootAll(ODriveDriver driver, CanNodeId node_id);

// ========================================================
// Function Prototypes - Heartbeat Monitoring
// ========================================================

/**
 * @brief Gets last heartbeat data for an axis
 *
 * Retrieves cached heartbeat data from most recent heartbeat message.
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Axis CanNodeId
 * @param[out] error - Pointer to receive axis error flags
 * @param[out] state - Pointer to receive current axis state
 * @param[out] is_armed - Pointer to receive armed status
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_GetHeartbeat(ODriveDriver driver,
                                       CanNodeId node_id,
                                       uint32_t* error,
                                       ODriveAxisState* state,
                                       bool* is_armed);

/**
 * @brief Checks if axis heartbeat is alive
 *
 * Returns true if heartbeat received within timeout period.
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Axis CanNodeId
 *
 * @return true if heartbeat alive, false if timeout or error
 */
bool ODriveDriver_IsHeartbeatAlive(ODriveDriver driver, CanNodeId node_id);

// ========================================================

#endif // !ODRIVE_H

// ========================================================

/* [] END OF FILE */
