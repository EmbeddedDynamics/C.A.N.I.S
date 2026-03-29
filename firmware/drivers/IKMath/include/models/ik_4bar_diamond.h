/***************************************************************************//**
* \file ik_4bar_diamond.h
* \version 1.0.0
*
* \brief
* Public header for the 4-bar diamond IK model
*
* \details
* This header provides the public interface for the 4-bar diamond IK model. It defines the
* necessary types, functions, and constants for using the 4-bar diamond IK model within the IKMath library.
*
********************************************************************************
* \copyright
* Copyright 2025, Embedded Dynamics. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef IK_MATH_4BAR_DIAMOND_H
#define IK_MATH_4BAR_DIAMOND_H

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
#include "ik.h"

//========================================================
//      Model Parameter Config
//========================================================

typedef struct {
    float l1;   /* length of link 1 */
    float l2;   /* length of link 2 */

    float scale;  /* scale factor for output angles (e.g. to convert to degrees or encoder ticks) */
} ik_4bar_diamond_params_t;

//========================================================
//      Model Instance API's
//========================================================

/**
 * @brief Create a new instance of the 4-bar diamond IK model
 * 
 * @param[in] cfg - Pointer to a model configuration
 * @param[out] handle - Pointer to a model handle
 * 
 * @retval IK_RESULT_OK - if the model was successfully created
 * @retval IK_ERROR_NULL_POINTER - if @p cfg or @p handle is NULL
 * @retval IK_ERROR_INVALID_PARAM - if the configuration is invalid (e.g. unsupported mode)
 * @retval IK_ERROR_NO_MEMORY - if the model could not be created due to memory
 */
ik_result_t ik_4bar_create(ik_model_cfg_t* cfg, ik_model_h* handle);

//========================================================
//      Model API's
//========================================================

/**
 * @brief Fetch the capability flags for the 4-bar diamond model
 * 
 * These capability flags indicate what features the 4-bar diamond
 * model can support. The user can enable these features via the #ik_feature_flags_t
 * field in the #ik_model_cfg_t when initializing the model. The model will only operate
 * in modes that are supported by its capabilities.
 * 
 * @return ik_cap_flags_t - Bitmask of capability flags supported by the 4-bar diamond model
 */
ik_feature_flags_t ik_4bar_diamond_get_capabilities(void);

ik_cordic_h ik_4bar_diamond_get_cordic_handle(void);

/**
 * @brief Solve the IK for a given target position
 * 
 * @param[in] handle - Handle to the 4-bar diamond model instance
 * @param[in] target - Pointer to the target position for the end-effector (2D vector)
 * @param[out] solution - Pointer to a 2D vector where the joint angles solution will be stored
 */
ik_result_t ik_4bar_diamond_ik(ik_model_h handle, const ik_vector2f_t* target, ik_vector2f_t* solution);

//========================================================
//      End of File
//========================================================

#endif // !IK_MATH_4BAR_DIAMOND_H