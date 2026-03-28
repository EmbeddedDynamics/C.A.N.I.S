# RobotDog PSoC Project — CAN Bus

PSoC Creator project for the RobotDog firmware. This branch adds a CAN bus block to the top-level design, enabling communication between the PSoC5LP and the ODrive motor controllers.

## What this branch adds

- CAN peripheral block wired into `TopDesign.cysch`
- CAN bus configured for communication with ODrive controllers over the robot's CAN network

## Hardware

| Component | Detail |
|---|---|
| Microcontroller | Cypress PSoC5LP |
| Communication | CAN bus → ODrive motor controllers |

## Opening the project

Open the workspace in PSoC Creator/Designer:

```
firmware/platform/PSoC/PSoC-workspace/PSoC-workspace.cywrk
```

Then select the `RobotDog` project.

## Notes

The CAN bus runs at 1000 kbps using CAN 2.0 (11-bit identifiers), matching the ODrive CAN protocol expected by the ODrive driver.
