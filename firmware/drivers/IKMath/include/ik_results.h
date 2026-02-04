/***************************************************************************//**
* \file ik_results.h
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

#ifndef IK_MATH_RESULTS_H
#define IK_MATH_RESULTS_H

//========================================================
//      Standard Includes
//========================================================

/* Internal headers */
#include <stdint.h>

//========================================================
//      IK Result Categories
//========================================================

#define IK_RESULT_CATEGORY_MASK     0xF000

#define IK_RESULT_SUCCESS_BASE      0x0000
#define IK_RESULT_OPERATION_BASE    0x1000
#define IK_RESULT_CORDIC_BASE       0x2000


//========================================================
//      IK Result Enums
//========================================================

enum {
    //========================================================
    //      Success
    //========================================================

    /**
     * @brief Operation succeeded
     */
    IK_RESULT_OK = IK_RESULT_SUCCESS_BASE,

    IK_ERROR_NULL_POINTER   = IK_RESULT_OPERATION_BASE + 1u,
    IK_ERROR_INVALID_PARAM  = IK_RESULT_OPERATION_BASE + 2u,
    IK_ERROR_NO_MEMORY      = IK_RESULT_OPERATION_BASE + 3u,

    //========================================================
    //      CORDIC Codes
    //========================================================

    IK_ERROR_CORDIC_NO_BACKEND      = IK_RESULT_CORDIC_BASE + 1u,
    IK_ERROR_CORDIC_NOT_SUPPORTED   = IK_RESULT_CORDIC_BASE + 2u,
    IK_ERROR_CORDIC_INVALID_STATE   = IK_RESULT_CORDIC_BASE + 3u,
    IK_ERROR_CORDIC_INVALID_JOB_ID  = IK_RESULT_CORDIC_BASE + 4u,
    IK_ERROR_CORDIC_INVALID_JOB_STATE = IK_RESULT_CORDIC_BASE + 5u,
    IK_ERROR_CORDIC_BAD_QUEUE_SIZE  = IK_RESULT_CORDIC_BASE + 6u,
    IK_ERROR_CORDIC_QUEUE_FULL      = IK_RESULT_CORDIC_BASE + 7u,
    IK_ERROR_CORDIC_QUEUE_CORRUPT   = IK_RESULT_CORDIC_BASE + 8u,
    IK_ERROR_CORDIC_OUT_OF_ORDER_RELEASE = IK_RESULT_CORDIC_BASE + 9u,
    IK_ERROR_CORDIC_HARDWARE_ERROR  = IK_RESULT_CORDIC_BASE + 10u,
    
};
typedef uint16_t ik_result_t;

#define IK_CHECK(expr) do { \
    ik_result_t _result = (expr); \
    if (_result != IK_RESULT_OK) { \
        return _result; \
    } \
} while (0)

//========================================================
//      End of File
//========================================================

#endif // !IK_MATH_RESULTS_H