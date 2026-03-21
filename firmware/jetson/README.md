# Jetson AI

This directory contains code and configuration for the Jetson Orin Nano Super, responsible for high-level AI tasks such as depth perception and voice interaction.

## Hardware

| Component | Model | Notes |
|---|---|---|
| SBC | NVIDIA Jetson Orin Nano Super | Main compute platform — powered via LM2596 buck converter at 12V |
| Power supply | Parkside 20V Li-ion battery | Direct connection not safe (up to 21V fully charged, Jetson max 20V) |
| Voltage regulator | LM2596-based DC-DC buck converter | Steps down battery voltage to 12V, max 3A output |
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

### robot_dog_ai.py — Usage

```bash
python3 robot_dog_ai.py                       # auto-detect serial, default mic
python3 robot_dog_ai.py --no-serial           # test without PSoC connected
python3 robot_dog_ai.py --list-devices        # list audio device indices and exit
python3 robot_dog_ai.py --audio-device 2      # use mic at device index 2
python3 robot_dog_ai.py --output-device 3     # use speaker at device index 3
python3 robot_dog_ai.py --serial /dev/ttyACM1 # override serial port
python3 robot_dog_ai.py --model tiny          # faster but less accurate Whisper model
python3 robot_dog_ai.py --ai                  # enable Ollama AI classifier
python3 robot_dog_ai.py --threshold 0.02      # adjust mic sensitivity
```

### robot_dog_ai.py — Workflow

```
Microphone (USB)
      │
      ▼
VAD (RMS threshold)          ← filters silence, only captures speech
      │
      ▼
faster-whisper (Whisper STT) ← transcribes speech segment to text
      │
      ▼
┌──────────────────────────────────────────────────┐
│               Intent classification              │
│                                                  │
│  1. Mode switch?   → switch_mode()               │
│     e.g. "go funny mode"                         │
│                                                  │
│  2. Robot command? → detect_command()            │
│     e.g. "sit", "dance", "spin"                  │
│     (always keyword-based, reliable)             │
│                                                  │
│  3. AI classifier  → Ollama LLM  (--ai flag)     │
│     or keyword fallback → classify_input()       │
│     → category: negative / compliment /          │
│                 meme / command / confused        │
└──────────────────────────────────────────────────┘
      │                          │
      ▼                          ▼
Serial → PSoC5              SoundBoard
CMD:SIT\n                        │
                     ┌───────────┴───────────┐
                     ▼                       ▼
              WAV playback             Piper TTS
          sounds/<mode>/<cat>/      (neural voice)
          (pygame.mixer)            or espeak fallback
```

**Response types** (decided per-reaction by the AI, or by mode rules):

| Type | Behavior |
|---|---|
| `audio` | Play WAV sound effect only |
| `speech` | Speak TTS response only |
| `both` | Play WAV, then speak TTS response |

**Sound categories:**

| Category | Trigger |
|---|---|
| `negative` | Insults or mean comments directed at the robot |
| `compliment` | Praise or kind words |
| `meme` | Brainrot / internet meme keywords; also used as generic reaction in non-default modes |
| `command` | Acknowledgment after a movement command is sent |
| `confused` | Input not understood |
| `idle` | Periodic unprompted sounds (every 45s by default) |

### robot_dog_ai.py — Modes

Switch mode by saying `"go <name> mode"` (e.g. `"go funny mode"`, `"go default mode"`).

| Mode | Generic reactions use | Talking |
|---|---|---|
| `default` | `sounds/idle/` | Yes (TTS enabled) |
| `funny` | `sounds/meme/` | Yes (TTS enabled) |
| `uncensored` | `sounds/uncensored/meme/` → `sounds/meme/` | No (audio only by default) |

Custom modes can be added by creating a `sounds/<name>/` directory with category subdirectories inside.

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
| `piper-tts` | `pip3 install piper-tts` | Neural TTS (preferred over pyttsx3, optional) |
| `ollama` | `pip3 install ollama` | LLM integration, required for `--ai` mode (optional) |

Install all at once:
```bash
pip3 install faster-whisper sounddevice numpy pyserial pyttsx3 pygame piper-tts ollama
```

> **Note on `faster-whisper` on Jetson/ARM64:** if the install fails, try:
> ```bash
> pip3 install ctranslate2 --extra-index-url https://pypi.nvidia.com
> pip3 install faster-whisper
> ```

### Piper TTS (optional, replaced espeak)

Piper is a neural TTS engine that sounds significantly more natural than espeak.
It runs fully offline and is fast enough for real-time use on the Jetson.

**Install:**
```bash
pip3 install piper-tts
```

**Download a voice** (each voice is an `.onnx` + `.onnx.json` file pair).
Voices are stored in `firmware/jetson/piper-voices/` inside the project:
```bash
cd firmware/jetson/piper-voices

# en_US-ryan-high (currently configured in robot_dog_ai.py)
wget https://huggingface.co/rhasspy/piper-voices/resolve/main/en/en_US/ryan/high/en_US-ryan-high.onnx
wget https://huggingface.co/rhasspy/piper-voices/resolve/main/en/en_US/ryan/high/en_US-ryan-high.onnx.json
```

**Other recommended voices:**

| Voice | Style | Download path |
|---|---|---|
| `en_US-ryan-high` | American male, natural | `en/en_US/ryan/high/` |
| `en_US-amy-medium` | American female, natural | `en/en_US/amy/medium/` |
| `en_US-lessac-high` | American female, high quality | `en/en_US/lessac/high/` |
| `en_GB-alan-medium` | British male | `en/en_GB/alan/medium/` |

All voice files follow the same URL pattern:
```
https://huggingface.co/rhasspy/piper-voices/resolve/main/<path>/<voice>.onnx
https://huggingface.co/rhasspy/piper-voices/resolve/main/<path>/<voice>.onnx.json
```

**Switch voices** by updating `PIPER_VOICE` at the top of `robot_dog_ai.py`:
```python
PIPER_VOICE = os.path.expanduser("~/piper-voices/en_US-amy-medium.onnx")
```

The script automatically uses Piper if the `.onnx` file exists, and falls back to espeak if not.

### Ollama (optional, for `--ai` mode)

Ollama runs a local LLM on the Jetson to replace keyword-based classification with
context-aware intent detection. Required only when running `robot_dog_ai.py --ai`.

**Install Ollama:**
```bash
curl -fsSL https://ollama.com/install.sh | sh
```

Ollama runs as a systemd service and starts automatically on boot. It listens on `localhost:11434`.

**Recommended models:**

| Model | Size | Speed on Jetson | Notes |
|---|---|---|---|
| `qwen2.5:1.5b` | ~1GB | ~0.5–1s | Recommended — fastest, sufficient for classification |
| `gemma2:2b` | ~1.6GB | ~1–2s | Backup — better reasoning, slightly slower |

```bash
# Pull recommended model
ollama pull qwen2.5:1.5b

# Pull backup model
ollama pull gemma2:2b
```

**Switch models** by changing the `AI_MODEL` constant at the top of `robot_dog_ai.py`:
```python
AI_MODEL = "qwen2.5:1.5b"   # or "gemma2:2b"
```

**Manage Ollama:**
```bash
ollama list                  # list downloaded models
ollama ps                    # show currently loaded model
sudo systemctl status ollama # check service status
sudo systemctl restart ollama
```

## Notes

### Audio signal sharing
The Jetson Orin Nano dev kit has no onboard 3.5mm audio jack. A USB audio adapter provides the analog output. The speaker amplifier is then driven via a **3.5mm Y-splitter** on the adapter's output. One branch feeds the amp, the other provides a line-level signal to the LED frequency visualizer project.

> **Note:** Tap the Y-splitter **before** the amp input, not after. This keeps the LED project's signal level independent of the boombox volume knob.

### Echo cancellation
The microphone has no hardware AEC. If speaker-to-mic echo becomes an issue during voice interaction, enable software AEC via PulseAudio:
```bash
pactl load-module module-echo-cancel use_master_format=1 aec_method=webrtc
```
