/*******************************************************************************
* File Name: `$INSTANCE_NAME`.h
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides constants, parameter values, and API definitions for the
*  CORDIC Component.
*
*  The CORDIC uses circular vectoring mode to accelerate atan2, sqrt(x^2 + y^2),
*  and acos functions.
*
* Note:
*
********************************************************************************
* Copyright 2011-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#if !defined(`$INSTANCE_NAME`_H)
#define `$INSTANCE_NAME`_H

//========================================================
//      Standard Includes
//========================================================

#include <stdbool.h>
#include <stdint.h>

#include "cytypes.h"
#include "cyfitter.h"
#include "CyLib.h"
    
//========================================================
//      CORDIC Custom Version
//========================================================
    
#define `$INSTANCE_NAME`_MAJOR 1u
#define `$INSTANCE_NAME`_MINOR 1u
#define `$INSTANCE_NAME`_PATCH 1u
    
//========================================================
//      CORDIC Custom Types
//========================================================

typedef uint32_t `$INSTANCE_NAME`_job_id_t;
    
//========================================================
//      CORDIC Control registers
//========================================================

/**
 * \defgroup cordic_control CORDIC Control register
 *
 * @{
 */

/**
 * @brief Control register pointer.
 */
#define `$INSTANCE_NAME`_CONTROL_PTR        (  (reg8 *) `$INSTANCE_NAME`_CtlReg__CONTROL_REG)

/**
 * @brief Control register value.
 *
 * @note Access this register via CY_GET_REGn and CY_SET_REGn (defined in
 * cytypes.h), where n is the width of the register.
 */
#define `$INSTANCE_NAME`_CONTROL_REG        (* `$INSTANCE_NAME`_CONTROL_PTR)

/**
 * @brief Control register auxiliary control pointer.
 */
#define `$INSTANCE_NAME`_CONTROL_AUX_CTRL   (  (reg8 *) `$INSTANCE_NAME`_CtlReg__CONTROL_AUX_CTL_REG)

/** @} */

//========================================================
//      CORDIC Status registers
//========================================================

/**
 * \defgroup cordic_status CORDIC Status register
 *
 * @{
 */

/**
 * @brief Status register pointer.
 */
#define `$INSTANCE_NAME`_STATUS_PTR         (  (reg8 *) `$INSTANCE_NAME`_StsReg__STATUS_REG)

/**
 * @brief Status register value.
 *
 * @note Access this register via CY_GET_REGn and CY_SET_REGn (defined in
 * cytypes.h), where n is the width of the register.
 */
#define `$INSTANCE_NAME`_STATUS_REG         (* `$INSTANCE_NAME`_STATUS_PTR)

/**
 * @brief Status register mask value.
 *
 * @details The status mask is used to mask the status bits used by this
 * component.
 */
#define `$INSTANCE_NAME`_STATUS_MASK        (* (reg8 *) `$INSTANCE_NAME`_StsReg__MASK_REG)

/**
 * @brief Status register auxiliary control value.
 *
 * @details The status mask is used to mask the status bits used by this
 * component.
 */
#define `$INSTANCE_NAME`_STATUS_AUX_CTRL    (* (reg8 *) `$INSTANCE_NAME`_StsReg__STATUS_AUX_CTL_REG)

/** @} */

//========================================================
//      CORDIC Interrupt status registers
//========================================================

/**
 * \defgroup cordic_istatus CORDIC Interrupt status register
 *
 * @{
 */

/**
 * @brief Interrupt status register pointer.
 */
#define `$INSTANCE_NAME`_INT_STATUS_PTR     (  (reg8 *) `$INSTANCE_NAME`_InterruptRegister__STATUS_REG)

/**
 * @brief Interrupt status register value.
 */
#define `$INSTANCE_NAME`_INT_STATUS_REG     (* `$INSTANCE_NAME`_INT_STATUS_PTR)

/**
 * @brief Interrupt status register mask value.
 */
#define `$INSTANCE_NAME`_INT_STATUS_MASK    (* (reg8 *) `$INSTANCE_NAME`_InterruptRegister__MASK_REG )

/**
 * @brief Interrupt status register auxiliary control value.
 */
#define `$INSTANCE_NAME`_INT_STATUS_AUX_CTRL    (* (reg8 *) `$INSTANCE_NAME`_InterruptRegister__STATUS_AUX_CTL_REG )

/** @} */

//========================================================
//      CORDIC X-engine registers
//========================================================

/**
 * \defgroup cordic_x_engine CORDIC X-engine registers
 *
 * @{
 */

#define `$INSTANCE_NAME`_X_ENGINE_A0_PTR    ((reg16 *) `$INSTANCE_NAME`_dp_x_engine_LSB__16BIT_A0_REG)
#define `$INSTANCE_NAME`_X_ENGINE_A0_REG    (* `$INSTANCE_NAME`_X_ENGINE_A0_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_A1_PTR    ((reg16 *) `$INSTANCE_NAME`_dp_x_engine_LSB__16BIT_A1_REG)
#define `$INSTANCE_NAME`_X_ENGINE_A1_REG    (* `$INSTANCE_NAME`_X_ENGINE_A1_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_D0_PTR    ((reg16 *) `$INSTANCE_NAME`_dp_x_engine_LSB__16BIT_D0_REG)
#define `$INSTANCE_NAME`_X_ENGINE_D0_REG    (* `$INSTANCE_NAME`_X_ENGINE_D0_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_D1_PTR    ((reg16 *) `$INSTANCE_NAME`_dp_x_engine_LSB__16BIT_D1_REG)
#define `$INSTANCE_NAME`_X_ENGINE_D1_REG    (* `$INSTANCE_NAME`_X_ENGINE_D1_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_F0_PTR    ((reg16 *) `$INSTANCE_NAME`_dp_x_engine_LSB__16BIT_F0_REG)
#define `$INSTANCE_NAME`_X_ENGINE_F0_REG    (* `$INSTANCE_NAME`_X_ENGINE_F0_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_F1_PTR    ((reg16 *) `$INSTANCE_NAME`_dp_x_engine_LSB__16BIT_F1_REG)
#define `$INSTANCE_NAME`_X_ENGINE_F1_REG    (* `$INSTANCE_NAME`_X_ENGINE_F1_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_AUX_CTRL  ((reg16 *) `$INSTANCE_NAME`_dp_x_engine_LSB__16BIT_DP_AUX_CTL_REG)

/* MSB Datapath registers */
#define `$INSTANCE_NAME`_X_ENGINE_A0_MSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_MSB__A0_REG)
#define `$INSTANCE_NAME`_X_ENGINE_A0_MSB_REG    (* `$INSTANCE_NAME`_X_ENGINE_A0_MSB_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_A1_MSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_MSB__A1_REG)
#define `$INSTANCE_NAME`_X_ENGINE_A1_MSB_REG    (* `$INSTANCE_NAME`_X_ENGINE_A1_MSB_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_D0_MSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_MSB__D0_REG)
#define `$INSTANCE_NAME`_X_ENGINE_D0_MSB_REG    (* `$INSTANCE_NAME`_X_ENGINE_D0_MSB_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_D1_MSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_MSB__D1_REG)
#define `$INSTANCE_NAME`_X_ENGINE_D1_MSB_REG    (* `$INSTANCE_NAME`_X_ENGINE_D1_MSB_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_F0_MSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_MSB__F0_REG)
#define `$INSTANCE_NAME`_X_ENGINE_F0_MSB_REG    (* `$INSTANCE_NAME`_X_ENGINE_F0_MSB_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_F1_MSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_MSB__F1_REG)
#define `$INSTANCE_NAME`_X_ENGINE_F1_MSB_REG    (* `$INSTANCE_NAME`_X_ENGINE_F1_MSB_PTR)

/* LSB Datapath registers */
#define `$INSTANCE_NAME`_X_ENGINE_A0_LSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_LSB__A0_REG)
#define `$INSTANCE_NAME`_X_ENGINE_A0_LSB_REG    (* `$INSTANCE_NAME`_X_ENGINE_A0_LSB_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_A1_LSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_LSB__A1_REG)
#define `$INSTANCE_NAME`_X_ENGINE_A1_LSB_REG    (* `$INSTANCE_NAME`_X_ENGINE_A1_LSB_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_D0_LSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_LSB__D0_REG)
#define `$INSTANCE_NAME`_X_ENGINE_D0_LSB_REG    (* `$INSTANCE_NAME`_X_ENGINE_D0_LSB_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_D1_LSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_LSB__D1_REG)
#define `$INSTANCE_NAME`_X_ENGINE_D1_LSB_REG    (* `$INSTANCE_NAME`_X_ENGINE_D1_LSB_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_F0_LSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_LSB__F0_REG)
#define `$INSTANCE_NAME`_X_ENGINE_F0_LSB_REG    (* `$INSTANCE_NAME`_X_ENGINE_F0_LSB_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_F1_LSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_LSB__F1_REG)
#define `$INSTANCE_NAME`_X_ENGINE_F1_LSB_REG    (* `$INSTANCE_NAME`_X_ENGINE_F1_LSB_PTR)

/** @} */

//========================================================
//      CORDIC Y-engine registers
//========================================================

/**
 * \defgroup cordic_y_engine CORDIC Y-engine registers
 *
 * @{
 */

#define `$INSTANCE_NAME`_Y_ENGINE_A0_PTR    ((reg16 *) `$INSTANCE_NAME`_dp_y_engine_LSB__16BIT_A0_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_A0_REG    (* `$INSTANCE_NAME`_Y_ENGINE_A0_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_A1_PTR    ((reg16 *) `$INSTANCE_NAME`_dp_y_engine_LSB__16BIT_A1_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_A1_REG    (* `$INSTANCE_NAME`_Y_ENGINE_A1_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_D0_PTR    ((reg16 *) `$INSTANCE_NAME`_dp_y_engine_LSB__16BIT_D0_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_D0_REG    (* `$INSTANCE_NAME`_Y_ENGINE_D0_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_D1_PTR    ((reg16 *) `$INSTANCE_NAME`_dp_y_engine_LSB__16BIT_D1_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_D1_REG    (* `$INSTANCE_NAME`_Y_ENGINE_D1_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_F0_PTR    ((reg16 *) `$INSTANCE_NAME`_dp_y_engine_LSB__16BIT_F0_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_F0_REG    (* `$INSTANCE_NAME`_Y_ENGINE_F0_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_F1_PTR    ((reg16 *) `$INSTANCE_NAME`_dp_y_engine_LSB__16BIT_F1_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_F1_REG    (* `$INSTANCE_NAME`_Y_ENGINE_F1_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_AUX_CTRL  ((reg16 *) `$INSTANCE_NAME`_dp_y_engine_LSB__16BIT_DP_AUX_CTL_REG)

/* MSB Datapath registers */
#define `$INSTANCE_NAME`_Y_ENGINE_A0_MSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_MSB__A0_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_A0_MSB_REG    (* `$INSTANCE_NAME`_Y_ENGINE_A0_MSB_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_A1_MSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_MSB__A1_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_A1_MSB_REG    (* `$INSTANCE_NAME`_Y_ENGINE_A1_MSB_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_D0_MSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_MSB__D0_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_D0_MSB_REG    (* `$INSTANCE_NAME`_Y_ENGINE_D0_MSB_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_D1_MSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_MSB__D1_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_D1_MSB_REG    (* `$INSTANCE_NAME`_Y_ENGINE_D1_MSB_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_F0_MSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_MSB__F0_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_F0_MSB_REG    (* `$INSTANCE_NAME`_Y_ENGINE_F0_MSB_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_F1_MSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_MSB__F1_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_F1_MSB_REG    (* `$INSTANCE_NAME`_Y_ENGINE_F1_MSB_PTR)

/* LSB Datapath registers */
#define `$INSTANCE_NAME`_Y_ENGINE_A0_LSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_LSB__A0_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_A0_LSB_REG    (* `$INSTANCE_NAME`_Y_ENGINE_A0_LSB_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_A1_LSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_LSB__A1_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_A1_LSB_REG    (* `$INSTANCE_NAME`_Y_ENGINE_A1_LSB_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_D0_LSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_LSB__D0_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_D0_LSB_REG    (* `$INSTANCE_NAME`_Y_ENGINE_D0_LSB_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_D1_LSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_LSB__D1_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_D1_LSB_REG    (* `$INSTANCE_NAME`_Y_ENGINE_D1_LSB_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_F0_LSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_LSB__F0_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_F0_LSB_REG    (* `$INSTANCE_NAME`_Y_ENGINE_F0_LSB_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_F1_LSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_LSB__F1_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_F1_LSB_REG    (* `$INSTANCE_NAME`_Y_ENGINE_F1_LSB_PTR)

/** @} */

//========================================================
//      CORDIC Z-engine registers
//========================================================

/**
 * \defgroup cordic_z_engine CORDIC Z-engine registers
 *
 * @{
 */

#define `$INSTANCE_NAME`_Z_ENGINE_A0_PTR    ((reg16 *) `$INSTANCE_NAME`_dp_z_engine_LSB__16BIT_A0_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_A0_REG    (* `$INSTANCE_NAME`_Z_ENGINE_A0_PTR)

#define `$INSTANCE_NAME`_Z_ENGINE_A1_PTR    ((reg16 *) `$INSTANCE_NAME`_dp_z_engine_LSB__16BIT_A1_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_A1_REG    (* `$INSTANCE_NAME`_Z_ENGINE_A1_PTR)

#define `$INSTANCE_NAME`_Z_ENGINE_D0_PTR    ((reg16 *) `$INSTANCE_NAME`_dp_z_engine_LSB__16BIT_D0_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_D0_REG    (* `$INSTANCE_NAME`_Z_ENGINE_D0_PTR)

#define `$INSTANCE_NAME`_Z_ENGINE_D1_PTR    ((reg16 *) `$INSTANCE_NAME`_dp_z_engine_LSB__16BIT_D1_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_D1_REG    (* `$INSTANCE_NAME`_Z_ENGINE_D1_PTR)

#define `$INSTANCE_NAME`_Z_ENGINE_F0_PTR    ((reg16 *) `$INSTANCE_NAME`_dp_z_engine_LSB__16BIT_F0_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_F0_REG    (* `$INSTANCE_NAME`_Z_ENGINE_F0_PTR)

#define `$INSTANCE_NAME`_Z_ENGINE_F1_PTR    ((reg16 *) `$INSTANCE_NAME`_dp_z_engine_LSB__16BIT_F1_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_F1_REG    (* `$INSTANCE_NAME`_Z_ENGINE_F1_PTR)

#define `$INSTANCE_NAME`_Z_ENGINE_AUX_CTRL  ((reg16 *) `$INSTANCE_NAME`_dp_z_engine_LSB__16BIT_DP_AUX_CTL_REG)

/* MSB Datapath registers */
#define `$INSTANCE_NAME`_Z_ENGINE_A0_MSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_MSB__A0_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_A0_MSB_REG    (* `$INSTANCE_NAME`_Z_ENGINE_A0_MSB_PTR)

#define `$INSTANCE_NAME`_Z_ENGINE_A1_MSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_MSB__A1_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_A1_MSB_REG    (* `$INSTANCE_NAME`_Z_ENGINE_A1_MSB_PTR)

#define `$INSTANCE_NAME`_Z_ENGINE_D0_MSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_MSB__D0_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_D0_MSB_REG    (* `$INSTANCE_NAME`_Z_ENGINE_D0_MSB_PTR)

#define `$INSTANCE_NAME`_Z_ENGINE_D1_MSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_MSB__D1_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_D1_MSB_REG    (* `$INSTANCE_NAME`_Z_ENGINE_D1_MSB_PTR)

#define `$INSTANCE_NAME`_Z_ENGINE_F0_MSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_MSB__F0_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_F0_MSB_REG    (* `$INSTANCE_NAME`_Z_ENGINE_F0_MSB_PTR)

#define `$INSTANCE_NAME`_Z_ENGINE_F1_MSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_MSB__F1_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_F1_MSB_REG    (* `$INSTANCE_NAME`_Z_ENGINE_F1_MSB_PTR)

/* LSB Datapath registers */
#define `$INSTANCE_NAME`_Z_ENGINE_A0_LSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_LSB__A0_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_A0_LSB_REG    (* `$INSTANCE_NAME`_Z_ENGINE_A0_LSB_PTR)

#define `$INSTANCE_NAME`_Z_ENGINE_A1_LSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_LSB__A1_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_A1_LSB_REG    (* `$INSTANCE_NAME`_Z_ENGINE_A1_LSB_PTR)

#define `$INSTANCE_NAME`_Z_ENGINE_D0_LSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_LSB__D0_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_D0_LSB_REG    (* `$INSTANCE_NAME`_Z_ENGINE_D0_LSB_PTR)

#define `$INSTANCE_NAME`_Z_ENGINE_D1_LSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_LSB__D1_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_D1_LSB_REG    (* `$INSTANCE_NAME`_Z_ENGINE_D1_LSB_PTR)

#define `$INSTANCE_NAME`_Z_ENGINE_F0_LSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_LSB__F0_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_F0_LSB_REG    (* `$INSTANCE_NAME`_Z_ENGINE_F0_LSB_PTR)

#define `$INSTANCE_NAME`_Z_ENGINE_F1_LSB_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_LSB__F1_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_F1_LSB_REG    (* `$INSTANCE_NAME`_Z_ENGINE_F1_LSB_PTR)

/** @} */

//========================================================
//      CORDIC Constans
//========================================================
    
/**
* @brief Accumulative cosine term for circular vectoring mode.
*
* @details This term approximates the accumulative gain (Kc) for circular
* vectoring mode with n = 16 iterations.
*/
#define `$INSTANCE_NAME`_KC             (1.64676f)

/**
* @brief Inverse accumulative cosine term.
*
* @details This is approximately 1 / `$INSTANCE_NAME`_KC for n = 16 iterations.
*/
#define `$INSTANCE_NAME`_KC_INVERSE     (0.60725f)

/* Status register constants */
#define `$INSTANCE_NAME`_STS_CFAULT         (0x01u)
#define `$INSTANCE_NAME`_STS_IDLE           (0x02u)
#define `$INSTANCE_NAME`_STS_X_F0_READY     (0x04u)
#define `$INSTANCE_NAME`_STS_Y_F0_READY     (0x08u)
#define `$INSTANCE_NAME`_STS_Z_FILLED       (0x10u)
#define `$INSTANCE_NAME`_STS_X_FILLED       (0x20u)
#define `$INSTANCE_NAME`_STS_DONE           (0x40u)
#define `$INSTANCE_NAME`_STS_OUT_FIFO_FULL  (0x80u)
    
// Output FIFO's filled status bits
#define `$INSTANCE_NAME`_STS_PENDING    (0x30u)

/* Interrupt constants */
#define `$INSTANCE_NAME`_INTR_ENBL      (0x10u)

/* Control register constants */
#define `$INSTANCE_NAME`_EN             (0x01u)
#define `$INSTANCE_NAME`_RST            (0x02u)
#define `$INSTANCE_NAME`_OPER_MODE      (0x04u)
#define `$INSTANCE_NAME`_CLR_DONE       (0x08u)
#define `$INSTANCE_NAME`_CONTROL_MASK   (0x7Fu)
    
/* Toggle the commit bit to the CORDIC.
   This mechanism must be used to prevent CDC with different clocks
*/
#define `$INSTANCE_NAME`_COMMIT_BM      (0x80u)

#define `$INSTANCE_NAME`_WRITE_CTRL(val) {
    `$INSTANCE_NAME`_CONTROL_REG |= (val & `$INSTANCE_NAME`_CONTROL_MASK) \
}
#define `$INSTANCE_NAME`_COMMIT_CTRL { \
    `$INSTANCE_NAME`_CONTROL_REG ^= `$INSTANCE_NAME`_COMMIT_BM; \
    CyDelayUs(1u); \
}
    
#define `$INSTANCE_NAME`_ROTATING_OPER  (0x00)
#define `$INSTANCE_NAME`_VECTORING_OPER (`$INSTANCE_NAME`_OPER_MODE)

//========================================================
//      CORDIC Structures
//========================================================

/**
 * @brief Input vector for CORDIC circular vectoring mode.
 *
 * This structure represents the Cartesian input vector `(x, y)` supplied to the
 * CORDIC engine. The meaning of the input values depends on the intended operation
 * (magnitude/angle extraction or inverse trigonometric evaluation).
 *
 * Primary use (polar conversion):
 * - When both `x` and `y` are provided, the CORDIC operates in circular vectoring mode
 *   and computes:
 *   - `r = sqrt(x^2 + y^2)`  (vector magnitude, possibly scaled by CORDIC gain)
 *   - `θ = atan2(y, x)`     (vector angle)
 *
 * Inverse trigonometric usage patterns:
 * - **`atan(a)`**
 *   - `x = 1`
 *   - `y = a`
 *
 * - **`asin(a)`**
 *   - `x = sqrt(1 - a^2)`
 *   - `y = a`
 *
 * - **`acos(a)`**
 *   - `x = a`
 *   - `y = sqrt(1 - a^2)`
 *
 * @note For `asin` and `acos`, the input must satisfy `|a| ≤ 1`.
 * @note The sign of `y` determines the sign of the resulting angle.
 * @note Supplying `(x, y) = (0, 0)` is invalid for angle computation.
 */
typedef struct {
    /** 
      * @brief X coordinate of the input vector. 
      */
    int16_t x; 
    
    /** 
      * @brief Y coordinate of the input vector. 
      */
    int16_t y; 
    
    /** 
      * @brief z coordinate of the input vector. 
      */
    int16_t z; 
} `$INSTANCE_NAME`_vector_t;

//========================================================
//      CORDIC Enable functions
//========================================================

/**
 * @brief Enable the CORDIC hardware.
 */
void `$INSTANCE_NAME`_enable(void) `=ReentrantKeil($INSTANCE_NAME . "_enable")`;

/**
 * @brief Disable the CORDIC hardware.
 */
void `$INSTANCE_NAME`_disable(void) `=ReentrantKeil($INSTANCE_NAME . "_disable")`;

//========================================================
//      CORDIC State functions
//========================================================
/**
 * @brief Check whether the CORDIC has output data pending.
 *
 * @retval true - Output data is available.
 * @retval false - No output data is available.
 */
bool `$INSTANCE_NAME`_has_pending(void) `=ReentrantKeil($INSTANCE_NAME . "_has_pending")`;

/**
 * @brief Initialize the CORDIC engine.
 *
 * @return void
 */
void `$INSTANCE_NAME`_init(void) `=ReentrantKeil($INSTANCE_NAME . "_init")`;

/**
 * @brief Start the CORDIC engine.
 *
 * @return uint8_t
 *          CYRET_SUCCESS:  CORDIC engine started.
 *          CYRET_TIMEOUT:  CORDIC engine did not respond.
 */
uint8_t `$INSTANCE_NAME`_start(void) `=ReentrantKeil($INSTANCE_NAME . "_start")`;

/**
 * @brief Halt the CORDIC engine.
 *
 * @return uint8_t
 */
uint8_t `$INSTANCE_NAME`_stop(void) `=ReentrantKeil($INSTANCE_NAME . "_stop")`;

/**
 * @brief Fetch data from the output FIFOs.
 *
 * @param[out] result - Pointer to the result structure
 * @param[out] job_id - Job id of the resulting vector
 *
 * @retval CYRET_SUCCESS - Data fetched.
 *
 */
uint8_t `$INSTANCE_NAME`_get_data(`$INSTANCE_NAME`_vector_t* result) `=ReentrantKeil($INSTANCE_NAME . "_get_data")`;

/**
 * @brief Queue input vector data for processing.
 *
 * @param[in] vector - Pointer to the vector to be processed
 * @param[out] job_id - Job id of the queued vector
 *
 * @retval CYRET_SUCCESS - Data queued.
 *
 */
uint8_t `$INSTANCE_NAME`_queue_data(const `$INSTANCE_NAME`_vector_t* vector) `=ReentrantKeil($INSTANCE_NAME . "_queue_data")`;

//========================================================
//      CORDIC Interrupt functions
//========================================================

/**
 * @brief Enable the status block interrupt.
 *
 * @details This function enables bit 4 in the interrupt AUX control register.
 * This "software start" is required to generate interrupts.
 *
 * @note See documentation: https://community.infineon.com/gfawx74859/attachments/gfawx74859/psoc4/46012/2/component_author_guide.pdf#page=67
 */
void `$INSTANCE_NAME`_enable_interrupt(void) `=ReentrantKeil($INSTANCE_NAME . "_EnableInterrupt")`;

/**
 * @brief Disable the status block interrupt.
 *
 * @details This function disables bit 4 in the interrupt AUX control register.
 *
 * @note See documentation: https://community.infineon.com/gfawx74859/attachments/gfawx74859/psoc4/46012/2/component_author_guide.pdf#page=67
 */
void `$INSTANCE_NAME`_disable_interrupt(void) `=ReentrantKeil($INSTANCE_NAME . "_DisableInterrupt")`;

CY_ISR_PROTO(`$INSTANCE_NAME`_ISR_Handler);

/**
 * @brief CORDIC calculation done callback
 *
 * This callback is fired after each succesfull calculation
 *
 * @note The interrupts must be enabled to use this callback
 */
void `$INSTANCE_NAME`_done_callback(void) `=ReentrantKeil($INSTANCE_NAME . "_done_callback")`;

//========================================================
//      End of File
//========================================================

#endif //!`$INSTANCE_NAME`_H
