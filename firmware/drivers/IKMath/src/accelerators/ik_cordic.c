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

//========================================================
//      CORDIC Helper Functions
//========================================================

static inline ik_cordic_op_caps_t ik_cordic_coord_caps(ik_cordic_caps_t* caps, ik_cordic_coord_t coord)
{
    switch (coord)
    {
    case IK_CORDIC_COORD_CIRCULAR:
        return caps->circular_ops;
    case IK_CORDIC_COORD_LINEAR:
        return caps->linear_ops;
    case IK_CORDIC_COORD_HYPERBOLIC:
        return caps->hyperbolic_ops;
    default:
        return IK_CORDIC_OP_CAP_NONE;
    }
}

//========================================================
//      CORDIC Synchronus API's
//========================================================

ik_result_t ik_cordic_vec_sync(
    ik_cordic_h handle,
    ik_cordic_coord_t coord,
    const ik_vector2f_t* vec_in,
    ik_vector2f_t* out)
{
    if (!handle || !vec_in || !out)
        return IK_ERROR_NULL_POINTER;

    ik_cordic_op_caps_t caps =
        ik_cordic_coord_caps(&handle->config.supported_caps, coord);

    if ((caps & IK_CORDIC_OP_CAP_VECTORING) == IK_CORDIC_OP_CAP_NONE)
        return IK_ERROR_CORDIC_NOT_SUPPORTED;

    if (handle->state != CORDIC_STATE_RUNNING)
        return IK_ERROR_CORDIC_INVALID_STATE;

    ik_cordic_job_t *job = NULL;
    uint8_t job_id = ik_cordic_acquire_job(handle, &job);
    if (job_id == IK_CORDIC_INVALID_JOB_ID || job == NULL)
        return IK_ERROR_CORDIC_QUEUE_FULL; // or INVALID_JOB_ID

    job->coord = coord;
    job->operation = IK_CORDIC_OPERATION_VECTORING;
    job->input.vec = *vec_in;
    job->output = out;

    IK_CHECK(ik_cordic_queue_job(handle, job_id));

    // timeout guard (choose a limit appropriate for your system)
    uint32_t guard = 100000u;

    while (job->state != CORDIC_JOB_STATE_FINISHED) {
        IK_CHECK(ik_cordic_poll(handle));
        if (guard-- == 0u)
            return IK_ERROR_CORDIC_TIMEOUT;
    }

    // FIFO enforcement should be inside release_job():
    IK_CHECK(ik_cordic_release_job(handle, job_id));

    return IK_RESULT_OK;
}

/**
 * @brief Perform a synchronous CORDIC rotation operation.
 *
 * Computes a CORDIC rotation in the specified coordinate system for the given
 * input angle.
 *
 * @param[in]  handle   CORDIC instance handle.
 * @param[in]  coord    Coordinate system to use.
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
    float theta,
    ik_vector2f_t* out
);

//========================================================
//      CORDIC Internal API's
//========================================================

ik_result_t ik_cordic_acquire_job(ik_cordic_h h, uint8_t *job_id)
{
    if (!h || !job_id) 
        return IK_ERROR_NULL_POINTER;

    uint8_t N = h->job_size;
    if ((N & (N - 1u)) != 0u) 
        return IK_ERROR_CORDIC_BAD_QUEUE_SIZE;

    if (h->job_count >= N) 
        return IK_ERROR_CORDIC_QUEUE_FULL;

    uint8_t idx = h->job_in_idx;
    if (h->jobs[idx].state != CORDIC_JOB_STATE_FREE)
        return IK_ERROR_CORDIC_QUEUE_CORRUPT;

    h->jobs[idx].state = CORDIC_JOB_STATE_RESERVED;
    h->job_in_idx = (uint8_t)((h->job_in_idx + 1u) & (N - 1u));
    h->job_count++;

    *job_id = idx;
    return IK_RESULT_OK;
}

ik_result_t ik_cordic_queue_job(ik_cordic_h h, uint8_t job_id)
{
    if (!h) 
        return IK_ERROR_NULL_POINTER;
        
    if (job_id >= h->job_size) 
        return IK_ERROR_CORDIC_INVALID_JOB_ID;

    if (h->jobs[job_id].state != CORDIC_JOB_STATE_RESERVED)
        return IK_ERROR_CORDIC_INVALID_JOB_STATE;

    h->jobs[job_id].state = CORDIC_JOB_STATE_QUEUED;
    return IK_RESULT_OK;
}

ik_result_t ik_cordic_release_job(ik_cordic_h h, uint8_t job_id)
{
    if (!h) 
        return IK_ERROR_NULL_POINTER;

    if (job_id >= h->job_size) 
        return IK_ERROR_CORDIC_INVALID_JOB_ID;

    // FIFO enforcement:
    if (job_id != h->job_out_idx) 
        return IK_ERROR_CORDIC_OUT_OF_ORDER_RELEASE;

    if (h->jobs[job_id].state != CORDIC_JOB_STATE_FINISHED)
        return IK_ERROR_CORDIC_INVALID_JOB_STATE;

    h->jobs[job_id].state = CORDIC_JOB_STATE_FREE;
    h->job_out_idx = (uint8_t)((h->job_out_idx + 1u) & (h->job_size - 1u));
    h->job_count--;

    return IK_RESULT_OK;
}

//========================================================
//      End of File
//========================================================