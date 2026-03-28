"""
depth_zones.py

This is the initial version made using mostly Claude Code.

Captures stereo frames from the Waveshare IMX219-83, computes a relative
disparity map, and slices it into 4 zones (left, middle, right, bottom).
Zone median disparity values are sent to the PSoC5 over USB serial.

Higher disparity value = object is closer.

Zone layout (front-facing view):
+--------+--------+--------+
|        |        |        |
|  LEFT  | MIDDLE | RIGHT  |
|        |        |        |
+--------+--------+--------+
|         BOTTOM           |  <- hole/drop detection
+--------------------------+

Serial packet format (11 bytes):
  [0xAA] [L_hi] [L_lo] [M_hi] [M_lo] [R_hi] [R_lo] [B_hi] [B_lo] [XOR] [0x55]
  - Values are uint16, little-endian
  - XOR is checksum of the 4 values bytes (bytes 1-8)
  - 0xAA = start, 0x55 = end

TODO: Add stereo calibration (cv2.stereoCalibrate) for metric depth in cm/mm.
"""

import cv2
import numpy as np
import serial
import struct
import time

# --- Config ---
WIDTH       = 640
HEIGHT      = 480
FPS         = 30
SERIAL_PORT = "/dev/ttyACM0"   # check with: ls /dev/ttyACM*
BAUD_RATE   = 115200

# Zone split: bottom zone starts at this fraction of frame height
BOTTOM_SPLIT = 0.70

# StereoSGBM parameters (tunable)
MIN_DISPARITY    = 0
NUM_DISPARITIES  = 64   # must be divisible by 16
BLOCK_SIZE       = 9


def gstreamer_pipeline(sensor_id: int, width: int, height: int, fps: int) -> str:
    return (
        f"nvarguscamerasrc sensor-id={sensor_id} ! "
        f"video/x-raw(memory:NVMM), width={width}, height={height}, framerate={fps}/1 ! "
        f"nvvidconv ! video/x-raw, format=BGRx ! "
        f"videoconvert ! video/x-raw, format=BGR ! appsink"
    )


def compute_zones(disparity: np.ndarray) -> dict:
    """
    Split disparity map into 4 directional zones.
    Returns median disparity per zone as uint16 (0 if no valid data).
    """
    h, w = disparity.shape
    third   = w // 3
    bot_row = int(h * BOTTOM_SPLIT)

    upper = disparity[:bot_row, :]
    zones = {
        "left":   upper[:, :third],
        "middle": upper[:, third : 2 * third],
        "right":  upper[:, 2 * third :],
        "bottom": disparity[bot_row:, :],
    }

    result = {}
    for name, region in zones.items():
        valid = region[region > 0]
        result[name] = int(np.median(valid)) if valid.size > 0 else 0

    return result


def build_packet(zones: dict) -> bytes:
    """
    Pack zone values into an 11-byte serial packet.
    [0xAA][L][M][R][B][XOR][0x55]  (values as uint16 little-endian)
    """
    values = [
        min(zones["left"],   0xFFFF),
        min(zones["middle"], 0xFFFF),
        min(zones["right"],  0xFFFF),
        min(zones["bottom"], 0xFFFF),
    ]
    payload = struct.pack("<HHHH", *values)
    checksum = 0
    for b in payload:
        checksum ^= b

    return struct.pack("B", 0xAA) + payload + struct.pack("BB", checksum, 0x55)


def main():
    # Open both cameras
    left_cap  = cv2.VideoCapture(gstreamer_pipeline(0, WIDTH, HEIGHT, FPS), cv2.CAP_GSTREAMER)
    right_cap = cv2.VideoCapture(gstreamer_pipeline(1, WIDTH, HEIGHT, FPS), cv2.CAP_GSTREAMER)

    if not left_cap.isOpened() or not right_cap.isOpened():
        print("Error: Could not open one or both cameras")
        return

    # Stereo matcher
    stereo = cv2.StereoSGBM_create(
        minDisparity=MIN_DISPARITY,
        numDisparities=NUM_DISPARITIES,
        blockSize=BLOCK_SIZE,
        P1=8  * 3 * BLOCK_SIZE ** 2,
        P2=32 * 3 * BLOCK_SIZE ** 2,
        disp12MaxDiff=1,
        uniquenessRatio=10,
        speckleWindowSize=100,
        speckleRange=32,
    )

    # USB serial to PSoC5
    ser = None
    try:
        ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
        print(f"Connected to PSoC5 on {SERIAL_PORT}")
    except serial.SerialException:
        print(f"Warning: Could not open {SERIAL_PORT} — printing to console only")

    print("Running. Press Ctrl+C to stop.\n")

    try:
        while True:
            ret_l, frame_l = left_cap.read()
            ret_r, frame_r = right_cap.read()

            if not ret_l or not ret_r:
                print("Frame capture failed")
                break

            gray_l = cv2.cvtColor(frame_l, cv2.COLOR_BGR2GRAY)
            gray_r = cv2.cvtColor(frame_r, cv2.COLOR_BGR2GRAY)

            disparity = stereo.compute(gray_l, gray_r).astype(np.float32) / 16.0

            zones = compute_zones(disparity)

            print(
                f"LEFT: {zones['left']:5d}  "
                f"MID: {zones['middle']:5d}  "
                f"RIGHT: {zones['right']:5d}  "
                f"BOTTOM: {zones['bottom']:5d}"
            )

            if ser:
                ser.write(build_packet(zones))

            # --- Visual debug output (remove when PSoC is ready) ---
            # Normalize disparity to 0-255 for visualization
            disp_visual = cv2.normalize(disparity, None, 0, 255, cv2.NORM_MINMAX)
            disp_visual = np.uint8(disp_visual)
            disp_color  = cv2.applyColorMap(disp_visual, cv2.COLORMAP_MAGMA)

            # Draw zone boundaries on depth image
            h, w = disp_color.shape[:2]
            third   = w // 3
            bot_row = int(h * BOTTOM_SPLIT)
            cv2.line(disp_color, (third,   0), (third,   bot_row), (255, 255, 255), 1)
            cv2.line(disp_color, (2*third, 0), (2*third, bot_row), (255, 255, 255), 1)
            cv2.line(disp_color, (0, bot_row), (w,       bot_row), (255, 255, 255), 1)

            # Draw zone labels and values on depth image
            cv2.putText(disp_color, f"L:{zones['left']}",   (5,           20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1)
            cv2.putText(disp_color, f"M:{zones['middle']}", (third + 5,   20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1)
            cv2.putText(disp_color, f"R:{zones['right']}",  (2*third + 5, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1)
            cv2.putText(disp_color, f"B:{zones['bottom']}", (5, bot_row + 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1)

            # Draw matching zone lines on regular frame for comparison
            frame_annotated = frame_l.copy()
            cv2.line(frame_annotated, (third,   0), (third,   bot_row), (255, 255, 255), 1)
            cv2.line(frame_annotated, (2*third, 0), (2*third, bot_row), (255, 255, 255), 1)
            cv2.line(frame_annotated, (0, bot_row), (w,       bot_row), (255, 255, 255), 1)
            cv2.putText(frame_annotated, "LEFT",   (5,           20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1)
            cv2.putText(frame_annotated, "MIDDLE", (third + 5,   20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1)
            cv2.putText(frame_annotated, "RIGHT",  (2*third + 5, 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1)
            cv2.putText(frame_annotated, "BOTTOM", (5, bot_row + 20), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255,255,255), 1)

            # Combine side by side: regular | depth
            combined = np.hstack((frame_annotated, disp_color))
            cv2.imwrite("depth_debug.jpg", combined)
            # --------------------------------------------------------

    except KeyboardInterrupt:
        print("\nStopped.")
    finally:
        left_cap.release()
        right_cap.release()
        if ser:
            ser.close()


if __name__ == "__main__":
    main()
