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
//      Standard Includes
//========================================================

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

/* Project headers */
#include "CanStackResult.h"
#include "CanStackConfig.h"

//========================================================
//      Library Information
//========================================================

#define CANSTACK_MAJOR 1
#define CANSTACK_MINOR 0
#define CANSTACK_PATCH 1

//========================================================
//      Version Packing Helpers
//========================================================

/**
 * @brief Pack a semantic version (major.minor.patch) into a single comparable integer.
 *
 * Uses 8 bits per component (0..255). Adjust shifts if you need larger ranges.
 */
#define CANSTACK_VERSION_ENCODE(major, minor, patch) \
    ((((major) & 0xFFu) << 16) | (((minor) & 0xFFu) << 8) | ((patch) & 0xFFu))

/** @brief Current library version as a single integer. */
#define CANSTACK_VERSION_STRING \
    "v" CANSTACK_STR(CANSTACK_MAJOR) "." CANSTACK_STR(CANSTACK_MINOR) "." CANSTACK_STR(CANSTACK_PATCH)

/**
 * @brief True if CanStack version is at least (major.minor.patch).
 *
 * Usage:
 *   #if CANSTACK_VERSION_AT_LEAST(1,0,0)
 *     ...
 *   #endif
 */
#define CANSTACK_VERSION_AT_LEAST(major, minor, patch) \
    (CANSTACK_VERSION >= CANSTACK_VERSION_ENCODE((major), (minor), (patch)))

/**
 * @brief True if CanStack version is exactly (major.minor.patch).
 */
#define CANSTACK_VERSION_IS(major, minor, patch) \
    (CANSTACK_VERSION == CANSTACK_VERSION_ENCODE((major), (minor), (patch)))

//========================================================
//      CanStack Typedefs
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

/**
 * @brief Generic void pointer used for command parameters
 */
typedef void* canstack_cmd_t;

//========================================================
//      CanStack States
//========================================================

/**
 * @brief Current state of the CanStack driver
 */
typedef enum {
    CANSTACK_STATE_UNKNOWN = 0x00,
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
typedef struct canstack_driver_T* canstack_driver;

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
    uint8_t         data[CANSTACK_MAX_PAYLOAD_SIZE]; /**< Message payload */
} canstack_message_t;

//========================================================
//      CanStack Hardware Filtering
//========================================================

#if CANSTACK_HAS_HW_FILTERS

    /**
     * @brief Hardware filter configuration (platform-specific)
     * 
     * @details Filter structure varies by platform:
     * - PSoC5: Uses AMR (Acceptance Mask) and ACR (Acceptance Code)
     * - STM32: Uses ID mask and ID list
     */
    #if defined(CANSTACK_PLATFORM_PSOC5)

        typedef struct {
            uint32_t amr;   /**< Acceptance Mask Register */
            uint32_t acr;   /**< Acceptance Code Register */
        } canstack_mb_filter_t;
    #elif defined(CANSTACK_PLATFORM_STM32)

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

#if !defined(CANSTACK_EXCLUDE_FULL_TX_MB)

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
//      CanStack Config
//========================================================

/**
 * @brief Driver configuration structure
 */
typedef struct {
    uint32_t bitrate;   /**< CAN bitrate in Hz (e.g., 500000 for 500 kbps) */
    canstack_ctx_t ctx; /**< Optional user context passed to callbacks */
} canstack_config_t;

//========================================================
//      CanStack Instance Methods
//========================================================

/**
 * @brief Create and initialize a CAN driver instance
 * 
 * @param[in]  cfg    - Driver configuration
 * @param[out] driver - Pointer to driver handle (allocated internally)
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 */
canstack_result_t canstack_create_driver(const canstack_config_t* cfg,
                                         canstack_driver* driver);

/**
 * @brief Destroy and deallocate a CAN driver instance
 * 
 * @param[in] driver - Driver handle to destroy
 * 
 * @return void
 */
void canstack_destroy_driver(canstack_driver driver);

//========================================================
//      CanStack Transmit Methods
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
canstack_result_t canstack_transmit(canstack_driver driver,
                                    const canstack_message_t* msg);


#if !defined(CANSTACK_EXCLUDE_FULL_TX_MB)

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
canstack_result_t canstack_transmit_mailbox(canstack_driver driver,
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
canstack_result_t canstack_register_tx_cb(canstack_driver driver,
                                          canstack_mb_id_t mb_id,
                                          canstack_pfn_tx_callback_t callback);

#endif /* !CAN_STACK_EXCLUDE_FULL_TX_MB */

//========================================================
//      CanStack Receive & Filtering API
//========================================================

#if CANSTACK_HAS_HW_FILTERS

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
canstack_result_t canstack_register_rx_cb(canstack_driver driver,
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
canstack_result_t canstack_configure_rx_filter(canstack_driver driver,
                                               canstack_mb_id_t mb_id,
                                               const canstack_mb_filter_t* filter);

#endif /* CAN_STACK_HAS_HW_FILTERS */

/**
 * @brief Number of buffered CAN frames
 * 
 * @param[in] driver - CanStack driver handle
 * 
 * @return uint8_t
 */
uint8_t canstack_get_rx_count(canstack_driver driver);

/**
 * @brief Retrieve a received CAN message from the RX buffer
 * 
 * @param[in] driver - Driver handle
 * @param[out] msg - Pointer to message buffer
 * 
 * @retval CAN_RESULT_OK - Message retrieved successfully
 * @retval CAN_ERROR_RX_EMPTY - No messages available
 * @retval CAN_ERROR_NULL_POINTER - msg is NULL
 */
canstack_result_t canstack_get_rx(canstack_driver driver,
                                  canstack_message_t* msg);

//========================================================
//      End of File
//========================================================

#endif /* !CAN_STACK_H */
