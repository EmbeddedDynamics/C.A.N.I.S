/***************************************************************************//**
* \file CanStackPSoC5.c
* \version 1.0.0
*
* \brief
* PSoC5 platform-specific implementation for CanStack library.
*
* \details
* Provides PSoC5 CAN driver implementation including TX/RX message handling,
* hardware filter configuration, and ISR callback dispatch.
*
********************************************************************************
* \copyright
* Copyright 2025, Embedded Dynamics. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#if defined(CANSTACK_PLATFORM_PSOC5)

#include "CanStackPSoC5.h"

//========================================================
//      Standard Includes
//========================================================

#include <stdbool.h>
#include <string.h>

//========================================================
//      PSoC5 Platform Context
//========================================================

static canstack_driver g_drv = NULL;

/**
 * @brief Static platform context for PSoC5
 * 
 * @details In a single-CAN system, this is sufficient.
 * For multiple CAN instances, allocate dynamically per driver.
 */
static canstack_psoc5_context_t g_psoc5_ctx = {
    .rx_enabled_mask = CANSTACK_PSOC5_RX_ENABLED_MASK,
    .tx_enabled_mask = CANSTACK_PSOC5_TX_ENABLED_MASK,
};

//========================================================
//      Static Inline Helpers
//========================================================

static inline bool canstack_psoc5_is_rx_enabled(canstack_mb_id_t mb_id)
{
    return (mb_id < 16u) &&
           CANSTACK_BIT_IS_SET(CANSTACK_PSOC5_RX_ENABLED_MASK, mb_id);
}

static inline bool canstack_psoc5_is_tx_enabled(canstack_mb_id_t mb_id)
{
    return (mb_id < 16u) &&
           CANSTACK_BIT_IS_SET(CANSTACK_PSOC5_TX_ENABLED_MASK, mb_id);
}

//========================================================
//      TX Message Handler
//========================================================

/**
 * @brief Transmit CAN message via specific TX mailbox
 * 
 * @param[in] hw_ctx - Platform context (canstack_psoc5_context_t*)
 * @param[in] mb_id  - TX mailbox ID (0-7)
 * @param[in] msg    - Message to transmit
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 * 
 * @details
 * - Validates mailbox is enabled
 * - Copies message data to mailbox registers
 * - Calls PSoC-generated CAN_SendMsg[n]() function
 */
static canstack_result_t psoc5_tx_message(canstack_ctx_t __unused hw_ctx,
                                          canstack_mb_id_t mb_id,
                                          const canstack_message_t* msg)
{
    /* Parameter validation */
    if (!msg) {
        return CAN_RESULT_INVALID_ARG;
    }

    // if (msg->dlc > CAN_STACK_MAX_PAYLOAD_SIZE) {
    //     return CAN_RESULT_INVALID_ARG;
    // }

    //canstack_psoc5_context_t* ctx = (canstack_psoc5_context_t*)hw_ctx;

    if (mb_id == CAN_STACK_MAILBOX_ID_ANY) {
        // Build PSoC5 CAN message structure
        CANSTACK_PSOC5_TYPE(TX_MSG) tx_msg = {
            .id = msg->id,
            .dlc = msg->dlc,
            .rtr = (uint8_t)msg->rtr,
            .ide = 0,           // Standard 11-bit CAN (not extended)
            .irq = 0,           // No interrupt on TX complete
            .msg = (CANSTACK_PSOC5_TYPE(DATA_BYTES_MSG)*)msg->data
        };
        
        // BASIC CAN: use first available mailbox (CAN_SendMsg)
        if (CANSTACK_PSOC5_CALL(SendMsg, &tx_msg) != CYRET_SUCCESS) {
            return CAN_RESULT_ERROR;
        }
    } else {
        /* Check if mailbox is enabled */
        if (!canstack_psoc5_is_tx_enabled(mb_id)) {
            return CAN_RESULT_NOT_CONFIGURED;
        }

        /* Write message data to mailbox */
        //memcpy((void*) CAN_TX[mb_id].txdata.byte,(void*) msg->data, msg->dlc);

        for (uint8_t j = 0u; (j < msg->dlc) && (j < CANSTACK_PSOC5_CONST(TX_DLC_MAX_VALUE)); j++)
        {
            //CAN_TX_DATA_BYTE(mb_id, j) =
            CANSTACK_PSOC5_MACRO_CALL(TX_DATA_BYTE, mb_id, j) = msg->data[j];
        }

        /* Dispatch to PSoC-generated send function */
        uint8_t result = (uint8_t) CYRET_UNKNOWN;
        
        switch (mb_id) {
            #if CAN_TX0_FUNC_ENABLE
            case 0u:
                result = CANSTACK_PSOC5_CALL(SendMsg0);
                break;
            #endif

            #if CAN_TX1_FUNC_ENABLE
            case 1u:
                result = CANSTACK_PSOC5_CALL(SendMsg1);
                break;
            #endif

            #if CAN_TX2_FUNC_ENABLE
            case 2u:
                result = CANSTACK_PSOC5_CALL(SendMsg2);
                break;
            #endif

            #if CAN_TX3_FUNC_ENABLE
            case 3u:
                result = CANSTACK_PSOC5_CALL(SendMsg3);
                break;
            #endif

            #if CAN_TX4_FUNC_ENABLE
            case 4u:
                result = CANSTACK_PSOC5_CALL(SendMsg4);
                break;
            #endif

            #if CAN_TX5_FUNC_ENABLE
            case 5u:
                result = CANSTACK_PSOC5_CALL(SendMsg5);
                break;
            #endif

            #if CAN_TX6_FUNC_ENABLE
            case 6u:
                result = CANSTACK_PSOC5_CALL(SendMsg6);
                break;
            #endif

            #if CAN_TX7_FUNC_ENABLE
            case 7u:
                result = CANSTACK_PSOC5_CALL(SendMsg7);
                break;
            #endif

            default:
                return CAN_RESULT_INVALID_ARG;
        }

        return (result == CYRET_SUCCESS) ? CAN_RESULT_OK : CAN_RESULT_ERROR;
    }

    return CAN_RESULT_ERROR;
}

//========================================================
//      RX Message Handler
//========================================================

/**
 * @brief Read CAN message from RX mailbox (polling)
 * 
 * @param[in] hw_ctx - Platform context (canstack_psoc5_context_t*)
 * @param[in] mb_id  - RX mailbox ID (0-15)
 * @param[out] msg   - Message buffer
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 * 
 * @details
 * Used for polling-based RX (alternative to interrupts).
 * Reads from PSoC mailbox registers.
 */
static canstack_result_t psoc5_rx_message(canstack_ctx_t __unused hw_ctx,
                                          canstack_mb_id_t mb_id,
                                          canstack_message_t* msg)
{
    if (!msg) {
        return CAN_RESULT_INVALID_ARG;
    }

    //canstack_psoc5_context_t* ctx = (canstack_psoc5_context_t*)hw_ctx;

    /* Check if mailbox is enabled */
    if (!canstack_psoc5_is_rx_enabled(mb_id)) {
        return CAN_RESULT_NOT_CONFIGURED;
    }

    /* Check if mailbox has new data (DLC != 0 indicates valid message) */
    //if (CAN_RX[mb_id].rxdlc == 0) {
    //    return CAN_STACK_RESULT_NO_DATA;
    //}

    /* Read message from mailbox */
    msg->id = (canstack_id_t)CANSTACK_PSOC5_MACRO_CALL(GET_RX_ID, mb_id);
    msg->dlc = (canstack_dlc_t)CANSTACK_PSOC5_MACRO_CALL(GET_DLC, mb_id);
    for (uint8_t j = 0u; (j < msg->dlc) && (j < CANSTACK_PSOC5_CONST(TX_DLC_MAX_VALUE)); j++)
    {
        msg->data[j] = CANSTACK_PSOC5_MACRO_CALL(RX_DATA_BYTE, mb_id, j);
    }
    
    //memcpy((void*)msg->data, (void*)CAN_RX[mb_id].rxdata.byte, msg->dlc);

    return CAN_RESULT_OK;
}

//========================================================
//      PSoC5 Hardware Filtering
//========================================================

#if CANSTACK_HAS_HW_FILTERS

/**
 * @brief Configure RX mailbox hardware filter
 * 
 * @param[in] hw_ctx - Platform context
 * @param[in] mb_id  - RX mailbox ID (0-15)
 * @param[in] filter - Filter configuration (AMR/ACR)
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 * 
 * @details
 * PSoC5 uses:
 * - ACR (Acceptance Code Register): ID to match
 * - AMR (Acceptance Mask Register): Which bits to care about
 *      - bit = 1 : Don't care
 * 
 * Example: Match ID 0x123 with mask 0x7FF (11-bit)
 *   acr = 0x123 << 5 (PSoC5 format)
 *   amr = 0x7FF << 5
 * 
 * @see For documentation of the acr and amr registers see: <a href="https://www.infineon.com/assets/row/public/documents/30/57/infineon-psoc-3-architecture-trm-additionaltechnicalinformation-en.pdf?fileId=8ac78c8c7d0d8da4017d0f9055f37cd5&utm_source=cypress&utm_medium=referral&utm_campaign=202110_globe_en_all_integration-technical_reference_manual&redirId=TRM103#page=235">docs</a>
 */
static canstack_result_t psoc5_configure_rx_filter(canstack_ctx_t __unused hw_ctx,
                                                   canstack_mb_id_t mb_id,
                                                   const canstack_mb_filter_t* filter)
{
    if (!filter) {
        return CAN_RESULT_INVALID_ARG;
    }

    //canstack_psoc5_context_t* ctx = (canstack_psoc5_context_t*)hw_ctx;

    /* Check if mailbox is enabled */
    if (!canstack_psoc5_is_rx_enabled(mb_id)) {
        return CAN_RESULT_NOT_CONFIGURED;
    }

    uint8 result = CANSTACK_PSOC5_CONST(FAIL);
    if (CANSTACK_PSOC5_CALL(RXRegisterInit, (reg32 *)&CANSTACK_PSOC5_CONST(RX)[mb_id].rxamr, filter->amr) == CYRET_SUCCESS)
    {
        if (CANSTACK_PSOC5_CALL(RXRegisterInit, (reg32 *)&CANSTACK_PSOC5_CONST(RX)[mb_id].rxacr, filter->acr) == CYRET_SUCCESS)
        {
            result = CYRET_SUCCESS;
        }
    }

    return (result == CYRET_SUCCESS) ? CAN_RESULT_OK : CAN_RESULT_ERROR;
}

#endif /* CAN_STACK_HAS_HW_FILTERS */

//========================================================
//      Platform Command Dispatch
//========================================================

/**
 * @brief Execute platform command
 * 
 * @param[in] hw_ctx - Platform context
 * @param[in] cmd    - Command to execute
 * @param[in] arg    - Command argument (type depends on command)
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 * 
 * @details
 * Dispatches commands to appropriate platform operations:
 * - CMD_INIT: Start CAN hardware
 * - CMD_DEINIT: Stop CAN hardware
 * - CMD_START: Enable communication
 * - CMD_STOP: Disable communication
 * - CMD_CONFIGURE_RX_FILTER: Setup hardware filter
 */
static canstack_result_t psoc5_run_command(canstack_ctx_t __unused hw_ctx,
                                           canstack_command_t cmd,
                                           void* arg)
{
    //canstack_psoc5_context_t* ctx = (canstack_psoc5_context_t*)hw_ctx;

    switch (cmd) {
        case CAN_STACK_CMD_INIT: {
            const canstack_config_t* cfg = (const canstack_config_t*)arg;

            /* Initialize PSoC CAN component */
            CANSTACK_PSOC5_CALL(Init);
            return CAN_RESULT_OK;
        }

        case CAN_STACK_CMD_DEINIT:
            CANSTACK_PSOC5_CALL(Stop);
            return CAN_RESULT_OK;

        case CAN_STACK_CMD_START:
            CANSTACK_PSOC5_CALL(Start);
            return CAN_RESULT_OK;

        case CAN_STACK_CMD_STOP:
            CANSTACK_PSOC5_CALL(Stop);
            return CAN_RESULT_OK;

        #if CANSTACK_HAS_HW_FILTERS
        case CAN_STACK_CMD_CONFIGURE_RX_FILTER: {
            const canstack_mb_filter_cmd_t* filter_cmd =
                (const canstack_mb_filter_cmd_t*)arg;

            if (!filter_cmd || !filter_cmd->filter) {
                return CAN_RESULT_INVALID_ARG;
            }

            return psoc5_configure_rx_filter(hw_ctx, filter_cmd->mb_id, filter_cmd->filter);
        }
        #endif

        default:
            return CAN_RESULT_INVALID_CMD;
    }
}

//========================================================
//      Platform Driver Initialization
//========================================================

void canstack_psoc5_bind(canstack_driver drv)
{
    CANSTACK_ASSERT(g_drv == NULL);
    g_drv = drv;
}

canstack_driver canstack_psoc5_get_bound_driver(void)
{
    return g_drv;
}

canstack_result_t canstack_psoc5_create_driver(
    canstack_platform_driver_t* platform_driver)
{
    if (!platform_driver) {
        return CAN_RESULT_INVALID_ARG;
    }

    /* Set up vtable */
    platform_driver->ops.run_cmd = psoc5_run_command;
    platform_driver->ops.tx_message = psoc5_tx_message;
    platform_driver->ops.rx_message = psoc5_rx_message;

    /* Set up hardware context */
    platform_driver->hw_ctx = (void*)&g_psoc5_ctx;

    return CAN_RESULT_OK;
}

//========================================================
//      ISR RX Functions
//========================================================

#if !defined(CANSTACK_EXCLUDE_FULL_RX_MB)
void canstack_psoc5_isr_rx_mailbox(canstack_driver drv, canstack_mb_id_t mb)
{
    canstack_message_t msg;
    if (drv->platform->ops.rx_message(drv->platform->hw_ctx, mb, &msg) == CAN_RESULT_OK) {
        // push into drv->rxq
    } else {
        // optional: count errors
    }
}
#endif

#if RX_ENABLE(CAN_COMPONENT_NAME, 0)

    /**
     * @brief RX interrupt receive function for mailbox 0
     */
    PSOC5_RX_CALLBACK(CAN_COMPONENT_NAME, 0)

#endif

#if RX_ENABLE(CAN_COMPONENT_NAME, 1)

    /**
     * @brief RX interrupt receive function for mailbox 1
     */
    PSOC5_RX_CALLBACK(CAN_COMPONENT_NAME, 1)

#endif

#if RX_ENABLE(CAN_COMPONENT_NAME, 2)

    /**
     * @brief RX interrupt receive function for mailbox 2
     */
    PSOC5_RX_CALLBACK(CAN_COMPONENT_NAME, 2)

#endif

#if RX_ENABLE(CAN_COMPONENT_NAME, 3)

    /**
     * @brief RX interrupt receive function for mailbox 3
     */
    PSOC5_RX_CALLBACK(CAN_COMPONENT_NAME, 3)

#endif

#if RX_ENABLE(CAN_COMPONENT_NAME, 4)

    /**
     * @brief RX interrupt receive function for mailbox 4
     */
    PSOC5_RX_CALLBACK(CAN_COMPONENT_NAME, 4)

#endif

#if RX_ENABLE(CAN_COMPONENT_NAME, 5)

    /**
     * @brief RX interrupt receive function for mailbox 5
     */
    PSOC5_RX_CALLBACK(CAN_COMPONENT_NAME, 5)

#endif

#if RX_ENABLE(CAN_COMPONENT_NAME, 6)

    /**
     * @brief RX interrupt receive function for mailbox 6
     */
    PSOC5_RX_CALLBACK(CAN_COMPONENT_NAME, 6)

#endif

#if RX_ENABLE(CAN_COMPONENT_NAME, 7)

    /**
     * @brief RX interrupt receive function for mailbox 7
     */
    PSOC5_RX_CALLBACK(CAN_COMPONENT_NAME, 7)

#endif

#if RX_ENABLE(CAN_COMPONENT_NAME, 8)

    /**
     * @brief RX interrupt receive function for mailbox 8
     */
    PSOC5_RX_CALLBACK(CAN_COMPONENT_NAME, 8)

#endif

#if RX_ENABLE(CAN_COMPONENT_NAME, 9)

    /**
     * @brief RX interrupt receive function for mailbox 9
     */
    PSOC5_RX_CALLBACK(CAN_COMPONENT_NAME, 9)

#endif

#if RX_ENABLE(CAN_COMPONENT_NAME, 10)

    /**
     * @brief RX interrupt receive function for mailbox 10
     */
    PSOC5_RX_CALLBACK(CAN_COMPONENT_NAME, 10)

#endif

#if RX_ENABLE(CAN_COMPONENT_NAME, 11)

    /**
     * @brief RX interrupt receive function for mailbox 11
     */
    PSOC5_RX_CALLBACK(CAN_COMPONENT_NAME, 11)

#endif

#if RX_ENABLE(CAN_COMPONENT_NAME, 12)

    /**
     * @brief RX interrupt receive function for mailbox 12
     */
    PSOC5_RX_CALLBACK(CAN_COMPONENT_NAME, 12)

#endif

#if RX_ENABLE(CAN_COMPONENT_NAME, 13)

    /**
     * @brief RX interrupt receive function for mailbox 13
     */
    PSOC5_RX_CALLBACK(CAN_COMPONENT_NAME, 13)

#endif

#if RX_ENABLE(CAN_COMPONENT_NAME, 14)

    /**
     * @brief RX interrupt receive function for mailbox 14
     */
    PSOC5_RX_CALLBACK(CAN_COMPONENT_NAME, 14)

#endif

#if RX_ENABLE(CAN_COMPONENT_NAME, 15)

    /**
     * @brief RX interrupt receive function for mailbox 15
     */
    PSOC5_RX_CALLBACK(CAN_COMPONENT_NAME, 15)

#endif

//========================================================
//      End of File
//========================================================

#endif /* CAN_STACK_PLATFORM_PSOC5 */