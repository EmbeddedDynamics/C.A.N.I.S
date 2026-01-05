/***************************************************************************//**
* \file ODrive.c
* \version 1.0.0
*
* \brief
* Private source file for the ODrive library
*
********************************************************************************
* \copyright
* Copyright 2025, Embedded Dynamics. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

//========================================================
//      Standard Includes
//========================================================
 
/* Internal headers */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
    
/* Project headers */
#include "ODriveInternal.h"


//========================================================
//      ODrive Axis Structure
//========================================================

odrive_result_t odrive_create_driver(const odrive_driver_config_t* config,
                                     odrive_driver* driver)
{
    if (!config || !driver)
        return ODRIVE_ERROR_NULL_POINTER;

    odrive_driver new_driver = malloc(sizeof(struct odrive_driver_T));
    if (!new_driver)
        return ODRIVE_ERROR_NO_MEMORY;

    new_driver->com = config->com;

    new_driver->state = DRIVER_STATE_UNINITIALIZED;
    new_driver->num_axes = 0;

    /* Initialize node map (all slots unused) */
    memset(new_driver->node_map, 0xFF, sizeof(new_driver->node_map));

    *driver = new_driver;

    return ODRIVE_RESULT_OK;
}

// odrive_result_t odrive_axis_request_state(odrive_axis axis, 
//                                           odrive_axis_state_t state)
// {
//     if (!axis)
//         return ODRIVE_ERROR_NULL_POINTER;

//     odrive_driver driver = axis->driver;

//     odrive_message_t message = {0};
//     switch (driver->com.transport)
//     {
//     case ODRIVE_TRANSPORT_CAN:
//         message.msg_id = ODRIVE_CAN_ID(axis->node_id, CAN_CMD_SET_AXIS_STATE);
//         ODRIVE_PACK_UINT32(message, state);

//         break;
//     case ODRIVE_TRANSPORT_USB:
//     case ODRIVE_TRANSPORT_UART:
//     default:
//         return ODRIVE_ERROR_INVALID_BACKEND;
//     }

//     /* Transmit message */
//     odrive_result_t result = driver->com.ops.transmit(&message);
//     if (result != ODRIVE_RESULT_OK) {
//         return ODRIVE_ERROR_CAN_TX_FAILED;
//     }

//     return ODRIVE_RESULT_OK;
// }

// odrive_result_t odrive_axis_get_current_state(odrive_axis axis, 
//                                               odrive_axis_state_t* state)
// {
//     if (!axis || !state)
//         return ODRIVE_ERROR_NULL_POINTER;

//     *state = axis->heartbeat.axis_state;

//     return ODRIVE_RESULT_OK;
// }

// odrive_result_t odrive_axis_set_setpoint(odrive_axis axis,
//                                          float position,
//                                          int16_t velocity_ff,
//                                          int16_t torque_ff)
// {
//     if (!axis)
//         return ODRIVE_ERROR_NULL_POINTER;

//     odrive_driver driver = axis->driver;

//     odrive_message_t message = {0};
//     switch (driver->com.transport)
//     {
//     case ODRIVE_TRANSPORT_CAN:
//         message.msg_id = ODRIVE_CAN_ID(axis->node_id, CAN_CMD_SET_INPUT_POS);
//         ODRIVE_PACK_FLOAT(message, position);
//         ODRIVE_PACK_INT16(message, velocity_ff);
//         ODRIVE_PACK_INT16(message, torque_ff);

//         break;
//     case ODRIVE_TRANSPORT_USB:
//     case ODRIVE_TRANSPORT_UART:
//     default:
//         return ODRIVE_ERROR_INVALID_BACKEND;
//     }

//     /* Transmit message */
//     odrive_result_t result = driver->com.ops.transmit(&message);
//     if (result != ODRIVE_RESULT_OK) {
//         return ODRIVE_ERROR_CAN_TX_FAILED;
//     }

//     return ODRIVE_RESULT_OK;
// }

// odrive_result_t odrive_axis_set_velocitysetpoint(odrive_axis axis,
//                                                  float velocity,
//                                                  float torque_ff)
// {
//     if (!axis)
//         return ODRIVE_ERROR_NULL_POINTER;

//     odrive_driver driver = axis->driver;

//     odrive_message_t message = {0};
//     switch (driver->com.transport)
//     {
//     case ODRIVE_TRANSPORT_CAN:
//         message.msg_id = ODRIVE_CAN_ID(axis->node_id, CAN_CMD_SET_INPUT_VEL);
//         ODRIVE_PACK_FLOAT(message, velocity);
//         ODRIVE_PACK_FLOAT(message, torque_ff);

//         break;
//     case ODRIVE_TRANSPORT_USB:
//     case ODRIVE_TRANSPORT_UART:
//     default:
//         return ODRIVE_ERROR_INVALID_BACKEND;
//     }

//     /* Transmit message */
//     odrive_result_t result = driver->com.ops.transmit(&message);
//     if (result != ODRIVE_RESULT_OK) {
//         return ODRIVE_ERROR_CAN_TX_FAILED;
//     }

//     return ODRIVE_RESULT_OK;
// }

// odrive_result_t odrive_axis_set_torquesetpoint(odrive_axis axis, 
//                                                float torque)
// {
//     if (!axis)
//         return ODRIVE_ERROR_NULL_POINTER;

//     odrive_driver driver = axis->driver;

//     odrive_message_t message = {0};
//     switch (driver->com.transport)
//     {
//     case ODRIVE_TRANSPORT_CAN:
//         message.msg_id = ODRIVE_CAN_ID(axis->node_id, CAN_CMD_SET_INPUT_TORQUE);
//         ODRIVE_PACK_FLOAT(message, torque);

//         break;
//     case ODRIVE_TRANSPORT_USB:
//     case ODRIVE_TRANSPORT_UART:
//     default:
//         return ODRIVE_ERROR_INVALID_BACKEND;
//     }

//     /* Transmit message */
//     odrive_result_t result = driver->com.ops.transmit(&message);
//     if (result != ODRIVE_RESULT_OK) {
//         return ODRIVE_ERROR_CAN_TX_FAILED;
//     }

//     return ODRIVE_RESULT_OK;
// }

// odrive_result_t odrive_axis_get_econder_frame(odrive_axis axis,
//                                               encoder_estimate_frame* frame)
// {
//     if (!axis || frame)
//         return ODRIVE_ERROR_NULL_POINTER;

//     *frame = axis->encoder_estimate;

//     return ODRIVE_RESULT_OK;
// }

// // TODO: Perform a IDLE state check
// odrive_result_t odrive_axis_reboot(odrive_axis axis, 
//                                    odrive_reboot_action_t action)
// {
//     if (!axis)
//         return ODRIVE_ERROR_NULL_POINTER;

//     odrive_driver driver = axis->driver;

//     odrive_message_t message = {0};
//     switch (driver->com.transport)
//     {
//     case ODRIVE_TRANSPORT_CAN:
//         message.msg_id = ODRIVE_CAN_ID(axis->node_id, CAN_CMD_SET_INPUT_TORQUE);
//         ODRIVE_PACK_UINT8(message, action);

//         break;
//     case ODRIVE_TRANSPORT_USB:
//     case ODRIVE_TRANSPORT_UART:
//     default:
//         return ODRIVE_ERROR_INVALID_BACKEND;
//     }

//     /* Transmit message */
//     odrive_result_t result = driver->com.ops.transmit(&message);
//     if (result != ODRIVE_RESULT_OK) {
//         return ODRIVE_ERROR_CAN_TX_FAILED;
//     }

//     return ODRIVE_RESULT_OK;
// }

//========================================================
//      End of File
//========================================================