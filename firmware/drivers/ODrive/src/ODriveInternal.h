/***************************************************************************//**
* \file ODriveInternal.h
* \version 1.0.0
*
* \brief
* Private header for the ODrive internal definitions
*
********************************************************************************
* \copyright
* Copyright 2025, Embedded Dynamics. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef ODRIVE_INTERNAL_H
#define ODRIVE_INTERNAL_H

//========================================================
//      Standard Includes
//========================================================
 
/* Internal headers */
#include <stdint.h>
#include <stdbool.h>
    
/* Project headers */
#include "ODrive.h"

//========================================================
//      Message Packing Macros (Little-Endian)
//========================================================

/**
 * @brief Pack signed 8-bit integer into message
 *
 * @param[in,out] msg Message structure
 * @param[in] val     Value to pack (int8_t or compatible)
 *
 * @note Increments msg.dlc; ensure dlc + 1 <= 64 before calling
 * @warning No bounds checking; caller responsible for buffer overflow
 */
#define ODRIVE_PACK_INT8(msg, val)  do { \
    if (msg.dlc < 64) { \
        msg.buffer[msg.dlc++] = (uint8_t)(val & 0xFF); \
    } \
} while(0)

/**
 * @brief Pack signed 16-bit integer into message (little-endian)
 *
 * @param[in,out] msg Message structure
 * @param[in] val     Value to pack (int16_t or compatible)
 *
 * @note Increments msg.dlc by 2; ensure dlc + 2 <= 64 before calling
 */
#define ODRIVE_PACK_INT16(msg, val)  do { \
    if (msg.dlc + 1 < 64) { \
        msg.buffer[msg.dlc++] = (uint8_t)(val & 0xFF); \
        msg.buffer[msg.dlc++] = (uint8_t)((val >> 8) & 0xFF); \
    } \
} while(0)  

/**
 * @brief Pack signed 32-bit integer into message (little-endian)
 *
 * @param[in,out] msg Message structure
 * @param[in] val     Value to pack (int32_t or compatible)
 *
 * @note Increments msg.dlc by 4; ensure dlc + 4 <= 64 before calling
 */
#define ODRIVE_PACK_INT32(msg, val)  do { \
    if (msg.dlc + 3 < 64) { \
        msg.buffer[msg.dlc++] = (uint8_t)(val & 0xFF); \
        msg.buffer[msg.dlc++] = (uint8_t)((val >> 8) & 0xFF); \
        msg.buffer[msg.dlc++] = (uint8_t)((val >> 16) & 0xFF); \
        msg.buffer[msg.dlc++] = (uint8_t)((val >> 24) & 0xFF); \
    } \
} while(0)  

/**
 * @brief Pack unsigned 8-bit integer into message
 */
#define ODRIVE_PACK_UINT8(msg, val) \
    ODRIVE_PACK_INT8(msg, val)

/**
 * @brief Pack unsigned 16-bit integer into message (little-endian)
 */
#define ODRIVE_PACK_UINT16(msg, val) \
    ODRIVE_PACK_INT16(msg, val)

/**
 * @brief Pack unsigned 32-bit integer into message (little-endian)
 */
#define ODRIVE_PACK_UINT32(msg, val) \
    ODRIVE_PACK_INT32(msg, val)

/**
 * @brief Pack 32-bit float into message (little-endian IEEE 754)
 *
 * @param[in,out] msg Message structure
 * @param[in] val     Float value to pack
 *
 * @note Uses union to reinterpret bits; safe for IEEE 754 platforms
 */
#define ODRIVE_PACK_FLOAT(msg, val) do { \
    union { float f; uint32_t u; } _u = {(val)}; \
    ODRIVE_PACK_UINT32(msg, _u.u); \
} while(0)

//========================================================
//      ODrive Axis Structure
//========================================================

struct odrive_axis_T {
    /**< Axis initialization flag */
    bool initialized;

    /**< CAN node ID (0-63) */
    odrive_node_id node_id;      

    /**< Cached alive state */
    odrive_alive_state_t alive_state;

    /**< Last received heartbeat */
    odrive_heartbeat_frame_t heartbeat;

    /**
     * @brief Last received encoder estimate frame
     */
    encoder_estimate_frame encoder_estimate;

    odrive_driver driver;    /**< CAN driver handle */
    
};

//========================================================
//      ODrive Driver Structure
//========================================================

struct odrive_driver_T {
    //odrive_driver_config_t config;

    odrive_backend          backend;            /**< Communication context (CAN, etc.) */

    struct odrive_axis_T    axes[ODRIVE_MAX_AXES]; /**< Axis handle table, indexed by slot */
    uint16_t                num_axes;              /**< Number of active axes */

    // Optional: mapping node_id -> axis index (0xFF = unused)
    odrive_node_id     node_map[ODRIVE_MAX_AXES]; /**< Node ID per slot, or 0xFF if free */

    odrive_driver_state_t state;
};  


//========================================================
//      End of File
//========================================================

#endif // !ODRIVE_INTERNAL_H