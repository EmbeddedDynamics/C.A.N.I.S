/***************************************************************************//**
* \file ODriveCAN.h
* \version 1.0.0
*
* \brief
* Private header for CANSimple protocol used by ODrive.
*
********************************************************************************
* \copyright
* Copyright 2025, Embedded Dynamics. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#ifndef ODRIVE_INTERNAL_CAN_H
#define ODRIVE_INTERNAL_CAN_H

// ========================================================
// Includes
// ========================================================

#include <stdint.h>
#include <stdbool.h>

#include "ODriveResult.h"
#include "ODriveCore.h"

// ========================================================
// Platform-specific macros
// ========================================================

#if defined(CY_PSOC5)
    #define ODRIVE_PLATFORM_PSOC5

    #if defined(CY_CAN_CAN_H)
        #define ODRIVE_CAN_AVAILABLE

        #include "CAN.h"

        #pragma Message("ODrive: Using PSoC5 CAN peripheral for ODrive CANSimple communication.")
    #endif // !defined(CY_CAN_CAN_H)
#endif // !defined(CY_PSOC5)

// ========================================================
// CAN Simple types
// ========================================================

typedef uint8_t ODriveBufferId;
typedef uint8_t ODrivePayloadLen;
typedef uint8_t* ODrivePayload;

typedef uint16_t ODriveCANId;

// ========================================================
// CAN Simple structs
// ========================================================

typedef struct {
    ODriveCANId id;
    ODrivePayloadLen dlc;
    ODrivePayload data;
} ODriveCANFrame;

/**
 * @brief ODrive CAN command buffer
 * 
 * Used for transmitting and receiving CAN frames.
 */
typedef struct {
    bool inUse;
    ODriveBufferId id;
    ODrivePayloadLen len;
    ODrivePayload buffer;
} ODriveTxBuffer;

// ========================================================
// CAN Simple function prototypes
// ========================================================

/**
 * @brief Fetches the transmit command buffer for a given CAN command.
 * 
 * @details This function is used when the CAN bus supports multiple
 * mailboxes for transmitting CAN frames.
 * 
 * @param[in] cmdId - The ODrive CAN command ID for which to fetch the transmit buffer.
 * @param[out] id - ODriveBufferId assigned to the transmit buffer.
 */
ODriveResult ODriveCAN_AcquireTxBuffer(ODriveCANDriver driver, 
                                       ODriveTxBuffer *buffer);

/**
 * @brief Release a command buffer for transmission.
 * 
 * @details This function is called when a CMD buffer is populated with data.
 * It signals the CAN driver to transmit the payload to the designated bus.
 * 
 * @param[in] driver - ODriveCANDriver handle
 * 
 */
ODriveResult ODriveCAN_ReleaseTxBuffer(ODriveCANDriver driver,
                                        ODriveTxBuffer *buffer);

/**
 * @brief Checks if a CAN receive buffer is available for a given command ID.
 */
uint8_t ODriveCAN_IsAvailableCMD(ODriveCANDriver driver);

/**
 * @brief Fetches a received CAN frame for processing.
 * 
 * @details This function retrieves a received CAN frame from a internal
 * mailbox.
 */
// ODriveResult ODriveCAN_ReceiveCMDFrame(ODriveCANDriver driver,
//                                        ODriveCanCommandId cmdId,
//                                        ODriveCANFrame* frame);

ODriveResult ODriveCAN_ReceiveFrame(ODriveCANDriver driver,
                                    ODriveCANFrame* frame);

#endif // !ODRIVE_INTERNAL_CAN_H

/* [] END OF FILE */
