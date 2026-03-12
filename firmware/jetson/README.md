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

## Scripts

| Script | Description |
|---|---|
| `depth_zones.py` | Stereo depth perception — sends zone disparity values to PSoC5 over USB serial |
| `robot_dog_ai.py` | Voice command recognition + context-aware soundboard — listens for commands, sends them to PSoC5, reacts with sounds based on what it hears |

## Dependencies

### System packages
```bash
sudo apt install -y python3-pip espeak espeak-data libespeak1 portaudio19-dev libportaudio2
```

### Python packages

#### `depth_zones.py`
| Package | Install | Notes |
|---|---|---|
| `opencv-python` | `pip3 install opencv-python` | Stereo capture and disparity |
| `numpy` | `pip3 install numpy` | Array math |
| `pyserial` | `pip3 install pyserial` | USB serial to PSoC5 |

#### `robot_dog_ai.py`
| Package | Install | Notes |
|---|---|---|
| `faster-whisper` | `pip3 install faster-whisper` | Speech recognition (pulls in CTranslate2) |
| `sounddevice` | `pip3 install sounddevice` | Microphone capture (requires `libportaudio2`) |
| `numpy` | `pip3 install numpy` | Audio buffer math |
| `pyserial` | `pip3 install pyserial` | USB serial to PSoC5 |
| `pyttsx3` | `pip3 install pyttsx3` | TTS fallback via espeak |
| `pygame` | `pip3 install pygame` | WAV soundboard playback (optional) |

Install all at once:
```bash
pip3 install faster-whisper sounddevice numpy pyserial pyttsx3 pygame
```

> **Note on `faster-whisper` on Jetson/ARM64:** if the install fails, try:
> ```bash
> pip3 install ctranslate2 --extra-index-url https://pypi.nvidia.com
> pip3 install faster-whisper
> ```

## Notes

### Audio signal sharing
The Jetson Orin Nano dev kit has no onboard 3.5mm audio jack. A USB audio adapter provides the analog output. The speaker amplifier is then driven via a **3.5mm Y-splitter** on the adapter's output. One branch feeds the amp, the other provides a line-level signal to the LED frequency visualizer project.

> **Note:** Tap the Y-splitter **before** the amp input, not after. This keeps the LED project's signal level independent of the boombox volume knob.

### Echo cancellation
The microphone has no hardware AEC. If speaker-to-mic echo becomes an issue during voice interaction, enable software AEC via PulseAudio:
```bash
pactl load-module module-echo-cancel use_master_format=1 aec_method=webrtc
```
