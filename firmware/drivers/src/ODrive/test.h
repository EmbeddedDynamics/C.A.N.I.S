// ========================================================
// Function Prototypes - Protocol-Level Commands
// ========================================================

/**
 * @brief Sends axis state command
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Target axis CAN node ID
 * @param[in] state - Requested axis state
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_SendAxisState(ODriveDriver driver,
                                        uint8_t node_id,
                                        ODriveAxisState state);

/**
 * @brief Sends position setpoint
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Target axis CAN node ID
 * @param[in] position - Position setpoint [turns]
 * @param[in] vel_ff - Velocity feedforward [turns/s]
 * @param[in] torque_ff - Torque feedforward [Nm]
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_SendInputPos(ODriveDriver driver,
                                       uint8_t node_id,
                                       float position,
                                       float vel_ff,
                                       float torque_ff);

/**
 * @brief Sends velocity setpoint
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Target axis CAN node ID
 * @param[in] velocity - Velocity setpoint [turns/s]
 * @param[in] torque_ff - Torque feedforward [Nm]
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_SendInputVel(ODriveDriver driver,
                                       uint8_t node_id,
                                       float velocity,
                                       float torque_ff);

/**
 * @brief Sends torque setpoint
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Target axis CAN node ID
 * @param[in] torque - Torque setpoint [Nm]
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_SendInputTorque(ODriveDriver driver,
                                          uint8_t node_id,
                                          float torque);

/**
 * @brief Requests encoder estimates
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Target axis CAN node ID
 * @param[out] position - Pointer to receive position [turns]
 * @param[out] velocity - Pointer to receive velocity [turns/s]
 * @param[in] timeout_ms - Response timeout [ms]
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_RequestEncoderEstimates(ODriveDriver driver,
                                                  uint8_t node_id,
                                                  float* position,
                                                  float* velocity,
                                                  uint16_t timeout_ms);

/**
 * @brief Sets control and input modes
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Target axis CAN node ID
 * @param[in] control_mode - Control mode (position/velocity/torque)
 * @param[in] input_mode - Input mode (passthrough/trap_traj/etc)
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_SetControllerModes(ODriveDriver driver,
                                             uint8_t node_id,
                                             ODriveControlMode control_mode,
                                             ODriveInputMode input_mode);

/**
 * @brief Clears all errors on specified axis
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Target axis CAN node ID
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_ClearErrors(ODriveDriver driver, uint8_t node_id);

/**
 * @brief Gets last heartbeat data for an axis
 *
 * Retrieves cached heartbeat data from most recent heartbeat message.
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Axis CanNodeId
 * @param[out] error - Pointer to receive axis error flags
 * @param[out] state - Pointer to receive current axis state
 * @param[out] is_armed - Pointer to receive armed status
 *
 * @return Operation result
 */
ODriveResult ODriveDriver_GetHeartbeat(ODriveDriver driver,
                                       CanNodeId node_id,
                                       uint32_t* error,
                                       ODriveAxisState* state,
                                       bool* is_armed);

/**
 * @brief Checks if axis heartbeat is alive
 *
 * Returns true if heartbeat received within timeout period.
 *
 * @param[in] driver - ODriveDriver handle
 * @param[in] node_id - Axis CanNodeId
 *
 * @return true if heartbeat alive, false if timeout or error
 */
bool ODriveDriver_IsHeartbeatAlive(ODriveDriver driver, CanNodeId node_id);