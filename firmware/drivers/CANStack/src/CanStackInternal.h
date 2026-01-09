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

#include <stdatomic.h>

/* Project headers */
#include "CanStack.h"

//========================================================
//      CanStack Commands
//========================================================

/**
 * @brief Platform driver commands
 */
typedef enum {
    CAN_STACK_CMD_INIT = 0x01,      /**< Initialize hardware */
    CAN_STACK_CMD_DEINIT = 0x02,    /**< Deinitialize hardware */
    CAN_STACK_CMD_START = 0x03,     /**< Start CAN communication */
    CAN_STACK_CMD_STOP = 0x04,      /**< Stop CAN communication */
    
    #if CANSTACK_HAS_HW_FILTERS
        CAN_STACK_CMD_CONFIGURE_RX_FILTER = 0x05,  /**< Configure RX filter */
    #endif
} canstack_command_t;

//========================================================
//      Hardware Filter Command
//========================================================

#if CANSTACK_HAS_HW_FILTERS

    typedef struct {
        canstack_mb_id_t mb_id;
        const canstack_mb_filter_t* filter;
    } canstack_mb_filter_cmd_t;

#endif

//========================================================
//      Platform Function Pointers
//========================================================

/**
 * @brief Platform command execution callback
 * 
 * @param[in] hw_ctx - Platform-specific hardware context
 * @param[in] cmd - Command to execute
 * @param[in] arg - Pointer to the command argument
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 */
typedef canstack_result_t (*canstack_pfn_run_command_t)(canstack_ctx_t hw_ctx,
                                                        canstack_command_t cmd,
                                                        canstack_cmd_t arg);

/**
 * @brief Platform TX message function
 * 
 * @param[in] hw_ctx - Platform-specific hardware context
 * @param[in] mb_id - Mailbox ID
 * @param[in] msg - Message to transmit
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
 * @param[in] mb_id - Mailbox ID
 * @param[out] msg - Pointer to store received message
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 */
typedef canstack_result_t (*canstack_pfn_rx_message_t)(canstack_ctx_t hw_ctx,
                                                       canstack_mb_id_t mb_id,
                                                       canstack_message_t* msg);

                                                       
#if CANSTACK_HAS_HW_FILTERS

/**
 * @brief Platform RX mailbox filter configuration
 * 
 * @param[in] hw_ctx - Platform-specific hardware context
 * @param[in] mb_id - RX mailbox ID
 * @param[in] filter - Filter configuration
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 */
typedef canstack_result_t (*canstack_pfn_configure_rx_filter_t)(
    canstack_ctx_t hw_ctx,
    canstack_mb_id_t mb_id,
    const canstack_mb_filter_t* filter);

#endif /* CAN_STACK_HAS_HW_FILTERS */

//========================================================
//      CanStack Callback Structures
//========================================================

#if !defined(CANSTACK_EXCLUDE_FULL_RX_MB)

/**
 * @brief RX callback entry (callback + context)
 */
typedef struct {
    /**< Callback function pointer */
    canstack_pfn_rx_callback_t callback;  

    /**< User context */
    canstack_ctx_t ctx;                  
} canstack_rx_cb_entry_t;

#endif /* !CAN_STACK_EXCLUDE_FULL_RX_MB */

#if !defined(CANSTACK_EXCLUDE_FULL_TX_MB)

/**
 * @brief TX callback entry (callback + context)
 */
typedef struct {
    /**< Callback function pointer */
    canstack_pfn_tx_callback_t callback;  

    /**< User context */
    canstack_ctx_t ctx;                   
} canstack_tx_cb_entry_t;

#endif /* !CAN_STACK_EXCLUDE_FULL_TX_MB */

//========================================================
//      Rx ISR Queue
//========================================================

typedef struct {
    volatile uint8_t head;   // written by ISR
    volatile uint8_t tail;   // written by consumer
    volatile uint16_t drops;  // overflow counter
    canstack_message_t buf[CANSTACK_RX_QUEUE_SIZE];
} can_rx_queue_t;

//========================================================
//      Platform Driver Interface
//========================================================

/**
 * @brief Platform driver operation table
 * 
 * @details Contains function pointers to platform-specific implementations.
 * Each platform (PSoC5, STM32, etc.) implements this interface.
 */
typedef struct {
    /**< Command execution */
    canstack_pfn_run_command_t run_cmd;     
    
    /**< Transmit message */
    canstack_pfn_tx_message_t tx_message;  
    
    /**< Receive message */
    canstack_pfn_rx_message_t rx_message;    

} canstack_platform_ops_t;

/**
 * @brief Opaque platform driver context
 */
typedef struct {
    /**< Operation vtable */
    canstack_platform_ops_t ops;  

    /**< Hardware-specific context */
    canstack_ctx_t hw_ctx; 

} canstack_platform_backend_t;

//========================================================
//      CanStack Driver Structure
//========================================================

/**
 * @brief Main CanStack driver structure
 * 
 * Contains driver state, callbacks, and platform abstraction layer.
 */
struct canstack_driver_T {
    /**< CAN bitrate */
    uint32_t bitrate;           
    
    /**< Initialization flag */
    bool initialized;           

    /**< Running state flag */
    canstack_state_t state;     

    can_rx_queue_t rx_queue;

    #if !defined(CANSTACK_EXCLUDE_FULL_RX_MB)

    /* RX Callbacks */
    canstack_rx_cb_entry_t rx_callbacks[CANSTACK_TOTAL_RX_MAILBOXES];

    #endif

    #if !defined(CANSTACK_EXCLUDE_FULL_TX_MB)

    /* TX Callbacks */
    canstack_tx_cb_entry_t tx_callbacks[CANSTACK_TOTAL_TX_MAILBOXES];

    #endif

    /**< Platform backend */
    canstack_platform_backend_t* backend;  

    /**< User-provided context */
    canstack_ctx_t user_ctx;    
};

//========================================================
//      CanStack Queue Methods
//========================================================

bool canstack_pop_rx(can_rx_queue_t* queue, canstack_message_t* msg);

void canstack_push_rx(can_rx_queue_t* queue, const canstack_message_t* msg);

//========================================================
//      End of File
//========================================================

#endif /* !CAN_STACK_INTERNAL_H */