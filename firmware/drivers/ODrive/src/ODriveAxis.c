/***************************************************************************//**
* \file ODriveInternal.c
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
//      Axis State Methods
//========================================================

odrive_result_t odrive_axis_request_state(odrive_driver driver, 
                                          odrive_node_id node,
                                          odrive_axis_state_t state)
{
    if (!driver)
        return ODRIVE_ERROR_NULL_POINTER;

    if (node >= ODRIVE_AXES_ID_RANGE)
        return ODRIVE_ERROR_INVALID_PARAM;

    if (driver->state != DRIVER_STATE_RUNNING)
        return ODRIVE_ERROR_INVALID_STATE;

    odrive_axis axis;
    ODRIVE_CHECK(odrive_get_axis_by_node_id(driver, node, &axis));

    odrive_can_message_t msg = {
        .id = ODRIVE_CAN_ID(axis->node_id, CAN_CMD_SET_AXIS_STATE),
        .dlc = 4u,
        .rtr = 0u,
        .data = { 0u },
    };

    odrive_axis_state_t _state = state;
    memcpy(msg.data, &_state, sizeof(odrive_axis_state_t));

    ODRIVE_CHECK(driver->config.backend->ops.can_ops.transmit(
        driver->config.backend->hw_ctx,
        &msg
    ));

    return ODRIVE_RESULT_OK;
}

odrive_result_t odrive_axis_get_current_state(odrive_driver driver, 
                                              odrive_node_id node,
                                              odrive_axis_state_t* state)
{
    if (!driver)
        return ODRIVE_ERROR_NULL_POINTER;

    if (node >= ODRIVE_AXES_ID_RANGE)
        return ODRIVE_ERROR_INVALID_PARAM;

    if (driver->state != DRIVER_STATE_RUNNING)
        return ODRIVE_ERROR_INVALID_STATE;

    odrive_axis axis;
    ODRIVE_CHECK(odrive_get_axis_by_node_id(driver, node, &axis));

    *state = axis->heartbeat.axis_state;

    return ODRIVE_RESULT_OK;
}

odrive_result_t odrive_force_feedback_homing(odrive_driver driver,
                                             odrive_node_id node)
{
    if (!driver)
        return ODRIVE_ERROR_NULL_POINTER;

    if (node > ODRIVE_AXES_ID_RANGE)
        return ODRIVE_ERROR_INVALID_PARAM;

    odrive_axis axis;
    ODRIVE_CHECK(odrive_get_axis_by_node_id(driver, node, &axis));

    odrive_can_message_t msg = {
        .id = ODRIVE_CAN_ID(axis->node_id, CAN_CMD_SET_AXIS_STATE),
        .dlc = 4u,
        .rtr = 0u,
        .data = { 0u },
    };
    uint32_t state = (uint32_t)AXIS_STATE_HOMING;
    memcpy(msg.data, &state, sizeof(state));

    ODRIVE_CHECK(driver->config.backend->ops.can_ops.transmit(
        driver->config.backend->hw_ctx,
        &msg
    ));

    axis->force_homing_cfg.is_force_homing = true;

    return ODRIVE_RESULT_OK;
}

odrive_result_t odrive_axis_set_position(odrive_driver driver,
                                         odrive_node_id node,
                                         odrive_setpoint_t* setpoint)
{
    if (!driver || !setpoint)
        return ODRIVE_ERROR_NULL_POINTER;

    if (node > ODRIVE_AXES_ID_RANGE)
        return ODRIVE_ERROR_INVALID_PARAM;

    odrive_axis axis;
    ODRIVE_CHECK(odrive_get_axis_by_node_id(driver, node, &axis));

    odrive_can_message_t msg = {
        .id = ODRIVE_CAN_ID(axis->node_id, CAN_CMD_SET_INPUT_POS),
        .dlc = 8u,
        .rtr = 0u,
        .data = { 0u },
    };

    uint8_t* p = msg.data;

    memcpy(msg.data, &setpoint->position, sizeof(setpoint->position));
    p += sizeof(setpoint->position);

    memcpy(msg.data, &setpoint->velocity_ff, sizeof(setpoint->velocity_ff));
    p += sizeof(setpoint->velocity_ff);

    memcpy(msg.data, &setpoint->torque_ff, sizeof(setpoint->torque_ff));
    p += sizeof(setpoint->torque_ff);

    ODRIVE_CHECK(driver->config.backend->ops.can_ops.transmit(
        driver->config.backend->hw_ctx,
        &msg
    ));

    return ODRIVE_RESULT_OK;
}

bool odrive_axis_is_homed(odrive_driver driver,
                          odrive_node_id node)
{
    if (!driver)
        return 0;

    if (node > ODRIVE_AXES_ID_RANGE)
        return 0;

    odrive_axis axis;
    if (odrive_get_axis_by_node_id(driver, node, &axis) != ODRIVE_RESULT_OK)
        return 0;

    return axis->is_homed;
}

//========================================================
//      End of File
//========================================================