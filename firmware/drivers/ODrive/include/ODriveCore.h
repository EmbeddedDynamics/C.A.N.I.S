/***************************************************************************//**
* \file ODriveCore.h
* \version 1.0.0
*
*  \brief
*  Public header containing the core features.
*
********************************************************************************
* \copyright
* Copyright 2025, Embedded Dynamics. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef ODRIVE_CORE_H
#define ODRIVE_CORE_H

//========================================================
//      Standard Includes
//========================================================
 
/* Internal headers */
#include <stdint.h>
#include <stddef.h>
    
/* Project headers */
#include "ODriveResult.h"

//========================================================
//      Library Information
//========================================================

#define ODRIVE_MAJOR 1u
#define ODRIVE_MINOR 0u
#define ODRIVE_PATCH 1u

//========================================================
//      Version Packing Helpers
//========================================================

/**
 * @brief Pack a semantic version (major.minor.patch) into a single comparable integer.
 *
 * Uses 8 bits per component (0..255). Adjust shifts if you need larger ranges.
 */
#define ODRIVE_VERSION_ENCODE(major, minor, patch) \
    ((((major) & 0xFFu) << 16) | (((minor) & 0xFFu) << 8) | ((patch) & 0xFFu))

/** @brief Current library version as a single integer. */
#define ODRIVEK_VERSION \
    ODRIVE_VERSION_ENCODE(ODRIVE_MAJOR, ODRIVE_MINOR, ODRIVE_PATCH)

/**
 * @brief True if ODrive version is at least (major.minor.patch).
 *
 * Usage:
 *   #if ODRIVE_VERSION_AT_LEAST(1,0,0)
 *     ...
 *   #endif
 */
#define ODRIVE_VERSION_AT_LEAST(major, minor, patch) (ODRIVE_VERSION >= ODRIVE_VERSION_ENCODE((major), (minor), (patch)))

/**
 * @brief True if ODrive version is exactly (major.minor.patch).
 */
#define ODRIVE_VERSION_IS(major, minor, patch) \
(ODRIVE_VERSION == ODRIVE_VERSION_ENCODE((major), (minor), (patch)))

//========================================================
//      ODrive Firmware Version Encoding
//========================================================

#define ODRIVE_FW_VERSION(major, minor, patch) \
    ((((major) & 0xFFu) << 16) | (((minor) & 0xFFu) << 8) | ((patch) & 0xFFu))

#define ODRIVE_FW_VERSION_MAJOR(version) (((version) >> 16) & 0xFFu)
#define ODRIVE_FW_VERSION_MINOR(version) (((version) >> 8)  & 0xFFu)
#define ODRIVE_FW_VERSION_PATCH(version) ((version) & 0xFFu)

//========================================================
//      Known ODrive Firmware Versions
//========================================================

#define ODRIVE_FW_V0_5_4   ODRIVE_FW_VERSION(0, 5, 4)
#define ODRIVE_FW_V0_5_5   ODRIVE_FW_VERSION(0, 5, 5)
#define ODRIVE_FW_V0_5_6   ODRIVE_FW_VERSION(0, 5, 6)

#define ODRIVE_FW_V0_6_0   ODRIVE_FW_VERSION(0, 6, 0)
#define ODRIVE_FW_V0_6_1   ODRIVE_FW_VERSION(0, 6, 1)
#define ODRIVE_FW_V0_6_2   ODRIVE_FW_VERSION(0, 6, 2)
#define ODRIVE_FW_V0_6_3   ODRIVE_FW_VERSION(0, 6, 3)
#define ODRIVE_FW_V0_6_4   ODRIVE_FW_VERSION(0, 6, 4)
#define ODRIVE_FW_V0_6_5   ODRIVE_FW_VERSION(0, 6, 5)
#define ODRIVE_FW_V0_6_6   ODRIVE_FW_VERSION(0, 6, 6)
#define ODRIVE_FW_V0_6_7   ODRIVE_FW_VERSION(0, 6, 7)
#define ODRIVE_FW_V0_6_8   ODRIVE_FW_VERSION(0, 6, 8)
#define ODRIVE_FW_V0_6_9   ODRIVE_FW_VERSION(0, 6, 9)
#define ODRIVE_FW_V0_6_10  ODRIVE_FW_VERSION(0, 6, 10)
#define ODRIVE_FW_V0_6_11  ODRIVE_FW_VERSION(0, 6, 11)

//========================================================
//      Firmware Version Comparison Helpers
//========================================================

/* Exact match */
#define ODRIVE_FW_VERSION_IS(version) \
    ((ODRIVE_FW_TARGET) == (version))

/* >= comparison */
#define ODRIVE_FW_VERSION_AT_LEAST(version) \
    ((ODRIVE_FW_TARGET) >= (version))

/* < comparison */
#define ODRIVE_FW_VERSION_IS_LOWER(version) ((ODRIVE_FW_TARGET) <= (version))

//========================================================
//      Target Firmware Selection
//========================================================

#ifndef ODRIVE_FW_TARGET
#define ODRIVE_FW_TARGET ODRIVE_FW_V0_5_6
#endif

//========================================================
//      Core Macro's
//========================================================

/*!
 * @brief Bit position to bit mask conversion macro
 *
 * @param[in] n Bit position (0-31)
 * 
 * @return Bitmask with bit n set (value: 2^n)
 */
#define ODRIVE_BIT(n) (1 << n)

#define ODRIVE_BIT64(n) (UINT64_C(1) << (n))

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


#if defined(ODRIVE_ASSERT)
    #define ODRIVE_STATIC_ASSERT(cond, msg) \
        _Static_assert(cond, msg)

#else
    #define ODRIVE_STATIC_ASSERT(cond, msg)

#endif

//========================================================
//      Core Type Definitions
//========================================================

/**
  * @brief Node id for the CAN bus controller
  */
typedef uint16_t    odrive_node_id;

typedef void*       odrive_ctx_t;

typedef uint32_t    odrive_flags;

//========================================================
//      Axis Error Flags
//========================================================

/**
 * @brief ODrive axis error flags
 *
 * Bitfield of axis-level errors (axis.error / axis.active_errors). Multiple
 * bits can be set at once. These indicate faults specific to a single axis
 * including motor, encoder, controller and local power issues.
 *
 * In firmware v0.5.x/v0.6.x, these errors appear in both axis.error (latched)
 * and axis.active_errors (current state). Clear with odrv.clear_errors().
 */
typedef enum odrive_axis_error_T {

#if ODRIVE_FW_VERSION_IS_LOWER(ODRIVE_FW_V0_6_0)
    /**
     * No reported axis error
     */
    AXIS_ERROR_UNKNOWN = 0x0000,

    /**
     * Axis state machine in invalid state.
     *
     * Attempted to enter a state that is not allowed given current
     * configuration or calibration status.
     */
    AXIS_ERROR_INVALID_STATE = ODRIVE_BIT(0u),

#if ODRIVE_FW_VERSION_AT_LEAST(ODRIVE_FW_V0_5_5)

    /**
     * Axis motor error occured
     */
    AXIS_ERROR_MOTOR_FAILED = ODRIVE_BIT(6u),

    /**
     * Sensorless estimator failed
     */
    AXIS_ERROR_SENSORLESS_ESTIMATOR_FAILED = ODRIVE_BIT(7u),

    /**
     * Encoder failed
     * 
     */
    AXIS_ERROR_ENCODER_FAILED = ODRIVE_BIT(8u),

#endif    

    /**
     * The axis watchdog expired 
     */
    AXIS_ERROR_WATCHDOG_TIMER_EXPIRED = ODRIVE_BIT(11u),

    /**
     * The min endstop is pressed 
     */
    AXIS_ERROR_MIN_ENDSTOP_PRESSED = ODRIVE_BIT(12u),

    /**
     * The max endstop is pressed 
     */
    AXIS_ERROR_MAX_ENDSTOP_PRESSED = ODRIVE_BIT(13u),

    /**
     * The estop message was sent over CAN 
     */
    AXIS_ERROR_ESTOP_REQUESTED = ODRIVE_BIT(14u),

    /**
     * The axis is homing without a endstop 
     */
    AXIS_ERROR_HOMING_WITHOUT_ENDSTOP = ODRIVE_BIT(17u),

    /**
     * The axis motor overheated 
     */
    AXIS_ERROR_OVER_TEMP = ODRIVE_BIT(18u),

    /**
     * There isn’t a valid position estimate available. 
     */
    AXIS_ERROR_UNKNOWN_POSITION = ODRIVE_BIT(19u),

#endif

} odrive_axis_error_t;

//========================================================
//      Motor Error Flags
//========================================================

/**
 * @brief ODrive motor error flags
 *
 * Bitfield of motor-specific errors (axis.motor.error). Multiple bits can
 * be set at once. 
 *
 * Motor errors typically trigger AXIS_ERROR_MOTOR_FAILED at the axis level.
 */
typedef enum odrive_motor_error_T {
    /**
     * @brief No motor error.
     *
     * Motor subsystem operating normally.
     */
    MOTOR_ERROR_NONE = 0x0000000000000000,


    /**
     * @brief The measured motor phase resistance is outside of the plausible range.
     */
    MOTOR_ERROR_PHASE_RESISTANCE_OUT_OF_RANGE = ODRIVE_BIT64(0u),

    /**
     * @brief The measured motor phase inductance is outside of the plausible range
     */
    MOTOR_ERROR_PHASE_INDUCTANCE_OUT_OF_RANGE = ODRIVE_BIT(1u),

    /**
     * @brief The gate driver chip reported an error
     */
    MOTOR_ERROR_DRV_FAULT = ODRIVE_BIT(3u),

    /**
     * @brief The closed-loop system missed its deadline
     */
    MOTOR_ERROR_CONTROL_DEADLINE_MISSED = ODRIVE_BIT(4u),

    /**
     * @brief The bus voltage was insufficent to push the requested current through the motor
     */
    MOTOR_ERROR_MODULATION_MAGNITUDE= ODRIVE_BIT(7u),
    
    /**
     * @brief The current sense circuit saturated the current sense amplifier
     */
    MOTOR_ERROR_CURRENT_SENSE_SATURATION = ODRIVE_BIT(10u),

    /**
     * @brief The motor current exceeded motor.config.current_lim + motor.config.current_lim_margin
     */
    MOTOR_ERROR_CURRENT_LIMIT_VIOLATION = ODRIVE_BIT(12u),

    /**
     * @brief The motor modulation is invalid
     */
    MOTOR_ERROR_MODULATION_IS_NAN = ODRIVE_BIT(16u),

    /**
     * @brief The motor thermistor measured a temperature above motor.motor_thermistor.config.temp_limit_upper
     */
    MOTOR_ERROR_THERMISTOR_OVER_TEMP = ODRIVE_BIT(17u),

    /**
     * @brief The inverter thermistor measured a temperature above motor.fet_thermistor.config.temp_limit_upper
     */
    MOTOR_ERROR_FET_THERMISTOR_OVER_TEMP = ODRIVE_BIT(18u),

    /**
     * @brief A timer update event was missed
     */
    MOTOR_ERROR_TIMER_UPDATE_MISSED = ODRIVE_BIT(19u),

    /**
     * @brief The phase current measurement is not available
     */
    MOTOR_ERROR_CURRENT_MEASUREMENT_UNAVAILABLE = ODRIVE_BIT(20u),

    /**
     * @brief The motor was disarmed because the underlying controller failed
     */
    MOTOR_ERROR_CONTROLLER_FAILED = ODRIVE_BIT(21u),

    /**
     * @brief The DC current sourced/sunk by this motor exceeded the configured hard limits
     */
    MOTOR_ERROR_I_BUS_OUT_OF_RANGE = ODRIVE_BIT(22u),

    /**
     * @brief An attempt was made to run the motor PWM while the brake resistor was configured as 
     * enabled (config.enable_brake_resistor) but disarmed
     */
    MOTOR_ERROR_BRAKE_RESISTOR_DISARMED = ODRIVE_BIT(23u),

    /**
     * @brief The motor had to be disarmed because of a system level error
     */
    MOTOR_ERROR_SYSTEM_LEVEL = ODRIVE_BIT(24u),

    /**
     * @brief The main control loop got out of sync with the motor control loop
     */
    MOTOR_ERROR_BAD_TIMING = ODRIVE_BIT(25u),

    /**
     * @brief The current controller did not get a valid angle input
     */
    MOTOR_ERROR_UNKNOWN_PHASE_ESTIMATE = ODRIVE_BIT(26u),

    /**
     * @brief The motor controller did not get a valid phase velocity input
     */
    MOTOR_ERROR_UNKNOWN_PHASE_VEL = ODRIVE_BIT(27u),

    /**
     * @brief The motor controller did not get a valid torque input
     */
    MOTOR_ERROR_UNKNOWN_TORQUE = ODRIVE_BIT(28u),

    /**
     * @brief The current controller did not get a valid current setpoint
     */
    MOTOR_ERROR_UNKNOWN_CURRENT_COMMAND = ODRIVE_BIT(29u),

    /**
     * @brief The current controller did not get a valid current measurement
     */
    MOTOR_ERROR_UNKNOWN_CURRENT_MEASUREMENT = ODRIVE_BIT64(30u),

    /**
     * @brief The current controller did not get a valid ODrive:vbus_voltage measurement
     */
    MOTOR_ERROR_UNKNOWN_VBUS_VOLTAGE = ODRIVE_BIT64(31u),

    /**
     * @brief The current controller did not get a valid feedforward voltage setpoint.
     */
    MOTOR_ERROR_UNKNOWN_VOLTAGE_COMMAND = ODRIVE_BIT64(32u),

    /**
     * @brief The current controller gains were not configured.
     */
    MOTOR_ERROR_UNKNOWN_GAINS = ODRIVE_BIT64(33u),

    /**
     * @brief Internal value used while the controller is not yet ready to generate PWM timings
     */
    MOTOR_ERROR_CONTROLLER_INITIALIZING = ODRIVE_BIT64(34u),

    /**
     * @brief The motor phases are not balanced
     */
    MOTOR_ERROR_UNBALANCED_PHASES = ODRIVE_BIT64(35u),

} odrive_motor_error_t;

//========================================================
//      Encoder Error Flags
//========================================================

/**
 * @brief ODrive encoder error flags
 *
 * Bitfield of encoder-specific errors (axis.encoder.error). Multiple bits
 * can be set at once.
 *
 * Encoder errors typically trigger AXIS_ERROR_ENCODER_FAILED at axis level.
 */
typedef enum odrive_encoder_error_T {
    /**
     * @brief No encoder error.
     *
     * Encoder subsystem operating normally and providing valid feedback.
     */
    ENCODER_ERROR_NONE                      = 0x0000,

    /**
     * @brief Encoder gain/signal unstable.
     *
     * Encoder readings fluctuating excessively or failing consistency
     * checks during calibration.
     */
    ENCODER_ERROR_UNSTABLE_GAIN             = 0x0001,

    /**
     * @brief CPR and pole pairs mismatch.
     *
     * The configured encoder.config.cpr and motor.config.pole_pairs
     * values are incompatible.
     */
    ENCODER_ERROR_CPR_POLEPAIRS_MISMATCH    = 0x0002,

    /**
     * @brief Encoder not responding or no signal detected.
     *
     * No valid encoder signal received during calibration or operation.
     */
    ENCODER_ERROR_NO_RESPONSE               = 0x0004,

    /**
     * @brief Encoder mode not supported or invalid.
     *
     * The configured encoder.config.mode is not supported by firmware
     * or hardware.
     */
    ENCODER_ERROR_UNSUPPORTED_ENCODER_MODE  = 0x0008,

    /**
     * @brief Illegal hall sensor state detected.
     *
     * Hall sensors reported invalid state combination (000 or 111
     * binary).
     */
    ENCODER_ERROR_ILLEGAL_HALL_STATE        = 0x0010,

    /**
     * @brief Encoder index pulse not yet found.
     *
     * Index search (AXIS_STATE_ENCODER_INDEX_SEARCH) did not detect
     * index pulse within expected travel distance. Causes:
     *  - Index signal not connected
     *  - encoder.config.use_index is true but encoder has no index
     *  - Index pulse missed due to noise or timing
     *  - Insufficient search distance before giving up
     */
    ENCODER_ERROR_INDEX_NOT_FOUND_YET       = 0x0020,

    /**
     * @brief SPI absolute encoder communication timeout.
     *
     * SPI transaction with absolute encoder did not complete within
     * timeout period. Indicates:
     *  - Encoder not responding (power/connection issue)
     *  - Wrong encoder.config.abs_spi_cs_gpio_pin
     *  - SPI bus conflict or misconfiguration
     *  - Defective encoder
     */
    ENCODER_ERROR_ABS_SPI_TIMEOUT           = 0x0040,

    /**
     * @brief SPI absolute encoder communication failure.
     *
     * SPI communication completed but data was invalid (parity error,
     * CRC failure, or protocol violation). Causes:
     *  - Electrical noise on SPI bus
     *  - Incorrect encoder mode (wrong SPI protocol selected)
     *  - Loose wiring or poor signal integrity
     *  - Encoder malfunction
     */
    ENCODER_ERROR_ABS_SPI_COM_FAIL          = 0x0080,

    /**
     * @brief SPI absolute encoder not ready.
     *
     * SPI absolute encoder has not completed initialization or is
     * reporting not-ready status. Usually transient during power-up.
     * If persistent, check encoder power supply and initialization
     * timing requirements.
     */
    ENCODER_ERROR_ABS_SPI_NOT_READY         = 0x0100,

    /**
     * @brief Hall encoder not calibrated yet.
     *
     * Hall sensor calibration (polarity and phase) has not been
     * successfully completed. Must run:
     *  - AXIS_STATE_ENCODER_HALL_POLARITY_CALIBRATION
     *  - AXIS_STATE_ENCODER_HALL_PHASE_CALIBRATION
     * Save config afterward to skip calibration on future boots.
     */
    ENCODER_ERROR_HALL_NOT_CALIBRATED_YET   = 0x0200

} odrive_encoder_error_t;

//========================================================
//      Controller Error Flags
//========================================================

/**
 * @brief ODrive controller error flags
 *
 * Bitfield of motion controller errors (axis.controller.error). Multiple
 * bits can be set at once. These indicate faults in trajectory generation,
 * setpoint validation, or control loop stability.
 *
 * Controller errors trigger AXIS_ERROR_CONTROLLER_FAILED at axis level.
 *
 * @see ODrive v0.5.x controller documentation
 */
typedef enum odrive_controller_error_T {
    /**
     * @brief No controller error.
     *
     * Motion controller operating normally.
     */
    CONTROLLER_ERROR_NONE                   = 0x00,

    /**
     * @brief Velocity exceeded limit.
     *
     * Measured velocity exceeded controller.config.vel_limit *
     * controller.config.vel_limit_tolerance. May indicate:
     *  - External force accelerating motor beyond control
     *  - Loss of load allowing runaway
     *  - Control loop instability
     *  - Incorrect velocity estimation
     * Disable with controller.config.enable_overspeed_error = false.
     */
    CONTROLLER_ERROR_OVERSPEED              = 0x01,

    /**
     * @brief Invalid input mode selected.
     *
     * The configured controller.config.input_mode is not valid or
     * not supported by current control mode. Verify input_mode
     * setting is compatible with control_mode.
     */
    CONTROLLER_ERROR_INVALID_INPUT_MODE     = 0x02,

    /**
     * @brief Control gains unstable.
     *
     * Controller detected instability or oscillation, typically during
     * gain scheduling or autotuning. May indicate:
     *  - controller.config.pos_gain too high
     *  - controller.config.vel_gain too high
     *  - Mechanical resonance
     * Reduce gains or enable gain scheduling with appropriate parameters.
     */
    CONTROLLER_ERROR_UNSTABLE_GAIN          = 0x04,

    /**
     * @brief Invalid mirror axis configuration.
     *
     * controller.config.axis_to_mirror specifies non-existent axis
     * or circular mirroring dependency. For dual-axis systems, ensure
     * mirror_axis points to valid axis (0 or 1) or 255 for disabled.
     */
    CONTROLLER_ERROR_INVALID_MIRROR_AXIS    = 0x08,

    /**
     * @brief Invalid load encoder configuration.
     *
     * controller.config.load_encoder_axis specifies invalid axis for
     * load encoder feedback. Used in dual-encoder setups where
     * commutation uses one encoder and position control uses another.
     * Set to same axis number for single-encoder, or valid second
     * axis for dual-encoder.
     */
    CONTROLLER_ERROR_INVALID_LOAD_ENCODER   = 0x10,

    /**
     * @brief Invalid position/velocity estimate.
     *
     * Controller received NaN, infinite, or otherwise invalid
     * estimate from encoder or sensorless estimator. Indicates
     * upstream encoder or estimator fault. Check encoder.error
     * or sensorless_estimator.error for root cause.
     */
    CONTROLLER_ERROR_INVALID_ESTIMATE       = 0x20

} odrive_controller_error_t;

//========================================================
//      Sensorless Estimator Error Flags
//========================================================

/**
 * @brief ODrive sensorless estimator error flags
 *
 * Bitfield of sensorless estimator errors (axis.sensorless_estimator.error).
 * Multiple bits can be set at once. These indicate faults in sensorless
 * (encoder-less) position/velocity estimation using motor back-EMF.
 *
 * Only relevant when operating in AXIS_STATE_SENSORLESS_CONTROL mode.
 * Estimator errors trigger AXIS_ERROR_SENSORLESS_ESTIMATOR_FAILED.
 *
 * @see ODrive v0.5.x sensorless mode documentation
 *      https://docs.odriverobotics.com/v/0.6.1/fibre_types/com_odriverobotics_ODrive.html#ODrive.SensorlessEstimator
 */
typedef enum odrive_sensorless_estimate_error_T {
    /**
     * @brief No sensorless estimator error.
     *
     * Sensorless estimator operating normally (if active).
     */
    SENSORLESS_ESTIMATOR_ERROR_NONE                 = 0x00,

    /**
     * @brief Estimator gain unstable or diverging.
     *
     * Sensorless estimator detected instability in observer or PLL.
     * Common causes:
     *  - sensorless_estimator.config.observer_gain too high
     *  - sensorless_estimator.config.pll_bandwidth too high
     *  - Motor speed too low (below ~200 RPM)
     *  - Incorrect sensorless_estimator.config.pm_flux_linkage
     *  - Excessive motor load causing stall
     * Sensorless control requires minimum speed for back-EMF detection.
     */
    SENSORLESS_ESTIMATOR_ERROR_UNSTABLE_GAIN        = 0x01,

    /**
     * @brief Unknown or invalid current command.
     *
     * Sensorless estimator received unexpected current command that
     * does not match any known operating mode. Internal firmware error;
     * should not occur in normal operation.
     */
    SENSORLESS_ESTIMATOR_ERROR_UNKNOWN_CURRENT_COMMAND = 0x02

} odrive_sensorless_estimate_error_t;

//========================================================
//     ODrive Error Flags
//========================================================

/**
 * @brief ODrive top-level error flags
 *
 * Bitfield of system-wide ODrive errors (odrv.error). Multiple bits can be
 * set at once. These indicate faults at the controller, DC bus or global
 * configuration level rather than per-axis details.
 *
 * Top-level errors affect entire ODrive board and may prevent operation of
 * all axes. Clear with odrv.clear_errors() after resolving root cause.
 *
 * @see ODrive Error documentation (v0.6.x format applies to v0.5.x):
 *      https://docs.odriverobotics.com/v/latest/fibre_types/com_odriverobotics_ODrive.html#ODrive.Error
 */
typedef enum odrive_error_T {
    /**
     * @brief No system error.
     *
     * ODrive controller operating normally at system level.
     */
    ODRIVE_ERROR_NONE                       = 0x00000000,

    /**
     * @brief System initializing or reconfiguring.
     *
     * Transient state during boot sequence or when major configuration
     * changes are being applied. Not an actual fault; indicates
     * incomplete initialization.
     */
    ODRIVE_ERROR_INITIALIZING               = 0x00000001,

    /**
     * @brief Unexpected low-level system error.
     *
     * Severe firmware fault such as:
     *  - Memory corruption detected
     *  - Stack overflow
     *  - Thread deadlock or freeze
     *  - Failed assertion
     * Indicates firmware bug or hardware fault. System cannot operate
     * safely. Requires power cycle and potential firmware reflash.
     */
    ODRIVE_ERROR_SYSTEM_LEVEL               = 0x00000002,

    /**
     * @brief Internal hard real-time timing violation.
     *
     * Critical timing deadline was missed, typically in motor control
     * or communication interrupt. Causes:
     *  - Computational overload (too many axes, too high bandwidth)
     *  - Firmware bug
     *  - Hardware fault (CPU, memory)
     * Similar severity to SYSTEM_LEVEL; should not occur in normal
     * operation within device specifications.
     */
    ODRIVE_ERROR_TIMING_ERROR               = 0x00000004,

    /**
     * @brief Required estimate (pos/vel/phase) invalid or missing.
     *
     * System needed position, velocity, or electrical phase estimate
     * but none was available or valid. Common causes:
     *  - Encoder not calibrated (run calibration sequence)
     *  - Absolute position control before homing
     *  - Encoder misbehaving or disconnected
     *  - Sensorless mode failed (speed too low)
     * Prevent by ensuring encoder calibration complete before
     * closed-loop operation.
     */
    ODRIVE_ERROR_MISSING_ESTIMATE           = 0x00000008,

    /**
     * @brief Global configuration invalid or incomplete.
     *
     * ODrive configuration contains errors such as:
     *  - motor.config.direction not -1 or +1
     *  - Inconsistent torque limits (soft_min > soft_max)
     *  - Brake resistor enabled but resistance = 0
     *  - Invalid phase resistance/inductance values
     *  - Incompatible parameter combinations
     * Review all config parameters, especially those recently changed.
     * Use odrivetool to validate configuration.
     */
    ODRIVE_ERROR_BAD_CONFIG                 = 0x00000010,

    /**
     * @brief Gate driver (DRV8301/DRV8305) fault.
     *
     * Gate driver chip reported fault via nFAULT pin. Retrieve
     * specific fault code with odrv.get_drv_fault(). Common causes:
     *  - Phase short to ground or Vbus
     *  - Gate driver overtemperature
     *  - Gate driver undervoltage
     *  - Bootstrap capacitor issue
     * Persistent faults under normal conditions indicate hardware
     * damage. Check for damaged FETs, shorted motor phases, or
     * failed gate driver IC.
     */
    ODRIVE_ERROR_DRV_FAULT                  = 0x00000020,

    /**
     * @brief Required control input not provided.
     *
     * No valid value received for required input (input_pos, input_vel,
     * or input_torque) before entering closed-loop control. Common with:
     *  - RC PWM input mode without pulse detected
     *  - Step/dir mode without step pulse
     *  - CAN/UART control without initial setpoint
     * Ensure input signal present and correctly configured before
     * requesting CLOSED_LOOP_CONTROL.
     */
    ODRIVE_ERROR_MISSING_INPUT              = 0x00000040,

    /**
     * @brief DC bus voltage exceeded overvoltage limit.
     *
     * Vbus rose above config.dc_bus_overvoltage_trip_level. Usually
     * during regenerative braking when brake resistor cannot dissipate
     * power fast enough. Causes:
     *  - No brake resistor installed but config.brake_resistance != 0
     *  - Brake resistor undersized (resistance too high)
     *  - Brake resistor wiring disconnected
     *  - config.brake_resistance value incorrect (too high)
     * Verify: (Vbus_max / brake_resistance) > max_regen_current
     */
    ODRIVE_ERROR_DC_BUS_OVER_VOLTAGE        = 0x00000100,

    /**
     * @brief DC bus voltage fell below undervoltage limit.
     *
     * Vbus dropped below config.dc_bus_undervoltage_trip_level during
     * operation. Causes:
     *  - Power supply capacity insufficient
     *  - High resistance in power wiring
     *  - Loose power connections
     *  - Aggressive acceleration/torque demand exceeding PSU capability
     *  - Undersized PSU for motor current draw
     * For battery operation, may indicate low battery. For PSU, check
     * wiring gauge and connection quality, or reduce current/velocity
     * limits to lower power demand.
     */
    ODRIVE_ERROR_DC_BUS_UNDER_VOLTAGE       = 0x00000200,

    /**
     * @brief DC bus positive (motoring) current limit exceeded.
     *
     * DC bus current from PSU exceeded limit, either:
     *  - Per-axis: axis.motor.I_bus > axis.config.I_bus_hard_max
     *  - Global: odrv.ibus > config.dc_max_positive_current
     * Indicates current demand exceeds configured safe limits. May
     * damage power supply or wiring. Reduce motor.config.current_lim
     * or increase dc_max_positive_current if PSU supports it.
     */
    ODRIVE_ERROR_DC_BUS_OVER_CURRENT        = 0x00000400,

    /**
     * @brief DC bus negative (regenerative) current limit exceeded.
     *
     * Regenerative current back to PSU exceeded limit, either:
     *  - Per-axis: axis.motor.I_bus < axis.config.I_bus_hard_min
     *  - Global: odrv.ibus < config.dc_max_negative_current
     * Occurs during braking if brake resistor cannot handle power.
     * Solutions:
     *  - Install/upgrade brake resistor
     *  - Increase config.dc_max_negative_current if PSU can sink current
     *  - Reduce deceleration rates
     * Verify: (Vbus / brake_resistance) > motor.current_hard_max
     */
    ODRIVE_ERROR_DC_BUS_OVER_REGEN_CURRENT  = 0x00000800,

    /**
     * @brief Motor phase current exceeded hard limit.
     *
     * Measured motor current went beyond motor.config.current_hard_max
     * (or config.inverterN.current_hard_max on S1). Current controller
     * targets current_soft_max with margin for overshoot, but if hard
     * limit is reached, protection triggers. Causes:
     *  - Insufficient margin between soft and hard limits (<20%)
     *  - Current controller instability (wrong motor R/L parameters)
     *  - Motor stall or jam (excessive load)
     *  - Phase short circuit
     * Increase margin to 40% or investigate control loop stability.
     */
    ODRIVE_ERROR_CURRENT_LIMIT_VIOLATION    = 0x00001000,

    /**
     * @brief Motor over-temperature.
     *
     * Motor thermistor reading exceeded
     * motor_thermistor.config.temp_limit_upper. Motor winding
     * temperature too high. Causes:
     *  - Excessive current (I²R heating)
     *  - Insufficient cooling (blocked airflow, no heatsink)
     *  - High ambient temperature
     *  - Continuous high-torque operation beyond motor rating
     * Allow motor to cool before clearing error. Reduce duty cycle,
     * current limit, or improve cooling.
     */
    ODRIVE_ERROR_MOTOR_OVER_TEMP            = 0x00002000,

    /**
     * @brief Inverter/FET over-temperature.
     *
     * Power stage thermistor reading exceeded
     * fet_thermistor.config.temp_limit_upper. ODrive FETs overheating.
     * Causes:
     *  - Excessive current (FET conduction loss)
     *  - High PWM frequency (switching loss)
     *  - Insufficient cooling (blocked airflow, no heatsink)
     *  - High ambient temperature
     *  - Undersized ODrive for continuous motor current
     * Allow ODrive to cool. Reduce current limit, add cooling, or
     * upsize to higher-current ODrive model.
     */
    ODRIVE_ERROR_INVERTER_OVER_TEMP         = 0x00004000,

    /**
     * @brief Velocity exceeded limit.
     *
     * Measured velocity exceeded controller.config.vel_limit *
     * controller.config.vel_limit_tolerance. Protection against
     * runaway or overspeed. May indicate:
     *  - Load disconnected (no-load runaway)
     *  - External force driving motor
     *  - Control loop instability
     * Can be disabled with controller.config.enable_overspeed_error = false
     * if overspeed protection not needed.
     */
    ODRIVE_ERROR_VELOCITY_LIMIT_VIOLATION   = 0x00008000,

    /**
     * @brief Position exceeded software limits.
     *
     * Position estimate went beyond configured software position limits.
     * Requires limits enabled in controller configuration. Used to
     * prevent travel beyond mechanical limits. Configure limits and
     * perform homing to establish absolute reference.
     */
    ODRIVE_ERROR_POSITION_LIMIT_VIOLATION   = 0x00010000,

    /**
     * @brief Axis watchdog timer expired.
     *
     * Watchdog not fed within axis.config.watchdog_timeout period,
     * indicating loss of communication or host failure. Motor disarmed
     * to prevent uncontrolled operation. Feed watchdog periodically
     * with axis.watchdog_feed() when watchdog enabled.
     */
    ODRIVE_ERROR_WATCHDOG_TIMER_EXPIRED     = 0x01000000,

    /**
     * @brief Emergency stop requested.
     *
     * E-stop triggered by:
     *  - CAN e-stop message received
     *  - Endstop configured as e-stop activated
     *  - Software e-stop command
     * All axes disarm immediately. Clear errors and verify safety
     * before resuming operation. E-stop is latching; requires manual
     * error clearing.
     */
    ODRIVE_ERROR_ESTOP_REQUESTED            = 0x02000000,

    /**
     * @brief Spinout detected (loss of traction).
     *
     * Mismatch between electrical power and mechanical power indicates
     * spinout (wheels slipping, loss of load coupling). Used in
     * robotics to detect traction loss. Requires spinout detection
     * enabled and tuned. See spinout detection configuration in docs.
     */
    ODRIVE_ERROR_SPINOUT_DETECTED           = 0x04000000,

    /**
     * @brief Brake resistor disarmed.
     *
     * Brake resistor safety interlock triggered due to another fault
     * (commonly undervoltage or overtemperature). Axis cannot operate
     * without functional brake resistor if regen power expected.
     * Resolve root cause (undervoltage, brake overtemp) then clear
     * errors to re-arm.
     */
    ODRIVE_ERROR_BRAKE_RESISTOR_DISARMED    = 0x08000000,

    /**
     * @brief Motor thermistor disconnected.
     *
     * Motor thermistor enabled but appears disconnected. Analog reading
     * saturated near 0V or 3.3V indicating open circuit. Check:
     *  - Thermistor wiring and connections
     *  - Correct thermistor GPIO pin configured
     *  - Thermistor not damaged
     * Cannot safely monitor motor temperature without functional
     * thermistor if enabled.
     */
    ODRIVE_ERROR_THERMISTOR_DISCONNECTED    = 0x10000000,

    /**
     * @brief Calibration procedure failed.
     *
     * A calibration routine (motor, encoder, anticogging, etc.) failed
     * to complete successfully. Check axis.procedure_result for specific
     * failure reason. Common causes:
     *  - Motor not connected or wrong type
     *  - Encoder not connected or wrong mode
     *  - Insufficient bus voltage
     *  - Mechanical obstruction preventing motion
     * Resolve indicated issue and retry calibration.
     */
    ODRIVE_ERROR_CALIBRATION_ERROR          = 0x40000000

} odrive_error_t;

//========================================================
//      ODrive CAN Commands
//========================================================

/**
 * @brief ODrive CAN Protocol Command IDs
 *
 * Defines the CAN message command identifiers for the ODrive CANSimple protocol.
 * The CAN message ID is constructed as: (node_id << 5) | cmd_id
 *
 * @note All values are encoded in little endian
 * @note Floats use IEEE 754 standard format
 * 
 * @see ODrive CAN Protocol Documentation: https://docs.odriverobotics.com/v/latest/manual/can-protocol.html
 */
typedef enum odrive_can_command_T {
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
} odrive_can_command_t;

//========================================================
//      ODrive Motor Types
//========================================================

/**
 * @brief Motor type selection (axis.motor.config.motor_type)
 *
 * Motor types used for describing wich motor will be used by the ODrive controller.
 *
 * @see ODrive Error documentation:
 *      https://docs.odriverobotics.com/v/latest/fibre_types/com_odriverobotics_ODrive.html#ODrive.MotorType
 */
typedef enum {
    /**< Used for Permanant Magnet AC (PMAC), Brushless DC (BLDC) and Permanent Magnet Synchronous Motors (PMSM). */
    MOTOR_TYPE_PMSM_CURRENT_CONTROL     = 0x00,
    
    /**< Similar to MOTOR_TYPE_PMSM_CURRENT_CONTROL, but bypasses the closed loop current controller, using the feedforward term V=IR only */
    MOTOR_TYPE_PMSM_CURRENT_VOLTAGE     = 0x02,
    
    /**< Used for FOC control of AC Induction Motors (ACIM), aka Asynchronous motors. */
    MOTOR_TYPE_ACIM                     = 0x03,
} odrive_motor_type;

//========================================================
//      ODrive Input modes
//========================================================

/**
 * @brief Input mode selection (axis.controller.config.input_mode)
 *
 * Determines how input setpoints are processed before being passed to
 * the controller. Different modes provide filtering, ramping, or
 * trajectory planning.
 *
 * @see ODrive v0.6.1 InputMode documentation:
 *      https://docs.odriverobotics.com/v/0.6.1/fibre_types/com_odriverobotics_ODrive.html#ODrive.Controller.InputMode
 */
typedef enum {
    /**
     * @brief Inactive - inputs disabled, setpoints frozen.
     *
     * All inputs are ignored and setpoints retain their last value.
     * Motor continues to track the frozen setpoint.
     *
     * Valid inputs: None
     * Valid control modes: All
     */
    INPUT_MODE_INACTIVE = 0,

    /**
     * @brief Passthrough - direct input to setpoint.
     *
     * Input values pass directly through to setpoints without filtering
     * or processing. Fastest response but no smoothing.
     *
     * Valid inputs: input_pos, input_vel, input_torque
     * Valid control modes: All (VOLTAGE_CONTROL, TORQUE_CONTROL,
     *                           VELOCITY_CONTROL, POSITION_CONTROL)
     */
    INPUT_MODE_PASSTHROUGH = 1,

    /**
     * @brief Velocity ramping.
     *
     * Ramps velocity command from current value to target value at
     * controlled acceleration rate. Smooths velocity changes.
     *
     * Configuration:
     *  - config.vel_ramp_rate [turn/s²] - Max acceleration/deceleration
     *  - config.inertia [N·m/(turn/s²)] - System inertia
     *
     * Valid inputs: input_vel
     * Valid control modes: VELOCITY_CONTROL
     */
    INPUT_MODE_VEL_RAMP = 2,

    /**
     * @brief Position filter (2nd order).
     *
     * Applies 2nd order filter to position commands for smooth tracking.
     * Reduces mechanical shock from step changes. Ideal for step/dir
     * interface or discrete position commands.
     *
     * Configuration:
     *  - config.input_filter_bandwidth [Hz] - Filter cutoff frequency
     *  - config.inertia [N·m/(turn/s²)] - System inertia
     *
     * Valid inputs: input_pos
     * Valid control modes: POSITION_CONTROL
     */
    INPUT_MODE_POS_FILTER = 3,

    /**
     * @brief Channel mixing (not implemented).
     *
     * Reserved for future use. Do not use.
     */
    INPUT_MODE_MIX_CHANNELS = 4,

    /**
     * @brief Trapezoidal trajectory planner.
     *
     * Online trajectory generation with trapezoidal velocity profile.
     * Automatically plans acceleration, constant velocity, and
     * deceleration phases to reach target position smoothly.
     *
     * Configuration:
     *  - trap_traj.config.vel_limit [turn/s] - Max velocity
     *  - trap_traj.config.accel_limit [turn/s²] - Max acceleration
     *  - trap_traj.config.decel_limit [turn/s²] - Max deceleration
     *  - config.inertia [N·m/(turn/s²)] - System inertia
     *
     * Valid inputs: input_pos
     * Valid control modes: POSITION_CONTROL
     */
    INPUT_MODE_TRAP_TRAJ = 5,

    /**
     * @brief Torque ramping.
     *
     * Ramps torque command from current value to target value at
     * controlled rate. Reduces mechanical shock and current spikes.
     *
     * Configuration:
     *  - config.torque_ramp_rate [Nm/s] - Max torque change rate
     *
     * Valid inputs: input_torque
     * Valid control modes: TORQUE_CONTROL
     */
    INPUT_MODE_TORQUE_RAMP = 6,

    /**
     * @brief Electronic mirroring.
     *
     * Mirrors movement of another axis with fixed ratio. Useful for
     * mechanically coupled axes (e.g., dual-motor gantry). Target
     * axis follows source axis encoder estimates automatically.
     *
     * Configuration:
     *  - config.axis_to_mirror - Source axis number (0 or 1)
     *  - config.mirror_ratio - Position scaling factor
     *
     * Valid inputs: None (reads from source axis encoder)
     * Valid control modes: POSITION_CONTROL
     */
    INPUT_MODE_MIRROR = 7,

    /**
     * @brief Tuning mode (for controller tuning).
     *
     * Generates sine wave reference signal at specified frequency
     * with 1 turn amplitude. Used for measuring frequency response
     * and tuning control gains.
     *
     * Set control_mode for loop to tune, then configure frequency.
     * ODrive generates sine wave input automatically.
     *
     * Valid inputs: Automatic sine generation
     * Valid control modes: All (depends on tuning target)
     */
    INPUT_MODE_TUNING = 8

} odrive_input_mode_t;

//========================================================
//      ODrive State's
//========================================================

/**
 * @brief ODrive alive state
 *
 * Represents whether the ODrive controller is currently responsive,
 * based on periodic heartbeat messages. Useful for monitoring connection
 * health, detecting timeouts, and determining whether a reboot or power
 * cycle has occurred.
 *
 * This is an application-level abstraction. ODrive firmware does not
 * provide a direct alive-state enum; instead, the alive state is inferred
 * from heartbeat timing and reconnect behavior.
 */
typedef enum {

    /**
     * @brief Invalid / uninitialized state.
     *
     * Indicates that no heartbeat has been received yet, or that the
     * monitoring system has not been started. This state exists to
     * distinguish between "never connected" and "connected but timed out."
     *
     * Typical causes:
     *  - Startup before the first heartbeat arrives
     *  - Monitoring system not initialized
     *  - Invalid or corrupt state
     */
    ALIVE_STATE_INVALID = 0,

    /**
     * @brief Active - controller is alive and responsive.
     *
     * Heartbeats are received within the expected time window (typically
     * every 100 ms for ODrive). The controller is considered online and
     * communication is healthy.
     *
     * Source of state change:
     *  - Heartbeat received on time
     *
     * Expected behavior:
     *  - Commands may be issued reliably
     *  - Axis states may be monitored in real time
     */
    ALIVE_STATE_ACTIVE = 1,

    /**
     * @brief Timeout - controller has stopped responding.
     *
     * Indicates that no heartbeat has been received within the configured
     * timeout threshold. The ODrive is presumed offline, disconnected,
     * powered down, or rebooting.
     *
     * Typical causes:
     *  - Heartbeat timeout
     *  - CAN cable unplugged
     *  - Power loss
     *  - Reboot in progress
     *  - Firmware crash
     *
     * Recovery behavior:
     *  - State may transition back to ACTIVE when heartbeats resume
     */
    ALIVE_STATE_TIMEOUT = 2,

    /**
     * @brief Reboot detected.
     *
     * Indicates that the ODrive has recently restarted. This state is
     * typically inferred when a heartbeat resumes after a timeout and
     * reports a rebooted uptime value or reset counters.
     *
     * This is optional logic and depends on how the application interprets
     * heartbeat data. It can be used to trigger controller reconfiguration
     * after a power cycle.
     *
     * Typical use cases:
     *  - Automatic reconfiguration of ODrive axis parameters
     *  - Logging unexpected restarts for diagnostics
     */
    ALIVE_STATE_REBOOT = 3

} odrive_alive_state_t;

//========================================================
//      ODrive Result codes
//========================================================

/**
 * @brief Procedure result codes (axis.procedure_result)
 *
 * Indicates the outcome of the last high-level procedure executed on the
 * axis (e.g. calibration, homing, etc.). This is reported in the heartbeat
 * and can be inspected after a procedure completes or fails.
 *
 * Typical usage:
 *  - Check this after changing axis state (e.g. MOTOR_CALIBRATION,
 *    ENCODER_OFFSET_CALIBRATION, HOMING, etc.).
 *  - On DISARMED, inspect axis.disarm_reason and axis.error for details.
 * 
 * @see ODrive v0.6.1 InputMode documentation:
 *      https://docs.odriverobotics.com/v/latest/fibre_types/com_odriverobotics_ODrive.html#ODrive.ProcedureResult
 */
typedef enum {
    /**
     * @brief Success - procedure completed without faults.
     *
     * The requested procedure finished normally and the axis remained
     * in an armed state. No corrective action is required.
     */
    PROCEDURE_RESULT_SUCCESS = 0,

    /**
     * @brief Busy - procedure still in progress.
     *
     * The requested procedure has not yet finished. This is typically
     * reported while a calibration, homing or other long-running
     * operation is still executing.
     *
     * Action:
     *  - Wait until the procedure finishes (result changes to SUCCESS
     *    or an error code).
     *  - Avoid starting conflicting procedures while BUSY.
     */
    PROCEDURE_RESULT_BUSY = 1,

    /**
     * @brief Cancelled by user.
     *
     * The last procedure was explicitly cancelled by the user, for
     * example by changing axis state or issuing a command that aborts
     * the current task.
     *
     * Action:
     *  - Re-run the procedure if it is still required.
     */
    PROCEDURE_RESULT_CANCELLED = 2,

    /**
     * @brief Disarmed due to fault.
     *
     * A fault occurred during the procedure and the axis was disarmed
     * for safety.
     *
     * Details:
     *  - See axis.disarm_reason for the high-level disarm cause.
     *  - See axis.error and sub-component error fields for specifics.
     *
     * Action:
     *  - Diagnose the underlying error (wiring, configuration, mechanical
     *    issues, etc.) before re-arming the axis.
     */
    PROCEDURE_RESULT_DISARMED = 3,

    /**
     * @brief No response from component.
     *
     * A procedure component did not respond as expected. This is most
     * commonly caused by encoder problems (not powered, not connected,
     * incorrect configuration, etc.).
     *
     * Typical causes:
     *  - Encoder not powered or wrong supply.
     *  - Missing / swapped encoder signals (A/B/Z, Hall, SPI, etc.).
     *  - Incorrect encoder type or config (cpr, mode, pins).
     *
     * Action:
     *  - Verify encoder wiring and power.
     *  - Verify encoder configuration matches the hardware.
     */
    PROCEDURE_RESULT_NO_RESPONSE = 4,

    /**
     * @brief Pole pair / encoder CPR mismatch.
     *
     * The configured motor pole pairs and/or incremental encoder CPR
     * do not match the measured motion during calibration.
     *
     * Typical causes:
     *  - config.motor.pole_pairs incorrect for the motor.
     *  - inc_encoder0.config.cpr incorrect for the encoder.
     *
     * Action:
     *  - Verify motor pole pair count.
     *  - Verify encoder CPR and configuration.
     *  - See observed_encoder_scale_factor for further diagnostics.
     *
     * Note:
     *  - If not using an incremental encoder, the CPR value is not used
     *    and can be ignored.
     */
    PROCEDURE_RESULT_POLE_PAIR_CPR_MISMATCH = 5,

    /**
     * @brief Phase resistance out of range.
     *
     * The measured motor phase resistance is outside the plausible range
     * or the calibration parameters are unsuitable.
     *
     * Typical causes:
     *  - Loose, missing or shorted motor phase connection.
     *  - Motor with very low or very high resistance relative to
     *    calibration settings.
     *  - config.motor.resistance_calib_max_voltage too low or too high
     *    for the motor and bus voltage.
     *
     * Useful relations:
     *  - resistance_calib_max_voltage > calibration_current * phase_resistance
     *  - resistance_calib_max_voltage < 0.5 * vbus_voltage
     *
     * Action:
     *  - Check all motor phase connections.
     *  - Inspect phase_resistance and phase_inductance values.
     *  - Adjust resistance_calib_max_voltage and/or calibration_current,
     *    especially for small or gimbal-type motors.
     */
    PROCEDURE_RESULT_PHASE_RESISTANCE_OUT_OF_RANGE = 6,

    /**
     * @brief Phase inductance out of range.
     *
     * The measured motor phase inductance is outside the plausible range
     * or calibration parameters are not appropriate.
     *
     * Typical causes:
     *  - Similar to PHASE_RESISTANCE_OUT_OF_RANGE: incorrect wiring,
     *    inappropriate calibration settings, or a motor outside the
     *    expected inductance range.
     *
     * Action:
     *  - Check motor wiring and connections.
     *  - Adjust calibration_current and resistance_calib_max_voltage as
     *    needed and repeat calibration.
     *  - See motor.phase_inductance and phase_resistance readings for
     *    reference.
     */
    PROCEDURE_RESULT_PHASE_INDUCTANCE_OUT_OF_RANGE = 7,

    /**
     * @brief Unbalanced motor phases.
     *
     * The measured motor phase resistances are not balanced. This usually
     * indicates wiring or motor issues.
     *
     * Typical causes:
     *  - Bad crimp or solder joint on one phase.
     *  - Damaged motor winding.
     *  - Asymmetric cable or connector failures.
     *
     * Action:
     *  - Verify all phase connections and continuity.
     *  - Inspect motor and cabling for damage.
     */
    PROCEDURE_RESULT_UNBALANCED_PHASES = 8,

    /**
     * @brief Invalid motor type.
     *
     * The configured motor type is not a valid entry in the MotorType
     * enum or is unsupported by the firmware.
     *
     * Typical causes:
     *  - config.motor.motor_type set to an undefined or reserved value.
     *
     * Action:
     *  - Set motor_type to a valid value (e.g. high current, gimbal,
     *    high torque, etc.) matching the hardware and firmware version.
     */
    PROCEDURE_RESULT_INVALID_MOTOR_TYPE = 9,

    /**
     * @brief Illegal Hall state during calibration.
     *
     * During Hall sensor calibration, the controller observed too many
     * invalid or illegal Hall states (combinations of Hall inputs that
     * should never occur).
     *
     * Typical causes:
     *  - Incorrect Hall wiring or pin assignment.
     *  - Excessive electrical noise or poor signal integrity.
     *  - Damaged Hall sensors.
     *
     * Action:
     *  - Verify Hall wiring and pull-ups.
     *  - Ensure clean signals and proper grounding.
     *  - Re-run calibration after fixing wiring.
     */
    PROCEDURE_RESULT_ILLEGAL_HALL_STATE = 10,

    /**
     * @brief Timeout.
     *
     * The procedure did not complete within the expected time limit.
     *
     * Typical causes:
     *  - Mechanically blocked motion (stall, jam, endstop hit).
     *  - Excessive load preventing the requested movement.
     *  - Very slow response due to configuration or hardware issues.
     *
     * Action:
     *  - Check mechanical system for binding or endstops.
     *  - Inspect motor current limits, torque capability and load.
     */
    PROCEDURE_RESULT_TIMEOUT = 11,

    /**
     * @brief Homing requested without endstop.
     *
     * A homing procedure was requested but no endstop was enabled for
     * the direction in which the axis is trying to home.
     *
     * Typical causes:
     *  - <axis>.min_endstop.config.enabled (or appropriate endstop)
     *    is false.
     *
     * Action:
     *  - Enable the relevant endstop in configuration.
     *  - Verify wiring and endstop detection.
     */
    PROCEDURE_RESULT_HOMING_WITHOUT_ENDSTOP = 12,

    /**
     * @brief Invalid state requested.
     *
     * The requested axis state is not a valid AxisState value or is not
     * allowed in the current configuration.
     *
     * Typical causes:
     *  - Requesting an undefined or unsupported state.
     *  - Incompatible transitions (e.g. skipping required calibrations).
     *
     * Action:
     *  - Ensure the requested state is a valid AxisState enum value.
     *  - Follow the recommended state transition sequence (e.g. run
     *    calibrations before CLOSED_LOOP_CONTROL).
     */
    PROCEDURE_RESULT_INVALID_STATE = 13,

    /**
     * @brief Required calibration not completed.
     *
     * The requested state could not be entered because the axis is not
     * fully calibrated for the current configuration and control mode.
     *
     * Typical causes:
     *  - Skipping MOTOR_CALIBRATION or ENCODER_OFFSET_CALIBRATION before
     *    requesting CLOSED_LOOP_CONTROL.
     *  - Configuration changes that require re-calibration.
     *
     * Action:
     *  - Run the required calibration sequence for the current setup.
     *  - Ensure all necessary calibrations complete with SUCCESS.
     */
    PROCEDURE_RESULT_NOT_CALIBRATED = 14,

    /**
     * @brief Calibration did not converge.
     *
     * The calibration procedure failed to converge to a reliable
     * solution. Measured quantities did not reach sufficient statistical
     * significance or consistency.
     *
     * Typical causes:
     *  - Encoder did not move sufficiently or consistently during
     *    ENCODER_OFFSET_CALIBRATION.
     *  - Excessive mechanical play, noise or backlash.
     *  - Inappropriate calibration parameters (speed, current, etc.).
     *
     * Action:
     *  - Verify mechanical setup and encoder mounting.
     *  - Adjust calibration parameters (current, speed, duration).
     *  - Ensure the axis can move freely during calibration.
     * 
     * @note This enum is not available in ODrive v0.6.1
     */
    PROCEDURE_RESULT_NOT_CONVERGING = 15

} odrive_procedure_result_t;

//========================================================
//      ODrive Reboot Actions
//========================================================

/**
 * @brief ODrive reboot actions.
 *
 * These values define what action the ODrive should perform when a
 * CAN-based Reboot command is issued. The action is passed as the
 * payload to the ODrive Reboot message (cmd_id = 0x16).
 *
 * Actions include simple rebooting, saving/erasing configuration,
 * or entering DFU (firmware upgrade) mode.
 *
 * @note Refer to the ODrive v0.6.11 CAN protocol documentation:
 *       https://docs.odriverobotics.com/v/latest/manual/can-protocol.html#reboot
 */
typedef enum {
    /**
     * @brief Reboot the ODrive normally.
     *
     * Performs a standard MCU reset without modifying configuration
     * or entering any special bootloader mode.
     */
    REBOOT_ACTION_REBOOT         = 0x00000000,

    /**
     * @brief Save the current configuration to persistent storage, then reboot.
     *
     * Equivalent to issuing a "save configuration" command followed
     * by a restart. Useful after modifying parameters via CAN.
     */
    REBOOT_ACTION_SAVE_CONFIG    = 0x00000001,

    /**
     * @brief Erase the configuration (factory reset), then reboot.
     *
     * Clears all saved configuration values and restarts the board.
     * After reboot, the ODrive will return to default settings.
     */
    REBOOT_ACTION_ERASE_CONFIG   = 0x00000002,

    /**
     * @brief Enter DFU (Device Firmware Update) mode.
     *
     * Reboots into the bootloader for firmware flashing over USB.
     * The device will not return to normal operation until reset.
     */
    REBOOT_ACTION_DFU_MODE       = 0x00000003,

} odrive_reboot_action_t;

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
//      ODrive Handles
//========================================================

/**
 * @brief Opaque handle to an ODrive driver instance
 * 
 * @note Obtained via ODriveDriver_Create(). Do not dereference directly.
 */
ODRIVE_DEFINE_HANDLE(odrive_driver);

/**
 * @brief Opaque handle to an ODrive axis instance
 * 
 * @note Obtained via ODriveDriver_Create(). Do not dereference directly.
 */
ODRIVE_DEFINE_HANDLE(odrive_axis);

/**
 * @brief Opaque handle to an ODrive CAN driver instance
 * 
 * @note Obtained via ODriveDriver_Create(). Do not dereference directly.
 */
ODRIVE_DEFINE_HANDLE(odrive_can_driver);

// ========================================================
// ODrive Core Structs
// ========================================================

/**
 * @brief Encoder estimate frame (position and velocity)
 * 
 * @details Contains current position and velocity estimates from the encoder
 * or sensorless estimator.
 */
typedef struct  {
    float Position;
    float Velocity;
} encoder_estimate_frame;

//========================================================
//      End of File
//========================================================

#endif // !ODRIVE_CORE_H