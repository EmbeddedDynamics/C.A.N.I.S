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
//      Solver Enums
//========================================================

typedef enum {
    IK_SOLVE_MODE_UNKNOWN = 0x00,
    IK_SOLVE_MODE_ANALYTIC,
    IK_SOLVE_MODE_NUMERIC,
    IK_SOLVE_MODE_PREFER_ANALYTIC,
} ik_solve_mode_t;

//========================================================
//      End of File
//========================================================

#endif // !IK_MATH_H