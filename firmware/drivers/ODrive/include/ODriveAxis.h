/***************************************************************************//**
* \file ODriveAxis.h
* \version 1.0.0
*
* \brief
* Public header for axis control on ODrive controllers.
*
********************************************************************************
* \copyright
* Copyright 2025, Embedded Dynamics. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/


#ifndef ODRIVE_AXIS_H
#define ODRIVE_AXIS_H

//========================================================
//      Standard Includes
//========================================================

/* Internal headers */
#include <stdint.h>
    
/* Project headers */
#include "ODriveResult.h"
#include "ODriveCore.h"

//========================================================
//      Axis Structures
//========================================================

/**
 * @brief ODrive heartbeat frame
 * 
 * @details Heartbeat frames are sent periodically by an ODrive controller as a keep-alive message.
 * These messages contain the overall status of the driver.
 */
typedef struct  {
    /**
     * @brief Axis error of the ODrive controller
     */
    odrive_axis_error axis_error;

    /**
     * @brief Axis state of the ODrive controller
     */
    odrive_axis_state axis_state;

    /**
     * @brief Latest error of a procedure
     */
    odrive_procedure_result procedure_result;

    /**
     * @brief Trajectory planner done bit
     */
    bool    trajDone;
} odrive_heartbeat_frame_t;
    
//========================================================
//      Axis Instance Methods
//========================================================

/**
 * @brief Creates an ODrive axis instance
 *
 * Associates an axis (motor) with a specific CAN node ID. The axis
 * is registered with the provided driver for CAN communication.
 *
 * @param[in] driver - odrive_driver handle
 * @param[in] node_id - CAN node ID (0-63, configured in axis.config.can_node_id)
 * @param[out] axis - Pointer to receive odrive_axis handle
 *
 * @return odrive_result_t:
 *          - ODRIVE_RESULT_OK - Axis created successfully
 *          - ODRIVE_ERROR_NULL_POINTER - NULL parameter
 *          - ODRIVE_ERROR_INVALID_PARAMETER - Invalid parameter
 *          - ODRIVE_ERROR_INVALID_CAN_ID - node_id out of range (>63)
 *          - ODRIVE_ERROR_OUT_OF_MEMORY - Allocation failed
 *          - ODRIVE_ERROR_AXIS_ALREADY_REGISTERED - node_id already in use
 * 
 * Example usage:
 * @code
 *   odrive_driver driver;
 *   odrive_axis axis0, axis1;
 *   
 *   // Create driver
 *   ODriveDriver_Create(&config, &driver);
 *   
 *   // Create axis for ODrive board #0, axis 0 (CAN ID 0)
 *   odrive_axis_create(driver, 0, &axis0);
 *   
 *   // Create axis for ODrive board #0, axis 1 (CAN ID 1)
 *   odrive_axis_create(driver, 1, &axis1);
 * @endcode
 */
odrive_result_t odrive_axis_create(odrive_driver driver, 
                                   odrive_node_id node_id, 
                                   odrive_axis* axis);

/**
 * @brief Destroys ODrive axis instance
 *
 * Unregisters axis from driver and frees resources.
 *
 * @param[in] axis - odrive_axis handle
 *
 * @return void
 */
void odrive_axis_destroy(odrive_axis axis);

//========================================================
//      Axis State Methods
//========================================================

/**
 * @brief Requests axis state transition
 *
 * Commands the axis to transition to the specified state. Common usage:
 * 
 * @code
 *   // Enter closed-loop control
 *   odrive_axis_request_state(axis, AXIS_STATE_CLOSED_LOOP_CONTROL);
 *   
 *   // Return to idle
 *   odrive_axis_request_state(axis, AXIS_STATE_IDLE);
 * @endcode
 *
 * @param[in] axis - odrive_axis handle
 * @param[in] state - Requested state from odrive_axis_state_t enum
 *
 * @return odrive_result_t:
 *          - ODRIVE_RESULT_OK - Axis created successfully
 *          - ODRIVE_ERROR_INVALID_PARAMETER - Invalid parameter
 *          - ODRIVE_ERROR_NULL_POINTER - NULL parameter
 * 
 */
odrive_result_t odrive_axis_request_state(odrive_axis axis, 
                                          odrive_axis_state_t state);

/**
 * @brief Gets current axis state (from cached heartbeat)
 *
 * Returns the most recent axis state from heartbeat messages.
 * For real-time state query, use odrive_axis_GetCurrentStateSync().
 *
 * @param[in] axis - odrive_axis handle
 * @param[out] state - Pointer to receive current odrive_axis_state_t
 *
* @return odrive_result_t:
 *          - ODRIVE_RESULT_OK - Axis created successfully
 *          - ODRIVE_ERROR_INVALID_PARAMETER - Invalid parameter
 *          - ODRIVE_ERROR_NULL_POINTER - NULL parameter
 */
odrive_result_t odrive_axis_get_current_state(odrive_axis axis, 
                                              odrive_axis_state_t* state);

//========================================================
//      Axis Control Methods
//========================================================

/**
 * @brief Sets position setpoint
 *
 * Commands target position. Only effective when:
 * - control_mode = CONTROL_MODE_POSITION_CONTROL
 * - axis in AXIS_STATE_CLOSED_LOOP_CONTROL
 *
 * @param[in] axis - odrive_axis handle
 * @param[in] position - Target position [turns]
 * @param[in] velocity_ff - Velocity feedforward [turns/s] (0 if unused)
 * @param[in] torque_ff - Torque feedforward [Nm] (0 if unused)
 *
 * @return odrive_result_t:
 *          - ODRIVE_RESULT_OK - Axis created successfully
 *          - ODRIVE_ERROR_INVALID_PARAMETER - Invalid parameter
 *          - ODRIVE_ERROR_NULL_POINTER - NULL parameter
 * 
 * @note control_mode must be equal to CONTROL_MODE_POSITION_CONTROL for this operation to work.
 * 
 * @see https://docs.odriverobotics.com/v/latest/manual/can-protocol.html#set-input-pos
 */
odrive_result_t odrive_axis_set_setpoint(odrive_axis axis,
                                         float position,
                                         int16_t velocity_ff,
                                         int16_t torque_ff);

/**
 * @brief Sets velocity setpoint
 *
 * Commands target velocity. Only effective when:
 * - control_mode = CONTROL_MODE_VELOCITY_CONTROL
 * - axis in AXIS_STATE_CLOSED_LOOP_CONTROL
 *
 * @param[in] axis - odrive_axis handle
 * @param[in] velocity - Target velocity [turns/s]
 * @param[in] torque_ff - Torque feedforward [Nm] (0 if unused)
 *
 * @return Operation result
 * 
 * @note control_mode must be equal to CONTROL_MODE_VELOCITY_CONTROL for this operation to work.
 * 
 * @see https://docs.odriverobotics.com/v/latest/manual/can-protocol.html#set-input-vel
 */
odrive_result_t odrive_axis_set_velocitysetpoint(odrive_axis axis,
                                                 float velocity,
                                                 float torque_ff);

/**
 * @brief Sets torque setpoint
 *
 * Commands target torque. Only effective when:
 * - control_mode = CONTROL_MODE_TORQUE_CONTROL
 * - axis in AXIS_STATE_CLOSED_LOOP_CONTROL
 *
 * @param[in] axis - odrive_axis handle
 * @param[in] torque - Target torque [Nm]
 *
 * @return Operation result
 * 
 * @note control_mode must be equal to CONTROL_MODE_TORQUE_CONTROL for this operation to work.
 * 
 * @see https://docs.odriverobotics.com/v/latest/manual/can-protocol.html#set-input-torque
 */
odrive_result_t odrive_axis_set_torquesetpoint(odrive_axis axis, 
                                               float torque);

// // ========================================================
// // Function Prototypes - Feedback
// // ========================================================

/**
 * @brief Gets encoder position and velocity estimates
 *
 * Sends CAN request and waits for response with encoder feedback.
 *
 * @param[in] axis - odrive_axis handle
 * @param[in] timeout - Timeout until fail [ms] (default 100ms)
 * @param[out] frame - EncoderEstimateFrame pointer
 *
 * @return Operation result
 */
odrive_result_t odrive_axis_get_econder_frame(odrive_axis axis,
                                              encoder_estimate_frame* frame);

/**
 * @brief Reboots ODrive controller
 *
 * Note: Reboots entire ODrive board.
 *
 * @param[in] axis - odrive_axis handle
 * @param[in] action - Rebooot action to be executed (ODriveRebootAction)
 *
 * @return Operation result
 * 
 * @note Reboots a specific ODrive controller connected to the CAN bus.
 * 
 * @see https://docs.odriverobotics.com/v/latest/manual/can-protocol.html#reboot
 */
odrive_result_t odrive_axis_reboot(odrive_axis axis, 
                                   odrive_reboot_action_t action);


// /**
//  * @brief Checks ODrive controller status
//  * 
//  * Returns the current state of the ODrive controller. The result is based on the 
//  * #ODriveAliveState enum.
//  *
//  * @param[in] axis - odrive_axis handle
//  *
//  * @return #ODriveAliveState
//  */
// ODriveAliveState odrive_axis_IsAlive(odrive_axis axis);

// /**
//  * @brief Gets CAN node ID of this axis
//  *
//  * @param[in] axis - odrive_axis handle
//  *
//  * @return CAN node ID (0-63), or 0xFF if axis NULL
//  */
// ODriveNodeId odrive_axis_GetNodeId(odrive_axis axis);

// /**
//  * @brief Gets driver handle associated with this axis
//  *
//  * @param[in] axis - odrive_axis handle
//  *
//  * @return ODriveDriver handle, or NULL if axis NULL
//  */
// ODriveDriver odrive_axis_GetDriver(odrive_axis axis);
    
// ========================================================

#endif // !ODRIVE_AXIS_H

// ========================================================

/* [] END OF FILE */
