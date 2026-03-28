# Complementary Filter — MPU6050

PSoC5 firmware for IMU-based attitude estimation using the MPU6050 and a complementary filter.

## What it does

This project reads roll, pitch, and yaw angles from an MPU6050 IMU (accelerometer + gyroscope) connected to the PSoC5 over I2C. A complementary filter fuses the gyroscope integration (good short-term, drifts over time) with the accelerometer angle estimate (good long-term, noisy short-term) to produce stable angle readings.

The filter runs at 250 Hz, driven by a hardware timer ISR (4 ms period). On startup, the gyroscope is calibrated by averaging readings at rest. The built-in LED lights up when any axis exceeds ±45°, providing a simple tilt alarm.

**MPU6050 I2C address:** `0x68`

## Libraries / dependencies

- No external libraries — MPU6050 driver is implemented directly in `MPU6050_ComplentaryFilter.c/.h`
- PSoC Creator I2C component (`project.h`) for the hardware I2C peripheral
- `<math.h>` — `atan2`, `sqrt` for accelerometer angle calculation

## File structure

```
firmware/platform/PSoC/PSoC-workspace/
└── MPU6050_ComplementaryFilter.cydsn/
    ├── MPU6050_ComplementaryFilter.cyprj       # PSoC Creator project — I2C + timer components
    ├── TopDesign/
    │   └── TopDesign.cysch                     # Schematic — I2C and timer placed here
    ├── MPU6050_ComplentaryFilter.c             # MPU6050 driver + complementary filter implementation
    ├── MPU6050_ComplentaryFilter.h             # Public API — init, calibrate, read angle
    └── main.c                                  # Firmware entry point — 250 Hz filter loop
```
