/***************************************************************************//**
* \file ODriveCanStack.h
* \version 1.0.0
*
* \brief
* CanStack abstraction layer for the use of the ODrive library
*
* \details
* Transport-agnostic interface supporting CAN, ABZ, USB, SPI, etc.
* Enables ODrive library to work with any communication backend via vtable
* abstraction. Each transport encodes/decodes ODrive protocol independently.
*
*******************************************************************************/

#ifndef ODRIVE_CAN_STACK_H
#define ODRIVE_CAN_STACK_H

//========================================================
//      Standard Includes
//========================================================

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "ODriveCore.h"
#include "driver/ODriveBackend.h"

#ifndef ODRIVE_STATIC_ASSERT
    #define ODRIVE_STATIC_ASSERT(cond, msg) \
    _Static_assert(cond, msg)
#endif

#ifndef ODRIVE_HAVE_CANSTACK
    #if defined(__has_include)
        #if __has_include("CanStack.h")
            #define ODRIVE_HAVE_CANSTACK 1u
        #else
            #define ODRIVE_HAVE_CANSTACK 0u
        #endif
    #else
        #define ODRIVE_HAVE_CANSTACK 0u
    #endif
#endif

#if ODRIVE_HAVE_CANSTACK
  #include "CanStack.h"
#endif

#if (ODRIVE_HAVE_CANSTACK)

//========================================================
//      CanStack Conditions
//========================================================

#if !defined(ODRIVE_CANSTACK_FORCE_CONVERT) && \
    (defined(CANSTACK_VERSION) && defined(CANSTACK_VERSION_AT_LEAST) && CANSTACK_VERSION_AT_LEAST(1,0,1))
  #define ODRIVE_CANSTACK_MATCH_FRAME_ALIAS 1
#else
  #define ODRIVE_CANSTACK_MATCH_FRAME_ALIAS 0
#endif

#if (ODRIVE_CANSTACK_MATCH_FRAME_ALIAS)

ODRIVE_STATIC_ASSERT(sizeof(canstack_id_t)  == sizeof(odrive_can_id_t),
                     "canstack_id_t size mismatch");

ODRIVE_STATIC_ASSERT(sizeof(canstack_dlc_t) == sizeof(odrive_can_dlc_t),
                    "canstack_dlc_t size mismatch");

ODRIVE_STATIC_ASSERT(CANSTACK_MAX_PAYLOAD_SIZE == ODRIVE_CAN_DLC_MAX,
                    "CAN payload size mismatch");

/* --- Struct size check --- */
ODRIVE_STATIC_ASSERT(sizeof(odrive_can_message_t) ==
                    sizeof(canstack_message_t),
                    "CAN message size mismatch");

/* --- Field offset checks --- */
ODRIVE_STATIC_ASSERT(offsetof(odrive_can_message_t, id) ==
                    offsetof(canstack_message_t, id),
                    "id offset mismatch");

ODRIVE_STATIC_ASSERT(offsetof(odrive_can_message_t, dlc) ==
                    offsetof(canstack_message_t, dlc),
                    "dlc offset mismatch");

ODRIVE_STATIC_ASSERT(offsetof(odrive_can_message_t, rtr) ==
                    offsetof(canstack_message_t, rtr),
                    "rtr offset mismatch");

ODRIVE_STATIC_ASSERT(offsetof(odrive_can_message_t, data) ==
                    offsetof(canstack_message_t, data),
                    "data offset mismatch");

#endif

//========================================================
//      Helper Macro's
//========================================================

#if defined(CANSTACK_PLATFORM_PSOC5)
    /**
     * @brief Generate PSoC5 hardware filter for ODrive CANSimple command ID
     * 
     * @details
     * ODrive CANSimple message format (11-bit CAN ID):
     *   [10:5] = Node ID (6 bits, 0-63)
     *   [4:0]  = Command ID (5 bits, 0-31)
     * 
     * This macro creates a filter that matches any command ID regardless of node.
     * 
     * @param cmd_id - Command ID (5 bits, 0-31)
     * @param amr    - Output: Acceptance Mask Register (uint32_t variable)
     * @param acr    - Output: Acceptance Code Register (uint32_t variable)
     * 
     * @example
     *   uint32_t amr, acr;
     *   CANSTACK_ODRIVE_FILTER_CMD(ODrive_Heartbeat, amr, acr);
     *   canstack_mb_filter_t filter = {.amr = amr, .acr = acr};
     *   canstack_configure_rx_filter(driver, 0, &filter);
     */
    #define CANSTACK_ODRIVE_FILTER_CMD(cmd_id, amr, acr) do { \
        /* Canonical ID with node=0, cmd in bits [4:0] */ \
        uint16_t canonical_id = ((cmd_id) & 0x1Fu); \
        \
        /* AMR: ignore node bits [10:5], compare cmd bits [4:0] */ \
        /* PSoC5 format: bits [31:21] = ID, bits [20:3] = unused/stuff */ \
        (amr) = ((uint32_t)0x7E0u << 21u) | ((uint32_t)0x3FFFFu << 3u); \
        \
        /* ACR: full pattern with node=0 */ \
        (acr) = ((uint32_t)canonical_id << 21u) | ((uint32_t)0x3FFFFu << 3u); \
    } while(0)

    /**
     * @brief Generate PSoC5 hardware filter for ODrive CANSimple node ID
     * 
     * @details
     * Match ALL commands from a specific node ID.
     * 
     * @param node_id - Node ID (6 bits, 0-63)
     * @param amr     - Output: Acceptance Mask Register
     * @param acr     - Output: Acceptance Code Register
     * 
     * @example
     *   uint32_t amr, acr;
     *   CANSTACK_ODRIVE_FILTER_NODE(42, amr, acr);
     */
    #define CANSTACK_ODRIVE_FILTER_NODE(node_id, amr, acr) do { \
        /* Canonical ID with node in bits [10:5] */ \
        uint16_t canonical_id = (((node_id) & 0x3Fu) << 5u); \
        \
        /* AMR: compare node bits [10:5], ignore cmd bits [4:0] */ \
        (amr) = ((uint32_t)0x1Fu << 21u) | ((uint32_t)0x3FFFFu << 3u); \
        \
        /* ACR: full pattern with specified node */ \
        (acr) = ((uint32_t)canonical_id << 21u) | ((uint32_t)0x3FFFFu << 3u); \
    } while(0)
    
    #define CANSTACK_ODRIVE_REGISTER_FILTER(canstack, cmd, mb) { \
        uint32_t amr, acr; \
        CANSTACK_ODRIVE_FILTER_CMD(cmd, amr, acr); \
        canstack_mb_filter_t rx_filter = {.amr = amr, .acr = acr}; \
        CANSTACK_ERROR_CHECK(canstack_configure_rx_filter(canstack, mb, &rx_filter)); \
    }

#endif 

//========================================================
//      CanStack Configuration
//========================================================

typedef struct {
    canstack_driver driver;
    odrive_gpio_driver_t* gpio;
} odrive_canstack_config_t;

//========================================================
//      CanStack Methods
//========================================================

/**
 * @brief Create a new ODrive backend using a CanStack driver
 * 
 * @param[in] cfg - Configuration for the CANStack backend
 * @param[out] backend - Pointer to output ODrive backend handle
 * 
 * @retval ODRIVE_RESULT_OK - Backend created successfully
 * @retval ODRIVE_ERROR_NULL_POINTER - If @p cfg or @p backend is
 * 
 */
odrive_result_t odrive_create_canstack_backend(
    const odrive_canstack_config_t *cfg,
    odrive_backend* backend);

odrive_result_t odrive_canstack_filter_cmd(
    odrive_backend backend,
    odrive_can_command_t cmd_id);
  
//========================================================
//      End of File
//========================================================

#endif // !ODRIVE_HAVE_CANSTACK

#endif // !ODRIVE_CAN_STACK_H
