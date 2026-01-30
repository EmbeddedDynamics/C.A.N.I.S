/***************************************************************************//**
* \file ODriveCanStack.c
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
//      Backend Methods
//========================================================

odrive_result_t odrive_canstack_transmit(odrive_ctx_t ctx,
                                         const odrive_can_message_t* msg)
{
    if (!ctx || !msg)
        return ODRIVE_ERROR_NULL_POINTER;

    canstack_driver driver = (canstack_driver) ctx;

    #if (ODRIVE_CANSTACK_MATCH_FRAME_ALIAS)

    // Directly use the ODrive CAN message and type cast it to a CanStack CAN message
    if (canstack_transmit(driver, (canstack_message_t*) msg) != CAN_RESULT_OK)
    {
        return ODRIVE_ERROR_CAN_TX_FAILED;
    }
    #else

    // Manually convert ODrive CAN message to CanStack CAN message
    canstack_message_t cs_msg = { 0u };
    cs_msg.id  = msg->id;
    cs_msg.dlc = msg->dlc;
    cs_msg.rtr = msg->rtr;

    for (uint8_t i = 0; i < msg->dlc; i++)
    {
        cs_msg.data[i] = msg->data[i];
    }

    if (canstack_transmit(driver, &cs_msg) != CAN_RESULT_OK)
    {
        return ODRIVE_ERROR_CAN_TX_FAILED;
    }

    #endif

    return ODRIVE_RESULT_OK;
}

odrive_result_t odrive_canstack_receive(odrive_ctx_t ctx,
                                        odrive_can_message_t* msg)
{
    if (!ctx || !msg)
        return ODRIVE_ERROR_NULL_POINTER;

    canstack_driver driver = (canstack_driver) ctx;

    #if (ODRIVE_CANSTACK_MATCH_FRAME_ALIAS)

    // Directly use the ODrive CAN message and type cast it to a CanStack CAN message
    if (canstack_get_rx(driver, (canstack_message_t*) msg) != CAN_RESULT_OK)
    {
        return ODRIVE_WARNING_NO_RX_MESSAGES;
    }

    #else

    // Manually convert CanStack CAN message to ODrive CAN message
    canstack_message_t cs_msg = {0};

    if (canstack_get_rx(driver, &cs_msg) != CAN_RESULT_OK)
    {
        return ODRIVE_WARNING_NO_RX_MESSAGES;
    }

    msg->id  = cs_msg.id;
    msg->dlc = cs_msg.dlc;
    msg->rtr = cs_msg.rtr;

    for (uint8_t i = 0; i < cs_msg.dlc; i++)
    {
        msg->data[i] = cs_msg.data[i];
    }

    #endif

    return ODRIVE_RESULT_OK;
}

uint8_t odrive_canstack_get_rx_count(odrive_ctx_t ctx)
{
    if (!ctx)
        return 0;
    
    canstack_driver driver = (canstack_driver) ctx;

    return canstack_get_rx_count(driver);
}

const char* odrive_canstack_backend_name(void* ODRIVE_UNUSED(ctx))
{
    return "ODrive CanStack " CANSTACK_VERSION_STRING;
}

//========================================================
//      Backend Creation
//========================================================

odrive_result_t odrive_create_canstack_backend(const odrive_canstack_config_t *cfg,
                                               odrive_backend* backend)
{
    /* Validate parameters */
    if (!cfg || !backend)
        return ODRIVE_ERROR_NULL_POINTER;

    /* Validate config */
    if (!cfg->driver || !cfg->gpio)
        return ODRIVE_ERROR_INVALID_PARAM;

    /* Allocate memory for ODrive communication backend*/
    odrive_backend newBackend = (odrive_backend) malloc(sizeof(odrive_backend_t));
    if (!newBackend)
        return ODRIVE_ERROR_NO_MEMORY;

    /* Initialize the backend */
    newBackend->transport = ODRIVE_TRANSPORT_CAN;
    newBackend->ops.can_ops.transmit = odrive_canstack_transmit;
    newBackend->ops.can_ops.receive = odrive_canstack_receive;
    newBackend->ops.can_ops.get_rx_count = odrive_canstack_get_rx_count;
    newBackend->get_name = odrive_canstack_backend_name;
    newBackend->gpio = *cfg->gpio;

    newBackend->hw_ctx = (odrive_ctx_t) cfg->driver;

    *backend = newBackend;

    return ODRIVE_RESULT_OK;
}

//========================================================
//      End of File
//========================================================

#endif // !ODRIVE_HAVE_CANSTACK

