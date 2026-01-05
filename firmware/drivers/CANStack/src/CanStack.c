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
//      CanStack Methods
//========================================================

canstack_result_t canstack_create_driver(const canstack_config_t* cfg,
                                         canstack_driver* driver)
{
    /* Parameter validation */
    if (!driver || !cfg)
        return CAN_RESULT_INVALID_ARG;
    
    /* Allocating platform driver */
    canstack_platform_driver_t* platform_driver = (canstack_platform_driver_t*) malloc(sizeof(canstack_platform_driver_t));
    if (!platform_driver)
        return CAN_RESULT_NO_MEMORY;

    /* Perform platform specific operations*/
    #if defined(CAN_STACK_PLATFORM_PSOC5)
        /* Initialize the platform driver with PSoC5 specific functions */
        CAN_STACK_ERROR_CHECK(canstack_psoc5_create_driver(platform_driver));

        /* Bind the CanStack driver to the PSoC5 platform*/
        canstack_psoc5_bind(&driver);
        
    #else
        return CAN_RESULT_NO_PLATFORM_DRIVER;
    #endif

    canstack_driver new_driver = malloc(sizeof(struct canstack_driver_T));
    if (!new_driver)
        return CAN_RESULT_NO_MEMORY;

    new_driver->initialized = false;
    new_driver->state = CANSTACK_STATE_IDLE;
    new_driver->platform = platform_driver;
    new_driver->user_ctx = cfg->ctx;

    *driver = new_driver;

    return CAN_RESULT_OK;
}

void canstack_destroy_driver(canstack_driver driver)
{
    
}

canstack_result_t canstack_transmit(canstack_driver driver,
                                    const canstack_message_t* msg)
{
    if (!driver || !msg)
        return CAN_RESULT_INVALID_ARG;

    /* Check for proper dlc bounds even though the memory buffer is fixed */
    if (msg->dlc > CANSTACK_MAX_PAYLOAD_SIZE)
        return CAN_RESULT_PAYLOAD_OVERFLOW;

    if (driver->state == CANSTACK_STATE_RUNNING)
        return CAN_RESULT_INVALID_STATE;

    return driver->platform->ops.tx_message(driver->platform->hw_ctx, CANSTACK_MAILBOX_ID_ANY, msg);
}

#if CANSTACK_HAS_HW_FILTERS

    canstack_result_t canstack_register_rx_cb(canstack_driver driver,
                                              canstack_mb_id_t mb_id,
                                              canstack_pfn_rx_callback_t callback)
    {
        if (!driver || !callback)
            return CAN_RESULT_INVALID_ARG;

        if (mb_id > CAN_STACK_TOTAL_RX_MAILBOXES)
            return CAN_RESULT_INVALID_ARG;

        driver->rx_callbacks[mb_id].callback = callback;

        return CAN_RESULT_OK;
    }

    canstack_result_t canstack_configure_rx_filter(canstack_driver driver,
                                                   canstack_mb_id_t mb_id,
                                                   const canstack_mb_filter_t* filter)
    {
        if (!driver || !filter)
            return CAN_RESULT_INVALID_ARG;

        if (mb_id > CAN_STACK_TOTAL_RX_MAILBOXES)
            return CAN_RESULT_INVALID_ARG;

        if (driver->state == CANSTACK_STATE_RUNNING)
        return CAN_RESULT_INVALID_STATE;

        canstack_mb_filter_cmd_t filter_cmd = {
            .mb_id = mb_id,
            .filter = filter,
        };
        
        return driver->platform->ops.run_cmd(driver->platform->hw_ctx, CAN_STACK_CMD_CONFIGURE_RX_FILTER, (void*) &filter_cmd);
    }
#endif

//========================================================
//      End of File
//========================================================