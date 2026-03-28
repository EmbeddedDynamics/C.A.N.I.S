# 5-Bar Linkage Prototype

Arduino Nano prototype for validating the 5-bar linkage mechanism and inverse kinematics formulas on a physical test setup before integrating into the robot dog.

## Hardware

| Component | Detail |
|---|---|
| Microcontroller | Arduino Nano |
| Servos | 2× MG50 |
| Servo pins | Pin 9 (servo 1), Pin 10 (servo 2) |

## How it works

The sketch implements inverse kinematics for a 5-bar parallel linkage. Given a target end-effector coordinate `(x, y)`, it calculates the required joint angles `α` and `β` for both servo arms using the law of cosines.

**Link lengths:**
- `L1 = 65 mm`
- `L2 = 85 mm`
- `d = 15 mm` (pivot offset)

## Usage

1. Open `5Bar_linkage_ArduinoNano/5Bar_linkage_ArduinoNano.ino` in the Arduino IDE
2. Connect two MG50 servos to pins 9 and 10
3. Upload to an Arduino Nano
4. Modify `xC` / `yC` in the sketch to move the end-effector to a target position

## References

- Ilja Rukin
- Tazar Technical
- circuiTician
