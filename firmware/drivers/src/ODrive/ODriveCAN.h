/***************************************************************************//**
* \file ODriveCAN.h
* \version 1.0.0
*
* \brief
* Private header for CANSimple protocol used by ODrive.
*
********************************************************************************
* \copyright
* Copyright 2025, Embedded Dynamics. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef ODRIVE_INTERNAL_CAN_H
#define ODRIVE_INTERNAL_CAN_H

// ========================================================
// Includes
// ========================================================

#include <stdint.h>
#include <stdbool.h>

#include "ODriveResult.h"
#include "ODriveCore.h"

// ========================================================
// CAN bus macros
// ========================================================

/**
 * @name ODrive CAN identifier utilities
 * @brief Helpers for constructing and decoding ODrive CANSimple IDs.
 *
 * ODrive uses a packed 11-bit standard CAN identifier (CAN 2.0A):
 *
 *   - Bits [10:5] : node_id   (0–63)
 *   - Bits [4:0]  : cmd_id    (0–31)
 *
 * These macros assist in validating node IDs, constructing arbitration IDs,
 * and extracting fields from received identifiers.
 *
 * @{
 */

/**
 * @brief Maximum valid ODrive CAN node ID.
 *
 * ODrive allocates 6 bits for node addressing (0–63).
 *
 * Node ID 63 (0x3F) is reserved:
 *   - As a broadcast address when sent host -> ODrive.
 *   - As an “unassigned” or “unconfigured” state when reported by ODrive.
 */
#define ODRIVE_CAN_ID_MAX           (63u)

/**
 * @brief Default timeout for blocking CAN operations (milliseconds).
 *
 * Used as a convenient standard timeout for:
 *   - Waiting for responses to RTR (Remote Transmission Request).
 *   - Awaiting specific message types.
 *   - Ensuring CAN operations do not block indefinitely.
 *
 * Adjust this as needed based on system latency and bus load.
 */
#define ODRIVE_CAN_TIMEOUT_MS       (100u)

/**
 * @brief Validate an ODrive CAN node ID.
 *
 * @param[in] n Candidate node ID.
 * @return true if \p n is within the valid range [0, 63].
 *
 * Useful when parsing configuration, ensuring user input is valid,
 * or validating IDs extracted from incoming frames.
 */
#define ODRIVE_CAN_ID_VALID(n)      ((n) <= ODRIVE_CAN_ID_MAX)

/**
 * @brief Construct an ODrive CAN arbitration ID.
 *
 * Packs a node ID and command ID into the standard 11-bit CAN identifier.
 *
 * Layout:
 *   - Bits [10:5] = node_id (0–63)
 *   - Bits [4:0]  = cmd_id  (0–31)
 *
 * @param node_id ODrive axis node ID (0–63).
 * @param cmd_id  ODrive command ID (0–31).
 *
 * @return 11-bit CAN identifier ready for transmission.
 *
 * @code
 * CANId id = ODRIVE_CAN_ID(0x01, CAN_CMD_SET_INPUT_POS);  // Set_Input_Pos on node 1
 * @endcode
 */
#define ODRIVE_CAN_ID(node_id, cmd_id) \
    (((uint16_t)(node_id) << 5) | ((uint16_t)(cmd_id) & 0x1F))

/**
 * @brief Extract the node ID from an ODrive CAN identifier.
 *
 * @param can_id The 11-bit CAN identifier (extended bits ignored).
 * @return node_id (0–63).
 */
#define ODRIVE_NODE_ID(can_id)      (((can_id) >> 5) & 0x3F)

/**
 * @brief Extract the command ID from an ODrive CAN identifier.
 *
 * @param can_id The 11-bit CAN identifier (extended bits ignored).
 * @return cmd_id (0–31).
 */
#define ODRIVE_CMD_ID(can_id)       ((can_id) & 0x1F)

/** @} */  // end of CAN identifier utilities

#define ODRIVE_CAN_PACK_INT8(frame, off, val)  frame.data[off] = (int8_t)(val & 0xFF)

#define ODRIVE_CAN_PACK_INT16(frame, off, val)  frame.data[off] = (uint8_t)(val & 0xFF); \
                                                frame.data[off+1] = (uint8_t)((velocity_ff >> 8) & 0xFF);\

#define ODRIVE_CAN_PACK_INT32(frame, off, val)  frame.data[off] = (uint8_t)(val & 0xFF); \
                                                frame.data[off+1] = (uint8_t)((val >> 8) & 0xFF);\
                                                frame.data[off+2] = (uint8_t)((val >> 16) & 0xFF);\
                                                frame.data[off+3] = (uint8_t)((val >> 24) & 0xFF);\

// ========================================================
// Typedefs
// ========================================================

/**
 * @brief Combined ODrive CAN identifier (standard 11-bit ID).
 *
 * Represents the packed CAN message identifier used by the ODrive
 * CANSimple protocol. The 11-bit identifier encodes:
 *
 *   - Bits [10:5] : node_id   (0–63)
 *   - Bits [4:0]  : cmd_id    (0–31)
 *
 * The CAN controller transmits this value as the arbitration ID for
 * both host -> ODrive and ODrive -> host messages.
 *
 * Usage:
 *   Construct a CANId with #ODRIVE_CAN_ID().
 *
 * @note This type stores the full 11-bit arbitration ID; the upper bits
 *       of the 32-bit integer are unused.
 * @note ODrive uses only standard (CAN 2.0A) identifiers; extended IDs
 *       are not supported.
 * @note node_id = 0x3F (63) is reserved for broadcast (host -> ODrive) and
 *       indicates an unaddressed axis (ODrive -> host).
 */
typedef uint16_t CANId;

/**
 * @brief ODrive CANSimple frame container.
 *
 * Represents a single CAN 2.0A frame as used by the ODrive CANSimple protocol.
 * This structure is used for both transmitting commands to ODrive and receiving
 * feedback or status messages from ODrive.
 *
 * All ODrive communication uses standard (11-bit) CAN identifiers.
 * Only the lower 11 bits of @ref id are used.
 */
typedef struct ODriveCANFrame_T {

    /**
     * @brief 11-bit CAN arbitration ID (packed node_id + cmd_id).
     *
     * Only bits [10:0] are valid; higher bits must be zero.
     * Use ODRIVE_NODE_ID() and ODRIVE_CMD_ID() to decode.
     *
     * @code
     * frame.id = ODRIVE_CAN_ID(1, CAN_CMD_SET_INPUT_POS);   // Set_Input_Pos to node 1
     * @endcode
     */
    CANId id;

    /**
     * @brief Data Length Code (DLC).
     *
     * Specifies the number of payload bytes in @ref data.
     * Must be between 0 and 8 for standard CAN 2.0 frames.
     */
    uint8_t dlc;

    /**
     * @brief CAN data payload (up to 8 bytes).
     *
     * Multi-byte values must be encoded little-endian as required
     * by the ODrive CANSimple protocol.
     *
     * Unused bytes should be set to zero when transmitting.
     */
    uint8_t data[8];

    /**
     * @brief Remote Transmission Request flag.
     *
     * - `0` = Data frame (normal CAN message with payload)
     * - `1` = RTR frame (request for ODrive to transmit this cmd_id)
     *
     * RTR frames are used to request "Get\_\*" messages on-demand.
     */
    uint8_t rtr;

} ODriveCANFrame;

/**
 * @brief CAN receive callback function type.
 *
 * Called when a CAN message is received from an ODrive axis.
 *
 * @param[in] can_id    CANId message ID (encodes node_id and command_id).
 * @param[in] data      Pointer to CAN data payload (0–8 bytes).
 * @param[in] dlc       Data length in bytes (0–8).
 * @param[in] user_data User context pointer from registration.
 *
 * @note Callback may be invoked from ISR context. Keep execution time minimal.
 */
typedef void (*ODriveCANRxCallback)(CANId can_id,
                                    const uint8_t *data,
                                    uint8_t dlc,
                                    void *user_data);

/**
 * @brief CAN transmit callback function type.
 *
 * Called when a CAN message is being transmitted to an ODrive axis.
 *
 * @param[in] can_id    CANId message ID (encodes node_id and command_id).
 * @param[in] data      Pointer to CAN data payload (0–8 bytes).
 * @param[in] dlc       Data length in bytes (0–8).
 * @param[in] user_data User context pointer from registration.
 *
 * @note Callback may be invoked from ISR context. Keep execution time minimal.
 */
typedef void (*ODriveCANTxCallback)(CANId can_id,
                                    const uint8_t *data,
                                    uint8_t dlc,
                                    void *user_data);

// ========================================================
// Private Methods
// ========================================================

/**
 * @brief Send a CAN frame on the ODrive CAN bus.
 *
 * Sends a preconstructed CAN frame through the underlying CAN hardware.
 *
 * @param[in] frame Pointer to CAN frame to transmit (must not be NULL).
 *
 * @retval ODRIVE_RESULT_OK           Frame queued or transmitted successfully.
 * @retval ODRIVE_ERROR_NULL_POINTER  frame was NULL.
 * @retval ODRIVE_ERROR_COMM_ERROR    Underlying CAN driver reported an error.
 */
ODriveResult ODriveCAN_SendCommand(const ODriveCANFrame *frame);

#endif // !ODRIVE_INTERNAL_CAN_H

/* [] END OF FILE */
