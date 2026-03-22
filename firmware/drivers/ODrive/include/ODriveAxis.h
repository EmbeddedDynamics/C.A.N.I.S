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
#include <stdbool.h>
    
/* Project headers */
#include "ODriveResult.h"
#include "ODriveCore.h"

//========================================================
//      Axis Structures
//========================================================

#if ODRIVE_FW_VERSION_IS(ODRIVE_FW_V0_5_4)
    /**
     * @brief ODrive heartbeat frame
     * 
     * Heartbeat frames are sent periodically by an ODrive controller as a keep-alive message.
     * These messages contain the overall status of the driver.
     */
    typedef struct  {
        /**
         * @brief Axis error of the ODrive controller
         */
        odrive_axis_error_flags_t axis_error;

        /**
         * @brief Axis state of the ODrive controller
         */
        odrive_axis_state_flags_t axis_state;
    } odrive_heartbeat_frame_t;
#elif ODRIVE_FW_VERSION_IS(ODRIVE_FW_V0_5_6)
    #define ODRIVE_MOTOR_ERROR_FLAG_BM  0x01
    #define ODRIVE_ENCODER_ERROR_FLAG_BM  0x01
    #define ODRIVE_CONTROLLER_ERROR_FLAG_BM  0x01
    #define ODRIVE_TRAJECTORY_DONE_FLAG_BM  0x80

    #define ODRIVE_MOTOR_ERROR_FLAG 0x01
    #define ODRIVE_ENCODER_ERROR_FLAG 0x02
    #define ODRIVE_CONTROLLER_ERROR_FLAG 0x04
    #define ODRIVE_TRAJECTORY_DONE_FLAG 0x08

    #define ODRIVE_MOTOR_ERROR_FLAG_BP 0u
    #define ODRIVE_ENCODER_ERROR_FLAG_BP 1u
    #define ODRIVE_CONTROLLER_ERROR_FLAG_BP 2u
    #define ODRIVE_TRAJECTORY_DONE_FLAG_BP 3u

    /**
     * @brief ODrive heartbeat frame
     * 
     * Heartbeat frames are sent periodically by an ODrive controller as a keep-alive message.
     * These messages contain the overall status of the driver.
     */
    typedef struct  {
        /**
         * @brief Axis error of the ODrive controller
         */
        odrive_axis_error_flags_t axis_error;

        /**
         * @brief Axis state of the ODrive controller
         */
        odrive_axis_state_flags_t axis_state;

        /**
         * @brief Axis error flags
         */
        uint8_t axis_flags;

    } odrive_heartbeat_frame_t;
#endif

/**
 * @brief Encoder estimate frame (position and velocity)
 * 
 * Contains current position and velocity estimates from the encoder
 * or sensorless estimator.
 */
typedef struct  {
    /**
     * @brief Position estimate [turns]
     */
    float position_estimate;

    /**
     * @brief Velocity estimate [turns/s]
     */
    float velocity_estimate;
} odrive_encoder_estimate_frame_t;

/**
 * @brief IQ frame (current setpoint and measured)
 * 
 * Contains the torque producing current setpoint and measured current.
 */
typedef struct {
    /**
     * @brief Current setpoint [A]
     */
    float iq_setpoint;

    /**
     * @brief Measured current [A]
     */
    float iq_measured;
} odrive_iq_frame_t;


#if ODRIVE_FW_VERSION_AT_LEAST(ODRIVE_FW_V0_6_0)
/**
 * @brief Temperature frame (FET and motor temperatures)
 * 
 * Contains the FET and motor temperature readings.
 */
typedef struct {
    /**
     * @brief FET and motor temperatures [°C]
     */
    float fet_temp;

    /**
     * @brief Motor temperature [°C]
     */
    float motor_temp;
} odrive_temperature_frame_t;

#endif

typedef struct {
    /**
     * @brief DC bus voltage [V]
     */
    float voltage;

#if ODRIVE_FW_VERSION_AT_LEAST(ODRIVE_FW_V0_5_6)
    /**
     * @brief DC bus current [A]
     */
    float current;
#endif
} odrive_bus_frame_t;

//========================================================
//      Axis Setpoint Structure
//========================================================

typedef struct {
    float position;
    int16_t velocity_ff;
    int16_t torque_ff;
} odrive_setpoint_t;

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
 * @param[in] driver - Odriver driver handle
 * @param[in] node - Axis node id
 * @param[in] state - Requested state from odrive_axis_state_t enum
 * 
 * @retval ODRIVE_RESULT_OK : ODrive message transmitted
 * @retval ODRIVE_ERROR_INVALID_PARAMETER - Invalid parameter
 * @retval ODRIVE_ERROR_NULL_POINTER - NULL parameter
 * 
 */
odrive_result_t odrive_axis_request_state(odrive_driver driver, 
                                          odrive_node_id node,
                                          odrive_axis_state_t state);

/**
 * @brief Gets current axis state (from cached heartbeat)
 *
 * Returns the most recent axis state from heartbeat messages.
 * 
 * @param[in] driver - Odriver driver handle
 * @param[in] node - Axis node id
 * @param[out] state - Address of the state
 *
 * @retval ODRIVE_RESULT_OK : Returned cached state
 * @retval ODRIVE_ERROR_INVALID_PARAMETER - Invalid parameter
 * @retval ODRIVE_ERROR_NULL_POINTER - NULL parameter
 * 
 */
odrive_result_t odrive_axis_get_current_state(odrive_driver driver, 
                                              odrive_node_id node,
                                              odrive_axis_state_t* state);

//========================================================
//      Axis Control Methods
//========================================================

/**
 * @brief Initiates force feedback homing procedure
 *
 * Starts the force feedback homing sequence on the specified axis.
 * The axis must be in AXIS_STATE_IDLE state before calling this function.
 *
 * @param[in] axis - Axis node ID
 *
 * @return odrive_result_t:
 *          - ODRIVE_RESULT_OK - Axis created successfully
 *          - ODRIVE_ERROR_INVALID_PARAMETER - Invalid parameter
 *          - ODRIVE_ERROR_NULL_POINTER - NULL parameter
 * 
 */
odrive_result_t odrive_force_feedback_homing(odrive_driver driver,
                                             odrive_node_id node);

/**
 * @brief Sets position setpoint
 *
 * Commands target position. Only effective when:
 * - control_mode = CONTROL_MODE_POSITION_CONTROL
 * - axis in AXIS_STATE_CLOSED_LOOP_CONTROL
 *
 * @param[in] driver - The ODrive driver handle
 * @param[in] node - Axis node id
 * @param[in] setpoint - ODrive setpoint structure
 *
 * @retval ODRIVE_RESULT_OK : ODrive message transmitted
 * @retval ODRIVE_ERROR_INVALID_PARAMETER - Invalid parameter
 * @retval ODRIVE_ERROR_NULL_POINTER - NULL parameter
 * 
 * @note control_mode must be equal to CONTROL_MODE_POSITION_CONTROL for this operation to work.
 * 
 * @see https://docs.odriverobotics.com/v/latest/manual/can-protocol.html#set-input-pos
 */
odrive_result_t odrive_axis_set_position(odrive_driver driver,
                                         odrive_node_id node,
                                         odrive_setpoint_t* setpoint);

bool odrive_axis_is_homed(odrive_driver driver,
                          odrive_node_id node);

// ========================================================

#endif // !ODRIVE_AXIS_H

// ========================================================

/* [] END OF FILE */
