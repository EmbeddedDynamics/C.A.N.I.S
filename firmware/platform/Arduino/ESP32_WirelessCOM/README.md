# ESP32 Bluetooth Controller

An ESP32-based wireless gamepad bridge — reads an Xbox/PlayStation controller over Bluetooth and forwards the input to the PSoC5 over I2C.

## What it does

The ESP32 runs the Bluepad32 library to connect Bluetooth gamepads (Xbox, PlayStation, Nintendo, etc.). On each update, it packs the controller state — two joystick axes, two triggers, button bitmask, D-pad, and misc buttons — into an 18-byte struct and sends it to the PSoC5 over I2C (ESP32 = master at address `0x20`, PSoC5 = slave).

This provides the robot with wireless manual control via a standard consumer gamepad.

**I2C configuration:** SDA on GPIO 8, SCL on GPIO 9, 400 kHz fast mode.

## Libraries / dependencies

- **[Bluepad32](https://bluepad32.readthedocs.io/)** — Bluetooth gamepad library for ESP32 (install via Arduino Library Manager or PlatformIO)
- **`Wire.h`** — Arduino I2C library (included with the ESP32 Arduino core)

## File structure

```
firmware/platform/Arduino/ESP32_WirelessCOM/
└── ESP32_WirelessCOM_Bluetooth_ExpressLRS/
    └── ESP32_WirelessCOM_Bluetooth_ExpressLRS.ino   # ESP32 sketch — Bluetooth gamepad → I2C bridge

firmware/platform/PSoC/PSoC-workspace/
└── PSoC5_Bluetooth.cydsn/                           # PSoC5 project — I2C slave receiver
    ├── PSoC5_Bluetooth.cyprj                        # PSoC Creator project file
    ├── TopDesign/
    │   └── TopDesign.cysch                          # Schematic — I2C component wired here
    └── main.c                                       # PSoC5 firmware — receives and processes I2C data
```
