# CanStack

A cross-platform CAN 2.0 transport library for the Robothond project.

## What it does

CanStack abstracts platform-specific CAN hardware behind a unified C API. It provides RX/TX mailboxes, callback-based message reception, and optional hardware filtering. The library is the transport layer that sits below the ODrive driver — it handles raw CAN frame I/O so that higher-level drivers don't need to know which microcontroller they're running on.

The PSoC5 platform adapter (`CanStackPSoC5`) is included. The design allows future ports to other platforms (STM32, etc.) by implementing a new platform file.

## Libraries / dependencies

- No external libraries — uses only the Cypress PSoC Creator component API (`project.h`) for the PSoC5 platform adapter
- Standard C headers: `<stdint.h>`, `<stdbool.h>`, `<stddef.h>`, `<string.h>`

## File structure

```
firmware/drivers/CANStack/
├── include/
│   ├── CanStack.h          # Public API — driver creation, TX, RX, filtering
│   ├── CanStackConfig.h    # Compile-time configuration (mailbox counts, payload size, platform selection)
│   └── CanStackResult.h    # Result/error codes
└── src/
    ├── CanStack.c          # Core driver logic
    ├── CanStackInternal.h  # Internal types not exposed in the public API
    └── platform/
        ├── CanStackPSoC5.c # PSoC5 platform adapter (CAN peripheral setup, ISR)
        └── CanStackPSoC5.h # PSoC5 platform internal header
```
