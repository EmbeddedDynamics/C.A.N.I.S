# Capstan Cycle Tool

A Python script for testing capstan drive motion on the ODrive motor controller.

## What it does

This tool connects to an ODrive over USB, configures position control with passthrough input mode, and then runs the motor back and forth between two positions in a continuous cycle. It implements its own software velocity and acceleration limiting (trapezoidal profile) to protect the capstan drive during testing.

The motion loop runs at a fixed timestep and applies velocity ramp-up/braking logic to smoothly reach the target position. When the motor arrives at a target (within 0.002 turns at near-zero speed), it dwells briefly and then switches to the opposite target.

**Motion parameters** (configured at the top of the script):
- `POS_LOW`, `POS_HIGH` — position endpoints in turns
- `VEL_LIMIT` — maximum velocity (turns/s)
- `ACCEL_LIMIT` — acceleration/deceleration (turns/s²)
- `CURRENT_LIMIT` — motor current limit (A)
- `DWELL_TIME` — pause time at each endpoint (s)

## Libraries / dependencies

- **[odrive](https://docs.odriverobotics.com/)** (`pip install odrive`) — ODrive Python library for USB connection and control

## File structure

```
tools/
└── capstan-cycle.py    # Motion test script — connect ODrive, run capstan back-and-forth cycle
```

## Usage

```bash
# Requires ODrive connected over USB
python3 tools/capstan-cycle.py
```

Press `Ctrl+C` to stop — the script will set the axis to IDLE before exiting.
