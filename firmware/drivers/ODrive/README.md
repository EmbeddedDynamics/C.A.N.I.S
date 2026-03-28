# ODrive Driver

A C driver for ODrive motor controllers, communicating over CAN bus.

## What it does

This driver provides a high-level interface to control ODrive motor controllers from embedded firmware. It handles the full ODrive CAN protocol — sending commands (set velocity, set position, set torque, request state changes) and receiving responses (heartbeat, encoder estimates, etc.).

The driver is transport-agnostic: it communicates through a backend vtable (`ODriveBackend`), so it can work over CAN, UART, USB, or SPI without any changes to the driver logic. The `ODriveCanStack` adapter connects it to CanStack for CAN bus operation on the PSoC5.

Key features:
- Supports up to 12 ODrive axes per driver instance
- Per-axis heartbeat monitoring with timeout detection
- Driver state machine: `UNKNOWN → UNINITIALIZED → IDLE → STOPPED → RUNNING → REBOOTING`
- Little-endian message serialization matching the ODrive CAN protocol

## Libraries / dependencies

- **CanStack** — CAN transport layer (required for `ODriveCanStack` adapter)
- Standard C headers: `<stdint.h>`, `<stdbool.h>`
- No dynamic memory allocation

## File structure

```
firmware/drivers/ODrive/
├── include/
│   ├── ODrive.h              # Public API — driver creation, control, state management
│   ├── ODriveAxis.h          # Per-axis control interface (velocity, position, torque, state)
│   ├── ODriveCore.h          # Protocol types, enums, and constants (1913 lines)
│   ├── ODriveResult.h        # Error and result code definitions
│   └── driver/
│       ├── ODriveBackend.h   # Transport HAL — vtable interface for swappable backends
│       └── CanStack/
│           └── ODriveCanStack.h  # CanStack CAN bus adapter
└── src/
    ├── ODriveAxis.c              # Axis control implementation
    ├── ODriveInternal.c          # Core driver logic and message handling
    ├── ODriveInternal.h          # Internal types and serialization macros
    └── driver/
        └── CanStack/
            └── ODriveCanStack.c  # CanStack adapter implementation
```
