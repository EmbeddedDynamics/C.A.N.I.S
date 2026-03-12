# Jetson AI

This directory contains code and configuration for the Jetson Orin Nano Super, responsible for high-level AI tasks such as depth perception and voice interaction.

## Hardware

| Component | Model | Notes |
|---|---|---|
| SBC | NVIDIA Jetson Orin Nano Super | Main compute platform |
| Stereo camera | Waveshare IMX219-83 | Depth perception |
| Microphone | USB omnidirectional (AliExpress) | See specs below |
| USB audio adapter | USB to 3.5mm dongle | Provides analog audio out (Jetson dev kit has no onboard jack) |
| Speaker amplifier | Hema cardboard boombox amp (EPSR01901AA / XR-V002) | Driven from USB audio adapter 3.5mm out, 5V powered |
| Speakers | 2× 4Ω 3W (included with boombox kit) | Connected to amp, 20Hz–20kHz |

### Microphone specs
- Polar pattern: omnidirectional
- Sensitivity: -42dB ± 3dB
- S/N ratio: >60dB
- Frequency range: 100Hz–16kHz
- Range: recommended within 2m
- Interface: USB 2.0 (USB-A to Type-C)
- Power: 5V via USB

## Notes

### Audio signal sharing
The Jetson Orin Nano dev kit has no onboard 3.5mm audio jack. A USB audio adapter provides the analog output. The speaker amplifier is then driven via a **3.5mm Y-splitter** on the adapter's output. One branch feeds the amp, the other provides a line-level signal to the LED frequency visualizer project.

> **Note:** Tap the Y-splitter **before** the amp input, not after. This keeps the LED project's signal level independent of the boombox volume knob.

### Echo cancellation
The microphone has no hardware AEC. If speaker-to-mic echo becomes an issue during voice interaction, enable software AEC via PulseAudio:
```bash
pactl load-module module-echo-cancel use_master_format=1 aec_method=webrtc
```
