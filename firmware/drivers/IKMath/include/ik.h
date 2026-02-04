/***************************************************************************//**
* \file IK.h
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

#ifndef IK_MATH_H
#define IK_MATH_H

//========================================================
//      Standard Includes
//========================================================

/* Internal headers */
#include <stdint.h>
#include <stdbool.h>
    
/* Project headers */
#include "ik_results.h"
#include "ik_types.h"

//========================================================
//      IK Types
//========================================================

typedef void* ik_ctx_t;

//========================================================
//      Solver Enums
//========================================================

/**
 * @brief Solving mode requested by the user
 * 
 * A solving mode must be provided by the user. The model
 * must be able to support the selected mode
 */
typedef enum {
    IK_SOLVE_MODE_UNKNOWN = 0x00,
    IK_SOLVE_MODE_ANALYTIC,
    IK_SOLVE_MODE_NUMERIC,
    IK_SOLVE_MODE_PREFER_ANALYTIC,
    IK_SOLVE_MODE_PREFER_NUMERIC,
} ik_solve_mode_t;

typedef enum {
    IK_NUMERICAL_SOLVER_NONE = 0x00,
    IK_NUMERICAL_SOLVER_JACOBIAN_TRANSPOSE,
    IK_NUMERICAL_SOLVER_PSEUDO_INVERSE,
    IK_NUMERICAL_SOLVER_DLS,
} ik_numerical_solver_t;

//========================================================
//      IK Model Feature flags
//========================================================

enum {
    IK_FEATURE_NONE               = 0u,
    IK_FEATURE_FORWARD_KINEMATICS = IK_BIT(0),
    IK_FEATURE_JOINT_LIMITS       = IK_BIT(1),
    IK_FEATURE_ANALYTIC_IK        = IK_BIT(2),
    IK_FEATURE_NUMERIC_IK         = IK_BIT(3),
};
typedef ik_flags32_t ik_feature_flags_t;

//========================================================
//      IK Model Capabilities
//========================================================

/**
 * @brief IK capability flags
 * 
 * The capability flags are supplied to a IK model. These
 * flags are used to to what the model can be capable of.
 */
enum {
    IK_CAP_NONE             = 0u,
    IK_CAP_FK               = IK_BIT(0),
    IK_CAP_JACOBIAN         = IK_BIT(1),
    IK_CAP_ANALYTIC_IK      = IK_BIT(2),
    IK_CAP_LIMITS           = IK_BIT(3),
    IK_CAP_CORDIC_SUPPORT   = IK_BIT(4),
    IK_CAP_DMA_SUPPORT      = IK_BIT(5),
};
typedef ik_flags32_t ik_cap_flags_t;

//========================================================
//      IK Model Configuration
//========================================================

typedef struct {
    ik_solve_mode_t solve_mode;

    /* Numeric solver selection and parameters (used when numeric is selected). */
    ik_numerical_solver_t numerical_solver;
    uint16_t max_iters;

    /* Tolerances (units depend on your pose conventions; typically meters/radians). */
    //ik_real_t tol_pos;
    //ik_real_t tol_rot;

    /* Optional numeric tuning (e.g. DLS lambda, step limiting). */
    //ik_real_t damping;          /* used by DLS; 0 = disable */
    //ik_real_t step_limit;       /* 0 = disable */

#if defined(IK_ENABLE_HEAP)

    /**
     * @brief Buffer used for no-heap builds
     * 
     * For no-heap builds the user must provide pre-allocated memory
     */
    void*  buffer;

    
    size_t buffer_len;

#endif

    // Model feature flags
    ik_feature_flags_t features;
} ik_model_cfg_t;

//========================================================
//      End of File
//========================================================

#endif // !IK_MATH_H