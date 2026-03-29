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

//========================================================
//      Standard Includes
//========================================================

/* Internal headers */
#include <stdint.h>
#include <stdbool.h>
    
/* Project headers */
#include "accelerators/ik_cordic_internal.h"

#if defined(__has_include)
    #if __has_include("cyfitter.h")
        #include "cyfitter.h"
        
        #if defined(CYDEV_CHIP_FAMILY_USED)
            #if (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3)
                #define IK_MATH_PLATFORM_PSOC3
            #elif (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4)
                #define IK_MATH_PLATFORM_PSOC4
            #elif (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5)
                #define IK_MATH_PLATFORM_PSOC5
            #elif (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC6)
                #define IK_MATH_PLATFORM_PSOC6
            #endif
        #endif
    #endif
#endif

#if defined(IK_MATH_PLATFORM_PSOC5)

#define IK_PSOC5_CONST(name) \
    IK_CAT3(CORDIC_COMPONENT_NAME, _, name)

#define IK_PSOC5_TYPE(name) \
    IK_CAT3(CORDIC_COMPONENT_NAME, _, name)

#define IK_PSOC5_MACRO_CALL_(component, macro, ...) \
    IK_CAT3(component, _, macro)(__VA_ARGS__)

#define IK_PSOC5_MACRO_CALL(macro, ...) \
    IK_PSOC5_MACRO_CALL_(CORDIC_COMPONENT_NAME, macro, __VA_ARGS__)

#define IK_PSOC5_CALL_(component, func, ...) \
    IK_CAT3(component, _, func)(__VA_ARGS__)

#define IK_PSOC5_CALL(func, ...) \
    IK_PSOC5_CALL_(CORDIC_COMPONENT_NAME, func, __VA_ARGS__)
    
#if __has_include("cyapicallbacks.h")
    #include "cyapicallbacks.h"
#else
    #error "CY API callbacks header now found"
#endif

#if __has_include(IK_STR(CORDIC_COMPONENT_NAME.h))
    #include IK_STR(CORDIC_COMPONENT_NAME.h)
#else
    #error "CY API callbacks header now found"
#endif

#define CORDIC_WAIT_UNTIL_EMPTY(timeout_us, ok_out)                             \
    do {                                                                        \
        uint16_t _t = (uint16_t)(timeout_us);                                   \
        uint8_t  _filled;                                                       \
        do {                                                                    \
            _filled = (uint8_t)(IK_PSOC5_CONST(STATUS_REG) &                    \
                                IK_PSOC5_CONST(STS_X_FILLED));                  \
            if (_filled == 0u) break;                                           \
            if (_t == 0u) break;                                                \
            _t--;                                                               \
        } while (1);                                                            \
        (ok_out) = (_filled == 0u);                                             \
    } while (0)

//========================================================
//      PSoC5 CORDIC Globals
//========================================================

static ik_cordic_h g_cordic = NULL;

static const ik_cordic_caps_t psoc5_cordic_caps = {
    .linear_ops = IK_CORDIC_OP_CAP_NONE,
    .circular_ops = IK_CORDIC_OP_CAP_ROTATION | IK_CORDIC_OP_CAP_VECTORING,
    .hyperbolic_ops = IK_CORDIC_OP_CAP_NONE
};

//========================================================
//      PSoC5 CORDIC Callbacks
//========================================================

static volatile uint32_t finish_errs;

#if (IK_PSOC5_CONST(DONE_CALLBACK))
            
void CORDIC_done_callback(void)
{
    if (g_cordic == NULL)
        return;
    
    uint8 interrupt_state = CyEnterCriticalSection();
  
    if (ik_cordic_finish_head(g_cordic) != IK_RESULT_OK)
        finish_errs++;
    
    CyExitCriticalSection( interrupt_state );
}
        
#endif

//========================================================
//      PSoC5 CORDIC Methods
//========================================================

ik_result_t ik_cordic_psoc5_submit(ik_ctx_t ctx, ik_cordic_job_t* job)
{
    if (!ctx || !job)
        return IK_ERROR_NULL_POINTER;

    uint8_t current_mode = (uint8_t)(IK_PSOC5_CONST(CONTROL_REG) & IK_PSOC5_CONST(OPER_MODE));

    uint8_t desired_mode = (job->operation == IK_CORDIC_OPERATION_ROTATION)
        ? IK_PSOC5_CONST(ROTATING_OPER)
        : IK_PSOC5_CONST(VECTORING_OPER);

    // If mode must change, drain/flush first
    if (current_mode != desired_mode)
    {
        // Wait until the X-fifo is empty
        ik_result_t res;
        CORDIC_WAIT_UNTIL_EMPTY(500u, res);

        // Write mode field (clear then set)
        IK_PSOC5_CONST(CONTROL_REG) = (IK_PSOC5_CONST(CONTROL_REG) & (uint8_t)~ IK_PSOC5_CONST(OPER_MODE)) | desired_mode;
        CORDIC_COMMIT_CTRL;
        
        CyDelayUs(1);
        //CORDIC_CONTROL_REG |= CORDIC_OPER_MODE;
        
        while ((IK_PSOC5_CONST(CONTROL_REG) & IK_PSOC5_CONST(OPER_MODE)) != desired_mode) {}
    }
    
    ik_cordic_h h = (ik_cordic_h) ctx;

    // Build the vector to queue
    IK_PSOC5_TYPE(vector_t) v = {0u, 0u, 0u};
    
    /* Only do quadrant pre-rotation for VECTORING */
    float xin = IK_VEC3_X(job->input);
    float yin = IK_VEC3_Y(job->input);

    job->z_offset_rad = (job->operation == IK_CORDIC_OPERATION_VECTORING) ? IK_VEC3_Z(job->input) : 0.0f; /* <-- add this field, or store elsewhere */

    if (job->operation == IK_CORDIC_OPERATION_VECTORING && xin < 0.0f)
    {
        xin = -xin;
        yin = -yin;
        job->z_offset_rad += (float)M_PI; /* add to z_out later */
    }

    /* X/Y packing (FIX: handle both branches) */
    if (h->config.use_gain_compensation)
    {
        v.x = IK_QFMT_TO_I16(xin * IK_PSOC5_CONST(KC_INVERSE), job->fmt); //h->config.vec_fmt);
        v.y = IK_QFMT_TO_I16(yin * IK_PSOC5_CONST(KC_INVERSE), job->fmt); //h->config.vec_fmt);
    }
    else
    {
        v.x = IK_QFMT_TO_I16(xin, job->fmt); //h->config.vec_fmt);
        v.y = IK_QFMT_TO_I16(yin, job->fmt); //h->config.vec_fmt);
    }

    float zin = (desired_mode == IK_PSOC5_CONST(ROTATING_OPER)) ? IK_VEC3_Z(job->input) : 0.0f;
    v.z = IK_RAD_TO_BAMS16_F(zin);
    
    job->state = CORDIC_JOB_STATE_QUEUED;
    
    if (IK_PSOC5_CALL(queue_data, &v) != CYRET_SUCCESS)
        return IK_ERROR_CORDIC_HARDWARE_ERROR;
    
    //uint8_t intr = CyEnterCriticalSection();

    //CyExitCriticalSection(intr);

    return IK_RESULT_OK;
};

uint8_t ik_cordic_psoc5_available(ik_ctx_t __unused ctx)
{
    return IK_PSOC5_CALL(has_pending) ? 1u : 0u;
}

ik_result_t ik_cordic_psoc5_acquire(
    ik_ctx_t ctx,
    ik_cordic_job_t* job)
{
    if (!ctx || !job)
        return IK_ERROR_NULL_POINTER;

    //uint8_t used_mode = IK_PSOC5_CONST(CONTROL_REG) & IK_PSOC5_CONST(OPER_MODE);
    
    IK_PSOC5_TYPE(vector_t) vector;
    
    if (IK_PSOC5_CALL(get_data, &vector) != CYRET_SUCCESS)
        return IK_ERROR_CORDIC_HARDWARE_ERROR;
    
    ik_cordic_h h = (ik_cordic_h) ctx;

    IK_VEC3_XP(job->output) = IK_I16_TO_QFMT_F(
        vector.x, 
        job->fmt
        //(used_mode == IK_PSOC5_CONST(ROTATING_OPER)) ? h->config.unit_fmt : h->config.vec_fmt
    );
    IK_VEC3_YP(job->output) = IK_I16_TO_QFMT_F(
        vector.y,
        job->fmt
        //(used_mode == IK_PSOC5_CONST(ROTATING_OPER)) ? h->config.unit_fmt : h->config.vec_fmt
    );
    IK_VEC3_ZP(job->output) = IK_BAMS16_TO_RAD_F(vector.z) + job->z_offset_rad;

    return IK_RESULT_OK;
}

//========================================================
//      CORDIC Creation Methods
//========================================================

ik_result_t ik_cordic_create_backend(
    ik_cordic_cfg_t* cfg,
    ik_cordic_h* handle)
{
    if (!cfg || !handle)
        return IK_ERROR_NULL_POINTER;

#if defined(IK_ENABLE_HEAP)
    ik_cordic_h new_cordic = IK_MALLOC(sizeof(struct ik_cordic_T), NULL);
    if (!new_cordic)
        return IK_ERROR_NO_MEMORY;
#else
    ik_cordic_h new_cordic = *handle;
#endif

    new_cordic->config = *cfg;
    new_cordic->state = CORDIC_STATE_IDLE;

    new_cordic->job_count = 0u;
    new_cordic->job_in_idx = 0u;
    new_cordic->job_out_idx = 0u;
    new_cordic->job_size = 4u;

    for (uint8_t i = 0; i < new_cordic->job_size; i++)
    {
        new_cordic->jobs[i].state = CORDIC_JOB_STATE_FREE;
    }

    new_cordic->submit = ik_cordic_psoc5_submit;
    new_cordic->available = ik_cordic_psoc5_available;
    new_cordic->acquire = ik_cordic_psoc5_acquire;
    
    new_cordic->hw_ctx = (void*) new_cordic;

    g_cordic = new_cordic;
    
    #if defined(IK_ENABLE_HEAP)
        
        *handle = new_cordic;
        
    #endif

    return IK_RESULT_OK;
};

//========================================================
//      CORDIC General API's
//========================================================

ik_cordic_caps_t ik_get_cordic_caps(void)
{
    return psoc5_cordic_caps;
}

//========================================================
//      End of File
//========================================================

#endif // !IK_PLATFORM_PSOC5