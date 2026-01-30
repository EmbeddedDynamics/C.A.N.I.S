/***************************************************************************//**
* \file IKTypes.h
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

#ifndef IK_MATH_TYPES_H
#define IK_MATH_TYPES_H

//========================================================
//      Standard Includes
//========================================================

/* Internal headers */
#include <stdint.h>
#include <stdbool.h>
#include <stdalign.h>
    
/* Project headers */
#include "ik_results.h"
#include "ik_config.h"

//========================================================
//      IK Custom Types
//========================================================

typedef uint32_t ik_flags32_t;
typedef uint64_t ik_flags64_t;

//========================================================
//      IK Vectors
//========================================================

#if IK_HAS_ANON_STRUCT

typedef union { 
    struct { float x, y; }; 
    float v[2]; 
} ik_vector2f_t;

typedef union { 
    struct { float x, y, z; }; 
    float v[3]; 
} ik_vector3f_t;

typedef union { 
    struct { float x, y, z, w; }; 
    float v[4]; 
} ik_vector4f_t;

typedef union { 
    struct { int16_t x, y; }; 
    int16_t v[2]; 
} ik_vector2i16_t;

typedef union { 
    struct { int16_t x, y, z; }; 
    int16_t v[3]; 
} ik_vector3i16_t;

typedef union { 
    struct { int16_t x, y, z, w; }; 
    int16_t v[4]; 
} ik_vector4i16_t;

#else

typedef struct { 
    float v[2]; 
} ik_vector2f_t;

typedef struct { 
    float v[3]; 
} ik_vector3f_t;

typedef struct { 
    float v[4]; 
} ik_vector4f_t;

typedef struct { 
    int16_t v[2]; 
} ik_vector2i16_t;

typedef struct { 
    int16_t v[3]; 
} ik_vector3i16_t;

typedef struct { 
    int16_t v[4]; 
} ik_vector4i16_t;

#define IK_VEC2_X(p) ((p).v[0])
#define IK_VEC2_Y(p) ((p).v[1])

#define IK_VEC3_X(p) ((p).v[0])
#define IK_VEC3_Y(p) ((p).v[1])
#define IK_VEC3_Z(p) ((p).v[2])

#define IK_VEC4_X(p) ((p).v[0])
#define IK_VEC4_Y(p) ((p).v[1])
#define IK_VEC4_Z(p) ((p).v[2])
#define IK_VEC4_W(p) ((p).v[3])

#define IK_VEC2_XP(p) ((p)->v[0])
#define IK_VEC2_YP(p) ((p)->v[1])

#define IK_VEC3_XP(p) ((p)->v[0])
#define IK_VEC3_YP(p) ((p)->v[1])
#define IK_VEC3_ZP(p) ((p)->v[2])

#define IK_VEC4_XP(p) ((p)->v[0])
#define IK_VEC4_YP(p) ((p)->v[1])
#define IK_VEC4_ZP(p) ((p)->v[2])
#define IK_VEC4_WP(p) ((p)->v[3])

#endif

//========================================================
//      End of File
//========================================================

#endif // !IK_MATH_TYPES_H