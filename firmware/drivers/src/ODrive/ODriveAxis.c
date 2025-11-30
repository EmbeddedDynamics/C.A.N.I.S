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

// ========================================================
// Constants
// ========================================================

#define ODRIVE_CAN_ID_MAX           (63u)
#define ODRIVE_CAN_TIMEOUT_MS       (100u)

#define ODRIVE_CAN_ID_VALID(n)      (n <= ODRIVE_CAN_ID_MAX)

// ========================================================
// Private Structs
// ========================================================

struct ODriveAxis_T {
    ODriveDriver driver;    /**< CAN driver handle */
    CanNodeId node_id;      /**< CAN node ID (0-63) */
    bool is_initialized;    /**< Initialization status */
};

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
    
    if (!ODRIVE_CAN_ID_VALID(id))
        return ODRIVE_ERROR_INVALID_CAN_ID;
    
    
    
    return ODRIVE_RESULT_OK;
};
    
// ========================================================

/* [] END OF FILE */
