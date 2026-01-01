/***************************************************************************//**
* \file CanStackInternal.h
* \version 1.0.0
*
* \brief
* Private internal header for CanStack library.
*
* \details
* Contains driver structures, platform abstraction callbacks, and internal
* type definitions not exposed to public API.
*
********************************************************************************
* \copyright
* Copyright 2025, Embedded Dynamics. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CAN_STACK_INTERNAL_H
#define CAN_STACK_INTERNAL_H

//========================================================
//      Standard Includes
//========================================================

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Project headers */
#include "CanStack.h"

//========================================================
//      Platform Commands
//========================================================

/**
 * @brief Platform driver commands
 */
typedef enum {
    CAN_STACK_CMD_INIT = 0x01,      /**< Initialize hardware */
    CAN_STACK_CMD_DEINIT = 0x02,    /**< Deinitialize hardware */
    CAN_STACK_CMD_START = 0x03,     /**< Start CAN communication */
    CAN_STACK_CMD_STOP = 0x04,      /**< Stop CAN communication */
    
    #if CAN_STACK_HAS_HW_FILTERS
        CAN_STACK_CMD_CONFIGURE_RX_FILTER = 0x05,  /**< Configure RX filter */
    #endif
} canstack_command_t;

#if CAN_STACK_HAS_HW_FILTERS

    typedef struct {
        canstack_mb_id_t mb_id;
        const canstack_mb_filter_t* filter;
    } canstack_mb_filter_cmd_t;

#endif

//========================================================
//      Platform Abstraction Layer
//========================================================

/**
 * @brief Platform command execution callback
 * 
 * @param[in] hw_ctx - Platform-specific hardware context
 * @param[in] cmd    - Command to execute
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 */
typedef canstack_result_t (*canstack_pfn_run_command_t)(canstack_ctx_t hw_ctx,
                                                        canstack_command_t cmd,
                                                        void* arg);

/**
 * @brief Platform TX message function
 * 
 * @param[in] hw_ctx - Platform-specific hardware context
 * @param[in] mb_id  - Mailbox ID
 * @param[in] msg    - Message to transmit
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 */
typedef canstack_result_t (*canstack_pfn_tx_message_t)(canstack_ctx_t hw_ctx,
                                                       canstack_mb_id_t mb_id,
                                                       const canstack_message_t* msg);

/**
 * @brief Platform RX message function
 * 
 * @param[in] hw_ctx - Platform-specific hardware context
 * @param[in] mb_id  - Mailbox ID
 * @param[out] msg   - Pointer to store received message
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 */
typedef canstack_result_t (*canstack_pfn_rx_message_t)(canstack_ctx_t hw_ctx,
                                                       canstack_mb_id_t mb_id,
                                                       canstack_message_t* msg);

#if CAN_STACK_HAS_HW_FILTERS

/**
 * @brief Platform RX mailbox filter configuration
 * 
 * @param[in] hw_ctx - Platform-specific hardware context
 * @param[in] mb_id  - RX mailbox ID
 * @param[in] filter - Filter configuration
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 */
typedef canstack_result_t (*canstack_pfn_configure_rx_filter_t)(
    void* hw_ctx,
    canstack_mb_id_t mb_id,
    const canstack_mb_filter_t* filter);

#endif /* CAN_STACK_HAS_HW_FILTERS */

//========================================================
//      CanStack Callback storage
//========================================================

#if !defined(CAN_STACK_EXCLUDE_FULL_RX_MB)

/**
 * @brief RX callback entry (callback + context)
 */
typedef struct {
    canstack_pfn_rx_callback_t callback;  /**< Callback function pointer */
    canstack_ctx_t ctx;                   /**< User context */
} canstack_rx_cb_entry_t;

#endif /* !CAN_STACK_EXCLUDE_FULL_RX_MB */

#if !defined(CAN_STACK_EXCLUDE_FULL_TX_MB)

/**
 * @brief TX callback entry (callback + context)
 */
typedef struct {
    canstack_pfn_tx_callback_t callback;  /**< Callback function pointer */
    canstack_ctx_t ctx;                   /**< User context */
} canstack_tx_cb_entry_t;

#endif /* !CAN_STACK_EXCLUDE_FULL_TX_MB */

//========================================================
//      Platform Driver Interface
//========================================================

/**
 * @brief Platform driver vtable
 * 
 * @details Contains function pointers to platform-specific implementations.
 * Each platform (PSoC5, STM32, etc.) implements this interface.
 */
typedef struct {
    /* Core operations */
    canstack_pfn_run_command_t run_cmd;      /**< Command execution */
    canstack_pfn_tx_message_t tx_message;    /**< Transmit message */
    canstack_pfn_rx_message_t rx_message;    /**< Receive message */

} canstack_platform_ops_t;

/**
 * @brief Opaque platform driver context
 */
typedef struct {
    canstack_platform_ops_t ops;  /**< Operation vtable */
    void* hw_ctx;                 /**< Hardware-specific context */
} canstack_platform_driver_t;

//========================================================
//      CanStack Driver Instance Structure
//========================================================

/**
 * @brief Main CAN driver instance (pointed to by canstack_driver_t)
 * 
 * @details Contains driver state, callbacks, and platform abstraction layer.
 */
typedef struct canstack_driver_T {
    /* Configuration */
    uint32_t bitrate;           /**< CAN bitrate */
    canstack_ctx_t user_ctx;    /**< User-provided context */

    /* RX Callbacks */
    #if !defined(CAN_STACK_EXCLUDE_FULL_RX_MB)
        canstack_rx_cb_entry_t rx_callbacks[CAN_STACK_TOTAL_RX_MAILBOXES];
    #endif

    /* TX Callbacks */
    #if !defined(CAN_STACK_EXCLUDE_FULL_TX_MB)
        canstack_tx_cb_entry_t tx_callbacks[CAN_STACK_TOTAL_TX_MAILBOXES];
    #endif

    /* Platform abstraction */

    /**< Platform driver */
    canstack_platform_driver_t* platform;  

    /* State */
    bool initialized;           /**< Initialization flag */
    canstack_state_t state;     /**< Running state flag */
} canstack_driver_struct_t;

//========================================================
//      End of File
//========================================================

#endif /* !CAN_STACK_INTERNAL_H */