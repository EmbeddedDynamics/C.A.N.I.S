/***************************************************************************//**
* \file ODrive.h
* \version 1.0.0
*
* \brief
* Public API for ODrive motor controller library.
*
* \details
* Main header providing access to ODrive driver creation, CAN communication
* control, and axis management. This is the primary entry point for ODrive
* library users.
*
********************************************************************************
* \copyright
* Copyright 2025, Embedded Dynamics. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef ODRIVE_H
#define ODRIVE_H

//========================================================
//      Standard Includes
//========================================================

/* Internal headers */
#include <stdint.h>
#include <stdbool.h>
    
/* Project headers */
#include "ODriveResult.h"
#include "ODriveCore.h"
#include "ODriveAxis.h"

#include "driver/ODriveBackend.h"

//========================================================
//      Configuration Constants
//========================================================

/**
 * @brief Maximum number of axes per driver instance
 */
#define ODRIVE_MAX_AXES 12u

/**
 * @brief Default heartbeat timeout (milliseconds)
 */
#define ODRIVE_HEARTBEAT_DEFAULT_TIMEOUT_MS 100u

//========================================================
//      ODrive State Enums
//========================================================

typedef enum {
    DRIVER_STATE_UNKNOWN = 0x00,
    DRIVER_STATE_UNINITIALIZED,
    DRIVER_STATE_IDLE,
    DRIVER_STATE_STOPPED,
    DRIVER_STATE_RUNNING,
    DRIVER_STATE_REBOOTING,
} odrive_driver_state_t;

//========================================================
//      Driver Configuration
//========================================================

/**
 * @brief ODrive driver configuration
 * 
 * @details
 * Contains all parameters needed to initialize an ODrive driver instance,
 * including communication interface selection, context, and optional
 * heartbeat monitoring.
 */
typedef struct {
    
    odrive_backend backend;
   
    /**< Monitor axis heartbeat messages */
    bool enable_heartbeat_monitor;  
    
    /**< Heartbeat timeout [ms] (if monitoring enabled) */
    uint16_t heartbeat_timeout_ms;  
} odrive_driver_config_t;

//========================================================
//      Driver Instance Methods
//========================================================

/**
 * @brief Create and initialize an ODrive driver instance
 * 
 * @param[in] config - Driver configuration
 * @param[out] driver - Pointer to receive driver handle
 * 
 * @return Operation result:
 *          - ODRIVE_RESULT_OK - Driver created successfully
 *          - ODRIVE_ERROR_NULL_POINTER - config or driver parameter is NULL
 *          - ODRIVE_ERROR_OUT_OF_MEMORY - Memory allocation failed
 * 
 * @example
 *   odrive_driver_config_t cfg = {
 *       .com_type = ODRIVE_COMMUNICATION_CAN,
 *       .com_ctx = &can_driver,
 *       .enable_heartbeat_monitor = true,
 *       .heartbeat_timeout_ms = 100
 *   };
 *   odrive_driver driver;
 *   ODRIVE_CHECK(odrive_create_driver(&cfg, &driver));
 */
odrive_result_t odrive_create_driver(const odrive_driver_config_t* config,
                                     odrive_driver* driver);

/**
 * @brief Destroy and deallocate an ODrive driver instance
 * 
 * @param[in] driver - Driver handle to destroy
 * 
 * @return Operation result:
 *          - ODRIVE_RESULT_OK - Driver destroyed successfully
 *          - ODRIVE_ERROR_NULL_POINTER - driver is NULL
 * 
 * @warning After calling this function, the driver handle is invalid
 *          and must not be used.
 */
void odrive_destroy_driver(odrive_driver driver);

//========================================================
//      Driver Communication Control
//========================================================

/**
 * @brief Start ODrive driver CAN communication
 * 
 * @param[in] driver - Driver handle
 * 
 * @return Operation result:
 *          - ODRIVE_RESULT_OK - Communication started successfully
 *          - ODRIVE_ERROR_NULL_POINTER - driver is NULL
 *          - ODRIVE_ERROR_NOT_INITIALIZED - Driver not initialized
 *          - ODRIVE_ERROR_OPERATION_FAILED - Failed to start CAN peripheral
 * 
 */
odrive_result_t odrive_start_driver(odrive_driver driver);

/**
 * @brief Stop ODrive driver CAN communication
 * 
 * @param[in] driver - Driver handle
 * 
 * @return Operation result:
 *          - ODRIVE_RESULT_OK - Communication stopped successfully
 *          - ODRIVE_ERROR_NULL_POINTER - driver is NULL
 *          - ODRIVE_ERROR_NOT_INITIALIZED - Driver not initialized
 * 
 */
odrive_result_t odrive_stop_driver(odrive_driver driver);

//========================================================
//      Driver Methods
//========================================================

/**
 * @brief Activate 
 */

/**
 * @brief Reboot all ODrive controllers on the CAN bus
 * 
 * @param[in] driver - Driver handle
 * @param[in] action - Reboot action (REBOOT_ACTION_*)
 * 
 * @return Operation result:
 *          - ODRIVE_RESULT_OK - Reboot command sent successfully
 *          - ODRIVE_ERROR_NULL_POINTER - driver is NULL
 *          - ODRIVE_ERROR_NOT_INITIALIZED - Driver not initialized
 *          - ODRIVE_ERROR_TRANSMIT_FAILED - Failed to transmit message
 * 
 * @warning All axes will disarm immediately. Ensure motors are safe
 *          before calling this function.
 * 
 * @example
 *   // Reboot and save configuration
 *   ODRIVE_CHECK(odrive_reboot_all(driver, REBOOT_ACTION_SAVE_CONFIG));
 */
odrive_result_t odrive_reboot_all(odrive_driver driver, 
                                  odrive_reboot_action_t action);

//========================================================
//      End of File
//========================================================

#endif // !ODRIVE_H