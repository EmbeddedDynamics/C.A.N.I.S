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
#include "ODrive.h"
#include "ODriveAxis.h"
#include "ODriveCAN.h"

#include "ODriveInternal.h"

// ========================================================
// Private Structs
// ========================================================


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
    
    if (!ODRIVE_CAN_ID_VALID(id) ||  (id < ODRIVE_MAX_AXISES))
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
    uint32_t s = (uint32_t)state;
    frame.data[0] = (uint8_t)(s & 0xFF);
    frame.data[1] = (uint8_t)((s >> 8) & 0xFF);
    frame.data[2] = (uint8_t)((s >> 16) & 0xFF);
    frame.data[3] = (uint8_t)((s >> 24) & 0xFF);

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
    if (axis == NULL) {
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
    frame.data[4] = (uint8_t)(velocity_ff & 0xFF);
    frame.data[5] = (uint8_t)((velocity_ff >> 8) & 0xFF);

    // Pack torque feed-forward (int16, little-endian)
    frame.data[6] = (uint8_t)(torque_ff & 0xFF);
    frame.data[7] = (uint8_t)((torque_ff >> 8) & 0xFF);

    return ODriveCAN_SendCommand(&frame);
}
    
// ========================================================

/* [] END OF FILE */
