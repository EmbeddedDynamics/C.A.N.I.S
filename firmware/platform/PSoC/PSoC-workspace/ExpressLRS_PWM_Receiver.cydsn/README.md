# ExpressLRS PWM Receiver

PSoC Creator project that reads PWM signals from an ExpressLRS RC receiver and decodes them into usable channel values on the PSoC5LP.

## How it works

Each RC channel is measured using a dedicated hardware timer and interrupt pair. On each rising edge the timer captures a timestamp; on the falling edge the pulse width is calculated. This gives accurate PWM readings without polling.

**Channels:** 4 (CH1–CH4)

```
Rising edge  → capture timer start value
Falling edge → calculate width = now − start
```

PWM pulse widths follow standard RC convention: ~1000 µs (min) to ~2000 µs (max), with 1500 µs as centre.

## Hardware

| Component | Detail |
|---|---|
| Microcontroller | Cypress PSoC5LP |
| Receiver | ExpressLRS (PWM output mode) |
| Channels | 4× PWM input |

## Opening the project

Open the workspace in PSoC Creator/Designer:

```
firmware/platform/PSoC/PSoC-workspace/PSoC-workspace.cywrk
```

Then select the `ExpressLRS_PWM_Receiver` project.

## Usage

Call `getRadioPWM(channel)` with a channel number (1–4) to get the current pulse width in microseconds.
