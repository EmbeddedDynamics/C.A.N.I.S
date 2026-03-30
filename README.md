# C.A.N.I.S. — Robotdog

> **Embedded Systems project · NHL Stenden University of Applied Sciences · BD.ELT 2nd Year**
>
> 🌐 [canis.embeddeddynamics.nl](https://canis.embeddeddynamics.nl/)

A robot dog demonstrator built around 2 Cypress PSoC5 microcontrollers, ODrive motor controllers, and an NVIDIA Jetson Orin Nano for AI-powered perception and voice interaction.

---

## Table of Contents

- [Overview](#overview)
- [System Architecture](#system-architecture)
- [Hardware](#hardware)
- [Repository Structure](#repository-structure)
- [Modules](#modules)
  - [ODrive Driver](#odrive-driver)
  - [CAN Stack](#can-stack)
  - [Inverse Kinematics](#inverse-kinematics)
  - [CORDIC Accelerator](#cordic-accelerator)
  - [Jetson AI & Perception](#jetson-ai--perception)
  - [Tools](#tools)
- [Getting Started](#getting-started)
- [PDF Outputs](#pdf-outputs)
- [Documentation](#documentation)
- [Branches](#branches)

---

## Introduction

C.A.N.I.S. is a legged robot dog demonstrator built as a Bachelor's Embedded Systems project at NHL Stenden.

## Overview

Two **Cypress PSoC5** microcontrollers divide responsibilities across the system:

- **PSoC5 #1 — Command controller:** Receives and arbitrates all external input — Bluetooth commands via an ESP32 bridge, RC input from an ExpressLRS receiver, and movement/depth data from the Jetson over USB serial. Also hosts the MPU-6050 IMU for orientation tracking. Forwards motion intents to the motion controller over a direct UART or I2C link.
- **PSoC5 #2 — Motion controller:** Runs the inverse kinematics solver (hardware-accelerated by a custom CORDIC unit) and resolves joint angles for the diamond-linkage leg mechanism. Drives **12 motors** over CAN bus via ODrive v3 controllers — 4 leg modules (2 motors each) and 2 shoulder modules (2 motors each, controlling the pitch of 2 legs per module).

All firmware is written in **C** with no dynamic memory allocation. Motor controllers communicate over **CAN 2.0 at 1000 kbps**. An NVIDIA Jetson Orin Nano provides stereo depth perception and a voice-interactive AI soundboard.

---

## System Architecture

```
  ┌──────────────────────────────────────────────────────────────┐
  │                     NVIDIA Jetson Orin Nano                  │
  │  ┌──────────────────────┐   ┌─────────────────────────────┐  │
  │  │  Stereo Depth        │   │  Voice Command +            │  │
  │  │  Perception          │   │  AI Soundboard              │  │
  │  │  depth_zones.py      │   │  robot_dog_ai.py            │  │
  │  └──────────┬───────────┘   └─────────────────────────────┘  │
  │             │ USB Serial                                     │
  └─────────────┼────────────────────────────────────────────────┘
                │
  ┌─────────────▼────────────────────────────────────────────────┐
  │            PSoC5 #1 — Command Controller                     │
  │                                                              │
  │   ┌──────────────┐  ┌──────────────┐  ┌──────────────────┐   │
  │   │  ESP32       │  │  ExpressLRS  │  │  Jetson Serial   │   │
  │   │  Bluetooth   │  │  RC Receiver │  │  Depth / Cmds    │   │
  │   └──────────────┘  └──────────────┘  └──────────────────┘   │
  └─────────────────────────────┬────────────────────────────────┘
                                │ Direct UART / I2C
  ┌─────────────────────────────▼────────────────────────────────┐
  │            PSoC5 #2 — Motion Controller                      │
  │                                                              │
  │  ┌────────────┐  ┌──────────────┐                            │
  │  │  IKMath    │  │  bCORDIC     │                            │
  │  │  Solver    ├──►  Accelerator │                            │
  │  └────────────┘  └──────────────┘                            │
  │                                                              │
  │  ┌────────────────────────────────────────────────────────┐  │
  │  │                    ODrive Driver                       │  │
  │  │   12 axes · 6 modules × 2 motors · heartbeat · FSM     │  │
  │  └──────────────────────────┬─────────────────────────────┘  │
  │                             │                                │
  │  ┌──────────────────────────▼─────────────────────────────┐  │
  │  │                       CanStack                         │  │
  │  │                    (CAN 2.0 HAL)                       │  │
  │  └──────────────────────────┬─────────────────────────────┘  │
  └─────────────────────────────┼────────────────────────────────┘
                                │ CAN bus · 1000 kbps
           ┌────────────────────┘
           │
  ┌────────▼──────────────┐  ┌───────────────────────┐  ┌───────────────────────┐
  │      ODrive v3        │  │      ODrive v3        │  │      ODrive v3        │
  │  Leg module 1 (2ax)   ├──►  Leg module 2 (2ax)   ├──►  Leg module 3 (2ax)   │
  └───────────────────────┘  └───────────────────────┘  └───────────┬───────────┘
           ┌────────────────────────────────────────────────────────┘
           │
  ┌────────▼──────────────┐  ┌───────────────────────┐  ┌───────────────────────┐
  │      ODrive v3        │  │      ODrive v3        │  │      ODrive v3        │
  │  Leg module 4 (2ax)   ├──►  Shoulder mod 1 (2ax) ├──►  Shoulder mod 2 (2ax) │
  └───────────────────────┘  └───────────────────────┘  └───────────────────────┘
```

---

## Hardware

| Component | Part | Notes |
|---|---|---|
| Command controller | Cypress PSoC5LP | Handles all input arbitration (Bluetooth, RC, Jetson) |
| Motion controller | Cypress PSoC5LP | IK solver, CORDIC, ODrive driver, CAN bus |
| Motor controllers | ODrive v3 | 12 axes — 6 modules × 2 motors (4 leg modules + 2 shoulder pitch modules) |
| AI compute | NVIDIA Jetson Orin Nano Super | Powered at 12 V via LM2596 from Parkside 20 V battery |
| RC receiver | ExpressLRS (PWM output) | 4 channels read via PSoC5 input capture timers |
| IMU | MPU-6050 | Complementary filter on PSoC5 #1 (command controller) |
| Wireless comms | ESP32 | Bluetooth bridge to command controller |
| Bus | CAN 2.0 | 11-bit identifiers · 1000 kbps |

---

## Repository Structure

```
CANIS/
├── firmware/
│   ├── drivers/
│   │   ├── ODrive/              # ODrive motor controller driver (C)
│   │   │   ├── include/
│   │   │   │   ├── ODrive.h             # Public API — driver lifecycle & control
│   │   │   │   ├── ODriveAxis.h         # Per-axis control interface
│   │   │   │   ├── ODriveCore.h         # Protocol types, enums, constants
│   │   │   │   ├── ODriveResult.h       # Error & result code definitions
│   │   │   │   └── driver/
│   │   │   │       ├── ODriveBackend.h          # Transport abstraction (vtable)
│   │   │   │       └── CanStack/
│   │   │   │           └── ODriveCanStack.h     # CAN bus adapter
│   │   │   └── src/
│   │   │       ├── ODriveAxis.c
│   │   │       ├── ODriveInternal.c     # Core implementation (message encode/decode)
│   │   │       └── driver/CanStack/
│   │   │           └── ODriveCanStack.c
│   │   ├── CANStack/            # Cross-platform CAN 2.0 transport library (C)
│   │   │   ├── include/
│   │   │   │   ├── CanStack.h           # Public API
│   │   │   │   ├── CanStackConfig.h     # Platform configuration
│   │   │   │   └── CanStackResult.h     # Error codes
│   │   │   └── src/
│   │   │       ├── CanStack.c
│   │   │       └── platform/
│   │   │           └── CanStackPSoC5.c  # PSoC5 implementation
│   │   └── IKMath/              # Inverse kinematics solver (C)
│   │       ├── include/
│   │       │   ├── ik.h                 # Public API
│   │       │   ├── ik_types.h
│   │       │   ├── ik_config.h
│   │       │   ├── ik_results.h
│   │       │   └── accelerators/
│   │       │       ├── ik_accelerators.h    # Hardware accelerator interface
│   │       │       └── ik_cordic.h          # CORDIC backend
│   │       └── src/
│   │           └── accelerators/
│   │               ├── ik_cordic.c
│   │               └── ik_cordic_psoc5.c    # PSoC5 CORDIC driver
│   ├── platform/
│   │   ├── PSoC/
│   │   │   └── PSoC-workspace/
│   │   │       ├── PSoC-workspace.cywrk         # PSoC Creator workspace
│   │   │       └── RobotDog.cydsn/              # Main firmware project
│   │   │           ├── main.c                   # Entry point
│   │   │           ├── TopDesign/
│   │   │           │   └── TopDesign.cysch      # Schematic / pin assignment
│   │   │           └── bCORDIC/                 # Custom CORDIC accelerator
│   │   │               ├── bCORDIC.v            # Verilog implementation
│   │   │               └── API/
│   │   │                   ├── CirV.h
│   │   │                   └── CirV.c
│   │   ├── Jetson/              # NVIDIA Jetson scripts (Python)
│   │   │   ├── depth_zones.py       # Stereo depth → serial packets to PSoC5
│   │   │   ├── robot_dog_ai.py      # Voice command recognition + soundboard
│   │   │   └── sounds/              # WAV audio assets
│   │   │       ├── compliment/      # Positive responses (praise triggers)
│   │   │       ├── negative/        # Growl sounds (insult triggers)
│   │   │       ├── meme/            # Meme sound effects (keyword triggers)
│   │   │       ├── idle/            # Ambient sounds (periodic, unprompted)
│   │   │       ├── command/         # Acknowledgement sounds (on command)
│   │   │       └── confused/        # Error sounds (unrecognised input)
│   │   └── Arduino/
│   │       ├── ESP32_WirelessCOM/           # ESP32 Bluetooth bridge
│   │       └── Prototype_5-Bar_Linkage/     # Early 5-bar linkage prototype (Arduino Nano, deprecated)
│   └── HAL/                     # Hardware Abstraction Layer (in progress)
└── tools/
    └── capstan-cycle.py         # ODrive motion test — cyclic positional sweep
```

---

## Modules

### ODrive Driver

**Location:** `firmware/drivers/ODrive/`

A transport-agnostic C driver for ODrive v3 motor controllers. The driver manages up to **12 axes** in a single instance and exposes a clean API for initialisation, axis control, and telemetry.

**Key design decisions:**

- **Vtable transport abstraction** — `ODriveBackend.h` defines a HAL interface that decouples the driver from the physical transport. Swapping CAN for UART or USB requires only a different backend, not changes to driver logic.
- **State machine** — the driver transitions through `UNKNOWN → UNINITIALIZED → IDLE → STOPPED → RUNNING → REBOOTING`. All operations are state-guarded.
- **Heartbeat monitoring** — each axis has an independent heartbeat timeout (default 100 ms). A missing heartbeat is surfaced as a result code rather than a hard fault.
- **No dynamic allocation** — all state is held in caller-supplied structs.

**Error code ranges:**

| Range | Category |
|---|---|
| `0x0000` | Success |
| `0x2000 – 0x2FFF` | Operation errors |
| `0x3000 – 0x3FFF` | CAN TX/RX errors |
| `0x4000 – 0x4FFF` | Parameter / validation errors |
| `0x5000 – 0x5FFF` | ODrive device errors |
| `0x6000 – 0x6FFF` | Warnings (non-fatal) |

**Primary API (`ODrive.h`):**

```c
odrive_result_t odrive_create_driver(odrive_driver_t *driver, const odrive_driver_cfg_t *cfg);
odrive_result_t odrive_start_driver(odrive_driver_t *driver);
odrive_result_t odrive_stop_driver(odrive_driver_t *driver);
odrive_result_t odrive_poll_driver(odrive_driver_t *driver);   // call from main loop
odrive_result_t odrive_register_axis(odrive_driver_t *driver, uint16_t node_id, odrive_axis_t **axis_out);
odrive_result_t odrive_reboot_all(odrive_driver_t *driver);
```

---

### CAN Stack

**Location:** `firmware/drivers/CANStack/`

A lightweight, cross-platform CAN 2.0 library that abstracts hardware-specific CAN peripherals behind a single API. The PSoC5 implementation (`CanStackPSoC5.c`) is the active backend.

- 11-bit standard frame identifiers
- Configurable RX/TX mailboxes with callbacks
- Optional hardware message filtering
- Result codes mapped to `CanStackResult.h`

---

### Inverse Kinematics

**Location:** `firmware/drivers/IKMath/`

Analytical inverse kinematics solver for the robot's leg geometry. Computes joint angles from a desired end-effector position.

- Joint limit enforcement
- Forward and inverse kinematics
- Pluggable accelerator backend — defaults to CORDIC on PSoC5

---

### CORDIC Accelerator

**Location:** `firmware/platform/PSoC/PSoC-workspace/RobotDog.cydsn/bCORDIC/`

A custom Verilog module synthesised into the PSoC5's programmable logic. Offloads trigonometric calculations (sine, cosine, vector rotation) from the CPU to dedicated hardware, reducing IK computation overhead.

- FIFO-based job queue
- Synchronous and asynchronous operation modes
- Circular vector and rotation modes
- Accessible via the `CirV` C API

---

### Jetson AI & Perception

**Location:** `firmware/platform/Jetson/`

Two Python scripts run on the NVIDIA Jetson Orin Nano Super, powered at 12 V from the robot's Parkside 20 V battery via an LM2596 buck converter.

**`depth_zones.py`** — Reads a stereo camera, divides the field of view into spatial zones, and streams zone depth data to the PSoC5 over USB serial. Enables proximity-aware behaviour.

**`robot_dog_ai.py`** — Voice command recognition with a context-aware audio soundboard. Listens for spoken keywords and triggers categorised WAV responses:

| Category | Trigger |
|---|---|
| `compliment/` | Praise ("good boy", "nice") |
| `negative/` | Insults / threats |
| `meme/` | Recognised meme phrases |
| `command/` | Acknowledged robot commands |
| `confused/` | Unrecognised input |
| `idle/` | Periodic ambient sounds |

```bash
# Run without PSoC connected (test mode)
python3 firmware/platform/Jetson/robot_dog_ai.py --no-serial

# List available audio devices
python3 firmware/platform/Jetson/robot_dog_ai.py --list-devices
```

Requires: `libportaudio2`, `espeak`

---

### Tools

**Location:** `tools/`

**`capstan-cycle.py`** — Standalone Python test script for ODrive motor controllers. Runs a continuous positional sweep between two endpoints with configurable velocity, acceleration, and current limits. Useful for validating motor tuning and transmission behaviour before integrating with the full firmware.

```bash
# Requires an ODrive connected over USB
python3 tools/capstan-cycle.py
```

**Configurable parameters:**

| Parameter | Default | Description |
|---|---|---|
| `POS_LOW / POS_HIGH` | 0.1 / 0.9 turns | Sweep endpoints |
| `VEL_LIMIT` | 40 turns/s | Maximum velocity |
| `ACCEL_LIMIT` | 50 turns/s² | Maximum acceleration |
| `CURRENT_LIMIT` | 5 A | Torque limit |
| `DT_TARGET` | 1 ms | Control loop interval (~1 kHz) |

---

## Getting Started

### Firmware

Firmware is built using the **Cypress PSoC Creator** IDE. There is no CLI build system.

1. Open the workspace:
   ```
   firmware/platform/PSoC/PSoC-workspace/PSoC-workspace.cywrk
   ```
2. Select the `RobotDog` project
3. Build and program via PSoC Creator's built-in programmer

### Jetson scripts

```bash
# Stereo depth perception
python3 firmware/platform/Jetson/depth_zones.py

# Voice command + soundboard
python3 firmware/platform/Jetson/robot_dog_ai.py
```

### API documentation

```bash
# Generate Doxygen HTML from firmware source
doxygen docs/Doxyfile
```

---

## PDF Outputs

Pre-compiled PDFs are committed directly and do not require a Typst installation to view.

| Location | Contents |
|---|---|
| `docs/PDF-outputs/` | Main project document, peer review, individual reflections |
| `docs/PDF-outputs/research/` | Research papers — AI, FOC, ExpressLRS, IMU, Inverse Kinematics, Transmissie |
| `pcb-schematics/PDF/` | PCB schematics — Control Unit, Leg Driver, Power Distribution Board |

---

## Documentation

Project documentation is written in **Typst** (Dutch) and lives in the `feat/Documentation` branch under `docs/typst/`. It covers:

- Project plan, stakeholder analysis and persona research
- MoSCoW-prioritised requirements (PvE) and block diagram
- Technical prototype description (mechanics, electronics, firmware, Jetson AI)
- Literature research, transmission/capstan analysis and AI perception research
- Test results (capstan wear, robot leg)
- Project conclusion and individual reflections

```bash
# Compile to PDF
typst compile docs/typst/Documenten/Hoofddocument.typ
```

3D design files are maintained in OnShape:
[C.A.N.I.S. CAD — OnShape](https://cad.onshape.com/documents/de215c13f85991496b305af5/w/90d06f27be5cf5466781f660/e/65f4320ee7f6d17dd1840a11)

---

## Branches

Each feature branch contains a `README.md` inside its own feature directory describing the implementation in detail.

| Branch | Description | README |
|---|---|---|
| `main` | Integration branch | — |
| `driver/ODrive` | ODrive C driver | `firmware/drivers/ODrive/README.md` |
| `feat/CanStack` | CAN transport library | `firmware/drivers/CANStack/README.md` |
| `feat/IK-math` | Inverse kinematics solver | `firmware/drivers/IKMath/README.md` |
| `feat/CORDIC` | CORDIC hardware accelerator | `firmware/platform/PSoC/PSoC-workspace/RobotDog.cydsn/bCORDIC/README.md` |
| `feat/PSoC-CAN-Bus` | PSoC5 CAN peripheral setup | `firmware/platform/PSoC/PSoC-workspace/RobotDog.cydsn/README.md` |
| `feat/PSoC-workspace` | PSoC workspace configuration | `firmware/platform/PSoC/PSoC-workspace/README.md` |
| `feat/FreeRTOS` | FreeRTOS integration on PSoC5 | `firmware/platform/PSoC/PSoC-workspace/FreeRTOS_PSoC.cydsn/README.md` |
| `feat/ComplementaryFilter_MPU6050` | IMU driver + complementary filter | `firmware/platform/PSoC/PSoC-workspace/MPU6050_ComplementaryFilter.cydsn/README.md` |
| `feat/ExpressLRS_PWM_Receiver` | RC receiver PWM decoding | `firmware/platform/PSoC/PSoC-workspace/ExpressLRS_PWM_Receiver.cydsn/README.md` |
| `feat/ESP32_Bluetooth` | ESP32 Bluetooth bridge | `firmware/platform/Arduino/ESP32_WirelessCOM/README.md` |
| `feat/5Bar_linakge_Prototype` | Early 5-bar linkage prototype (deprecated — replaced by diamond linkage) | `firmware/platform/Arduino/Prototype_5-Bar_Linkage/README.md` |
| `feat/Jetson-AI` | Jetson AI scripts | `firmware/platform/Jetson/README.md` |
| `feat/3D-design` | 3D CAD designs | `3d-design/README.md` |
| `feat/Documentation` | Typst project documentation | `docs/README.md` |
| `tools/capstan-cycle` | ODrive motion test tool | `tools/README.md` |

---

*NHL Stenden University of Applied Sciences — BD.ELT — 2025/2026*
