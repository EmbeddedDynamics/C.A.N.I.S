/***************************************************************************//**
* \file CanStack.C
* \version 1.0.0
*
* \brief
* Private internal source file for CanStack library.
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

//========================================================
//      Standard Includes
//========================================================

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

#include "CanStackInternal.h"

/* Project headers */
#if defined(CANSTACK_PLATFORM_PSOC5)
    #include "CanStackPSoC5.h"
#endif

//========================================================
//      CanStack Instance Methods
//========================================================

canstack_result_t canstack_create_driver(const canstack_config_t* cfg,
                                         canstack_driver* driver)
{
    /* Parameter validation */
    if (!driver || !cfg)
        return CAN_ERROR_NULL_POINTER;
    
    
    /* Allocating platform driver */
    canstack_platform_backend_t* backend = (canstack_platform_backend_t*) malloc(sizeof(canstack_platform_backend_t));
    if (!backend)
        return CAN_ERROR_NO_MEMORY;

    /* Perform platform specific operations*/
    #if defined(CAN_STACK_PLATFORM_PSOC5)
        /* Initialize the platform driver with PSoC5 specific functions */
        CAN_STACK_ERROR_CHECK(canstack_psoc5_create_backend(backend));

        /* Bind the CanStack driver to the PSoC5 platform*/
        canstack_psoc5_bind(&driver);
        
    #else
        return CAN_ERROR_INVALID_BACKEND;
    #endif

    canstack_driver new_driver = malloc(sizeof(struct canstack_driver_T));
    if (!new_driver)
        return CAN_ERROR_NO_MEMORY;

    new_driver->initialized = false;
    new_driver->state = CANSTACK_STATE_IDLE;
    new_driver->backend = backend;
    new_driver->user_ctx = cfg->ctx;
    
    *driver = new_driver;

    return CAN_RESULT_OK;
}

void canstack_destroy_driver(canstack_driver driver)
{
    if (!driver)
        return

    /* Perform platform specific operations*/
    #if defined(CAN_STACK_PLATFORM_PSOC5)
        /* Destroy PSoC5 platform driver*/
        canstack_psoc5_destroy_backend(driver->backend);
    #endif

    free(driver);

    return;
}

//========================================================
//      CanStack Transmit Methods
//========================================================

canstack_result_t canstack_transmit(canstack_driver driver,
                                    const canstack_message_t* msg)
{
    if (!driver || !msg)
        return CAN_ERROR_NULL_POINTER;

    /* Check for proper dlc bounds even though the memory buffer is fixed */
    if (msg->dlc > CANSTACK_MAX_PAYLOAD_SIZE)
        return CAN_ERROR_PAYLOAD_OVERFLOW;

    if (driver->state == CANSTACK_STATE_RUNNING)
        return CAN_ERROR_INVALID_STATE;

    return driver->backend->ops.tx_message(driver->backend->hw_ctx, CANSTACK_MAILBOX_ID_ANY, msg);
}

#if !defined(CANSTACK_EXCLUDE_FULL_TX_MB)


canstack_result_t canstack_transmit_mailbox(canstack_driver driver,
                                            canstack_mb_id_t mb_id,
                                            const canstack_message_t* msg)
{
    if (!driver)
        return CAN_ERROR_NULL_POINTER;

    if (msg->dlc > CANSTACK_MAX_PAYLOAD_SIZE)
        return CAN_ERROR_PAYLOAD_OVERFLOW;

    if (driver->state == CANSTACK_STATE_RUNNING)
        return CAN_ERROR_INVALID_STATE;

    return driver->backend->ops.tx_message(driver->backend->hw_ctx, mb_id, msg);
}


canstack_result_t canstack_register_tx_cb(canstack_driver driver,
                                          canstack_mb_id_t mb_id,
                                          canstack_pfn_tx_callback_t callback)
{
    if (!driver || !callback)
        return CAN_ERROR_NULL_POINTER;

    if (mb_id > CANSTACK_TOTAL_TX_MAILBOXES)
        return CAN_ERROR_INVALID_PARAMTER;

    driver->tx_callbacks[mb_id] = callback;

    return CAN_RESULT_OK;
}

#endif /* !CAN_STACK_EXCLUDE_FULL_TX_MB */

#if CANSTACK_HAS_HW_FILTERS

canstack_result_t canstack_register_rx_cb(canstack_driver driver,
                                          canstack_mb_id_t mb_id,
                                          canstack_pfn_rx_callback_t callback)
{
    if (!driver || !callback)
        return CAN_ERROR_NULL_POINTER;

    if (mb_id > CANSTACK_TOTAL_RX_MAILBOXES)
        return CAN_ERROR_INVALID_PARAMTER;

    driver->rx_callbacks[mb_id].callback = callback;

    return CAN_RESULT_OK;
}

canstack_result_t canstack_configure_rx_filter(canstack_driver driver,
                                                canstack_mb_id_t mb_id,
                                                const canstack_mb_filter_t* filter)
{
    if (!driver || !filter)
        return CAN_ERROR_NULL_POINTER;

    if (mb_id > CANSTACK_TOTAL_RX_MAILBOXES)
        return CAN_ERROR_INVALID_PARAMTER;

    if (driver->state == CANSTACK_STATE_RUNNING)
        return CAN_ERROR_INVALID_STATE;

    canstack_mb_filter_cmd_t filter_cmd = {
        .mb_id = mb_id,
        .filter = filter,
    };
    
    return driver->backend->ops.run_cmd(driver->backend->hw_ctx, CAN_STACK_CMD_CONFIGURE_RX_FILTER, (void*) &filter_cmd);
}
#endif

//========================================================
//      CanStack Queue Methods
//========================================================

bool canstack_pop_rx(can_rx_queue_t* queue, canstack_message_t* msg)
{
    uint8_t tail = queue->tail;
    uint8_t head = queue->head;

    if (tail == head) {
        return false;         // empty
    }

    *msg = queue->buf[tail];
    queue->tail = (uint8_t)((tail + 1u) & CANSTACK_RX_QUEUE_MASK);
    return true;
}

void canstack_push_rx(can_rx_queue_t* queue, const canstack_message_t* msg)
{
    CANSTACK_DISABLE_INTERRUPT();
    
    uint8_t head = queue->head;
    uint8_t next = (uint8_t)((head + 1u) & CANSTACK_RX_QUEUE_MASK);
    
    if (next == queue->tail) {
        queue->drops++;           // queue full: drop newest (or choose another policy)
        return;
    }

    queue->buf[head] = *msg;        // struct copy
    queue->head = next;

    CANSTACK_ENABLE_INTERRUPT();
}

uint8_t canstack_get_rx_count(canstack_driver driver)
{
    if (!driver) return 0;

    uint8_t head = driver->rx_queue.head;
    uint8_t tail = driver->rx_queue.tail;

    return (uint8_t)((head - tail) & CANSTACK_RX_QUEUE_MASK);
}

canstack_result_t canstack_get_rx(canstack_driver driver,
                                  canstack_message_t* msg)
{
    if (!driver|| !msg)
        return CAN_ERROR_NULL_POINTER;

    if (!canstack_pop_rx(&driver->rx_queue, msg))
        return CAN_ERROR_RX_EMPTY;
    
    return CAN_RESULT_OK;
}

//========================================================
//      End of File
//========================================================