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

#if !defined(CAN_STACK_PLATFORM_PSOC5)
    #error "CanStack: PSoC5 platform not detected. Include this header only when building for PSoC5."
#endif

//========================================================
//      Standard Includes
//========================================================

#include <stdbool.h>
#include <stdint.h>

#include "CanStackInternal.h"

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
#define CAN_STACK_PSOC5_RX_ENABLED_MASK ( \
    (CAN_RX0_FUNC_ENABLE  ? (1u << 0)  : 0) | \
    (CAN_RX1_FUNC_ENABLE  ? (1u << 1)  : 0) | \
    (CAN_RX2_FUNC_ENABLE  ? (1u << 2)  : 0) | \
    (CAN_RX3_FUNC_ENABLE  ? (1u << 3)  : 0) | \
    (CAN_RX4_FUNC_ENABLE  ? (1u << 4)  : 0) | \
    (CAN_RX5_FUNC_ENABLE  ? (1u << 5)  : 0) | \
    (CAN_RX6_FUNC_ENABLE  ? (1u << 6)  : 0) | \
    (CAN_RX7_FUNC_ENABLE  ? (1u << 7)  : 0) | \
    (CAN_RX8_FUNC_ENABLE  ? (1u << 8)  : 0) | \
    (CAN_RX9_FUNC_ENABLE  ? (1u << 9)  : 0) | \
    (CAN_RX10_FUNC_ENABLE ? (1u << 10) : 0) | \
    (CAN_RX11_FUNC_ENABLE ? (1u << 11) : 0) | \
    (CAN_RX12_FUNC_ENABLE ? (1u << 12) : 0) | \
    (CAN_RX13_FUNC_ENABLE ? (1u << 13) : 0) | \
    (CAN_RX14_FUNC_ENABLE ? (1u << 14) : 0) | \
    (CAN_RX15_FUNC_ENABLE ? (1u << 15) : 0) \
)

/**
 * @brief TX mailbox enabled bitmask
 * 
 * @details Each bit represents one mailbox (0-7).
 * Bit set = mailbox enabled in PSoC Creator.
 */
#define CAN_STACK_PSOC5_TX_ENABLED_MASK ( \
    (CAN_TX0_FUNC_ENABLE ? (1u << 0) : 0) | \
    (CAN_TX1_FUNC_ENABLE ? (1u << 1) : 0) | \
    (CAN_TX2_FUNC_ENABLE ? (1u << 2) : 0) | \
    (CAN_TX3_FUNC_ENABLE ? (1u << 3) : 0) | \
    (CAN_TX4_FUNC_ENABLE ? (1u << 4) : 0) | \
    (CAN_TX5_FUNC_ENABLE ? (1u << 5) : 0) | \
    (CAN_TX6_FUNC_ENABLE ? (1u << 6) : 0) | \
    (CAN_TX7_FUNC_ENABLE ? (1u << 7) : 0) \
)

//========================================================
//      PSoC5 Helper Functions
//========================================================

/**
 * @brief Check if an RX mailbox is enabled
 * 
 * @param[in] mb - Mailbox ID (0-15)
 * 
 * @return true if mailbox is enabled, false otherwise
 * 
 * @note Validates mailbox range (0-15)
 */
static inline bool canstack_psoc5_is_rx_enabled(uint8_t mb)
{
    return (mb < 16) && (CAN_STACK_PSOC5_RX_ENABLED_MASK & (1u << mb));
}

/**
 * @brief Check if a TX mailbox is enabled
 * 
 * @param[in] mb - Mailbox ID (0-7)
 * 
 * @return true if mailbox is enabled, false otherwise
 * 
 * @note Validates mailbox range (0-7)
 */
static inline bool canstack_psoc5_is_tx_enabled(uint8_t mb)
{
    return (mb < 8) && (CAN_STACK_PSOC5_TX_ENABLED_MASK & (1u << mb));
}

/**
 * @brief Count enabled RX mailboxes
 * 
 * @return Number of enabled RX mailboxes (0-16)
 */
static inline uint8_t canstack_psoc5_count_rx_enabled(void)
{
    uint16_t mask = CAN_STACK_PSOC5_RX_ENABLED_MASK;
    uint8_t count = 0x00;
    while (mask) {
        count += mask & 0x01;
        mask >>= 0x01;
    }
    return count;
}

/**
 * @brief Count enabled TX mailboxes
 * 
 * @return Number of enabled TX mailboxes (0-8)
 */
static inline uint8_t canstack_psoc5_count_tx_enabled(void)
{
    uint8_t mask = CAN_STACK_PSOC5_TX_ENABLED_MASK;
    uint8_t count = 0x00;
    while (mask) {
        count += mask & 0x01;
        mask >>= 0x01;
    }
    return count;
}

//========================================================
//      PSoC5 Driver methods
//========================================================

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