# PSoC Workspace

The main Cypress PSoC5 firmware project — integrates CanStack and the ODrive driver for robot dog motor control.

## What it does

This is the primary PSoC5 firmware workspace for the Robothond project. It brings together the CanStack CAN transport library and the ODrive motor controller driver into a single runnable project.

`main.c` sets up the full communication stack:
1. Creates a CanStack driver at 1 Mbps
2. Creates an ODrive-over-CanStack backend adapter
3. Creates an ODrive driver instance
4. Configures a hardware CAN filter for ODrive heartbeat messages

## Libraries / dependencies

- **CanStack** — CAN 2.0 transport library (`firmware/drivers/CANStack/`)
- **ODrive driver** — ODrive motor controller driver (`firmware/drivers/ODrive/`)
- PSoC Creator component API (`project.h`) — CAN peripheral, timers, etc.
- Standard C: `<stdint.h>`, `<stdio.h>`, `<math.h>`

## File structure

```
firmware/platform/PSoC/PSoC-workspace/
├── PSoC-workspace.cywrk                    # PSoC Creator workspace — open this in PSoC Creator/Designer
└── RobotDog.cydsn/
    ├── RobotDog.cyprj                      # Project file — source and component configs
    ├── TopDesign/
    │   └── TopDesign.cysch                 # Schematic — CAN peripheral and other components placed here
    ├── bCORDIC/                            # Custom CORDIC UDB component
    ├── main.c                              # Firmware entry point — CanStack + ODrive initialisation
    └── cyapicallbacks.h                    # PSoC Creator interrupt callback hooks
```

## Building

Open `PSoC-workspace.cywrk` in **Cypress PSoC Creator** or **PSoC Designer**. There is no CLI build system.
