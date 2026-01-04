/***************************************************************************//**
* \file CanStack.h
* \version 1.0.0
*
* \brief
* Public API for the CanStack library, providing a cross-platform implementation
* of the CAN 2.0 protocol with hardware filtering support.
*
* \details
* CanStack abstracts platform-specific CAN implementations (PSoC5, STM32, etc.)
* behind a unified C API. Supports RX/TX mailboxes, callbacks, and optional
* hardware filtering.
*
********************************************************************************
* \copyright
* Copyright 2025, Embedded Dynamics. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CAN_STACK_H
#define CAN_STACK_H

//========================================================
//      CanStack Standard Includes
//========================================================

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Project headers */
#include "CanStackResult.h"
#include "CanStackConfig.h"

//========================================================
//      CanStack Typpe Definitions
//========================================================

/**
 * @brief CAN message identifier (11-bit or 29-bit)
 */
typedef uint32_t canstack_id_t;

/**
 * @brief CAN Data Length Code (0-8)
 */
typedef uint8_t canstack_dlc_t;

/**
 * @brief Mailbox identifier (0-15 for RX, 0-7 for TX)
 */
typedef uint8_t canstack_mb_id_t;

/**
 * @brief Generic context pointer for callbacks
 */
typedef void* canstack_ctx_t;

typedef enum {
    CANSTACK_STATE_UNKOWN = 0x00,
    CANSTACK_STATE_IDLE,
    CANSTACK_STATE_RUNNING,
    CANSTACK_STATE_STOPPED,
    CANSTACK_STATE_TRANSMITTING,
} canstack_state_t;

//========================================================
//      CanStack Opaque Driver Handle
//========================================================

/**
 * @brief Opaque handle to CAN driver instance
 * 
 * Actual structure defined in CanStack_Internal.h. Users interact
 * with this as an opaque pointer.
 */
typedef struct canstack_driver_T* canstack_driver_t;

//========================================================
//      CanStack Core Data Structs
//========================================================

/**
 * @brief CAN 2.0 message structure
 * 
 * @details Contains message ID, data, length, and RTR flag.
 * Payload size limited by CAN_STACK_MAX_PAYLOAD_SIZE.
 */
typedef struct {
    canstack_id_t   id;                              /**< CAN message ID */
    canstack_dlc_t  dlc;                             /**< Data length (0-8) in bytes */
    bool            rtr;                             /**< Remote transmission request */
    uint8_t         data[CAN_STACK_MAX_PAYLOAD_SIZE]; /**< Message payload */
} canstack_message_t;

/**
 * @brief Driver configuration structure
 */
typedef struct {
    uint32_t bitrate;   /**< CAN bitrate in Hz (e.g., 500000 for 500 kbps) */
    canstack_ctx_t ctx; /**< Optional user context passed to callbacks */
} canstack_config_t;

//========================================================
//      CanStack Hardware Filtering
//========================================================

#if CAN_STACK_HAS_HW_FILTERS

    /**
     * @brief Hardware filter configuration (platform-specific)
     * 
     * @details Filter structure varies by platform:
     * - PSoC5: Uses AMR (Acceptance Mask) and ACR (Acceptance Code)
     * - STM32: Uses ID mask and ID list
     */
    #if defined(CAN_STACK_PLATFORM_PSOC5)

        typedef struct {
            uint32_t amr;   /**< Acceptance Mask Register */
            uint32_t acr;   /**< Acceptance Code Register */
        } canstack_mb_filter_t;

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

    #elif defined(CAN_STACK_PLATFORM_STM32)

        typedef struct {
            uint32_t id_mask;  /**< Mask for ID matching */
            uint32_t id_list;  /**< List of IDs to accept */
        } canstack_mb_filter_t;
    #endif
#endif /* CAN_STACK_HAS_HW_FILTERS */

//========================================================
//      CanStack Callback Function Pointers
//========================================================

/**
 * @brief RX message callback function signature
 * 
 * @param[in] msg - Pointer to received CAN message
 * @param[in] ctx - User context passed during callback registration
 */
typedef void (*canstack_pfn_rx_callback_t)(const canstack_message_t* msg,
                                           canstack_ctx_t ctx);

#if !defined(CAN_STACK_EXCLUDE_FULL_TX_MB)

/**
 * @brief TX completion callback function signature
 * 
 * @param[in] msg - Pointer to transmitted CAN message
 * @param[in] ctx - User context passed during callback registration
 */
typedef void (*canstack_pfn_tx_callback_t)(const canstack_message_t* msg,
                                           canstack_ctx_t ctx);

#endif /* !CAN_STACK_EXCLUDE_FULL_TX_MB */

//========================================================
//      CanStack Public Driver API
//========================================================

/**
 * @brief Create and initialize a CAN driver instance
 * 
 * @param[out] driver - Pointer to driver handle (allocated internally)
 * @param[in]  cfg    - Driver configuration
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 */
canstack_result_t canstack_create_driver(canstack_driver_t* driver,
                                         const canstack_config_t* cfg);

/**
 * @brief Destroy and deallocate a CAN driver instance
 * 
 * @param[in] driver - Driver handle to destroy
 * 
 * @return void
 */
void canstack_destroy_driver(canstack_driver_t driver);

//========================================================
//      CanStack Transmit API
//========================================================

/**
 * @brief Transmit a CAN message (auto-allocated mailbox)
 * 
 * @param[in] driver - Driver handle
 * @param[in] msg    - Message to transmit
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 * 
 * @note Selects first available TX mailbox automatically
 */
canstack_result_t canstack_transmit(canstack_driver_t driver,
                                    const canstack_message_t* msg);


#if !defined(CAN_STACK_EXCLUDE_FULL_TX_MB)

/**
 * @brief Transmit using a specific TX mailbox
 * 
 * @param[in] driver - Driver handle
 * @param[in] mb_id  - TX mailbox ID (0-7)
 * @param[in] msg    - Message to transmit
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 */
canstack_result_t canstack_transmit_mailbox(canstack_driver_t driver,
                                            canstack_mb_id_t mb_id,
                                            const canstack_message_t* msg);

/**
 * @brief Register TX completion callback
 * 
 * @param[in] driver   - Driver handle
 * @param[in] mb_id    - TX mailbox ID (0-7)
 * @param[in] callback - Callback function (NULL to unregister)
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 */
canstack_result_t canstack_register_tx_cb(canstack_driver_t driver,
                                          canstack_mb_id_t mb_id,
                                          canstack_pfn_tx_callback_t callback);

#endif /* !CAN_STACK_EXCLUDE_FULL_TX_MB */

//========================================================
//      CanStack Receive & Filtering API
//========================================================

#if CAN_STACK_HAS_HW_FILTERS

/**
 * @brief Register RX message callback for a specific mailbox
 * 
 * @param[in] driver   - Driver handle
 * @param[in] mb_id    - RX mailbox ID (0-15)
 * @param[in] callback - Callback function (NULL to unregister)
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 */
canstack_result_t canstack_register_rx_cb(canstack_driver_t driver,
                                          canstack_mb_id_t mb_id,
                                          canstack_pfn_rx_callback_t callback);

/**
 * @brief Configure hardware filter for an RX mailbox
 * 
 * @param[in] driver - Driver handle
 * @param[in] mb_id  - RX mailbox ID (0-15)
 * @param[in] filter - Filter configuration (platform-specific)
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 * 
 * @note PSoC5: @p filter uses AMR/ACR registers
 * @note STM32: @p filter uses ID mask/list
 */
canstack_result_t canstack_configure_rx_filter(canstack_driver_t driver,
                                               canstack_mb_id_t mb_id,
                                               const canstack_mb_filter_t* filter);

#endif /* CAN_STACK_HAS_HW_FILTERS */

//========================================================
//      End of File
//========================================================

/*
typedef void (*canstack_pfn_rx_callback) (const canstack_message_t* msg,
                                          canstack_ctx ctx);


typedef canstack_result (*canstack_pfn_configure_rx) (canstack_ctx hw_ctx, 
                                                      canstack_mb_id_t mb_id, 
                                                      canstack_mb_filter* filter);


typedef canstack_result (*canstack_register_rx_callback) (canstack_ctx hw_ctx, 
                                                          canstack_mb_id_t mb_id, 
                                                          canstack_pfn_rx_callback cb);
*/

#endif /* !CAN_STACK_H */
