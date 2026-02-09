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

#ifndef IK_MATH_ACCELERATORS_H
#define IK_MATH_ACCELERATORS_H

//========================================================
//      Standard Includes
//========================================================

/* Internal headers */
#include <stdint.h>
#include <stdbool.h>
    
/* Project headers */
#include "ik_results.h"
#include "ik_types.h"

typedef struct ik_accel_desc_T ik_accel_descriptor_t;
typedef ik_accel_descriptor_t* ik_accel_descriptor_h;

//========================================================
//      Accelerator Kinds
//========================================================

typedef enum {
    IK_ACCEL_KIND_NONE = 0,
    IK_ACCEL_KIND_CORDIC,
} ik_accel_kind_t;

//========================================================
//      Accelerator Kinds
//========================================================

typedef struct ik_accel_desc_T {
    ik_accel_kind_t kind;
    const void* config;
    ik_accel_descriptor_h next;
};

//========================================================
//      End of File
//========================================================

#endif // !IK_MATH_ACCELERATORS_H