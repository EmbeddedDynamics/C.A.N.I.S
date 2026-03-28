# IK Math

An inverse kinematics math library for the Robothond leg mechanism.

## What it does

This library computes joint angles from a desired end-effector position (inverse kinematics) for the robot leg geometry. It supports both analytic and numeric solving modes, and is designed for use on the PSoC5 embedded platform.

The library includes a CORDIC hardware accelerator backend (`ik_cordic`) that offloads trigonometric computations (`atan2`, `sqrt`, `acos`) to the PSoC5's UDB-based CORDIC component, replacing expensive floating-point math with fixed-point hardware iteration.

Supported solvers:
- **Analytic IK** — closed-form solution for supported geometries
- **Numeric IK** — iterative solvers: Jacobian Transpose, Pseudo-Inverse, DLS (Damped Least Squares)
- **Forward kinematics** — compute end-effector position from joint angles

## Libraries / dependencies

- **bCORDIC** (PSoC5 UDB component) — hardware CORDIC accelerator, used by the `ik_cordic` backend
- Standard C headers: `<stdint.h>`, `<stdbool.h>`
- No dynamic memory allocation

## File structure

```
firmware/drivers/IKMath/
├── include/
│   ├── ik.h                          # Public API — solver interface, model capabilities, feature flags
│   ├── ik_config.h                   # Compile-time configuration
│   ├── ik_results.h                  # Result/error codes
│   ├── ik_types.h                    # Shared types (vectors, joint angles, etc.)
│   └── accelerators/
│       ├── ik_accelerators.h         # Accelerator backend interface
│       └── ik_cordic.h               # CORDIC accelerator public API
└── src/
    └── accelerators/
        ├── ik_cordic.c               # CORDIC accelerator implementation (platform-agnostic)
        ├── ik_cordic_internal.h      # Internal CORDIC types
        └── ik_cordic_psoc5.c         # PSoC5-specific CORDIC hardware interface
```
