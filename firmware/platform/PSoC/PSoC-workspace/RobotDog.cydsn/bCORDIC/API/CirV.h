/*******************************************************************************
* File Name: `$INSTANCE_NAME`.h
* Version `$CY_MAJOR_VERSION`.`$CY_MINOR_VERSION`
*
* Description:
*  This file provides constants and parameter values and API definition for the
*  CORDIC Component.
*
*  The CORDIC uses the circular vectoring mode for accelerating atan2, sqrt(x^2 + y^2) and acos functions
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

/* Check to see if required defines such as CY_PSOC5A are available */
/* They are defined starting with cy_boot v3.0 */
#ifndef CY_PSOC5A
    #error Component `$CY_COMPONENT_NAME` requires cy_boot v3.0 or later
#endif /* CY_PSOC5A */

//========================================================
//      CORDIC Control registers
//========================================================

/** 
 * \defgroup cordic_control CORDIC Control register 
 * 
 * @{
*/

/**
 * @brief Control register value
 *
 * 
*/
#define `$INSTANCE_NAME`_CONTROL_PTR        (  (reg8 *) `$INSTANCE_NAME`_CtlReg__CONTROL_REG)

/**
 * @brief Address of the control register
 *
 * @note This register pointer should be accessed via
 * CY_GET_REGn and CY_SET_REGn macros (defined in cytypes.h),
 * where n is the width of the register
*/
#define `$INSTANCE_NAME`_CONTROL_REG        (* `$INSTANCE_NAME`_CONTROL_PTR)

/**
 * @brief Address of the control register
 *
 * 
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
 * @brief Status register value
 *
 * 
*/
#define `$INSTANCE_NAME`_STATUS_PTR         (  (reg8 *) `$INSTANCE_NAME`_StsReg__STATUS_REG)

/**
 * @brief Status register value
 *
 * @note This register pointer should be accessed via
 * CY_GET_REGn and CY_SET_REGn macros (defined in cytypes.h),
 * where n is the width of the register
*/
#define `$INSTANCE_NAME`_STATUS_REG         (* `$INSTANCE_NAME`_STATUS_PTR)

/**
 * @brief Status register mask
 *
 * @details The status mask is used for masking the used status bits
*/
#define `$INSTANCE_NAME`_STATUS_MASK        (* (reg8 *) `$INSTANCE_NAME`_StsReg__MASK_REG)

/**
 * @brief Status register mask
 *
 * @details The status mask is used for masking the used status bits
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
 * @brief Interrupt status pointer
 *
 * 
*/
#define `$INSTANCE_NAME`_INT_STATUS_PTR     (  (reg8 *) `$INSTANCE_NAME`_InterruptRegister__STATUS_REG)

/**
 * @brief Interrupt status value
 *
 * 
*/
#define `$INSTANCE_NAME`_INT_STATUS_REG     (* `$INSTANCE_NAME`_INT_STATUS_PTR)


/**
 * @brief Interrupt status mask
 *
 * 
*/
#define `$INSTANCE_NAME`_INT_STATUS_MASK    (* (reg8 *) `$INSTANCE_NAME`_InterruptRegister__MASK_REG )


/**
 * @brief Interrupt status value
 *
 * 
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

#define `$INSTANCE_NAME`_ANGLE_SIGN     (0x80000u)
#define `$INSTANCE_NAME`_KC             (1.64676f)
#define `$INSTANCE_NAME`_KC_INVERSE     (0.60725f)

/* Status register constants */
#define `$INSTANCE_NAME`_STS_CFAULT     (0x01u)
#define `$INSTANCE_NAME`_STS_IDLE       (0x02u)
#define `$INSTANCE_NAME`_STS_X_F0_READY (0x04u)
#define `$INSTANCE_NAME`_STS_Y_F0_READY (0x08u)
#define `$INSTANCE_NAME`_STS_Z_FILLED   (0x10u)
#define `$INSTANCE_NAME`_STS_X_FILLED   (0x20u)
#define `$INSTANCE_NAME`_STS_DONE       (0x40u)

#define `$INSTANCE_NAME`_STS_PENDING    (0x30u)

/* Interrupt constants */
#define `$INSTANCE_NAME`_INTR_ENBL      (0x10u)


/* Control register constants */
#define `$INSTANCE_NAME`_EN             (0x01u)
#define `$INSTANCE_NAME`_RST            (0x02u)

//========================================================
//      Optimized KC-Embedded Conversions (16 iterations)
//========================================================

#define `$INSTANCE_NAME`_Q12_4_SCALE     (9.716)    // 16 * KC_16iter
#define `$INSTANCE_NAME`_Q12_4_INV_SCALE (0.0625f)  // (1/16)

#define `$INSTANCE_NAME`_BAMS_SCALE      (182.044444f)   // 65536/360

//========================================================
//      Single-Multiply Conversions (Zero KC overhead)
//========================================================

#define Q12_4_FROM_MM(mm)       ((int16_t)((mm) * `$INSTANCE_NAME`_Q12_4_SCALE))
#define Q12_4_TO_MM(q12_4)      ((float)(q12_4) * `$INSTANCE_NAME`_Q12_4_INV_SCALE)

#define BAMS_PER_DEG   (32768.0f/180.0f)   // 182.044444...

#define BAMS_FROM_DEG_SIGNED(deg)  ((int16_t)((deg) * BAMS_PER_DEG))
#define DEG_FROM_BAMS_SIGNED(bams) ((float)(bams) * (180.0f/32768.0f))

//========================================================
//      CORDIC Structures
//========================================================

typedef struct {
    float x;
    float y;
} `$INSTANCE_NAME`_vector_t;

typedef struct {
    float mag;
    float angle;
} `$INSTANCE_NAME`_result_t;

//========================================================
//      CORDIC Enable functions
//========================================================

/**
 * @brief Enable the CORDICs hardware
*/
void `$INSTANCE_NAME`_enable(void) `=ReentrantKeil($INSTANCE_NAME . "_enable")`;

/**
 * @brief Disables the CORDICs hardware
*/
void `$INSTANCE_NAME`_disable(void) `=ReentrantKeil($INSTANCE_NAME . "_disable")`;

//========================================================
//      CORDIC State functions
//========================================================
/**
 * @brief Checks if the CORDIC has output data
 *
 * @return bool
*/
bool `$INSTANCE_NAME`_has_pending(void) `=ReentrantKeil($INSTANCE_NAME . "_has_pending")`;

/**
 * @brief Init the CORDIC engine
 *
 * @return void
*/
void `$INSTANCE_NAME`_init(void) `=ReentrantKeil($INSTANCE_NAME . "_init")`;

/**
 * @brief Start the CORDIC engine
 *
 * @return uint8_t
 *          CYRET_SUCCES: CORDIC engine started
 *          CYRET_TIMEOUT: CORDIC engine didn't respond
*/
uint8_t `$INSTANCE_NAME`_start(void) `=ReentrantKeil($INSTANCE_NAME . "_start")`;

/**
 * @brief Halts the CORDIC engine
*/
uint8_t `$INSTANCE_NAME`_stop(void) `=ReentrantKeil($INSTANCE_NAME . "_stop")`;

/**
 * @brief Fetch data from the output FIFO's
 *
 * @details this function enables bit 4 in the interrupts AUX control register.
 * This is the software start needed for interrupts 
 *
 * @param[inout] result - Pointer to the result struct
 *
 * @return uint8_t
 *          CYRET_SUCCES: Data fetched
 *
 * @note See documentation: https://community.infineon.com/gfawx74859/attachments/gfawx74859/psoc4/46012/2/component_author_guide.pdf#page=67
*/
uint8_t `$INSTANCE_NAME`_get_data(`$INSTANCE_NAME`_result_t* result) `=ReentrantKeil($INSTANCE_NAME . "_get_data")`;

/**
 * @brief Fetch data from the output FIFO's
 *
 * @details this function enables bit 4 in the interrupts AUX control register.
 * This is the software start needed for interrupts 
 *
 * @param[inout] result - Pointer to the result struct
 *
 * @return uint8_t
 *          CYRET_SUCCES: Data fetched
 *
 * @note See documentation: https://community.infineon.com/gfawx74859/attachments/gfawx74859/psoc4/46012/2/component_author_guide.pdf#page=67
*/
uint8_t `$INSTANCE_NAME`_queue_data(`$INSTANCE_NAME`_vector_t* result) `=ReentrantKeil($INSTANCE_NAME . "_queue_data")`;

//========================================================
//      CORDIC Interrupt functions
//========================================================

/**
 * @brief Enable the statusi block to interrupt
 *
 * @details this function enables bit 4 in the interrupts AUX control register.
 * This is the software start needed for interrupts 
 *
 * @note See documentation: https://community.infineon.com/gfawx74859/attachments/gfawx74859/psoc4/46012/2/component_author_guide.pdf#page=67
*/
void `$INSTANCE_NAME`_enable_interrupt(void) `=ReentrantKeil($INSTANCE_NAME . "_EnableInterrupt")`;

/**
 * @brief Disable the statusi block to interrupt
 *
 * @details this function disables bit 4 in the interrupts AUX control register.
 *
 * @note See documentation: https://community.infineon.com/gfawx74859/attachments/gfawx74859/psoc4/46012/2/component_author_guide.pdf#page=67
*/
void `$INSTANCE_NAME`_disable_interrupt(void) `=ReentrantKeil($INSTANCE_NAME . "_DisableInterrupt")`;

CY_ISR_PROTO(`$INSTANCE_NAME`_ISR_Handler);

void `$INSTANCE_NAME`_done_callback(void) `=ReentrantKeil($INSTANCE_NAME . "_done_callback")`;

//========================================================
//      End of File
//========================================================
    
#endif //!`$INSTANCE_NAME`_H