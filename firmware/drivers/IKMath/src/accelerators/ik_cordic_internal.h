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

#ifndef IK_MATH_CORDIC_INTERNAL_H
#define IK_MATH_CORDIC_INTERNAL_H

//========================================================
//      Standard Includes
//========================================================

/* Internal headers */
#include <stdint.h>
#include <stdbool.h>
    
/* Project headers */
#include "ik.h"
#include "accelerators/ik_cordic.h"

//========================================================
//      CORDIC State
//========================================================

typedef enum {
    CORDIC_STATE_IDLE = 0x00,
    CORDIC_STATE_RUNNING = 0x01,
    CORDIC_STATE_HALTED  = 0x02,
    CORDIC_STATE_STOPPED = 0x03,
} ik_cordic_state_t;

//========================================================
//      CORDIC Job State
//========================================================

enum {
    CORDIC_JOB_STATE_FREE = 0x00,
    CORDIC_JOB_STATE_RESERVED = 0x01,
    CORDIC_JOB_STATE_QUEUED = 0x02,
    CORDIC_JOB_STATE_FINISHED = 0x03,
    CORDIC_JOB_STATE_CANCELLED = 0x04,
};
typedef ik_flags8_t ik_cordic_job_state_t;

//========================================================
//      CORDIC Job Structure
//========================================================

typedef struct {
    ik_cordic_job_state_t state;

    ik_cordic_coord_t coord;

    ik_cordic_operation_t operation;

    ik_vector3f_t input;

    ik_vector3f_t* output;
} ik_cordic_job_t;

//========================================================
//      CORDIC Function Prototypes
//========================================================

typedef ik_result_t (*ik_cordic_submit_fn_t) (
    ik_ctx_t ctx,
    ik_cordic_job_t* job
);

typedef uint8_t (*ik_cordic_available_fn_t) (
    ik_ctx_t ctx
);

//========================================================
//      CORDIC Handle
//========================================================

struct ik_cordic_T {
    ik_cordic_cfg_t config;

    ik_cordic_state_t state;

#if (IK_USE_STATIC_JOB_QUEUE)

    ik_cordic_job_t jobs[IK_CORDIC_JOB_QUEUE_SIZE];

    uint8_t job_size;

#else

    ik_cordic_job_t* jobs;
    uint8_t job_size;

#endif

    uint8_t job_count;

    uint8_t job_in_idx;

    uint8_t job_out_idx;

    ik_cordic_submit_fn_t submit;

    ik_cordic_available_fn_t available;

    ik_ctx_t hw_ctx;
};

//========================================================
//      CORDIC Internal API's
//========================================================

/**
 * @brief Acquire a CORDIC job structure
 * 
 * In order the queue work into the CORDIC the user must acquire a free to use job descriptor.
 * This job descriptor can than be populated with the necessary data to let the CORDIC work.
 * 
 * @param[in] handle - CORDIC handle
 * @param[out] job - Address of the job descriptor
 * 
 * @retval IK_RESULT_OK - Job fetched
 */
ik_result_t ik_cordic_acquire_job(ik_cordic_h h, uint8_t *job_id);

ik_result_t ik_cordic_queue_job(ik_cordic_h h, uint8_t job_id);

ik_result_t ik_cordic_release_job(ik_cordic_h h, uint8_t job_id);

ik_result_t ik_cordic_finish_head(ik_cordic_h h);

//========================================================
//      End of File
//========================================================

#endif // !IK_MATH_CORDIC_INTERNAL_H