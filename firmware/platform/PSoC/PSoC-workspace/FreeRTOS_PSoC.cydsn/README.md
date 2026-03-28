# FreeRTOS on PSoC5

FreeRTOS real-time operating system ported to the Cypress PSoC5.

## What it does

This branch integrates FreeRTOS into the PSoC5 firmware environment. It provides preemptive multitasking so that future firmware features (CAN communication, motion control, sensor reading, serial I/O) can run as independent tasks without blocking each other.

The included `main.c` demonstrates the setup: an `LED_Task` blinks the built-in LED every 500 ms using `vTaskDelay`, showing that the RTOS scheduler is running. The `RTOS_Start()` call initialises the FreeRTOS port before the scheduler is started.

The FreeRTOS port included uses `heap_4` (coalescing allocator) and a PSoC5-specific `port.c` for the ARM Cortex-M3 context switch.

## Libraries / dependencies

- **[FreeRTOS](https://www.freertos.org/)** v10.x — kernel source included directly in the project (no package manager)
- **FreeRTOS-Plus CLI** (`FreeRTOS_CLI.c`) — optional command-line interface extension, also included
- PSoC Creator component API (`project.h`)

## File structure

```
firmware/platform/PSoC/PSoC-workspace/
└── FreeRTOS_PSoC.cydsn/
    ├── FreeRTOS/
    │   ├── include/                    # FreeRTOS kernel headers (tasks, queues, semaphores, timers, etc.)
    │   ├── croutine.c                  # Co-routine support
    │   ├── event_groups.c              # Event group primitives
    │   ├── heap_4.c                    # Memory allocator (coalescing)
    │   ├── list.c                      # Linked list used internally by the kernel
    │   ├── port.c                      # Cortex-M3 port — context switch, tick ISR
    │   ├── queue.c                     # Queue and semaphore implementation
    │   ├── FreeRTOS_CLI.c              # CLI extension
    │   └── readme.txt                  # FreeRTOS licence and notes
    └── main.c                          # Firmware entry point — LED blink task demo
```
