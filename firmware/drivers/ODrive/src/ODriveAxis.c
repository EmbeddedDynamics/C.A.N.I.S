/***************************************************************************//**
* \file ODriveAxis.c
* \version 1.0.0
*
*  \brief
*  Private header for the ODrive axis definitions
*
********************************************************************************
* \copyright
* Copyright 2025, Embedded Dynamics. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

// ========================================================
// Includes
// ========================================================

/* Internal headers */
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
    
/* Project headers */
#include "ODriveAxis.h"
#include "ODriveCAN.h"

#include "ODriveInternal.h"

// ========================================================
// Private Structs
// ========================================================

// ========================================================
// Public helpet functions
// ========================================================

static inline ODriveResult transmitCANFrame(ODriveAxis axis,
                                            ODriveCANFrame* frame)
{
    if (!axis || !frame)
        return ODRIVE_ERROR_INVALID_PARAMETER;

    ODriveDriver driver = ODriveAxis_GetDriver(axis);
    if (!driver)
        return ODRIVE_ERROR_DEVICE_ERROR;

    driver->CANDriver.transmit(frame);
}

// ========================================================
// Public Methods
// ========================================================


ODriveResult createODriveAxis(ODriveDriver driver, CanNodeId id, ODriveAxis* axis)
{
    /* Parameter checking */
    if (!driver)
        return ODRIVE_ERROR_NULL_POINTER;
    
    if (!axis)
        return ODRIVE_ERROR_NULL_POINTER; 
    
    if (!ODRIVE_CAN_ID_VALID(id) ||  (id < ODRIVE_MAX_AXES))
        return ODRIVE_ERROR_INVALID_CAN_ID;
    
    if (driver->axes[id].is_initialized)
        return ODRIVE_ERROR_INVALID_CAN_ID;
    
    /* Fetch axis from axes table */
    ODriveAxis newAxis = &driver->axes[id];

    /* Populate parameters */
    newAxis->driver = driver;
    newAxis->node_id = id;
    newAxis->is_initialized = true;

    *axis = newAxis;
    return ODRIVE_RESULT_OK;
};

ODriveResult ODriveAxis_Destroy(ODriveAxis axis)
{
    if (!axis)
        return ODRIVE_ERROR_NULL_POINTER; 

    CanNodeId id = ODriveAxis_GetNodeId(axis);
    ODriveDriver driver = ODriveAxis_GetDriver(axis);
    if (!driver)
        return ODRIVE_ERROR_NULL_POINTER;

    if (!driver->axes[id].is_initialized)
        return ODRIVE_ERROR_INVALID_AXIS;

    ODriveAxis axis = &driver->axes[id];
    axis->driver = NULL;
    axis->node_id = 0;
    axis->is_initialized = false;

    return ODRIVE_RESULT_OK;
}


ODriveResult ODriveAxis_SetRequestedState(ODriveAxis axis, ODriveAxisState state)
{
    if (!axis) {
        return ODRIVE_ERROR_NULL_POINTER;
    }

    CanNodeId id = ODriveAxis_GetNodeId(axis);
    if (!ODRIVE_CAN_ID_VALID(id)) {
        return ODRIVE_ERROR_INVALID_CAN_ID; // or whatever fits your ODriveResult
    }

    ODriveCANFrame frame = {0};
    frame.id  = ODRIVE_CAN_ID(id, CAN_CMD_SET_AXIS_STATE);
    frame.dlc = 4;
    frame.rtr = 0;

    // Pack state as little-endian uint32_t into the first 4 bytes
    ODRIVE_CAN_PACK_INT32(frame, 0u, (uint32_t)state);

    return ODriveCAN_SendCommand(&frame);
}

ODriveResult ODriveAxis_GetCurrentState(ODriveAxis axis, ODriveAxisState* state)
{
    
}

ODriveResult ODriveAxis_SetPositionSetpoint(ODriveAxis axis,
                                            float position,
                                            int16_t velocity_ff,
                                            int16_t torque_ff)
{
    if (!axis) {
        return ODRIVE_ERROR_NULL_POINTER;
    }

    CanNodeId id = ODriveAxis_GetNodeId(axis);
    if (!ODRIVE_CAN_ID_VALID(id)) {
        return ODRIVE_ERROR_INVALID_CAN_ID;
    }

    ODriveCANFrame frame = {0};
    frame.id  = ODRIVE_CAN_ID(id, CAN_CMD_SET_INPUT_POS);
    frame.dlc = 8;                                          // 4 + 2 + 2 bytes
    frame.rtr = 0;

    // Pack position (float32) as little-endian
    // PSoC (ARM) is little-endian, so memcpy is fine here.
    memcpy(&frame.data[0], &position, sizeof(float));

    // Pack velocity feed-forward (int16, little-endian)
    ODRIVE_CAN_PACK_INT16(frame, 4u, velocity_ff);
    
    // Pack torque feed-forward (int16, little-endian)
    ODRIVE_CAN_PACK_INT16(frame, 6u, torque_ff);

    return ODriveCAN_SendCommand(&frame);
}

ODriveResult ODriveAxis_SetVelocitySetpoint(ODriveAxis axis,
                                           float velocity,
                                           float torque_ff)
{
    if (!axis) {
        return ODRIVE_ERROR_NULL_POINTER;
    }

    CanNodeId id = ODriveAxis_GetNodeId(axis);
    if (!ODRIVE_CAN_ID_VALID(id)) {
        return ODRIVE_ERROR_INVALID_CAN_ID;
    }

    ODriveCANFrame frame = {0};
    frame.id  = ODRIVE_CAN_ID(id, CAN_CMD_SET_INPUT_POS);
    frame.dlc = 8;  // 4 + 4 bytes
    frame.rtr = 0;

    // Pack velocity (float32) as little-endian
    // PSoC (ARM) is little-endian, so memcpy is fine here.
    memcpy(&frame.data[0], &velocity, sizeof(float));

    // Pack torque_ff (float32, little-endian)
    memcpy(&frame.data[4], &torque_ff, sizeof(float));

    return ODriveCAN_SendCommand(&frame);
}

ODriveResult ODriveAxis_SetTorqueSetpoint(ODriveAxis axis, 
                                          float torque)
{
    if (!axis) {
        return ODRIVE_ERROR_NULL_POINTER;
    }

    CanNodeId id = ODriveAxis_GetNodeId(axis);
    if (!ODRIVE_CAN_ID_VALID(id)) {
        return ODRIVE_ERROR_INVALID_CAN_ID;
    }

    ODriveCANFrame frame = {0};
    frame.id  = ODRIVE_CAN_ID(id, CAN_CMD_SET_INPUT_POS);
    frame.dlc = 4;  // 4 bytes
    frame.rtr = 0;

    // Pack torque (float32) as little-endian
    // PSoC (ARM) is little-endian, so memcpy is fine here.
    memcpy(&frame.data[0], &torque, sizeof(float));

    return ODriveCAN_SendCommand(&frame);
}

ODriveResult ODriveAxis_GetEncoderEstimates(ODriveAxis axis,
                                            uint16_t timeout,
                                            EncoderEstimateFrame* frame)
{

    return ODRIVE_RESULT_OK;
}

ODriveResult ODriveAxis_Reboot(ODriveAxis axis, 
                               ODriveRebootAction action)
{
    if (!axis) {
        return ODRIVE_ERROR_NULL_POINTER;
    }

    CanNodeId id = ODriveAxis_GetNodeId(axis);
    if (!ODRIVE_CAN_ID_VALID(id)) {
        return ODRIVE_ERROR_INVALID_CAN_ID;
    }

    ODriveCANFrame frame = {0};
    frame.id  = ODRIVE_CAN_ID(id, CAN_CMD_REBOOT);
    frame.dlc = 1;
    frame.rtr = 0;

    // Pack actioon (int8, little-endian)
    ODRIVE_CAN_PACK_INT8(frame, 0u, (uint8_t) action);

    return ODriveCAN_SendCommand(&frame);
}
    
// ========================================================

/* [] END OF FILE */
