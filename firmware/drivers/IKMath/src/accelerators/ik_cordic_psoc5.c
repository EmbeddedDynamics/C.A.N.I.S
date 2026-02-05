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
                #define CANSTACK_PLATFORM_PSOC3
            #elif (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4)
                #define CANSTACK_PLATFORM_PSOC4
            #elif (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5)
                #define CANSTACK_PLATFORM_PSOC5
            #elif (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC6)
                #define CANSTACK_PLATFORM_PSOC6
            #endif
        #endif
    #endif
#endif

#if defined(CANSTACK_PLATFORM_PSOC5)

#ifndef CORDIC_DONE_CALLBACK
    void ik_cordic_done_clb(void);
#endif

#define CORDIC_WAIT_UNTIL_EMPTY(timeout) \
( \
    uint16_t _timeout = timeout;
    uint8_t empty = (CORDIC_STATUS_REG & CORDIC_STS_X_FILLED) \
    while ((empty != 0) || (_timeout > 0)) \
    { \
        empty = (CORDIC_STATUS_REG & CORDIC_STS_X_FILLED); \
        _timeout--; \
        CyDelayUs(10); \
    } \
    if (_timeout <= 0)
        return IK_ERROR_CORDIC_TIMEOUT;
    break; \
)

//========================================================
//      PSoC5 CORDIC Methods
//========================================================

ik_result_t ik_cordic_psoc5_submit(ik_ctx_t ctx, ik_cordic_job_t* job)
{
    if (!ctx || !job)
        return IK_ERROR_NULL_POINTER;

    uint8_t current_mode = (uint8_t)(CORDIC_CONTROL_REG & CORDIC_OPER_MODE);

    uint8_t desired_mode = (job->operation == IK_CORDIC_OPERATION_ROTATION)
        ? CORDIC_ROTATING_OPER
        : CORDIC_VECTORING_OPER;

    // If mode must change, drain/flush first
    if (current_mode != desired_mode)
    {
        // Wait until the X-fifo is empry
        CORDIC_WAIT_UNTIL_EMPTY();

        // Write mode field (clear then set)
        CORDIC_CONTROL_REG = (CORDIC_CONTROL_REG & (uint8_t)~CORDIC_OPER_MODE) | desired_mode;
    }

    ik_cordic_h h = (ik_cordic_h) ctx;

    // Build the vector to queue
    CORDIC_vector_t v = {0};

    v.x = (int16_t) ik_float_to_fixed_i32_fast(job->input.vec.x, h->config.vec_xy_fmt);   // placeholder: you need a defined source
    v.y = (int16_t) ik_float_to_fixed_i32_fast(job->input.vec.y, h->config.vec_xy_fmt); // placeholder: usually 0
    v.z = (int16_t) IK_RAD_TO_BAMS16_F(theta_rad); // placeholder: BAMS16 or radians->BAMS16 conversion later

    if (CORDIC_queue_data(&v) != CYRET_SUCCESS)
        return IK_ERROR_CORDIC_HARDWARE_ERROR;

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

    new_cordic->submit = ik_cordic_psoc5_submit;

    return IK_RESULT_OK;
}

//========================================================
//      End of File
//========================================================

#endif // !CANSTACK_PLATFORM_PSOC5