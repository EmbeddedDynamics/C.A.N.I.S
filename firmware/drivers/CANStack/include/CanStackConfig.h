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

#ifndef CAN_STACK_CONFIG_H
#define CAN_STACK_CONFIG_H

//========================================================
//      Standard Includes
//========================================================

/* Standard headers */
#include <stdbool.h>
#include <stdint.h>

#if CANSTACK_ENABLE_ASSERT
#include <assert.h>
#define CANSTACK_ASSERT(x) assert(x)
#else
#define CANSTACK_ASSERT(x) ((void)0)
#endif

/* Project headers */
#include "CanStackResult.h"

//========================================================
//      PSoC Platform checker
//========================================================

#if defined(__has_include)
    #if __has_include("cyfitter.h")
        #include "cyfitter.h"
        
        #if defined(CYDEV_CHIP_FAMILY_USED)
            #if (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC3)
                #define CANSTACK_PLATFORM_PSOC3
            #elif (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC4)
                #define CANSTACK_PLATFORM_PSOC4
            #elif (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC5)
                #define CANSTACK_PLATFORM_PSOC5
            #elif (CYDEV_CHIP_FAMILY_USED == CYDEV_CHIP_FAMILY_PSOC6)
                #define CANSTACK_PLATFORM_PSOC6
            #endif
        #endif
    #endif
#endif

//========================================================
//      CanStack Macro Helpers
//========================================================

#define CANSTACK_STR_(x) #x
#define CANSTACK_STR(x)  CANSTACK_STR_(x)

#define CANSTACK_CAT2_(a,b) a##b
#define CANSTACK_CAT2(a,b)  CANSTACK_CAT2_(a,b)

#define CANSTACK_CAT3_(a,b,c) a##b##c
#define CANSTACK_CAT3(a,b,c)  CANSTACK_CAT3_(a,b,c)

#define CANSTACK_BIT_IS_SET(mask, bit) \
    (((mask) & (1u << (bit))) != 0u)

//========================================================
//      CanStack Constants
//========================================================

#define CANSTACK_RX_QUEUE_SIZE  32u
#define CANSTACK_RX_QUEUE_MASK (CANSTACK_RX_QUEUE_SIZE - 1u)

#define CANSTACK_MAX_PAYLOAD_SIZE 8u

#define CANSTACK_MAILBOX_ID_ANY 0xFF

//========================================================
//      PSoC5 Configuratioon
//========================================================

#if defined(CANSTACK_PLATFORM_PSOC5)
    #include "cyapicallbacks.h"
    
    #ifndef CAN_COMPONENT_NAME
    #error "CAN_COMPONENT_NAME must be defined (e.g. CAN, CAN0, CAN1)"
    #endif

    #define CANSTACK_PSOC5_CONST(name) \
        CANSTACK_CAT3(CAN_COMPONENT_NAME, _, name)

    #define CANSTACK_PSOC5_TYPE(name) \
        CANSTACK_CAT3(CAN_COMPONENT_NAME, _, name)

    #define CANSTACK_PSOC5_MACRO_CALL_(component, macro, ...) \
        CANSTACK_CAT3(component, _, macro)(__VA_ARGS__)

    #define CANSTACK_PSOC5_MACRO_CALL(macro, ...) \
        CANSTACK_PSOC5_MACRO_CALL_(CAN_COMPONENT_NAME, macro, __VA_ARGS__)

    #define CANSTACK_PSOC5_CALL_(component, func, ...) \
        CANSTACK_CAT3(component, _, func)(__VA_ARGS__)

    #define CANSTACK_PSOC5_CALL(func, ...) \
        CANSTACK_PSOC5_CALL_(CAN_COMPONENT_NAME, func, __VA_ARGS__)


    /* Include the CAN component header */
    #include CANSTACK_STR(CAN_COMPONENT_NAME.h)

    #define RX_ENABLE_(prefix, mbx) prefix##_RX##mbx##_FUNC_ENABLE
    #define RX_ENABLE(prefix, mbx)  RX_ENABLE_(prefix, mbx)

    #define TX_ENABLE_(prefix, mbx) prefix##_TX##mbx##_FUNC_ENABLE
    #define TX_ENABLE(prefix, mbx)  TX_ENABLE_(prefix, mbx)

    #define CANSTACK_TOTAL_RX_MAILBOXES CANSTACK_PSOC5_CONST(NUMBER_OF_RX_MAILBOXES)
    #define CANSTACK_TOTAL_TX_MAILBOXES CANSTACK_PSOC5_CONST(NUMBER_OF_TX_MAILBOXES)

    #define CANSTACK_DISABLE_INTERRUPT() (CyGlobalIntDisable)
    #define CANSTACK_ENABLE_INTERRUPT() (CyGlobalIntEnable)

    #define CANSTACK_FULL_RX_ENABLE_COUNT ( \
        RX_ENABLE(CAN_COMPONENT_NAME, 0)     + \
        RX_ENABLE(CAN_COMPONENT_NAME, 1)     + \
        RX_ENABLE(CAN_COMPONENT_NAME, 2)     + \
        RX_ENABLE(CAN_COMPONENT_NAME, 3)     + \
        RX_ENABLE(CAN_COMPONENT_NAME, 4)     + \
        RX_ENABLE(CAN_COMPONENT_NAME, 5)     + \
        RX_ENABLE(CAN_COMPONENT_NAME, 6)     + \
        RX_ENABLE(CAN_COMPONENT_NAME, 7)     + \
        RX_ENABLE(CAN_COMPONENT_NAME, 8)     + \
        RX_ENABLE(CAN_COMPONENT_NAME, 9)     + \
        RX_ENABLE(CAN_COMPONENT_NAME, 10)    + \
        RX_ENABLE(CAN_COMPONENT_NAME, 11)    + \
        RX_ENABLE(CAN_COMPONENT_NAME, 12)    + \
        RX_ENABLE(CAN_COMPONENT_NAME, 13)    + \
        RX_ENABLE(CAN_COMPONENT_NAME, 14)    + \
        RX_ENABLE(CAN_COMPONENT_NAME, 15)    \
    )

    #define CANSTACK_FULL_TX_ENABLE_COUNT ( \
        TX_ENABLE(CAN_COMPONENT_NAME, 0)     + \
        TX_ENABLE(CAN_COMPONENT_NAME, 1)     + \
        TX_ENABLE(CAN_COMPONENT_NAME, 2)     + \
        TX_ENABLE(CAN_COMPONENT_NAME, 3)     + \
        TX_ENABLE(CAN_COMPONENT_NAME, 4)     + \
        TX_ENABLE(CAN_COMPONENT_NAME, 5)     + \
        TX_ENABLE(CAN_COMPONENT_NAME, 6)     + \
        TX_ENABLE(CAN_COMPONENT_NAME, 7)     \
    )

    // Validate at least one mailbox is configured
    #if (CANSTACK_FULL_RX_ENABLE_COUNT < 1)
        #define CANSTACK_EXCLUDE_FULL_RX_MB
        #pragma message("No Full RX mailboxes configured in PSoC Creator!") 
    #endif

    #if (CANSTACK_FULL_TX_ENABLE_COUNT < 1)
        #define CANSTACK_EXCLUDE_FULL_TX_MB
        #pragma message("No Full TX mailboxes configured in PSoC Creator!") 
    #endif

    // Hardware filtering only for RX mailboxes (TX doesn't support filtering)
    #if (CANSTACK_FULL_RX_ENABLE_COUNT > 0) && defined(CANSTACK_ENABLE_HW_FILTERING)
        #define CANSTACK_HAS_HW_FILTERS  1
    #else
        #define CANSTACK_HAS_HW_FILTERS  0
    #endif
#else 
    #error "CanStack didn't detect any supported platform"
#endif

// ========================================================

#endif // !CAN_STACK_CONFIG_H

// ========================================================

/* [] END OF FILE */
