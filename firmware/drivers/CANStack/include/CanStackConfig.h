/***************************************************************************//**
* \file CANSimple_Config.h
* \version 1.0.0
*
* \brief
* Public header for CANSimple protocol.
*
********************************************************************************
* \copyright
* Copyright 2025, Embedded Dynamics. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

// ========================================================
// Include guard
// ========================================================

#ifndef CAN_STACK_CONFIG_H
#define CAN_STACK_CONFIG_H

// ========================================================
// Standard Includes
// ========================================================

/* Standard headers */
#include <stdbool.h>
#include <stdint.h>

/* Project headers */
#include "CanStackResult.h"

#define CAN_STACK_MAX_PAYLOAD_SIZE 8u

#define CAN_STACK_MAILBOX_ID_ANY 0xFF

#if defined(PLATFORM_PSOC5)

    #define CAN_STACK_PLATFORM_PSOC5
    #include "CAN.h"

    #define CAN_STACK_TOTAL_RX_MAILBOXES CAN_NUMBER_OF_RX_MAILBOXES
    #define CAN_STACK_TOTAL_TX_MAILBOXES CAN_NUMBER_OF_TX_MAILBOXES

    #define CAN_STACK_FULL_RX_ENABLE_COUNT ( \
        CAN_RX0_FUNC_ENABLE     + \
        CAN_RX1_FUNC_ENABLE     + \
        CAN_RX2_FUNC_ENABLE     + \
        CAN_RX3_FUNC_ENABLE     + \
        CAN_RX4_FUNC_ENABLE     + \
        CAN_RX5_FUNC_ENABLE     + \
        CAN_RX6_FUNC_ENABLE     + \
        CAN_RX7_FUNC_ENABLE     + \
        CAN_RX8_FUNC_ENABLE     + \
        CAN_RX9_FUNC_ENABLE     + \
        CAN_RX10_FUNC_ENABLE    + \
        CAN_RX11_FUNC_ENABLE    + \
        CAN_RX12_FUNC_ENABLE    + \
        CAN_RX13_FUNC_ENABLE    + \
        CAN_RX14_FUNC_ENABLE    + \
        CAN_RX15_FUNC_ENABLE    \
    )

    #define CAN_STACK_FULL_TX_ENABLE_COUNT ( \
        CAN_TX0_FUNC_ENABLE     + \
        CAN_TX1_FUNC_ENABLE     + \
        CAN_TX2_FUNC_ENABLE     + \
        CAN_TX3_FUNC_ENABLE     + \
        CAN_TX4_FUNC_ENABLE     + \
        CAN_TX5_FUNC_ENABLE     + \
        CAN_TX6_FUNC_ENABLE     + \
        CAN_TX7_FUNC_ENABLE     \
    )

    // Validate at least one mailbox is configured
    #if (CAN_STACK_FULL_RX_ENABLE_COUNT < 1)
        #define CAN_STACK_EXCLUDE_FULL_RX_MB
        #pragma message("No Full RX mailboxes configured in PSoC Creator!") 
    #endif

    #if (CAN_STACK_FULL_TX_ENABLE_COUNT < 1)
        #define CAN_STACK_EXCLUDE_FULL_TX_MB
        #pragma message("No Full TX mailboxes configured in PSoC Creator!") 
    #endif

    // Hardware filtering only for RX mailboxes (TX doesn't support filtering)
    #if (CAN_STACK_FULL_RX_ENABLE_COUNT > 0) && \
        defined(CAN_STACK_ENABLE_HW_FILTERING)
        #define CAN_STACK_HAS_HW_FILTERS  1
    #else
        #define CAN_STACK_HAS_HW_FILTERS  0
    #endif

#else 
    #define CAN_STACK_EXCLUDE_FULL_RX_MB
    #define CAN_STACK_EXCLUDE_FULL_TX_MB

    #define CAN_STACK_TOTAL_RX_MAILBOXES -1
    #define CAN_STACK_TOTAL_TX_MAILBOXES -1

    #error "CanStack: No supported platform detected"
#endif



// ========================================================

#endif // !CAN_STACK_CONFIG_H

// ========================================================

/* [] END OF FILE */
