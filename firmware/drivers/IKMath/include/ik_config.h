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

#ifndef IK_MATH_CONFIG_H
#define IK_MATH_CONFIG_H

//========================================================
//      Standard Includes
//========================================================

/* Internal headers */
#include <stdint.h>
#include <stdbool.h>
    
/* Project headers */
#include "ik_results.h"

//========================================================
//      Library Helper Macro's
//========================================================

#define IK_C11 201112L

#define IK_STR_(x) #x
#define IK_STR(x)  IK_STR_(x)

#define IK_BIT(n) (1u << n)

#if defined(_MSC_VER)
    #define IK_ALIGN(N) __declspec(align(N))
#elif defined(__GNUC__) || defined(__clang__)
    #define IK_ALIGN(N) __attribute__((aligned(N)))
#else
    #define IK_ALIGN(N)
#endif

#define IK_IS_ALIGNED(ptr, align) (((uintptr_t)ptr & (alignment - 1u)) == 0u)

#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= IK_C11)
    #define IK_HAS_ANON_STRUCT 1
#else
    #define IK_HAS_ANON_STRUCT 0
#endif

#define IK_HANDLE(name) typedef struct name##_T* name##_h

#define IK_CAT2_(a,b) a##b
#define IK_CAT2(a,b)  IK_CAT2_(a,b)

#define IK_CAT3_(a,b,c) a##b##c
#define IK_CAT3(a,b,c) IK_CAT3_(a,b,c)

//========================================================
//      Memory Management
//========================================================

#define IK_ENABLE_HEAP
#if defined(IK_ENABLE_HEAP)
    #include <stdlib.h>

    #if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= IK_C11)
        #define IK_MALLOC(size, align) (malloc(size))
    #else
        #define IK_MALLOC(size, align) (malloc(size))
    #endif
#else
    #define IK_MALLOC(size, align) (NULL)
#endif

//========================================================
//      Library Information
//========================================================

#define IK_MATH_MAJOR 0
#define IK_MATH_MINOR 1
#define IK_MATH_PATCH 2

/* ABI version for the vtable contract. Increment ONLY when breaking binary/API compatibility. */
#define IK_ABI_VERSION     (1u)

//========================================================
//      Version Packing Helpers
//========================================================

/**
 * @brief Pack a semantic version (major.minor.patch) into a single comparable integer.
 *
 * Uses 8 bits per component (0..255). Adjust shifts if you need larger ranges.
 */
#define IK_MATH_VERSION_ENCODE(major, minor, patch) \
    ((((major) & 0xFFu) << 16) | (((minor) & 0xFFu) << 8) | ((patch) & 0xFFu))

/** 
 * @brief Current library version as a single integer. 
 */
#define CIK_MATH_VERSION_STRING \
    "v" IK_STR(IK_MATH_MAJOR) "." IK_STR(IK_MATH_MINOR) "." IK_STR(IK_MATH_PATCH)

/**
 * @brief True if IK_MATH version is at least (major.minor.patch).
 *
 * @example
 *   #if IK_MATH_VERSION_AT_LEAST(1,0,0)
 *     ...
 *   #endif
 */
#define IK_MATH_VERSION_AT_LEAST(major, minor, patch) \
    (IK_MATH_VERSION >= IK_MATH_VERSION_ENCODE((major), (minor), (patch)))

/**
 * @brief True if IK_MATH version is exactly (major.minor.patch).
 */
#define IK_MATH_VERSION_IS(major, minor, patch) \
    (IK_MATH_VERSION == IK_MATH_VERSION_ENCODE((major), (minor), (patch)))


//========================================================
//      End of File
//========================================================

#endif // !IK_MATH_H