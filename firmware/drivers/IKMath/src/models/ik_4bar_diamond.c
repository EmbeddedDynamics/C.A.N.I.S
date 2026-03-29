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

//========================================================
//      Standard Includes
//========================================================

/* Internal headers */
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
    
/* Project headers */
#include "accelerators/ik_cordic.h"

#include "ik_model.h"
#include "models/ik_4bar_diamond.h"

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#ifndef M_PI_2
    #define M_PI_2 1.57079632679489661923
#endif

#ifndef M_PI_4
    #define M_PI_4 0.78539816339744830962
#endif

#ifndef M_2_PI
    #define M_2_PI 0.63661977236758134308
#endif

#ifndef M_2PI
    #define M_2PI (2.0 * M_PI)
#endif

#ifndef M_4PI
    #define M_4PI (4.0 * M_PI)
#endif

static inline float clampf(float x, float lo, float hi)
{
    return (x < lo) ? lo : (x > hi) ? hi : x;
}

//========================================================
//      Function Pointers
//========================================================

typedef ik_result_t (*ik_4bar_ik_fn_t)(
    ik_model_h handle, 
    const ik_vector2f_t* target, 
    ik_vector2f_t* solution
);

//========================================================
//      Model Context
//========================================================

typedef struct {
    float l1;   /* length of link 1 */
    float l2;   /* length of link 2 */

    float l1_pow; /* precomputed length of link 1 squared */
    float l2_pow; /* precomputed length of link 2 squared */

    float scale;  /* scale factor for output angles (e.g. to convert to degrees or encoder ticks) */
} ik_4bar_diamond_const_params_t;

typedef struct {
    bool is_initialized;

    ik_4bar_ik_fn_t solve_ik;

    ik_cordic_h cordic;

    ik_4bar_diamond_const_params_t const_params;
} ik_4bar_diamond_ctx_t;

//========================================================
//      Model Globals
//========================================================

static const ik_feature_flags_t g_4bar_diamond_capabilities = 
            IK_FEAT_CORDIC_SUPPORT | 
            IK_FEAT_DMA_SUPPORT |
            IK_FEAT_LIMITS |
            IK_FEAT_BATCHING_SUPPORT;

static ik_4bar_diamond_ctx_t g_4bar_diamond_ctx = {
    .is_initialized = false,
    .cordic = NULL
};

//========================================================
//      Model Internal API's
//========================================================

/* Vectoring: returns C = hypot(x,y) in X and theta = atan2(y,x) in Z (per your existing design) */
ik_result_t ik_4bar_diamond_cordic_ik(ik_model_h handle, const ik_vector2f_t* target, ik_vector2f_t* solution)
{
    ik_vector3f_t input   = { 
        .v = { 
            IK_VEC2_XP(target), 
            IK_VEC2_YP(target), 
            -(float)M_PI_2 
        } 
    };
    ik_vector3f_t vec_res = { 0 };

    ik_result_t res = ik_cordic_vec_sync(
        g_4bar_diamond_ctx.cordic,
        IK_CORDIC_COORD_CIRCULAR,
        IK_CORDIC_FORMAT_VECTOR,
        &input,
        &vec_res
    );

    const float C     = IK_VEC3_X(vec_res);
    const float theta = IK_VEC3_Z(vec_res);

    /* Use float ops only */
    const float C2 = C * C;
    
    //float cos_alpha = ( ((float)IK_L2_POW - (float)IK_L1_POW) - C2 ) * (IK_K_CONST / C);
    float cos_alpha = (g_4bar_diamond_ctx.const_params.l2_pow - g_4bar_diamond_ctx.const_params.l1_pow - (C*C)) / (-2.0f * g_4bar_diamond_ctx.const_params.l1 * C);
    
    /* Clamp to [-1, 1] to avoid domain issues */
    cos_alpha = clampf(cos_alpha, -1.0f, 1.0f);

    /* alpha = acos(cos_alpha) replaced by: alpha = atan2(sqrt(1-c^2), c) using CORDIC */
    float sin_alpha = 1.0f - (cos_alpha * cos_alpha);
    //if (sin_alpha < 0.0f) sin_alpha = 0.0f;       /* numerical safety */
    sin_alpha = sqrtf(sin_alpha);

    /* Use CORDIC vectoring on (x=cos_alpha, y=sin_alpha) => angle in Z */
    ik_vector3f_t a_in  = { .v = { cos_alpha, sin_alpha, 0.0f } };
    ik_vector3f_t a_out = { 0 };

    ik_cordic_vec_sync(
        g_4bar_diamond_ctx.cordic,
        IK_CORDIC_COORD_CIRCULAR,
        IK_CORDIC_FORMAT_UNIT,
        &a_in,
        &a_out
    );

    const float alpha = IK_VEC3_Z(a_out);

    /* End timing probe */
    //SysCntVal = (SYSTICK_MAXVAL - (SysTick->VAL)) - SysCntVal;

    /* Two solutions */
    const float phi1 = theta + alpha;
    const float phi2 = theta - alpha;

    /* Scale to your output units */
    IK_VEC2_XP(solution) = phi1 * g_4bar_diamond_ctx.const_params.scale;
    IK_VEC2_YP(solution) = phi2 * g_4bar_diamond_ctx.const_params.scale;
    
    return IK_RESULT_OK;
}

//========================================================
//      Model Instance API's
//========================================================

/**
 * @brief Create a new instance of the 4-bar diamond IK model
 * 
 * 
 * 
 * @param[in] cfg - Pointer to a model configuration
 * @param[out] handle - Pointer to a model handle
 * 
 * @retval IK_RESULT_OK - if the model was successfully created
 * @retval IK_ERROR_NULL_POINTER - if @p cfg or @p handle is NULL
 * @retval IK_ERROR_INVALID_PARAM - if the configuration is invalid (e.g. unsupported mode)
 * @retval IK_ERROR_NO_MEMORY - if the model could not be created due to memory
 */
ik_result_t ik_4bar_create(ik_model_cfg_t* cfg, ik_model_h* handle)
{
    // Parameter validation
    if (!cfg || !handle || !cfg->ctx) {
        return IK_ERROR_NULL_POINTER;
    }

    if (g_4bar_diamond_ctx.is_initialized) {
        return IK_ERROR_INVALID_PARAM; // Model instance already exists
    }

    #if defined(IK_ENABLE_HEAP)
        ik_model_h new_model = IK_MALLOC(sizeof(struct ik_model_T), alignof(struct ik_model_T));
        if (!new_model)
            return IK_ERROR_NO_MEMORY;
    #else
        ik_model_h new_model = *handle;
    #endif

    ik_feature_flags_t feat_caps = cfg->features;

    ik_4bar_diamond_params_t* params = (ik_4bar_diamond_params_t*) cfg->ctx;

    // Create and initialize the CORDIC engine if requested
    if (feat_caps & IK_FEAT_CORDIC_SUPPORT) {
        
        ik_cordic_h cordic_handle;

        // Check if the CORDIC backend can support the required operations for the 4-bar diamond model
        ik_cordic_caps_t caps = {0u, 0u, 0u};
        caps = ik_get_cordic_caps();

        ik_cordic_op_caps_t required_caps = IK_CORDIC_OP_CAP_ROTATION | IK_CORDIC_OP_CAP_VECTORING;
        if ((caps.circular_ops & required_caps) != required_caps) {
            return IK_ERROR_INVALID_PARAM;
        }

        ik_cordic_cfg_t cordic_cfg = {
            .use_gain_compensation = true,
            .supported_caps = caps,

            .vec_fmt = IK_QFMT(16u, 4u),
            .unit_fmt = IK_QFMT(16u, 14u),

            .tick = cfg->tick,
        };

        IK_CHECK(ik_cordic_create_backend(&cordic_cfg, &cordic_handle));
        g_4bar_diamond_ctx.cordic = cordic_handle;

        ik_cordic_start(cordic_handle);
        
        g_4bar_diamond_ctx.solve_ik = ik_4bar_diamond_cordic_ik;
    }

    // Store and calculate the constant parameters
    g_4bar_diamond_ctx.const_params.l1 = params->l1;
    g_4bar_diamond_ctx.const_params.l2 = params->l2;
    g_4bar_diamond_ctx.const_params.l1_pow = params->l1 * params->l1;
    g_4bar_diamond_ctx.const_params.l2_pow = params->l2 * params->l2;
    g_4bar_diamond_ctx.const_params.scale = params->scale;
    g_4bar_diamond_ctx.is_initialized = true;

    new_model->cfg = *cfg;

    *handle = new_model;

    return IK_RESULT_OK;
}

//========================================================
//      Model API's
//========================================================

ik_feature_flags_t ik_4bar_diamond_get_capabilities(void)
{
    return g_4bar_diamond_capabilities;
}

ik_cordic_h ik_4bar_diamond_get_cordic_handle(void)
{
    return g_4bar_diamond_ctx.cordic;
}

ik_result_t ik_4bar_diamond_ik(ik_model_h handle, const ik_vector2f_t* target, ik_vector2f_t* solution)
{
    if (!handle || !target || !solution) {
        return IK_ERROR_NULL_POINTER;
    }

    if (!g_4bar_diamond_ctx.is_initialized) {
        return IK_ERROR_INVALID_PARAM; // Model instance not initialized
    }

    IK_CHECK(g_4bar_diamond_ctx.solve_ik(handle, target, solution));

    return IK_RESULT_OK;
}

//========================================================
//      End of File
//========================================================













// void ik_cordic_f(float x, float y, ik_res_t* res)
// {
//     /* Vectoring: returns C = hypot(x,y) in X and theta = atan2(y,x) in Z (per your existing design) */
//     ik_vector3f_t input   = { .v = { x, y, -(float)M_PI_2 } };
//     ik_vector3f_t vec_res = { 0 };

//     ik_cordic_vec_sync(
//         ik_cordic,
//         IK_CORDIC_COORD_CIRCULAR,
//         IK_CORDIC_FORMAT_VECTOR,
//         &input,
//         &vec_res
//     );

//     const float C     = IK_VEC3_X(vec_res);
//     const float theta = IK_VEC3_Z(vec_res);

//     /* Timing probe (same idea as yours) */
//     //SysCntVal = SYSTICK_MAXVAL - (SysTick->VAL);

//     /* cos(alpha) = (L2^2 - L1^2 - C^2)/(-2*L1*C) */
//     /* Use float ops only */
//     const float C2 = C * C;
    
//     //float cos_alpha = ( ((float)IK_L2_POW - (float)IK_L1_POW) - C2 ) * (IK_K_CONST / C);
//     float cos_alpha = (IK_L2_POW - IK_L1_POW - (C*C)) / (-2.0f * IK_L1 * C);
    
//     /* Clamp to [-1, 1] to avoid domain issues */
//     cos_alpha = clampf(cos_alpha, -1.0f, 1.0f);

//     /* alpha = acos(cos_alpha) replaced by: alpha = atan2(sqrt(1-c^2), c) using CORDIC */
//     volatile float sin_alpha = 1.0f - (cos_alpha * cos_alpha);
//     if (sin_alpha < 0.0f) sin_alpha = 0.0f;       /* numerical safety */
//     sin_alpha = sqrtf(sin_alpha);

//     /* Use CORDIC vectoring on (x=cos_alpha, y=sin_alpha) => angle in Z */
//     ik_vector3f_t a_in  = { .v = { cos_alpha, sin_alpha, 0.0f } };
//     ik_vector3f_t a_out = { 0 };

//     ik_cordic_vec_sync(
//         ik_cordic,
//         IK_CORDIC_COORD_CIRCULAR,
//         IK_CORDIC_FORMAT_UNIT,
//         &a_in,
//         &a_out
//     );

//     const float alpha = IK_VEC3_Z(a_out);

//     /* End timing probe */
//     //SysCntVal = (SYSTICK_MAXVAL - (SysTick->VAL)) - SysCntVal;

//     /* Two solutions */
//     const float phi1 = theta + alpha;
//     const float phi2 = theta - alpha;

//     /* Scale to your output units */
//     res->phi1 = phi1 * IK_PHI_SCALE;
//     res->phi2 = phi2 * IK_PHI_SCALE;
// }

// void ik_simple(float x, float y, volatile ik_res_t* res)
// {
//     float C = sqrtf(x*x + y*y);
//     float theta = atan2(y, x);
    
//     float cos_alpha = (IK_L2_POW - IK_L1_POW - (C*C)) / (-2.0f * IK_L1 * C);
//     cos_alpha = fmax(fmin(cos_alpha, 1.f), -1.f);
    
//     float alpha = acos(cos_alpha);

//     float phi1 = theta + alpha - (M_PI_2);
//     float phi2 = theta - alpha - (M_PI_2);
    
//     res->phi1 =  -(phi1 / (M_2PI)) * 7.0f;
//     res->phi2 =  -(phi2 / (M_2PI)) * 7.0f;
// }

// float ik_cordic_acos_test(float c)
// {
//     /* Clamp input */
//     if (c >  1.0f) c =  1.0f;
//     if (c < -1.0f) c = -1.0f;

//     /* Compute sin = +sqrt(1 - c^2) */
//     float s = 1.0f - c * c;
//     if (s < 0.0f) s = 0.0f;
//     s = sqrtf(s);

//     /* Build vectoring job:
//        atan2(y=s, x=c) */
//     ik_vector3f_t in = {
//         .v = { c, s, 0.0f }   /* z=0, offset handled via submit */
//     };

//     ik_vector3f_t out = {0};

//     ik_cordic_vec_sync(
//         ik_cordic,
//         IK_CORDIC_COORD_CIRCULAR,
//         IK_CORDIC_FORMAT_UNIT,
//         &in,
//         &out
//     );

//     /* IMPORTANT:
//        submit() has already stored quadrant correction in job->z_offset_rad
//        vec_sync() should apply it when forming out.z
//     */
//     return IK_VEC3_Z(out);
// }

/*float test[] = { -1.0f, -0.5f, 0.0f, 0.5f, 1.0f };

    for (int i = 0; i < 5; i++)
    {
        float c = test[i];
        float hw = ik_cordic_acos_test(c);
        float sw = acosf(c);

        printf("c=%+.2f  hw=%f  sw=%f\n", c, hw, sw);
    }*/