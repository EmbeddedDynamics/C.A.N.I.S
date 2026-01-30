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
//      IK Result Enums
//========================================================

enum {
    //========================================================
    //      Result masks & bases
    //========================================================

    IK_RESULT_CATEGORY_MASK      = 0xF000,

    IK_RESULT_SUCCESS_BASE       = 0x0000,
    IK_RESULT_OPERATION_BASE     = 0x2000,
    IK_RESULT_CORDIC_BASE        = 0x3000,

    //========================================================
    //      Success
    //========================================================

    /**
     * @brief Operation succeeded
     */
    IK_RESULT_OK = IK_RESULT_SUCCESS_BASE,

    //========================================================
    //      CORDIC Codes
    //========================================================

    IK_ERROR_CORDIC_NO_BACKEND = IK_RESULT_CORDIC_BASE + 1u;
};
typedef uint16_t ik_result_t;
//========================================================
//      End of File
//========================================================

#endif // !IK_MATH_RESULTS_H