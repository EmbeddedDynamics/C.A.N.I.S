# CORDIC Hardware Accelerator

A custom PSoC5 UDB component implementing a CORDIC hardware accelerator for trigonometric math.

## What it does

This feature implements a CORDIC (COordinate Rotation DIgital Computer) algorithm as a custom digital logic component for the Cypress PSoC5's Universal Digital Blocks (UDB). It accelerates the trigonometric operations needed by the inverse kinematics solver — specifically `atan2`, `sqrt(x² + y²)`, and `acos` — replacing slow software floating-point math with fixed-point hardware iteration.

The component uses circular vectoring mode and processes vectors in a queue. Results are retrieved via interrupt (`CORDIC_done_callback`) or by polling (`CORDIC_has_pending` / `CORDIC_get_data`).

**Fixed-point format:**
- Coordinates (X/Y): Q12.4 signed fixed-point — `float_mm * 16 = int_input`, 0.0625 mm resolution, ±2047 mm range
- Angles (Z): 16-bit Binary Angle Measurement System (BAMS) — full circle maps to 0x0000–0xFFFF, 0.00549° resolution

## Libraries / dependencies

- **bCORDIC** — custom PSoC5 UDB Verilog component (defined in `bCORDIC.v`), instantiated in PSoC Creator's TopDesign schematic
- PSoC Creator component API (`project.h`, `cypress.v`)

## File structure

```
firmware/platform/PSoC/PSoC-workspace/RobotDog.cydsn/bCORDIC/
├── bCORDIC.v           # Verilog RTL — CORDIC state machine implemented in UDB logic
├── bCORDIC.cysym       # PSoC Creator component symbol (schematic icon)
└── API/
    ├── CirV.c          # C driver for the CORDIC component (queue, ISR, result retrieval)
    └── CirV.h          # CORDIC C API (init, start, queue_data, get_data, has_pending)
```
