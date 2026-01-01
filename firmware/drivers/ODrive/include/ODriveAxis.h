/***************************************************************************//**
* \file odrive_axis.h
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
//      ODrive Axis Enums
//========================================================
    
/**
 * @brief ODrive axis control modes
 *
 * Defines the different control mode for a ODrive controller.
 * These modes determine how the closed-loop system will work
 * and react to the system.
 *
 * @see ODrive CAN Protocol Documentation: https://docs.odriverobotics.com/v/latest/manual/can-protocol.html
 */
typedef enum {
    /**< Enables voltage control mode */
    CONTROL_MODE_VOLTAGE_CONTROL = 0x0000,
    
    /**< Enables torque control mode */
    CONTROL_MODE_TORQUE_CONTROL = 0x0001,
    
    /**< Enables velocity control mode */
    CONTROL_MODE_VELOCITY_CONTROL = 0x0002,
    
    /**< Enables position control mode */
    CONTROL_MODE_POSITION_CONTROL = 0x0003,
    
} odrive_control_mode_t;

/**
 * @brief ODrive axis state machine
 *
 * Enumerates the possible states of an ODrive axis (axis.requested_state /
 * axis.current_state). Each state represents an internal routine such as
 * calibration, homing or normal closed-loop operation.
 *
 * @see ODrive AxisState documentation:
 *      https://docs.odriverobotics.com/v/latest/fibre_types/com_odriverobotics_ODrive.html#ODrive.Axis.AxisState
 */
typedef enum {
    /**
     * @brief Undefined state, falls through to IDLE.
     *
     * Not intended as a stable operating state. If requested, the axis
     * transitions directly to IDLE.
     */
    AXIS_STATE_UNDEFINED = 0x0000,

    /**
     * @brief Idle: motor PWM disabled, no control action.
     *
     * Safe resting state. The motor is not driven and configuration and
     * error handling can be performed.
     */
    AXIS_STATE_IDLE = 0x0001,

    /**
     * @brief Run the startup sequence.
     *
     * Executes the startup procedure as defined by axis.config.startup_*
     * flags (e.g. auto-calibration, index search, homing).
     */
    AXIS_STATE_STARTUP_SEQUENCE = 0x0002,

    /**
     * @brief Run the full calibration sequence for the current setup.
     *
     * Runs all essential calibration routines for the configured motor
     * and encoder combination, typically including MOTOR_CALIBRATION and
     * encoder calibration steps.
     */
    AXIS_STATE_FULL_CALIBRATION_SEQUENCE = 0x0003,

    /**
     * @brief Motor calibration (phase resistance and inductance).
     *
     * Measures the motor phase resistance and inductance and writes them
     * into config.motor.phase_resistance and config.motor.phase_inductance.
     * Results persist across reboot if save_configuration() is called.
     */
    AXIS_STATE_MOTOR_CALIBRATION = 0x0004,

    /**
     * @brief Encoder index search.
     *
     * Rotates the motor in one direction until the encoder index pulse
     * is detected. Used for encoders with an index signal.
     */
    AXIS_STATE_ENCODER_INDEX_SEARCH = 0x0006,

    /**
     * @brief Encoder offset calibration.
     *
     * Rotates the motor forward and backward to determine the offset
     * between the encoder’s electrical angle and the motor electrical
     * phase. Requires valid motor calibration.
     */
    AXIS_STATE_ENCODER_OFFSET_CALIBRATION = 0x0007,

    /**
     * @brief Normal closed-loop control.
     *
     * Enables the main control loop (position, velocity or torque),
     * depending on controller.config.control_mode. Requires a calibrated
     * motor (and encoder if used).
     */
    AXIS_STATE_CLOSED_LOOP_CONTROL = 0x0008,

    /**
     * @brief Lock-in spin.
     *
     * Runs a configured lock-in spin sequence, typically used for initial
     * alignment or special spin-up procedures. Requires valid motor
     * calibration.
     */
    AXIS_STATE_LOCKIN_SPIN = 0x0009,

    /**
     * @brief Encoder direction find.
     *
     * Determines the sign/direction of the encoder relative to the motor
     * electrical angle. Requires valid motor calibration.
     */
    AXIS_STATE_ENCODER_DIR_FIND = 0x000A,

    /**
     * @brief Homing sequence using endstops.
     *
     * Drives the axis against configured endstops to establish an
     * absolute reference (home) position. Endstops must be enabled.
     */
    AXIS_STATE_HOMING = 0x000B,

    /**
     * @brief Hall encoder polarity calibration.
     *
     * Spins the motor in lock-in mode and determines the correct hall
     * polarity and spacing (60° vs 120° electrical).
     */
    AXIS_STATE_ENCODER_HALL_POLARITY_CALIBRATION = 0x000C,

    /**
     * @brief Hall encoder phase calibration.
     *
     * Rotates the motor for an extended period to measure and compensate
     * edge timing of hall sensors. Produces a relative phase map.
     */
    AXIS_STATE_ENCODER_HALL_PHASE_CALIBRATION = 0x000D,

    /**
     * @brief Anticogging calibration.
     *
     * Spins the motor for the configured coarse and fine tuning durations
     * to build a cogging torque map. The previous map is cleared when
     * this routine starts.
     */
    AXIS_STATE_ANTICOGGING_CALIBRATION = 0x000E,

    /**
     * @brief Harmonic compensation calibration (load encoder).
     *
     * Experimental. Briefly spins the motor to estimate 1st and 2nd
     * harmonic distortion of the (load) encoder and fills the harmonic
     * compensation coefficients.
     */
    AXIS_STATE_HARMONIC_CALIBRATION = 0x000F,

    /**
     * @brief Harmonic compensation calibration (commutation encoder).
     *
     * Experimental. Same as HARMONIC_CALIBRATION but for the commutation
     * encoder in a dual-encoder setup. Coefficients are stored in the
     * commutation harmonic compensation config.
     */
    AXIS_STATE_HARMONIC_CALIBRATION_COMMUTATION = 0x0010

} odrive_axis_state_t;

//========================================================
//      ODrive Encoder Mode
//========================================================

/**
 * @brief ODrive encoder mode (axis.encoder.config.mode)
 *
 * Selects the type of encoder connected to this axis. For ODrive v3
 * firmware 0.5.x / 0.6.x. Value 0 is incremental, higher values are
 * absolute encoders over various interfaces.
 *
 * @see https://docs.odriverobotics.com/v/0.5.6/encoders.html
 */
typedef enum {
    /**
     * @brief Incremental encoder (quadrature A/B signals).
     *
     * Standard incremental encoder with A and B channels. Optional
     * index pulse (Z) can be used for homing.
     */
    ENCODER_MODE_INCREMENTAL = 0,

    /**
     * @brief Hall effect sensors.
     *
     * Three hall sensor inputs providing 6-step commutation. CPR must
     * be set to pole_pairs * 6.
     */
    ENCODER_MODE_HALL = 1,

    /**
     * @brief Sin/cos analog encoder.
     *
     * Analog sin/cos encoder connected to the specified GPIO pins.
     */
    ENCODER_MODE_SINCOS = 2,

    /**
     * @brief Absolute SPI encoder (CUI protocol).
     *
     * SPI-based absolute encoder using CUI protocol (e.g., AMT23xx).
     */
    ENCODER_MODE_SPI_ABS_CUI = 256,

    /**
     * @brief Absolute SPI encoder (AMS protocol).
     *
     * SPI-based absolute encoder using AMS protocol (e.g., AS5047P,
     * AS5048A). This is value 257 (0x101).
     */
    ENCODER_MODE_SPI_ABS_AMS = 257,

    /**
     * @brief Absolute SPI encoder (AEAT protocol).
     *
     * SPI-based absolute encoder using Broadcom/Avago AEAT protocol.
     */
    ENCODER_MODE_SPI_ABS_AEAT = 258,

    /**
     * @brief Absolute SPI encoder (RLS protocol).
     *
     * SPI-based absolute encoder using RLS protocol (e.g., AksIM).
     */
    ENCODER_MODE_SPI_ABS_RLS = 259,

    /**
     * @brief Absolute SPI encoder (TLE5012B).
     *
     * SPI-based absolute encoder using Infineon TLE5012B protocol.
     */
    ENCODER_MODE_SPI_ABS_TLE = 260

} odrive_encoder_mode_t;

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
    odrive_axis_error_t         axis_error;

    /**
     * @brief Axis state of the ODrive controller
     */
    odrive_axis_state_t         axis_state;

    /**
     * @brief Latest error of a procedure
     */
    odrive_procedure_result_t   procedure_result;

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
