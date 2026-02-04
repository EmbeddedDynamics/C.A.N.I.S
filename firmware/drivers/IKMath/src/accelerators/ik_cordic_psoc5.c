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

//========================================================
//      PSoC5 CORDIC Methods
//========================================================

typedef ik_result_t (*ik_cordic_submit_fn_t) (
    ik_ctx_t ctx,
    ik_cordic_job_t* job,
    uint32_t* job_id
);

typedef uint8_t (*ik_cordic_available_fn_t) (
    ik_ctx_t ctx
);

ik_result_t ik_cordic_psoc5_submit(
    ik_ctx_t ctx,
    ik_cordic_job_t* job)
{
    if (!ctx || !job)
        return IK_ERROR_NULL_POINTER;

    CORDIC_vector_t vector = {
        .x = job->input.vec[0],
        .y = job->input.vec[1],
        .z = 0u
    }

    if (CORDIC_queue_data(&vector) != CYRET_SUCCESS)
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