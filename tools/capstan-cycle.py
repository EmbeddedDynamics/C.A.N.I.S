# #!/usr/bin/env python3
# import odrive
# from odrive.enums import *
# import time
# from odrive.utils import start_liveplotter

# # --------------------------------------------
# # Connect to ODrive
# # --------------------------------------------
# print("Finding ODrive...")
# odrv = odrive.find_any()
# print("Connected:", odrv)

# # Select axis (0 or 1)
# axis = odrv.axis0

# # --------------------------------------------
# # Configure trapezoidal trajectory mode
# # --------------------------------------------

# # Make sure we're idle before changing modes
# axis.requested_state = AXIS_STATE_IDLE
# time.sleep(0.1)

# # Use position control with trapezoidal trajectory on the input
# axis.controller.config.control_mode = CONTROL_MODE_POSITION_CONTROL
# # axis.controller.config.input_mode = INPUT_MODE_TRAP_TRAJ
# axis.controller.config.input_mode = INPUT_MODE_PASSTHROUGH

# # # Reasonable trapezoidal trajectory limits (turns, turns/s, turns/s^2)
# # axis.trap_traj.config.vel_limit   = 12.0   # max velocity (turns per second)
# # axis.trap_traj.config.accel_limit = 24.0  # acceleration (turns/s^2)
# # axis.trap_traj.config.decel_limit = 2.0  # deceleration (turns/s^2)

# print("Trap traj config:")
# print("  vel_limit   =", axis.trap_traj.config.vel_limit)
# print("  accel_limit =", axis.trap_traj.config.accel_limit)
# print("  decel_limit =", axis.trap_traj.config.decel_limit)

# # Enable closed-loop control
# axis.requested_state = AXIS_STATE_CLOSED_LOOP_CONTROL
# time.sleep(0.2)

# # --------------------------------------------
# # Motion parameters
# # --------------------------------------------
# pos_high = 1
# pos_low  = 0.1

# dwell_time = 0.3  # seconds to wait at each end

# print("Starting trapezoidal motion between", pos_low, "and", pos_high)

# # --------------------------------------------
# # Main loop
# # --------------------------------------------
# try:
#     while True:
#         print("→ Moving to", pos_high)
#         axis.controller.input_pos = pos_high
#         # Wait long enough for motion to complete + dwell
#         # Rough estimate: distance/vel_limit + small margin
#         travel_time = abs(pos_high - pos_low) / axis.trap_traj.config.vel_limit
#         time.sleep(travel_time + dwell_time)

#         print("→ Moving to", pos_low)
#         axis.controller.input_pos = pos_low
#         travel_time = abs(pos_high - pos_low) / axis.trap_traj.config.vel_limit
#         time.sleep(travel_time + dwell_time)

# except KeyboardInterrupt:
#     print("\nStopping...")
#     axis.requested_state = AXIS_STATE_IDLE
#     print("Done.")


#!/usr/bin/env python3
import time
import odrive
from odrive.enums import *
from odrive.utils import start_liveplotter

# ============================
# User parameters
# ============================

AXIS_INDEX = 0          # 0 or 1
POS_LOW    = 0.1        # [turns]
POS_HIGH   = 0.9        # [turns]

VEL_LIMIT      = 40.0    # [turns/s]   max commanded velocity
ACCEL_LIMIT    = 50.0    # [turns/s^2] max acceleration/deceleration
CURRENT_LIMIT  = 5.0    # [A]         torque limit (via current)

DT_TARGET      = 0.001  # [s] control loop interval (~500 Hz)
DWELL_TIME     = 0.5    # [s] wait at each end

# ============================
# Connect and configure
# ============================

print("Finding ODrive...")
odrv = odrive.find_any()
print("Connected:", odrv)

axis = odrv.axis0 if AXIS_INDEX == 0 else odrv.axis1

ibus_prop = odrv._ibus_property

# Go idle before changing config
axis.requested_state = AXIS_STATE_IDLE
time.sleep(0.1)

# Torque limit via current
axis.motor.config.current_lim = CURRENT_LIMIT

# Safety velocity limit in FW (backup clamp)
axis.controller.config.vel_limit = VEL_LIMIT * 1.2

# Position control with passthrough input
axis.controller.config.control_mode = CONTROL_MODE_POSITION_CONTROL
axis.controller.config.input_mode   = INPUT_MODE_PASSTHROUGH

# Enable closed-loop
axis.requested_state = AXIS_STATE_CLOSED_LOOP_CONTROL
time.sleep(0.2)

# start_liveplotter(properties=[ibus_prop])

# ============================
# Helper: sign function
# ============================

def sgn(x):
    if x > 0:
        return 1.0
    if x < 0:
        return -1.0
    return 0.0

# ============================
# Motion loop
# ============================

# Start from current position
pos_cmd = axis.encoder.pos_estimate
vel_cmd = 0.0

target = POS_HIGH
last_time = time.time()
last_switch_time = time.time()

print("Starting passthrough motion with vel/acc/torque limits...")
print(f"Targets: {POS_LOW} ↔ {POS_HIGH} turns")
print(f"VEL_LIMIT={VEL_LIMIT} turns/s, ACCEL_LIMIT={ACCEL_LIMIT} turns/s², CURRENT_LIMIT={CURRENT_LIMIT} A")

try:
    while True:
        now = time.time()
        dt  = now - last_time
        if dt <= 0:
            dt = DT_TARGET
        last_time = now

        # Distance to target
        dist = target - pos_cmd
        dir_to_target = sgn(dist)

        # Braking distance needed to stop at current speed
        # d_brake = v^2 / (2*a)
        braking_distance = (vel_cmd ** 2) / (2.0 * ACCEL_LIMIT) if ACCEL_LIMIT > 0 else 0.0

        # Decide whether to accelerate towards target or brake
        if abs(braking_distance) >= abs(dist):
            # Need to brake (reduce speed magnitude)
            if vel_cmd != 0.0:
                vel_cmd -= sgn(vel_cmd) * ACCEL_LIMIT * dt
                # Avoid overshooting sign change
                if sgn(vel_cmd) != sgn(axis.controller.input_vel if hasattr(axis.controller, "input_vel") else vel_cmd):
                    pass
        else:
            # Can still accelerate towards target
            vel_cmd += dir_to_target * ACCEL_LIMIT * dt

        # Clamp velocity
        if vel_cmd > VEL_LIMIT:
            vel_cmd = VEL_LIMIT
        elif vel_cmd < -VEL_LIMIT:
            vel_cmd = -VEL_LIMIT

        # Integrate position
        pos_cmd += vel_cmd * dt

        # Send command to ODrive (passthrough)
        axis.controller.input_pos = pos_cmd
        # Optional: supply feedforward velocity if you like
        # axis.controller.input_vel = vel_cmd

        # Check if we've arrived near the target and slowed down
        if abs(dist) < 0.002 and abs(vel_cmd) < 0.01:
            # Snap exactly to target
            pos_cmd = target
            axis.controller.input_pos = pos_cmd
            vel_cmd = 0.0

            # Dwell a bit, then switch target
            if now - last_switch_time > DWELL_TIME:
                target = POS_LOW if target == POS_HIGH else POS_HIGH
                last_switch_time = now
                print(f"Switching target to {target} turns")

        # Try to keep loop close to DT_TARGET
        remaining = DT_TARGET - (time.time() - now)
        if remaining > 0:
            time.sleep(remaining)

except KeyboardInterrupt:
    print("\nStopping, putting axis in IDLE...")
    axis.requested_state = AXIS_STATE_IDLE
    print("Done.")
