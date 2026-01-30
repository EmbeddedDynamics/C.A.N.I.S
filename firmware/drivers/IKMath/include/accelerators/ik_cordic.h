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
    
/* Project headers */
#include "ik_results.h"
#include "ik_types.h"

#ifndef IK_CORDIC_SUPPORT_FPU
#define IK_CORDIC_SUPPORT_FPU 0
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
    IK_CORDIC_OP_CAP_NONE = 0u,
    IK_CORDIC_OP_CAP_ROTATION = IK_BIT(0u), // theta -> (s,c) or rotated vector
    IK_CORDIC_OP_CAP_VECTORING = IK_BIT(1u), // (x,y) -> (r,theta)
};
typedef uint32_t ik_cordic_op_caps_t;

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

#endif

typedef enum {
    IK_ANGLE_TURNS = 0, // 1.0 = full turn (2π)
    IK_ANGLE_DEGREES,
    IK_ANGLE_RADIANS,
} ik_angle_unit_t;

//========================================================
//      CORDIC Job Structure
//========================================================

typedef struct {
    ik_vector2f_t vec;
    float theta;
} ik_cordic_job_input_t;

typedef struct {
    ik_cordic_coord_t coord;
    ik_cordic_operation_t operation;
    ik_cordic_job_input_t input;
    
} ik_cordic_job_t;

//========================================================
//      CORDIC Configuration
//========================================================

typedef struct {
    bool use_gain_compensation;

    ik_cordic_caps_t supported_caps;

#if (IK_CORDIC_SUPPORT_FPU == 0)    
    ik_fixed_fmt_t vec_xy_fmt; // x,y format for vectoring inputs
    ik_fixed_fmt_t mag_fmt; // r output format
    ik_fixed_fmt_t unit_fmt; // scalars in [-1,1] (asin/acos input)
    ik_fixed_fmt_t ang_fmt; // angle output format (atan2, asin, acos)
#endif

    ik_angle_unit_t angle_unit;
    
    uint8_t iterations;

} ik_cordic_cfg_t;

//========================================================
//      CORDIC Handle
//========================================================

IK_HANDLE(ik_cordic);

//========================================================
//      CORDIC Methods
//========================================================

/**
 * @brief Create the CORDIC backend
 * 
 * @param[in] cfg - CORDIC creation configuration
 * @param[out] handle - CORDIC backend handle
 * 
 * @retval IK_RESULT_OK : CORDIC backend created
 * 
 */
ik_result_t ik_cordic_create_backend(
    ik_cordic_cfg_t* cfg,
    ik_cordic* handle
);

/**
 * @brief Perform a synchronus vectoring operation
 * 
 * @param[in] handle - CORDIC Handle
 * @param[in] coord - Coordinate system
 * @param[in] vec_in - Input vector
 * @param[out] out - Output vector pointer
 * 
 * @retval
 */
ik_result_t ik_cordic_vec_sync(
    ik_cordic handle, 
    ik_cordic_coord_t coord,
    const ik_vector2f_t *vec_in, 
    ik_vector2f_t *out
);

/**
 * @brief Perform a synchronus rotational operation
 * 
 * Perform a synchronus rational CORDIC operation in the specified
 * Coordinate system.
 * 
 * @param[in] handle - CODRIC Handle
 * @param[in] coord - Coordinate system
 * @param[in] theta - Starting angle
 * @param[out] out - Output vector pointer
 */
ik_result_t ik_cordic_rot_sync(
    ik_cordic handle, 
    ik_cordic_coord_t coord,
    float theta, 
    ik_vector2f_t* out
);

//========================================================
//      End of File
//========================================================

#endif // !IK_MATH_CORDIC_H