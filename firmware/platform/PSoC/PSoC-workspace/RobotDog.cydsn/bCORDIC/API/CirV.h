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
#define `$INSTANCE_NAME`_INT_STATUS_PTR     (  (reg8 *) `$INSTANCE_NAME`_StatusIntReg_1_sts_intr_sts_reg__STATUS_REG)

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
#define `$INSTANCE_NAME`_INT_STATUS_MASK    (* (reg8 *) `$INSTANCE_NAME`_StatusIntReg_1_sts_intr_sts_reg__MASK_REG )


/**
 * @brief Interrupt status value
 *
 * 
*/
#define `$INSTANCE_NAME`_INT_STATUS_AUX_CTRL    (* (reg8 *) `$INSTANCE_NAME`_StatusIntReg_1_sts_intr_sts_reg__STATUS_AUX_CTL_REG )

/** @} */

//========================================================
//      CORDIC X-engine registers
//========================================================

/** 
 * \defgroup cordic_x_engine CORDIC X-engine registers
 * 
 * @{
*/


#define `$INSTANCE_NAME`_X_ENGINE_A0_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_LSB__16BIT_A0_REG)
#define `$INSTANCE_NAME`_X_ENGINE_A0_REG    (* `$INSTANCE_NAME`_X_ENGINE_A0_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_A1_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_LSB__16BIT_A1_REG)
#define `$INSTANCE_NAME`_X_ENGINE_A1_REG    (* `$INSTANCE_NAME`_X_ENGINE_A1_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_D0_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_LSB__16BIT_D0_REG)
#define `$INSTANCE_NAME`_X_ENGINE_D0_REG    (* `$INSTANCE_NAME`_X_ENGINE_D0_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_D1_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_LSB__16BIT_D1_REG)
#define `$INSTANCE_NAME`_X_ENGINE_D1_REG    (* `$INSTANCE_NAME`_X_ENGINE_D1_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_F0_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_LSB__16BIT_F0_REG)
#define `$INSTANCE_NAME`_X_ENGINE_F0_REG    (* `$INSTANCE_NAME`_X_ENGINE_F0_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_F1_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_LSB__16BIT_F1_REG)
#define `$INSTANCE_NAME`_X_ENGINE_F1_REG    (* `$INSTANCE_NAME`_X_ENGINE_F1_PTR)

#define `$INSTANCE_NAME`_X_ENGINE_AUX_CTRL  ((reg8 *) `$INSTANCE_NAME`_dp_x_engine_LSB__16BIT_DP_AUX_CTL_REG)

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


#define `$INSTANCE_NAME`_Y_ENGINE_A0_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_LSB__16BIT_A0_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_A0_REG    (* `$INSTANCE_NAME`_Y_ENGINE_A0_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_A1_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_LSB__16BIT_A1_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_A1_REG    (* `$INSTANCE_NAME`_Y_ENGINE_A1_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_D0_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_LSB__16BIT_D0_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_D0_REG    (* `$INSTANCE_NAME`_Y_ENGINE_D0_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_D1_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_LSB__16BIT_D1_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_D1_REG    (* `$INSTANCE_NAME`_Y_ENGINE_D1_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_F0_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_LSB__16BIT_F0_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_F0_REG    (* `$INSTANCE_NAME`_Y_ENGINE_F0_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_F1_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_LSB__16BIT_F1_REG)
#define `$INSTANCE_NAME`_Y_ENGINE_F1_REG    (* `$INSTANCE_NAME`_Y_ENGINE_F1_PTR)

#define `$INSTANCE_NAME`_Y_ENGINE_AUX_CTRL  ((reg8 *) `$INSTANCE_NAME`_dp_y_engine_LSB__16BIT_DP_AUX_CTL_REG)

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
//      CORDIC-engine registers
//========================================================

/** 
 * \defgroup cordic_z_engine CORDIC Z-engine registers
 * 
 * @{
*/


#define `$INSTANCE_NAME`_Z_ENGINE_A0_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_LSB__16BIT_A0_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_A0_REG    (* `$INSTANCE_NAME`_Z_ENGINE_A0_PTR)
                        
#define `$INSTANCE_NAME`_Z_ENGINE_A1_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_LSB__16BIT_A1_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_A1_REG    (* `$INSTANCE_NAME`_Z_ENGINE_A1_PTR)
                        
#define `$INSTANCE_NAME`_Z_ENGINE_D0_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_LSB__16BIT_D0_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_D0_REG    (* `$INSTANCE_NAME`_Z_ENGINE_D0_PTR)
                        
#define `$INSTANCE_NAME`_Z_ENGINE_D1_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_LSB__16BIT_D1_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_D1_REG    (* `$INSTANCE_NAME`_Z_ENGINE_D1_PTR)
                        
#define `$INSTANCE_NAME`_Z_ENGINE_F0_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_LSB__16BIT_F0_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_F0_REG    (* `$INSTANCE_NAME`_Z_ENGINE_F0_PTR)
                        
#define `$INSTANCE_NAME`_Z_ENGINE_F1_PTR    ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_LSB__16BIT_F1_REG)
#define `$INSTANCE_NAME`_Z_ENGINE_F1_REG    (* `$INSTANCE_NAME`_Z_ENGINE_F1_PTR)
                        
#define `$INSTANCE_NAME`_Z_ENGINE_AUX_CTRL  ((reg8 *) `$INSTANCE_NAME`_dp_z_engine_LSB__16BIT_DP_AUX_CTL_REG)

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

#define `$INSTANCE_NAME`_INTR_ENBL      (0x10u)

#define `$INSTANCE_NAME`_EN             (0x01u)
#define `$INSTANCE_NAME`_RST            (0x02u)

//========================================================
//      Optimized KC-Embedded Conversions (16 iterations)
//========================================================

#define `$INSTANCE_NAME`_Q12_4_SCALE     (9.71604696f)   // 16 * KC_16iter
#define `$INSTANCE_NAME`_Q12_4_INV_SCALE (0.1029375f)    // (1/16) * (1/KC)

#define `$INSTANCE_NAME`_BAMS_SCALE      (182.044444f)   // 65536/360

//========================================================
//      Single-Multiply Conversions (Zero KC overhead)
//========================================================

#define Q12_4_FROM_MM(mm)       ((int16_t)((mm) * `$INSTANCE_NAME`_Q12_4_SCALE))
#define Q12_4_TO_MM(q12_4)      ((float)(q12_4) * `$INSTANCE_NAME`_Q12_4_INV_SCALE)

#define BAMS_FROM_DEG(deg)      ((int16_t)(((deg) + 180.0f) * `$INSTANCE_NAME`_BAMS_SCALE))
#define BAMS_TO_DEG(bams)       ((((float)(bams)) / `$INSTANCE_NAME`_BAMS_SCALE) - 180.0f)

//========================================================
//      CORDIC Structures
//========================================================

typedef struct {
    int16_t x;     // 2 bytes (software view)
    int16_t y;     // 2 bytes (software view)
    
    // Hardware FIFO byte access (4 sequential bytes)
    struct {
        uint8_t x_lsb;
        uint8_t x_msb;
        uint8_t y_lsb;
        uint8_t y_msb;
    } fifo_bytes;
} `$INSTANCE_NAME`_vector_t;

typedef struct {
    int16_t mag;     // 2 bytes (software view)
    int16_t ang;     // 2 bytes (software view)
    
    // Hardware FIFO byte access (4 sequential bytes)
    struct {
        uint8_t x_lsb;
        uint8_t x_msb;
        uint8_t y_lsb;
        uint8_t y_msb;
    } fifo_bytes;
} `$INSTANCE_NAME`_result_t;

//========================================================
//      CORDIC Function prototypes
//========================================================

/**
 * @brief Initialize the CORDIC engine
*/
uint8_t inline `$INSTANCE_NAME`_init(void) `=ReentrantKeil($INSTANCE_NAME . "_Init")`;

//void  `$INSTANCE_NAME`_Start(uint16_t x, uint16_t y)  `=ReentrantKeil($INSTANCE_NAME . "_Start")`;

//========================================================
//      CORDIC Enable functions
//========================================================

/**
 * @brief Enable the CORDIC engine to process data
*/
void inline `$INSTANCE_NAME`_enable(void) `=ReentrantKeil($INSTANCE_NAME . "_Enable")`;

void inline `$INSTANCE_NAME`_disable(void) `=ReentrantKeil($INSTANCE_NAME . "_Enable")`;

//========================================================
//      CORDIC State functions
//========================================================

uint8_t inline `$INSTANCE_NAME`_start(void) `=ReentrantKeil($INSTANCE_NAME . "_start")`;

/**
 * @brief Halts the CORDIC engine
*/
uint8_t inline `$INSTANCE_NAME`_stop(void) `=ReentrantKeil($INSTANCE_NAME . "_stop")`;



uint8_t inline `$INSTANCE_NAME`_get_data(`$INSTANCE_NAME`_result_t* result) `=ReentrantKeil($INSTANCE_NAME . "_GetData")`;

//========================================================
//      CORDIC Interrupt functions
//========================================================

void inline `$INSTANCE_NAME`_enable_interrupt(void) `=ReentrantKeil($INSTANCE_NAME . "_EnableInterrupt")`;

void inline `$INSTANCE_NAME`_disable_interrupt(void) `=ReentrantKeil($INSTANCE_NAME . "_DisableInterrupt")`;


//========================================================
//      End of File
//========================================================
    
#endif //!`$INSTANCE_NAME`_H