/***************************************************************************//**
* \file ik_cordic.h
* \version 1.0.0
*
* \brief
* Public header for the inerverse kinematics library
*
* \details
* Main header for the inverse kinematic math library.
*
********************************************************************************
* \copyright
* Copyright 2025, Embedded Dynamics. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef IK_MATH_CORDIC_H
#define IK_MATH_CORDIC_H

//========================================================
//      Standard Includes
//========================================================

/* Internal headers */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
    
/* Project headers */
#include "ik_results.h"
#include "ik_types.h"

#ifndef IK_USE_STATIC_JOB_QUEUE
    #define IK_USE_STATIC_JOB_QUEUE 1u
#endif

#ifndef IK_CORDIC_JOB_QUEUE_SIZE
    #define IK_CORDIC_JOB_QUEUE_SIZE 4u
#endif

#define IK_CORDIC_INVALID_JOB_ID 0xFFu

#define IK_CORDIC_KC            (1.64676f)
#define IK_CORDIC_KC_INVERSE    (0.60725f)

//========================================================
//      CORDIC FPU Support
//========================================================

#ifndef IK_CORDIC_SUPPORT_FPU
#define IK_CORDIC_SUPPORT_FPU 0
#endif

#if (IK_CORDIC_SUPPORT_FPU == 0)

//========================================================
//      BAMS Constants
//========================================================

#define IK_BAMS8_FULL_TURN_U32    (256u)
#define IK_BAMS16_FULL_TURN_U32   (65536u)
#define IK_BAMS32_FULL_TURN_U64   (4294967296ull)

#define IK_BAMS8_QUARTER_TURN   (64)
#define IK_BAMS8_HALF_TURN      (128)

#define IK_BAMS16_QUARTER_TURN  (16384)
#define IK_BAMS16_HALF_TURN     (32768)

#define IK_BAMS32_QUARTER_TURN  (1073741824ll)
#define IK_BAMS32_HALF_TURN     (2147483648ll)

#define IK_BAMS8_DEG_PER_LSB_F    (360.0f / 256.0f)
#define IK_BAMS16_DEG_PER_LSB_F   (360.0f / 65536.0f)
#define IK_BAMS32_DEG_PER_LSB_F   (360.0f / 4294967296.0f)

#define IK_BAMS8_RAD_PER_LSB_F    (6.2831853071795864769f / 256.0f)
#define IK_BAMS16_RAD_PER_LSB_F   (6.2831853071795864769f / 65536.0f)
#define IK_BAMS32_RAD_PER_LSB_F   (6.2831853071795864769f / 4294967296.0f)

#define IK_BAMS8_TURNS_PER_LSB_F    (1.0f / 256.0f)
#define IK_BAMS16_TURNS_PER_LSB_F   (1.0f / 65536.0f)
#define IK_BAMS32_TURNS_PER_LSB_F   (1.0f / 4294967296.0f)

//========================================================
//      BAMS Float conversions
//========================================================

#define IK_DEG_TO_BAMS8_F(deg_f)   ((ik_bams8_t) ((deg_f) * (256.0f / 360.0f)))
#define IK_DEG_TO_BAMS16_F(deg_f)  ((ik_bams16_t)((deg_f) * (65536.0f / 360.0f)))
#define IK_DEG_TO_BAMS32_F(deg_f)  ((ik_bams32_t)((deg_f) * (4294967296.0f / 360.0f)))

#define IK_RAD_TO_BAMS8_F(rad_f)   ((ik_bams8_t) ((rad_f) * (256.0f / 6.2831853071795864769f)))
#define IK_RAD_TO_BAMS16_F(rad_f)  ((ik_bams16_t)((rad_f) * (65536.0f / 6.2831853071795864769f)))
#define IK_RAD_TO_BAMS32_F(rad_f)  ((ik_bams32_t)((rad_f) * (4294967296.0f / 6.2831853071795864769f)))

#define IK_TURNS_TO_BAMS8_F(rad_f)   ((ik_bams8_t) ((rad_f) * (256.0f / 1.0f)))
#define IK_TURNS_TO_BAMS16_F(rad_f)  ((ik_bams16_t)((rad_f) * (65536.0f / 1.0f)))
#define IK_TURNS_TO_BAMS32_F(rad_f)  ((ik_bams32_t)((rad_f) * (4294967296.0f / 1.0f)))

#define IK_BAMS8_TO_DEG_F(b)    ((float)(b) * IK_BAMS8_DEG_PER_LSB_F)
#define IK_BAMS16_TO_DEG_F(b)   ((float)(b) * IK_BAMS16_DEG_PER_LSB_F)
#define IK_BAMS32_TO_DEG_F(b)   ((float)(b) * IK_BAMS32_DEG_PER_LSB_F)

#define IK_BAMS8_TO_RAD_F(b)    ((float)(b) * IK_BAMS8_RAD_PER_LSB_F)
#define IK_BAMS16_TO_RAD_F(b)   ((float)(b) * IK_BAMS16_RAD_PER_LSB_F)
#define IK_BAMS32_TO_RAD_F(b)   ((float)(b) * IK_BAMS32_RAD_PER_LSB_F)

#define IK_BAMS8_TO_TURNS_F(b)    ((float)(b) * IK_BAMS8_TURNS_PER_LSB_F)
#define IK_BAMS16_TO_TURNS_F(b)   ((float)(b) * IK_BAMS16_TURNS_PER_LSB_F)
#define IK_BAMS32_TO_TURNS_F(b)   ((float)(b) * IK_BAMS32_TURNS_PER_LSB_F)

//========================================================
//      BAMS Cross-width conversions
//========================================================

#define IK_BAMS8_TO_BAMS16(b8)   ((ik_bams16_t)((int16_t)(b8) << 8))
#define IK_BAMS16_TO_BAMS8(b16)  ((ik_bams8_t)((int16_t)(b16) >> 8))

#define IK_BAMS16_TO_BAMS32(b16) ((ik_bams32_t)((int32_t)(b16) << 16))
#define IK_BAMS32_TO_BAMS16(b32) ((ik_bams16_t)((int32_t)(b32) >> 16))

#define IK_BAMS8_TO_BAMS32(b8)   ((ik_bams32_t)((int32_t)(b8) << 24))
#define IK_BAMS32_TO_BAMS8(b32)  ((ik_bams8_t)((int32_t)(b32) >> 24))

#endif

//========================================================
//      CORDIC Coordinate Systems
//========================================================

typedef enum {
    IK_CORDIC_COORD_CIRCULAR = 0,
    IK_CORDIC_COORD_LINEAR,
    IK_CORDIC_COORD_HYPERBOLIC,
} ik_cordic_coord_t;

typedef enum {
    IK_CORDIC_OPERATION_ROTATION,
    IK_CORDIC_OPERATION_VECTORING,
} ik_cordic_operation_t;

//========================================================
//      CORDIC Capabilities
//========================================================

enum {
    IK_CORDIC_OP_CAP_NONE = IK_BIT(0u),
    IK_CORDIC_OP_CAP_ROTATION = IK_BIT(1u), // theta -> (s,c) or rotated vector
    IK_CORDIC_OP_CAP_VECTORING = IK_BIT(2u), // (x,y) -> (r,theta)
};
typedef ik_flags8_t ik_cordic_op_caps_t;

typedef struct {
    ik_cordic_op_caps_t linear_ops;
    ik_cordic_op_caps_t circular_ops;
    ik_cordic_op_caps_t hyperbolic_ops;
} ik_cordic_caps_t;

//========================================================
//      CORDIC Formats
//========================================================

#if (IK_CORDIC_SUPPORT_FPU == 0)

typedef struct {
    uint8_t total_bits;
    uint8_t frac_bits;
    bool is_signed;
} ik_fixed_fmt_t;

#define IK_QFMT(bits, frac) ((ik_fixed_fmt_t){ .total_bits=bits, .frac_bits=(frac), .is_signed=1 })

#define IK_QSCALE(fmt)   ((float)(1u << ((fmt).frac_bits)))
#define IK_QINV(fmt)     (1.0f / IK_QSCALE(fmt))

#define IK_QFMT_TO_I8(f,  fmt)  ((int8_t) ((f)  * IK_QSCALE(fmt)))
#define IK_QFMT_TO_I16(f, fmt)  ((int16_t)((f)  * IK_QSCALE(fmt)))
#define IK_QFMT_TO_I32(f, fmt)  ((int32_t)((f)  * IK_QSCALE(fmt)))

#define IK_I8_TO_QFMT_F(i,  fmt)  ((float)(int8_t)(i)  *  IK_QINV(fmt))
#define IK_I16_TO_QFMT_F(i, fmt)  ((float)(int16_t)(i) *  IK_QINV(fmt))
#define IK_I32_TO_QFMT_F(i, fmt)  ((float)(int32_t)(i) *  IK_QINV(fmt))

#endif

typedef enum {
    IK_ANGLE_TURNS = 0, // 1.0 = full turn (2π)

    /**
     * @brief Binary Angular Measurement representation
     */
    IK_ANGLE_BAMS,
    
    IK_ANGLE_RADIANS,
} ik_angle_unit_t;

typedef enum {
    IK_CORDIC_FORMAT_VECTOR,
    IK_CORDIC_FORMAT_UNIT,
} ik_format_t;

//========================================================
//      CORDIC Configuration
//========================================================

typedef struct {
    bool use_gain_compensation;

    ik_cordic_caps_t supported_caps;

#if (IK_CORDIC_SUPPORT_FPU == 0)    
    ik_fixed_fmt_t vec_fmt; // x,y format for vectoring inputs
    ik_fixed_fmt_t unit_fmt; // scalars in [-1,1] (asin/acos input)
#endif

    uint16_t timeout;
    
    uint8_t iterations;
    
    ik_tick_fn_t tick;

} ik_cordic_cfg_t;

//========================================================
//      CORDIC Handle
//========================================================

IK_HANDLE(ik_cordic);

//========================================================
//      CORDIC Creation Methods
//========================================================

/**
 * @brief Create the CORDIC backend
 * 
 * @param[in] cfg - CORDIC creation configuration
 * @param[inout] handle - CORDIC backend handle
 * 
 * @retval IK_RESULT_OK : CORDIC backend created
 * @retval IK_ERROR_NULL_POINTER : Either @p cfg or @p handle is a NULL pointer
 */
ik_result_t ik_cordic_create_backend(
    ik_cordic_cfg_t* cfg,
    ik_cordic_h* handle
);

//========================================================
//      CORDIC General API's
//========================================================

/**
 * @brief Start the CORDIC backend
 * 
 * @param[in] handle - CORDIC backend handle
 * 
 * @retval IK_RESULT_OK : CORDIC backend started
 * @retval IK_ERROR_NULL_POINTER : The @p handle is a NULL pointer
 */
ik_result_t ik_cordic_start(ik_cordic_h handle);

/**
 * @brief Get the CORDIC capabilities
 * 
 * @return ik_cordic_caps_t - The capabilities of the CORDIC backend
 */
ik_cordic_caps_t ik_get_cordic_caps(void);

//========================================================
//      CORDIC Synchronus API's
//========================================================

/**
 * @brief Perform a synchronous CORDIC vectoring operation.
 *
 * Computes the CORDIC vectoring operation on the input vector in the specified
 * coordinate system.
 *
 * @param[in]  handle   CORDIC instance handle.
 * @param[in]  coord    Coordinate system to use.
 * @param[in]  format   Fixed-point format of the inputs.
 * @param[in]  vec_in   Input vector.
 * @param[out] out      Pointer to the output vector.
 *
 * @retval IK_ERROR_CORDIC_NO_BACKEND - The @p handle is not a valid CORDIC backend
 * @retval IK_ERROR_CORDIC_NOT_SUPPORTED - Either the vectoring operation or the specified coordinate system is not supported
 */
ik_result_t ik_cordic_vec_sync(
    ik_cordic_h handle,
    ik_cordic_coord_t coord,
    ik_format_t format,
    const ik_vector3f_t* vec_in,
    ik_vector3f_t* out
);

/**
 * @brief Perform a synchronous CORDIC rotation operation.
 *
 * Computes a CORDIC rotation in the specified coordinate system for the given
 * input angle.
 *
 * @param[in]  handle   CORDIC instance handle.
 * @param[in]  coord    Coordinate system to use.
 * @param[in]  format   Fixed-point format of the inputs.
 * @param[in]  theta    Rotation angle (radians unless otherwise specified).
 * @param[out] out      Pointer to the output vector.
 *
 * @retval IK_ERROR_NULL_POINTER - Either @p handle or @p out is a NULL pointer
 * @retval IK_ERROR_CORDIC_NO_BACKEND - The @p handle is not a valid CORDIC backend
 * @retval IK_ERROR_CORDIC_NOT_SUPPORTED - Either the rotational operation or the specified coordinate system is not supported
 */
ik_result_t ik_cordic_rot_sync(
    ik_cordic_h handle,
    ik_cordic_coord_t coord,
    ik_format_t format,
    const ik_vector3f_t* vec_in,
    ik_vector3f_t* out
);

//========================================================
//      CORDIC Asynchronus API's
//========================================================

/**
 * @brief Poll the CORDIC driver
 * 
 * Process internal CORDIC information and updates the
 * state machine
 * 
 * @param[in] handle - CORDIC handle
 * 
 * @retval IK_RESULT_OK - CORDIC polled succesfully
 */
ik_result_t ik_cordic_poll(ik_cordic_h handle);

/**
 * @brief Directly queue a CORDIC job
 * 
 * @param[in] handle - CORDIC handle
 * @param[in] job - Address to the CORDIC job
 * 
 * @note The CORDIC won't take ownership of @p job the structure must stay valid
 * in it's lifespan of the CORDIC statemachine.
 */
//ik_result_t ik_cordic_submit_job(
//    ik_cordic_h handle,
//    ik_cordic_job_t* job
//);

//========================================================
//      End of File
//========================================================

#endif // !IK_MATH_CORDIC_H