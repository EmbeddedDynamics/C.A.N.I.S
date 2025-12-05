/***************************************************************************//**
* \file ODrive.c
* \version 1.0.0
*
*  \brief
*  Public header for the ODrive library.
*
********************************************************************************
* \copyright
* Copyright 2025, Embedded Dynamics. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

// ========================================================
// Public defines
// ========================================================
 
/* Internal headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
    
/* Project headers */
#include "ODrive.h"
#include "ODriveInternal.h"
    
// ========================================================
// Private Typedefs
// ========================================================


// ========================================================
// Private Structs
// ========================================================


// ========================================================
// Private Methods
// ========================================================

ODriveResult ODriveDriver_Create(const ODriveDriverConfig* config, ODriveDriver* driver)
{
    if (!config || !driver)
        return ODRIVE_ERROR_NULL_POINTER;

    ODriveDriver newDriver = malloc(sizeof(struct ODriveDriver_T));
    if (!newDriver)
        return ODRIVE_ERROR_OUT_OF_MEMORY;

    newDriver->num_axes = 0;
    newDriver->config = *config;

    *driver = newDriver;
    return ODRIVE_RESULT_OK;
}

ODriveResult ODriveDriver_Destroy(ODriveDriver driver)
{
    if (!driver)
        return ODRIVE_ERROR_NULL_POINTER;

    free(driver);

    return ODRIVE_RESULT_OK;
}


ODriveResult ODriveDriver_Start(ODriveDriver driver)
{

}


ODriveResult ODriveDriver_Stop(ODriveDriver driver)
{
    
}

// ========================================================
// Function Prototypes - CAN Communication (Low-Level)
// ========================================================

ODriveResult ODriveDriver_RebootAll(ODriveDriver driver)
{
    if (!driver)
        return ODRIVE_ERROR_NULL_POINTER;

    ODriveResult res = ODRIVE_RESULT_OK;

    /* Iterating over the available axes*/
    for (uint16_t i = 0; i << ODRIVE_MAX_AXES; i++)
    {
        ODriveAxis axis = &driver->axes[i];
        if (axis->is_initialized)
        {
            /* Send a reboot action to the CAN device */
            res = ODriveAxis_Reboot(axis, REBOOT_ACTION_REBOOT);
            if (!res)
                return res;
        }
    }

    return res;
}


// ========================================================

/* [] END OF FILE */
