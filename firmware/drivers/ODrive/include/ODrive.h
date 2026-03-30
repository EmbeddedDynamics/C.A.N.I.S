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

#define ODRIVE_NODE_ID_UNUSED 0xFFFFu

#ifndef ODRIVE_SUPPORT_BROADCAST_ID
    #define ODRIVE_SUPPORT_BROADCAST_ID 0
    #define ODRIVE_AXES_ID_RANGE 0x3fu
#else
    #define ODRIVE_AXES_ID_RANGE 0x3eu
#endif

//========================================================
//      Callback Function Prototypes
//========================================================

typedef void (*odrive_on_homed_cb_t) (odrive_node_id node);

//========================================================
//      Driver State Enums
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
//      Force Feedback Homing Configuration
//========================================================

/**
 * @brief Force feedback homing configuration
 */
typedef struct {
    /**
     * @brief Enable force feedback homing
     */
    bool enable_force_feedback_homing;

    /**
     * @brief Flag used to distinguish if force feedback homing is enabled
     */
    bool is_force_homing;

    /**
     * @brief GPIO pin for minimum position feedback
     */
    uint8_t homing_pin;

    /**
     * @brief Force feedback IQ threshold for triggering homing
     */
    float iq_threshold;

    /**
     * @brief Position delta deadzone for force feedback homing
     */
    float position_deadzone;

    /**
     * @brief Current position delta used for force feedback homing
     */
    float position_delta;
} odrive_force_homing_cfg_t;

//========================================================
//      Datalogging Policy
//========================================================

enum {
    ODRIVE_LOGGING_PARAM_UNKNOWN    = 0x00,
    ODRIVE_LOGGING_PARAM_POWER      = ODRIVE_BIT(0u),
    ODRIVE_LOGGING_PARAM_TORQUE     = ODRIVE_BIT(1u),
    ODRIVE_LOGGING_PARAM_POSITION   = ODRIVE_BIT(2u),
    ODRIVE_LOGGING_PARAM_VELOCITY   = ODRIVE_BIT(3u),
};
typedef uint8_t odrive_logging_caps_t;

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
    odrive_logging_caps_t log_cap;

    /**< Monitor axis heartbeat messages */
    bool enable_heartbeat_monitor;  

    /**< Enable force feedback homing on axes */
    bool enable_force_feedback_homing;
    
    /**< Heartbeat timeout [ms] (if monitoring enabled) */
    uint16_t heartbeat_timeout_ms;  

    odrive_on_homed_cb_t on_homed;
    
    odrive_backend backend;
} odrive_driver_cfg_t;

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
odrive_result_t odrive_create_driver(const odrive_driver_cfg_t* config,
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
 * @brief Poll the ODrive driver for incoming messages and handle timeouts
 * 
 * @param[in] driver - Driver handle
 * @param[in] current_time - Current system time in milliseconds
 * 
 * @retval ODRIVE_RESULT_OK - Polling completed successfully
 * @retval ODRIVE_ERROR_NULL_POINTER - driver is NULL
 * @retval ODRIVE_ERROR_NOT_INITIALIZED - Driver not initialized
 * @retval ODRIVE_ERROR_INVALID_STATE - Driver in incorrect state for polling
 */
odrive_result_t odrive_poll_driver(odrive_driver driver,
                                   uint32_t current_time);

/**
 * @brief Reserve an axis slot for a specific node ID
 * 
 * @param[in] driver - Driver handle
 * @param[in] node_id - CAN node ID of the axis to register
 * 
 * @retval ODRIVE_RESULT_OK - Axis registered successfully
 * @retval ODRIVE_ERROR_NULL_POINTER - driver is NULL
 * @retval ODRIVE_ERROR_INVALID_PARAM - node_id is out of range
 * @retval ODRIVE_ERROR_NO_AVAILABLE_AXIS_SLOT - No free axis slots available
 */
odrive_result_t odrive_register_axis(odrive_driver driver,
                                     odrive_force_homing_cfg_t* cfg,
                                     odrive_node_id node_id);

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
