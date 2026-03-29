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
#include <stdio.h>
#include <string.h>
    
/* Project headers */
#include "ODriveInternal.h"

#include "USBUART.h"

#define COPY_CAN_PAYLOAD(dst, dst_size)            \
    do {                                           \
        size_t _len = msg.dlc;                     \
        if (_len > (dst_size)) _len = (dst_size);  \
        memcpy((dst), msg.data, _len);             \
    } while (0)

//========================================================
//      Driver Instance Methods
//========================================================

odrive_result_t odrive_create_driver(const odrive_driver_cfg_t* config,
                                     odrive_driver* driver)
{
    /* Validate parameters */
    if (!config || !driver)
        return ODRIVE_ERROR_NULL_POINTER;

    /* Allocate memory for the driver */
    odrive_driver new_driver = malloc(sizeof(struct odrive_driver_T));
    if (!new_driver)
        return ODRIVE_ERROR_NO_MEMORY;

    /* Initialize driver structure */
    new_driver->config = *config;
    new_driver->state = DRIVER_STATE_UNINITIALIZED;

    // Initialize node map (all slots unused)
    memset(new_driver->node_map, 0xFF, sizeof(new_driver->node_map));
    
    for (uint8_t i = 0; i < ODRIVE_MAX_AXES; i++)
    {
        new_driver->axes[i].is_used = false;
        new_driver->axes[i].is_homed = false;
        new_driver->axes[i].node_id = 0x00;
    }

    *driver = new_driver;

    return ODRIVE_RESULT_OK;
}

void odrive_destroy_driver(odrive_driver driver)
{
    if (!driver)
        return;

    free(driver);
}

//========================================================
//      Driver Communication Control
//========================================================

odrive_result_t odrive_start_driver(odrive_driver driver)
{
    if (!driver)
        return ODRIVE_ERROR_NULL_POINTER;

    if (driver->state == DRIVER_STATE_RUNNING)
        return ODRIVE_RESULT_OK;  // Already running

    driver->state = DRIVER_STATE_RUNNING;
    
    driver->sys_tick = 0u;
    driver->log_tick = 0u;

    return ODRIVE_RESULT_OK;
}

odrive_result_t odrive_stop_driver(odrive_driver driver)
{
    if (!driver)
        return ODRIVE_ERROR_NULL_POINTER;

    if (driver->state != DRIVER_STATE_RUNNING)
        return ODRIVE_RESULT_OK;  // Already stopped

    driver->state = DRIVER_STATE_STOPPED;

    return ODRIVE_RESULT_OK;
}

//========================================================
//      Driver Methods
//========================================================

odrive_result_t odrive_poll_driver(odrive_driver driver,
                                   uint32_t current_time)
{
    /* Parameter validation */
    if (!driver)
        return ODRIVE_ERROR_NULL_POINTER;

    if (driver->state != DRIVER_STATE_RUNNING)
        return ODRIVE_ERROR_NOT_INITIALIZED;
    
    driver->sys_tick = current_time;

    uint8_t rx_count = driver->config.backend->ops.can_ops.get_rx_count(
        driver->config.backend->hw_ctx
    );

    if (rx_count > 0)
    {
        // Process incoming messages
        for (uint8_t i = 0; i < rx_count; i++)
        {
            odrive_can_message_t msg;
            odrive_result_t result = driver->config.backend->ops.can_ops.receive(
                driver->config.backend->hw_ctx,
                &msg
            );

            if (result != ODRIVE_RESULT_OK)
            {
                return result;  // Reception error
            }

            // Handle the received message
            odrive_node_id node_id = ODRIVE_NODE_ID_FROM_CAN_ID(msg.id);  // Extract node ID
            odrive_node_id slot = driver->node_map[node_id];
            if (node_id >= ODRIVE_AXES_ID_RANGE || slot >= ODRIVE_NODE_ID_UNUSED)
                continue;  // Invalid node ID

            odrive_axis axis = &driver->axes[slot];
            odrive_can_command_t cmd_id = ODRIVE_CMD_ID_FROM_CAN_ID(msg.id);  // Extract command ID

            switch (cmd_id)
            {
            case CAN_CMD_GET_VERSION:
                /* code */
                break;
            case CAN_CMD_HEARTBEAT:
                axis->last_heartbeat_ms = current_time;
                
                #if ODRIVE_FW_VERSION_IS(ODRIVE_FW_V0_5_4)
                    uint8_t *p = msg.data;

                    memcpy(&axis->heartbeat.axis_error, p, sizeof(axis->heartbeat.axis_error));
                    p += sizeof(axis->heartbeat.axis_error);

                    memcpy(&axis->heartbeat.axis_state, p, sizeof(axis->heartbeat.axis_state));
                    p += sizeof(axis->heartbeat.axis_state);
                #elif ODRIVE_FW_VERSION_IS(ODRIVE_FW_V0_5_6)                  
                    uint8_t *p = msg.data;

                    memcpy(&axis->heartbeat.axis_error, p, sizeof(axis->heartbeat.axis_error));
                    p += sizeof(axis->heartbeat.axis_error);

                    memcpy(&axis->heartbeat.axis_state, p, sizeof(axis->heartbeat.axis_state));
                    p += sizeof(axis->heartbeat.axis_state);

                   axis->heartbeat.axis_flags |= (((uint32_t)(*p & ODRIVE_MOTOR_ERROR_FLAG_BM) != 0u) << ODRIVE_MOTOR_ERROR_FLAG_BP);
                    p++;

                    axis->heartbeat.axis_flags |= (((uint32_t)(*p & ODRIVE_ENCODER_ERROR_FLAG_BM) != 0u) << ODRIVE_ENCODER_ERROR_FLAG_BP);
                    p++;

                    // controller + trajectory done are in the same byte
                    axis->heartbeat.axis_flags |= (((uint32_t)(*p & ODRIVE_CONTROLLER_ERROR_FLAG_BM) != 0u) << ODRIVE_CONTROLLER_ERROR_FLAG_BP);

                    axis->heartbeat.axis_flags |= (((uint32_t)(*p & ODRIVE_TRAJECTORY_DONE_FLAG_BM) != 0u) << ODRIVE_TRAJECTORY_DONE_FLAG_BP);

                #endif
                
                //========================================================
                //     Homing status update
                //========================================================

                #if ODRIVE_FW_VERSION_IS(ODRIVE_FW_V0_5_6)
                    
                if (axis->force_homing_cfg.is_force_homing && axis->is_homing && 
                    axis->heartbeat.axis_state == AXIS_STATE_IDLE)
                {
                    // Mark the axis a homed
                    axis->is_homed = true;

                    // Set the homing flags to false
                    axis->is_homing = false;
                    axis->force_homing_cfg.is_force_homing = false;
                    
                    // Safety check if the gpio write function is available
                    if (!driver->config.backend->gpio.write)
                        return ODRIVE_ERROR_INVALID_BACKEND;

                    // Write the homing pin to GND
                    driver->config.backend->gpio.write(axis->force_homing_cfg.homing_pin, 0x00);
                    
                    /* Switch the axis to closed-loop control after homing */
                    ODRIVE_CHECK(odrive_axis_request_state(driver, node_id, AXIS_STATE_CLOSED_LOOP_CONTROL));

                    // Fire callback is set by the user
                    if (driver->config.on_homed != NULL)
                        driver->config.on_homed(axis->node_id);
                }
                
                #endif
                
                break;
            case CAN_CMD_GET_ENCODER_ESTIMATES:
            {
                float old_position = axis->encoder_estimate.position_estimate;
                COPY_CAN_PAYLOAD(&axis->encoder_estimate, msg.dlc);
                
                // Adjust delta_position if force feedback homing is enabled
                if (axis->force_homing_cfg.enable_force_feedback_homing) {
                    axis->force_homing_cfg.position_delta = old_position - axis->encoder_estimate.position_estimate;
                }
                break;  
            }
            case CAN_CMD_GET_IQ:
                COPY_CAN_PAYLOAD(&driver->axes[slot].iq, msg.dlc);
                break;

            #if ODRIVE_FW_VERSION_AT_LEAST(ODRIVE_FW_V0_6_0)
            case CAN_CMD_GET_TEMPERATURE:
                COPY_CAN_PAYLOAD(&driver->axes[slot].temperature, sizeof(driver->axes[slot].temperature));
                break;
            #endif
            
            case CAN_CMD_GET_BUS_VOLTAGE_CURRENT:
                COPY_CAN_PAYLOAD(&driver->axes[slot].bus, msg.dlc);
                break;
            
            default:
                continue;  // Unhandled command;
            }
        }
    }

    // Handle force feedback homing after processing messages
    if (driver->config.enable_force_feedback_homing) {
        // Update delta_position for all axes
        for (uint8_t slot = 0; slot < ODRIVE_MAX_AXES; slot++)
        {
            odrive_axis axis = &driver->axes[slot];
            if (!axis->is_used)
                continue;  // Skip unused slots

            odrive_force_homing_cfg_t* cfg = &axis->force_homing_cfg;
            if (axis->force_homing_cfg.enable_force_feedback_homing && axis->heartbeat.axis_state == AXIS_STATE_HOMING) {
                
                axis->is_homing = true;
                if (cfg->position_delta < cfg->position_deadzone
                    && axis->iq.iq_measured > cfg->iq_threshold) 
                {
                    
                    if (!driver->config.backend->gpio.write)
                        return ODRIVE_ERROR_INVALID_BACKEND;

                    driver->config.backend->gpio.write(cfg->homing_pin, 0xFF);
                }
            }
        }
    }
    
    /*if (driver->sys_tick >= (driver->log_tick + 200))
    {
        driver->log_tick = driver->sys_tick;
        
        char buffer[128] = {0u};
        char format[] = "%.8f,%.8f,";
        
        uint16_t written = 0u;
        
        for (uint8_t slot = 0; slot < ODRIVE_MAX_AXES; slot++)
        {
            odrive_node_id node_id = driver->node_map[slot];
            if (node_id == ODRIVE_NODE_ID_UNUSED)
                continue;  // Skip unused slots

            odrive_axis axis = &driver->axes[node_id];
            
            int n = snprintf(buffer + written, 128 - written ,format, axis->bus.current, axis->bus.voltage);
            if (n < 0) break;
            if ((size_t)n >= sizeof(buffer) - written) { written = sizeof(buffer) - 1; break; }
            written += (uint16_t)n;
            
            //USBUART_1_PutString(buffer);*/
        /*}
        
        // replace trailing comma with newline if we wrote something
        if (written > 0 && buffer[written - 1] == ',') {
            buffer[written - 1] = '\n';
        } else if (written < sizeof(buffer) - 1) {
            buffer[written++] = '\n';
        }

        buffer[written] = '\0';
        
        USBUART_PutString(buffer);
    }*/

    return ODRIVE_RESULT_OK;
}

odrive_result_t odrive_register_axis(odrive_driver driver,
                                     odrive_force_homing_cfg_t* cfg,
                                     odrive_node_id node_id)
{
    if (!driver || !cfg)
        return ODRIVE_ERROR_NULL_POINTER;

    if (node_id >= ODRIVE_AXES_ID_RANGE)
        return ODRIVE_ERROR_OUT_OF_RANGE;

    if (driver->node_map[node_id] != ODRIVE_NODE_ID_UNUSED)
        return ODRIVE_ERROR_INVALID_PARAM;  // Node ID already registered

    /* Find a free axis slot */
    for (uint8_t slot = 0; slot < ODRIVE_MAX_AXES; slot++)
    {
        if (!driver->axes[slot].is_used)
        {
            driver->node_map[node_id] = slot;
            driver->axes[slot].is_used = true;
            driver->axes[slot].node_id = node_id;
            driver->axes[slot].driver = driver;
            driver->axes[slot].force_homing_cfg = *cfg;
            return ODRIVE_RESULT_OK;
        }
    }

    return ODRIVE_ERROR_NO_AXIS_SLOT;
}


odrive_result_t odrive_reboot_all(odrive_driver driver, 
                                  odrive_reboot_action_t action)
{
    if (!driver)
        return ODRIVE_ERROR_NULL_POINTER;

    if (driver->state != DRIVER_STATE_RUNNING)
        return ODRIVE_ERROR_INVALID_STATE;

    driver->state = DRIVER_STATE_REBOOTING;
    
    #if (ODRIVE_SUPPORT_BROADCAST_ID)
        odrive_can_message_t msg = {
            .id = ODRIVE_CAN_BROADCAST_ID(CAN_CMD_REBOOT),
            .dlc = 1,
            .rtr = 0,
            .data = { (uint8_t)action }
        };

        driver->config.backend->ops.can_ops.transmit(
            driver->config.backend->hw_ctx,
            &msg
        );
    #else
        for (uint8_t slot = 0; slot < ODRIVE_MAX_AXES; slot++)
        {
            odrive_node_id node_id = driver->node_map[slot];
            if (node_id == ODRIVE_NODE_ID_UNUSED)
                continue;  // Skip unused slots

            odrive_can_message_t msg = {
                .id = ODRIVE_CAN_ID(node_id, CAN_CMD_REBOOT),
                .dlc = 1,
                .rtr = 0,
                .data = { (uint8_t) action }
            };

            driver->config.backend->ops.can_ops.transmit(
                driver->config.backend->hw_ctx,
                &msg
            );
        }
    #endif
    
    return ODRIVE_RESULT_OK;
}

odrive_result_t odrive_get_axis_by_node_id(odrive_driver driver,
                                        odrive_node_id node_id,
                                        odrive_axis* axis_out)
{
    /* Parameter validation */
    if (!driver || !axis_out)
        return ODRIVE_ERROR_NULL_POINTER;

    /* Map node id to allowed range */
    if (node_id >= ODRIVE_AXES_ID_RANGE)
        return ODRIVE_ERROR_OUT_OF_RANGE;

    odrive_node_id slot = driver->node_map[node_id];
    if (slot == ODRIVE_NODE_ID_UNUSED)
        return ODRIVE_ERROR_INVALID_PARAM;  // Node ID not registered

    *axis_out = &driver->axes[slot];
    return ODRIVE_RESULT_OK;
}

//========================================================
//      Public HAL Interface
//========================================================

float odrive_max_power(odrive_driver driver)
{
    if (!driver)
        return ODRIVE_ERROR_NULL_POINTER;

    if (driver->state != DRIVER_STATE_RUNNING)
        return ODRIVE_ERROR_INVALID_STATE;

    float power = 0.0f;
    for (uint8_t slot = 0; slot < ODRIVE_MAX_AXES; slot++)
    {
        odrive_node_id node_id = driver->node_map[slot];
        if (node_id == ODRIVE_NODE_ID_UNUSED)
            continue;  // Skip unused slots

        odrive_axis axis = &driver->axes[node_id];

        power += axis->bus.current * axis->bus.voltage;
    }

    return power;
}

odrive_transport_t odrive_backend_get_type(odrive_driver driver)
{
    if (!driver)
        return ODRIVE_TRANSPORT_UNKNOWN;

    return driver->config.backend->transport;
}

const char* odrive_backend_get_name(odrive_driver driver)
{
    if (!driver)
        return "Unknown";

    if (!driver->config.backend || !driver->config.backend->get_name)
        return "Unknown";

    return driver->config.backend->get_name(driver->config.backend->hw_ctx);  
}

//========================================================
//      End of File
//========================================================
