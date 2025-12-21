/***************************************************************************//**
* \file ODriveCAN.h
* \version 1.0.0
*
*  \brief
*  Private header for OpenCAN protocol used by ODrive
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

#ifndef ODRIVE_INTERNAL_H
#define ODRIVE_INTERNAL_H

// ========================================================
// Public defines
// ========================================================
 
/* Internal headers */
#include <stdint.h>
#include <stdbool.h>
    
/* Project headers */
#include "ODrive.h"
#include "ODriveAxis.h"
#include "ODriveCore.h"

#include "ODriveCAN.h"

    
// ========================================================
// Private Typedefs
// ========================================================


    
// ========================================================
// Private Structs
// ========================================================

struct ODriveDriver_T {
    struct ODriveAxis_T axes[ODRIVE_MAX_AXES];   // indexed by node_id
    uint16_t num_axes;

    ODriveCANDriver CANDriver;

    ODriveDriverConfig config;
};

struct ODriveAxis_T {
    ODriveDriver driver;    /**< CAN driver handle */
    CanNodeId node_id;      /**< CAN node ID (0-63) */
    bool is_initialized;    /**< Initialization status */
};

typedef struct {
    ODriveAxisError axis_error;
    ODriveAxisState  axis_state;
    ODriveProcedureResult  procedure_result;
    bool     traj_done;
} ODriveHeartbeatFrame;

// ========================================================
// Private Methods
// ========================================================

ODriveResult ODriveDriver_PollCANMessage(Can);


// ========================================================

#endif // !ODRIVE_INTERNAL_H

// ========================================================

/* [] END OF FILE */
