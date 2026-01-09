/***************************************************************************//**
* \file ODriveCanStack.h
* \version 1.0.0
*
* \brief
* CanStack abstraction layer for the use of the ODrive library
*
* \details
* Transport-agnostic interface supporting CAN, ABZ, USB, SPI, etc.
* Enables ODrive library to work with any communication backend via vtable
* abstraction. Each transport encodes/decodes ODrive protocol independently.
*
*******************************************************************************/

//========================================================
//      Standard Includes
//========================================================

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "driver/CanStack/ODriveCanStack.h"

#if (ODRIVE_HAVE_CANSTACK)

//========================================================
//      Driver Methods
//========================================================

odrive_result_t odrive_canstack_transmit(odrive_ctx_t ctx,
                                         const odrive_can_message_t* msg)
{
    if (!ctx || !msg)
        return ODRIVE_ERROR_NULL_POINTER;

    canstack_driver driver = (canstack_driver) ctx;

    #if ODRIVE_CAN_FRAME_MATCH

    if (canstack_transmit(driver, (canstack_message_t*) msg) != CAN_RESULT_OK)
    {
        return ODRIVE_ERROR_CAN_TX_FAILED;
    }

    #endif

    return ODRIVE_RESULT_OK;
}

//========================================================
//      Driver Creation
//========================================================

odrive_result_t odrive_create_canstack_backend(const odrive_canstack_config_t *cfg,
                                               odrive_backend* backend)
{
    if (!cfg || !backend)
        return ODRIVE_ERROR_NULL_POINTER;

    if (!cfg->driver)
        return ODRIVE_ERROR_INVALID_PARAM;

    odrive_backend newBackend = (odrive_backend) malloc(sizeof(odrive_backend_t));
    if (!newBackend)
        return ODRIVE_ERROR_NO_MEMORY;

    newBackend->transport = ODRIVE_TRANSPORT_CAN;
    newBackend->ops.can_ops.transmit = odrive_canstack_transmit;

    newBackend->hw_ctx = (odrive_ctx_t) cfg->driver;

    return ODRIVE_RESULT_OK;
}

//========================================================
//      End of File
//========================================================

#endif // !ODRIVE_HAVE_CANSTACK

