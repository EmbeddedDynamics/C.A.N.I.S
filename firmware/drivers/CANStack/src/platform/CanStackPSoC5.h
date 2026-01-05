/***************************************************************************//**
* \file CanStackPSoC5.h
* \version 1.0.0
*
* \brief
* PSoC5 platform-specific implementation for CanStack library.
*
* \details
* Provides PSoC5 CAN driver initialization and helper utilities for mailbox
* configuration. This header is internal to CanStack and not part of the
* public API.
*
********************************************************************************
* \copyright
* Copyright 2025, Embedded Dynamics. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef CAN_STACK_PSOC5_H
#define CAN_STACK_PSOC5_H

#if !defined(CANSTACK_PLATFORM_PSOC5)
    #error "CanStack: PSoC5 platform not detected. Include this header only when building for PSoC5."
#endif

//========================================================
//      Standard Includes
//========================================================

#include <stdbool.h>
#include <stdint.h>

#include "CanStackInternal.h"
#if defined(__has_include)
    #if __has_include("cyapicallbacks.h")
        #include "cyapicallbacks.h"
    #endif
#endif

//========================================================
//      PSoC5 Platform Context
//========================================================

/**
 * @brief PSoC5-specific driver context
 * 
 * @details Stores platform-specific state like mailbox masks
 */
typedef struct {
    uint16_t rx_enabled_mask;  /**< Bitmask of enabled RX mailboxes (0-15) */
    uint8_t tx_enabled_mask;   /**< Bitmask of enabled TX mailboxes (0-7) */
} canstack_psoc5_context_t;

//========================================================
//      Full Mailbox Enabled Masks
//========================================================

/**
 * @brief RX mailbox enabled bitmask
 * 
 * @details Each bit represents one mailbox (0-15).
 * Bit set = mailbox enabled in PSoC Creator.
 */
#define CANSTACK_PSOC5_RX_ENABLED_MASK ( \
    (RX_ENABLE(CAN_COMPONENT_NAME, 0)  ? (1u << 0)  : 0) | \
    (RX_ENABLE(CAN_COMPONENT_NAME, 1)  ? (1u << 1)  : 0) | \
    (RX_ENABLE(CAN_COMPONENT_NAME, 2)  ? (1u << 2)  : 0) | \
    (RX_ENABLE(CAN_COMPONENT_NAME, 3)  ? (1u << 3)  : 0) | \
    (RX_ENABLE(CAN_COMPONENT_NAME, 4)  ? (1u << 4)  : 0) | \
    (RX_ENABLE(CAN_COMPONENT_NAME, 5)  ? (1u << 5)  : 0) | \
    (RX_ENABLE(CAN_COMPONENT_NAME, 6)  ? (1u << 6)  : 0) | \
    (RX_ENABLE(CAN_COMPONENT_NAME, 7)  ? (1u << 7)  : 0) | \
    (RX_ENABLE(CAN_COMPONENT_NAME, 8)  ? (1u << 8)  : 0) | \
    (RX_ENABLE(CAN_COMPONENT_NAME, 9)  ? (1u << 9)  : 0) | \
    (RX_ENABLE(CAN_COMPONENT_NAME, 10) ? (1u << 10) : 0) | \
    (RX_ENABLE(CAN_COMPONENT_NAME, 11) ? (1u << 11) : 0) | \
    (RX_ENABLE(CAN_COMPONENT_NAME, 12) ? (1u << 12) : 0) | \
    (RX_ENABLE(CAN_COMPONENT_NAME, 13) ? (1u << 13) : 0) | \
    (RX_ENABLE(CAN_COMPONENT_NAME, 14) ? (1u << 14) : 0) | \
    (RX_ENABLE(CAN_COMPONENT_NAME, 15) ? (1u << 15) : 0) \
)

/**
 * @brief TX mailbox enabled bitmask
 * 
 * @details Each bit represents one mailbox (0-7).
 * Bit set = mailbox enabled in PSoC Creator.
 */
#define CANSTACK_PSOC5_TX_ENABLED_MASK ( \
    (TX_ENABLE(CAN_COMPONENT_NAME, 0) ? (1u << 0) : 0) | \
    (TX_ENABLE(CAN_COMPONENT_NAME, 1) ? (1u << 1) : 0) | \
    (TX_ENABLE(CAN_COMPONENT_NAME, 2) ? (1u << 2) : 0) | \
    (TX_ENABLE(CAN_COMPONENT_NAME, 3) ? (1u << 3) : 0) | \
    (TX_ENABLE(CAN_COMPONENT_NAME, 4) ? (1u << 4) : 0) | \
    (TX_ENABLE(CAN_COMPONENT_NAME, 5) ? (1u << 5) : 0) | \
    (TX_ENABLE(CAN_COMPONENT_NAME, 6) ? (1u << 6) : 0) | \
    (TX_ENABLE(CAN_COMPONENT_NAME, 7) ? (1u << 7) : 0) \
)

//========================================================
//      PSoC5 Helper Functions
//========================================================

#define PSOC5_RX_CALLBACK_(prefix, mbx)                             \
    void prefix##_ReceiveMsg_##mbx##_Callback(void)                  \
    {                                                                \
        canstack_driver drv = canstack_psoc5_get_bound_driver(); \
        if (drv != NULL) {                                           \
            canstack_psoc5_isr_rx_mailbox(drv, (canstack_mb_id_t)(mbx)); \
        }                                                            \
    }

#define PSOC5_RX_CALLBACK(prefix, mbx) PSOC5_RX_CALLBACK_(prefix, mbx) 

#define CANSTACK_PSOC5_CALL_(component, func, ...) \
    CANSTACK_CAT3(component, _, func)(__VA_ARGS__)

#define CANSTACK_PSOC5_CALL(func, ...) \
    CANSTACK_PSOC5_CALL_(CAN_COMPONENT_NAME, func, __VA_ARGS__)

//========================================================
//      PSoC5 Driver methods
//========================================================

#if !defined(CANSTACK_EXCLUDE_FULL_RX_MB) 
void canstack_psoc5_isr_rx_mailbox(canstack_driver drv, canstack_mb_id_t mb); 
#endif 

/**
 * @brief Bind the CanStack driver to the PSoC5 platform
 * 
 * @param[in] drv - CanStack driver handle
 * 
 * @return void
 */
void canstack_psoc5_bind(canstack_driver drv); 

/**
 * @brief Return the bound CanStack driver
 * 
 * @return canstack_driver
 */
canstack_driver canstack_psoc5_get_bound_driver(void);

/**
 * @brief Create and initialize PSoC5 platform driver
 * 
 * @param[out] platform_driver - Platform driver structure to initialize
 * 
 * @return canstack_result_t 
 *                  CAN_STACK_RESULT_OK: on success
 * 
 * @internal
 * Called by canstack_create_driver() to set up PSoC5-specific vtable
 */
canstack_result_t canstack_psoc5_create_driver(
    canstack_platform_driver_t* platform_driver);

//========================================================
//      End of File
//========================================================

#endif /* !CAN_STACK_PSOC5_H */