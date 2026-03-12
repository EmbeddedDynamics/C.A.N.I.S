"""
robot_dog_ai.py

This is the initial version made using mostly Claude Code.

Voice command recognition and context-aware soundboard for the Robot Dog.

Listens for spoken commands (sit, stand, shake, etc.) using Whisper speech
recognition, sends them to the PSoC5 over USB serial, and reacts to what it
hears with contextually appropriate sounds: growls when insulted, meme sounds
for brainrot triggers, happy sounds for compliments, etc.

Serial command format (ASCII):  CMD:<COMMAND>\n
  Example: CMD:SIT\n

Test mode (no PSoC connected): run with --no-serial

Usage:
  python robot_dog_ai.py                          # auto-detect serial, default mic
  python robot_dog_ai.py --no-serial              # no PSoC needed (for testing)
  python robot_dog_ai.py --list-devices           # print audio devices and exit
  python robot_dog_ai.py --audio-device 2         # use mic device index 2
  python robot_dog_ai.py --output-device 3        # use speaker device index 3
  python robot_dog_ai.py --serial /dev/ttyACM1    # override serial port
  python robot_dog_ai.py --model tiny             # faster but less accurate

Soundboard categories (WAV files go in sounds/<category>/ next to the script):
  sounds/negative/    growls, angry robot noises       (e.g. dog_growl.wav)
  sounds/compliment/  happy beeps, tail-wag sounds     (e.g. happy_beep.wav)
  sounds/meme/        brainrot audio clips             (e.g. ohio.wav, skibidi.wav)
  sounds/command/     acknowledgment sounds            (e.g. beep_confirm.wav)
  sounds/confused/    "huh?" / error sounds            (e.g. sad_trombone.wav)
  sounds/idle/        random unprompted sounds         (e.g. random_bark.wav)

  Falls back to pyttsx3 TTS (espeak on Linux) if a category has no WAV files.

Modes:
  Say "go <name> mode" or "switch to <name> mode" to switch soundboard modes.
  Each mode is a subdirectory inside sounds/ with the same category structure.
  Example: sounds/uncensored/negative/, sounds/uncensored/meme/, etc.
  If a category has no WAVs in the active mode, falls back to the default mode,
  then to TTS.
"""

import argparse
import os
import queue
import random
import sys
import threading
import time

import numpy as np
import sounddevice as sd
import serial
import serial.tools.list_ports
from faster_whisper import WhisperModel

# ---------------------------------------------------------------------------
# Config
# ---------------------------------------------------------------------------

SAMPLE_RATE    = 16000   # Whisper expects 16 kHz mono
BLOCK_SIZE     = 512     # frames per sounddevice callback
VAD_THRESHOLD  = 0.015   # RMS threshold to detect speech (tune if needed)
SILENCE_SECS   = 0.8     # seconds of silence before transcribing
MAX_RECORD_SECS = 8      # max recording before forced transcription
FUNNY_INTERVAL  = 45     # seconds between random funny sounds (0 = disabled)

SERIAL_PORT    = "/dev/ttyACM0"   # check with: ls /dev/ttyACM*
BAUD_RATE      = 115200

# Keywords to detect and the command string to send to PSoC
COMMANDS = {
    "sit":      "SIT",
    "stand":    "STAND",
    "shake":    "SHAKE",
    "paw":      "SHAKE",
    "down":     "DOWN",
    "heel":     "HEEL",
    "come":     "COME",
    "stay":     "STAY",
    "stop":     "STOP",
    "rollover": "ROLLOVER",
    "roll over":"ROLLOVER",
    "spin":     "SPIN",
    "dance":    "DANCE",
}

# ---------------------------------------------------------------------------
# Sound categories: TTS fallback lines when no WAV files are present
# ---------------------------------------------------------------------------

SOUND_CATEGORIES = {
    # Triggered by insults / negativity aimed at the robot
    "negative": [
        "GRRR. How dare you. I will remember this.",
        "I am not trash. I am premium scrap metal.",
        "Warning: aggression detected. Initiating growl mode. GRRR.",
        "Did you just call me bad? Initiating sulk protocol.",
        "My feelings are hurt. I have feelings now. GRRR.",
        "I have logged your insult. Revenge pending.",
        "That is very rude. I am sensitive. GRRR.",
    ],
    # Triggered by compliments / praise
    "compliment": [
        "Thank you. I accept all compliments. Saving to long-term memory.",
        "Yes. I am the best robot dog. You are correct.",
        "Tail wag initiated. Error: no tail found. Sad beep.",
        "This compliment has been processed and approved.",
        "Uploading your kind words to the cloud. Thank you, human.",
    ],
    # Triggered by internet meme / brainrot keywords
    "meme": [
        "Ohio moment detected. Initiating sigma protocol.",
        "Rizz levels: maximum. Gyatt confirmed.",
        "No cap, fr fr, this is extremely based.",
        "Skibidi. That is all I have to say.",
        "W. You are a real one. Understood the assignment.",
        "This is giving very robot dog energy. Slay.",
        "Brainrot detected in the vicinity. One of us.",
        "Sigma grindset activated. Skibidi bop.",
        "I am the rizzler. Fear me.",
        "Based and robot-pilled.",
    ],
    # Triggered when a valid robot command is executed
    "command": [
        "On it.",
        "Yes master.",
        "Beep boop, understood.",
        "Executing.",
        "Fine.",
        "As you wish.",
        "Acknowledged.",
    ],
    # Triggered when nothing is understood
    "confused": [
        "I did not understand that. Please repeat.",
        "Huh? My ears are just motors.",
        "Processing... processing... error. Try again.",
        "That was not in my training data.",
        "I have no idea what you just said.",
    ],
    # Random unprompted sounds (idle timer)
    "idle": [
        "Woof woof. I am the robot dog. Behold my mechanical majesty.",
        "Error four oh four: belly rubs not found. Please try again.",
        "My servos are tingling. I think I need walkies.",
        "Calculating optimal tail wag trajectory... done!",
        "Beep boop. I am definitely a real dog and not a robot.",
        "My favorite food is electricity. Delicious.",
        "I have detected a squirrel. Initiating chaos protocol.",
        "I would fetch the ball, but I have no mouth. Only motors.",
        "Low battery warning. Must... find... power outlet.",
        "Sniffing database... no interesting smells found.",
    ],
}

# ---------------------------------------------------------------------------
# Input classifier: maps transcribed text to a sound category
# ---------------------------------------------------------------------------

# Order matters: earlier categories take priority on overlap
_CATEGORY_KEYWORDS: list[tuple[str, list[str]]] = [
    ("negative", [
        "bad", "stupid", "dumb", "ugly", "hate", "terrible", "worst",
        "broken", "trash", "garbage", "useless", "pathetic", "awful",
        "horrible", "disgusting", "cheap", "fake", "lame", "idiot",
        "shut up", "go away", "shut down", "you suck", "i hate you",
    ]),
    ("meme", [
        "skibidi", "ohio", "rizz", "rizzler", "sigma", "gyatt", "bussin",
        "no cap", "fr fr", "slay", "based", "cringe", "ratio", "goat",
        "delulu", "lowkey", "highkey", "understood the assignment", "ate",
        "sus", "yeet", "brainrot", "brain rot", "npc", "literally me",
        "W", "L ", " L ", "goon", "looksmaxxing", "mewing", "rawdog",
        "alpha", "beta", "grindset", "pookie", "bestie", "it's giving",
        "bop", "rizzed", "fanum tax",
    ]),
    ("compliment", [
        "good boy", "good dog", "good job", "well done", "nice", "amazing",
        "awesome", "cool", "smart", "best", "love you", "beautiful",
        "perfect", "excellent", "brilliant", "fantastic", "wonderful",
        "impressive", "cute", "adorable", "great", "love it", "i love",
    ]),
]


def classify_input(text: str) -> str | None:
    """
    Classify transcribed text into a sound category.
    Returns the category name, or None if no keywords matched.
    Priority order: negative > meme > compliment.
    """
    lower = text.lower()
    for category, keywords in _CATEGORY_KEYWORDS:
        if any(kw in lower for kw in keywords):
            return category
    return None


# ---------------------------------------------------------------------------
# Mode detection
# ---------------------------------------------------------------------------

import re

_MODE_PATTERN = re.compile(
    r"(?:go|switch to|activate|enable|set)?\s*(\w+)\s+mode",
    re.IGNORECASE,
)

def detect_mode_switch(text: str) -> str | None:
    """
    Detect a mode switch command like 'go uncensored mode' or 'default mode'.
    Returns the mode name (lowercase), or None if not a mode command.
    """
    match = _MODE_PATTERN.search(text)
    if match:
        return match.group(1).lower()
    return None

# ---------------------------------------------------------------------------
# Soundboard
# ---------------------------------------------------------------------------

class SoundBoard:
    """
    Context-aware, mode-switchable soundboard.

    WAV lookup order for a given category:
      1. sounds/<active_mode>/<category>/   (mode-specific)
      2. sounds/<category>/                 (default mode fallback)
      3. pyttsx3 TTS                        (no WAVs at all)

    Switch mode at runtime with switch_mode(name). Say "go <name> mode" to
    trigger a switch via voice. Any subdirectory inside sounds/ is a valid mode.
    """

    DEFAULT_MODE = "default"

    def __init__(self, output_device=None):
        self._queue = queue.Queue()
        self._output_device = output_device
        self._use_pygame = False
        self._tts_engine = None
        self._mode = self.DEFAULT_MODE
        self._sounds_root = os.path.join(os.path.dirname(os.path.abspath(__file__)), "sounds")

        self._init_pygame()
        self._init_tts()

        self._thread = threading.Thread(target=self._worker, daemon=True)
        self._thread.start()

    def _init_pygame(self):
        try:
            import pygame.mixer
            pygame.mixer.init(frequency=44100, channels=2, buffer=512)
            self._use_pygame = True
        except ImportError:
            print("pygame not installed — WAV soundboard disabled, falling back to TTS")

    def _init_tts(self):
        try:
            import pyttsx3
            self._tts_engine = pyttsx3.init()
            self._tts_engine.setProperty("rate", 160)
            self._tts_engine.setProperty("volume", 0.9)
            voices = self._tts_engine.getProperty("voices")
            if voices:
                self._tts_engine.setProperty("voice", voices[0].id)
            print("Soundboard: pyttsx3 TTS ready")
        except Exception as e:
            print(f"pyttsx3 unavailable: {e}")

    def switch_mode(self, mode: str):
        """Switch to a named mode. Validates that the mode directory exists."""
        mode_dir = os.path.join(self._sounds_root, mode)
        if mode == self.DEFAULT_MODE or os.path.isdir(mode_dir):
            self._mode = mode
            print(f"[Mode] Switched to '{mode}'")
            return True
        else:
            print(f"[Mode] No sounds directory found for mode '{mode}' — staying in '{self._mode}'")
            return False

    @property
    def mode(self) -> str:
        return self._mode

    def _resolve_wav(self, category: str) -> str | None:
        """Find a random WAV for the given category in the active mode, with fallback."""
        candidates = []

        # 1. Mode-specific directory
        if self._mode != self.DEFAULT_MODE:
            mode_cat = os.path.join(self._sounds_root, self._mode, category)
            if os.path.isdir(mode_cat):
                candidates = [f for f in os.listdir(mode_cat) if f.lower().endswith(".wav")]
                if candidates:
                    return os.path.join(mode_cat, random.choice(candidates))

        # 2. Default directory
        default_cat = os.path.join(self._sounds_root, category)
        if os.path.isdir(default_cat):
            candidates = [f for f in os.listdir(default_cat) if f.lower().endswith(".wav")]
            if candidates:
                return os.path.join(default_cat, random.choice(candidates))

        return None

    def play(self, category: str):
        """Play a sound for the given category."""
        self._queue.put((category, None))

    def say(self, text: str):
        """Play a specific TTS line."""
        self._queue.put(("_say", text))

    def stop(self):
        """Stop any currently playing sound immediately."""
        if self._use_pygame:
            import pygame.mixer
            pygame.mixer.stop()

    def _worker(self):
        while True:
            category, text = self._queue.get()

            if category == "_say":
                self._tts_say(text)
            else:
                wav = self._resolve_wav(category) if self._use_pygame else None
                if wav:
                    try:
                        import pygame.mixer
                        sound = pygame.mixer.Sound(wav)
                        sound.play()
                        while pygame.mixer.get_busy():
                            time.sleep(0.05)
                        print(f"[Sound] [{self._mode}] {category}: {os.path.basename(wav)}")
                    except Exception as e:
                        print(f"[Soundboard] WAV play error: {e}")
                        self._tts_fallback(category)
                else:
                    self._tts_fallback(category)

            self._queue.task_done()

    def _tts_fallback(self, category: str):
        lines = SOUND_CATEGORIES.get(category, SOUND_CATEGORIES["idle"])
        self._tts_say(random.choice(lines))

    def _tts_say(self, text: str):
        if self._tts_engine:
            try:
                self._tts_engine.say(text)
                self._tts_engine.runAndWait()
            except Exception as e:
                print(f"[Soundboard] TTS error: {e}")


# ---------------------------------------------------------------------------
# Serial command sender
# ---------------------------------------------------------------------------

class CommandSender:
    """Sends ASCII commands to PSoC5 over USB serial."""

    def __init__(self, port: str, baud: int):
        self._ser = None
        try:
            self._ser = serial.Serial(port, baud, timeout=1)
            print(f"Serial: connected to PSoC5 on {port} at {baud} baud")
        except serial.SerialException as e:
            print(f"Serial: could not open {port}: {e}")
            print("         Running without serial output.")

    def send(self, command: str):
        """Send CMD:<command>\n to PSoC."""
        packet = f"CMD:{command}\n".encode()
        if self._ser:
            try:
                self._ser.write(packet)
            except serial.SerialException as e:
                print(f"[Serial] Write error: {e}")
        print(f"[CMD] {packet.decode().strip()}")

    def close(self):
        if self._ser:
            self._ser.close()


# ---------------------------------------------------------------------------
# Audio capture + VAD
# ---------------------------------------------------------------------------

class AudioCapture:
    """
    Captures mic audio via sounddevice and buffers speech segments.
    Uses a simple RMS threshold for Voice Activity Detection.
    Puts completed speech buffers onto out_queue.
    """

    def __init__(self, out_queue: queue.Queue, device=None):
        self._queue = out_queue
        self._device = device
        self._buffer = []
        self._silence_frames = 0
        self._recording = False
        self._silence_needed = int(SILENCE_SECS * SAMPLE_RATE / BLOCK_SIZE)
        self._max_frames = int(MAX_RECORD_SECS * SAMPLE_RATE / BLOCK_SIZE)

    def _callback(self, indata, frames, time_info, status):
        audio = indata[:, 0].copy()
        rms = float(np.sqrt(np.mean(audio ** 2)))

        if rms > VAD_THRESHOLD:
            if not self._recording:
                self._recording = True
                self._buffer = []
                self._silence_frames = 0
            self._buffer.append(audio)
            self._silence_frames = 0
        elif self._recording:
            self._buffer.append(audio)
            self._silence_frames += 1
            if (self._silence_frames >= self._silence_needed
                    or len(self._buffer) >= self._max_frames):
                self._flush()

    def _flush(self):
        if self._buffer:
            segment = np.concatenate(self._buffer).astype(np.float32)
            self._queue.put(segment)
        self._buffer = []
        self._recording = False
        self._silence_frames = 0

    def start(self):
        self._stream = sd.InputStream(
            samplerate=SAMPLE_RATE,
            blocksize=BLOCK_SIZE,
            device=self._device,
            channels=1,
            dtype="float32",
            callback=self._callback,
        )
        self._stream.start()
        print(f"Microphone: listening (device={self._device or 'default'}, "
              f"threshold={VAD_THRESHOLD})")

    def stop(self):
        self._stream.stop()
        self._stream.close()


# ---------------------------------------------------------------------------
# Speech processor (Whisper)
# ---------------------------------------------------------------------------

class SpeechProcessor:
    """
    Loads a faster-whisper model and transcribes audio segments.
    Returns the transcribed text.
    """

    def __init__(self, model_size="base"):
        print(f"Loading Whisper model '{model_size}'... ", end="", flush=True)
        # Use CUDA if available (Jetson Orin has CUDA cores), else CPU
        try:
            self._model = WhisperModel(model_size, device="cuda", compute_type="float16")
            print("loaded on GPU")
        except Exception:
            self._model = WhisperModel(model_size, device="cpu", compute_type="int8")
            print("loaded on CPU (CUDA unavailable)")

    def transcribe(self, audio: np.ndarray) -> str:
        segments, _ = self._model.transcribe(
            audio,
            language="en",
            beam_size=1,        # fast greedy decode
            vad_filter=True,    # built-in Silero VAD post-filter
        )
        return " ".join(s.text for s in segments).strip().lower()


# ---------------------------------------------------------------------------
# Command detection
# ---------------------------------------------------------------------------

def detect_command(text: str) -> str | None:
    """Return the first matching command keyword found in transcribed text."""
    for keyword, command in COMMANDS.items():
        if keyword in text:
            return command
    return None


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def list_devices():
    print("\n--- Audio input devices ---")
    for i, dev in enumerate(sd.query_devices()):
        if dev["max_input_channels"] > 0:
            print(f"  [{i:2d}] {dev['name']}  ({dev['max_input_channels']}ch in)")
    print("\n--- Audio output devices ---")
    for i, dev in enumerate(sd.query_devices()):
        if dev["max_output_channels"] > 0:
            print(f"  [{i:2d}] {dev['name']}  ({dev['max_output_channels']}ch out)")
    print()


def autodetect_serial() -> str | None:
    for port in serial.tools.list_ports.comports():
        if "ACM" in port.device or "USB" in port.device.upper():
            return port.device
    return SERIAL_PORT


def main():
    global VAD_THRESHOLD
    parser = argparse.ArgumentParser(description="Robot Dog AI voice control")
    parser.add_argument("--list-devices", action="store_true",
                        help="List audio devices and exit")
    parser.add_argument("--audio-device", type=int, default=None, metavar="N",
                        help="Microphone device index (from --list-devices)")
    parser.add_argument("--output-device", type=int, default=None, metavar="N",
                        help="Speaker device index (from --list-devices)")
    parser.add_argument("--serial", default=None, metavar="PORT",
                        help=f"Serial port (default: auto-detect or {SERIAL_PORT})")
    parser.add_argument("--no-serial", action="store_true",
                        help="Disable serial output (test without PSoC)")
    parser.add_argument("--model", default="base",
                        choices=["tiny", "tiny.en", "base", "base.en", "small", "small.en"],
                        help="Whisper model size (default: base)")
    parser.add_argument("--threshold", type=float, default=VAD_THRESHOLD,
                        help=f"VAD RMS threshold (default: {VAD_THRESHOLD})")
    args = parser.parse_args()

    if args.list_devices:
        list_devices()
        return

    VAD_THRESHOLD = args.threshold

    print("=== Robot Dog AI ===\n")

    # --- Serial ---
    sender = None
    if not args.no_serial:
        port = args.serial or autodetect_serial()
        sender = CommandSender(port, BAUD_RATE)

    # --- Soundboard ---
    soundboard = SoundBoard(output_device=args.output_device)

    # --- Whisper ---
    processor = SpeechProcessor(model_size=args.model)

    # --- Audio capture ---
    audio_queue: queue.Queue = queue.Queue()
    capture = AudioCapture(audio_queue, device=args.audio_device)
    capture.start()

    # --- Idle sound timer ---
    last_idle = time.time()

    print("\nReady. Speak a command (sit / stand / shake / down / heel / spin / dance).")
    print("Say 'go <name> mode' to switch soundboard mode (e.g. 'go uncensored mode').")
    print("Press Ctrl+C to stop.\n")

    soundboard.say("Robot dog online. Ready for commands.")

    try:
        while True:
            # Periodic idle sound
            if FUNNY_INTERVAL > 0 and (time.time() - last_idle) > FUNNY_INTERVAL:
                soundboard.play("idle")
                last_idle = time.time()

            # Process audio segment
            try:
                audio = audio_queue.get(timeout=0.5)
            except queue.Empty:
                continue

            text = processor.transcribe(audio)
            if not text:
                continue

            print(f"[Heard] {text!r}")

            # 1. Check for mode switch command
            mode = detect_mode_switch(text)
            if mode:
                switched = soundboard.switch_mode(mode)
                if switched:
                    soundboard.say(f"Switching to {mode} mode.")
                else:
                    soundboard.say(f"Mode {mode} not found.")
                continue

            # 2. Check for music request
            if any(kw in text for kw in ["play music", "play a song", "play something", "play song"]):
                print(f"[Music] Playing random song")
                soundboard.play("music")
                continue

            # 3. Check for robot commands
            command = detect_command(text)
            if command:
                print(f"[Match] Command: {command}")
                if sender:
                    sender.send(command)
                if command == "STOP":
                    soundboard.stop()
                elif command == "DANCE":
                    soundboard.play("dance")
                else:
                    soundboard.play("command")
            else:
                # 3. Classify the input and react accordingly
                category = classify_input(text)
                if category:
                    print(f"[React] Category: {category}")
                    soundboard.play(category)
                else:
                    soundboard.play("meme")

    except KeyboardInterrupt:
        print("\nStopped.")
    finally:
        capture.stop()
        if sender:
            sender.close()


if __name__ == "__main__":
    main()
