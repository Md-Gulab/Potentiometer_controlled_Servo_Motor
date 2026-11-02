# Pot-Servo: Potentiometer-Controlled Servo on STM32

Turn a knob, move a servo. A bare-metal STM32F446RE project built without HAL or CubeIDE: own Makefile, linker script, OpenOCD and GDB workflow, register-level drivers and host-side unit tests.

<!-- Add a demo video link or GIF and a wiring photo here -->

## Features

- 12-bit ADC read of a 10 kΩ potentiometer
- Hardware PWM (50 Hz, 1-2 ms pulse) for an SG90 servo using TIM3
- Exact 100 Hz control loop driven by a TIM6 interrupt, with the CPU sleeping in `WFI` between ticks
- 8-sample moving-average filter and a deadband to remove jitter
- Layered design: the filter and mapping logic have no hardware dependencies and are unit-tested on the PC with Unity

## Hardware

| Part | Notes |
|---|---|
| STM32 Nucleo-F446RE | Runs from the 16 MHz HSI |
| 10 kΩ linear potentiometer (B10K) | Input |
| SG90 micro servo | Output |
| Breadboard, jumper wires, USB cable | USB powers the board and the servo |

## Wiring

| From | To | Board pin |
|---|---|---|
| Pot outer pin 1 | 3.3 V | `3V3` |
| Pot outer pin 2 | Ground | `GND` |
| Pot wiper (middle) | ADC1 channel 0 | `A0` (PA0) |
| Servo red | 5 V | `5V` |
| Servo brown | Ground | `GND` |
| Servo orange | TIM3_CH1, AF2 | `D12` (PA6) |

All grounds are common. Do not feed 5 V to the pot, since PA0 is not 5 V tolerant.

## How it works

```
Pot -> ADC1 -> moving average -> deadband -> map 10..4085 to 1000..2000 us
    -> servo (clamp) -> TIM3 PWM on PA6 -> SG90

TIM6 (100 Hz interrupt) wakes the CPU from WFI and triggers one control step.
```

| Setting | Value |
|---|---|
| PWM timer | TIM3, `PSC = 15` (1 MHz tick), `ARR = 19999` (20 ms, 50 Hz), `CCR1` = pulse width in µs |
| Control loop | TIM6, 1 MHz tick, `ARR = 9999` (100 Hz) |
| ADC | 12-bit, 480-cycle sample time, software-triggered, polled with a timeout |
| Servo limits | 1000 to 2000 µs, clamped in one place (`servo.c`) |
| Filter / deadband | 8 samples / 5 counts (the deadband is bypassed at the ends of the range) |

## Project structure

```
.
├── Makefile            build, flash, debug, test
├── linker.ld           memory map (512 KB flash, 128 KB RAM)
├── openocd.cfg         ST-Link + STM32F4 target, connect under reset
├── gdbinit             GDB startup commands
├── CMSIS/              register definitions and core headers
├── inc/                headers
├── src/
│   ├── startup_stm32f446xx.s   vector table and reset handler
│   ├── main.c          init, then wait for tick and run one step
│   ├── app.c           ADC -> filter -> deadband -> map -> servo
│   ├── adc.c  gpio.c  pwm.c  tick.c    peripheral drivers
│   ├── servo.c         pulse-width clamping on top of PWM
│   └── filter.c  map.c pure logic (host-testable)
└── tests/
    ├── unity/          Unity test framework
    └── testFiles/      MAP_test.c, FILTER_test.c
```

Dependencies only point downward: `app` calls `servo`, `filter` and `map`, which call the drivers, which touch registers.

## Build, flash and debug

Requirements: `arm-none-eabi-gcc`, `make`, OpenOCD, `arm-none-eabi-gdb` (e.g. from STM32CubeCLT), and a native `gcc` for the tests.

```
make              # build into build/main.elf
make flash        # program, verify, reset and run with no debugger attached
make openocd      # terminal 1: start OpenOCD
make gdb          # terminal 2: attach GDB, load and break at main
make test         # build and run the Unity tests on the PC
```

Debug variables you can print in GDB or add to a live watch: `g_dbg_raw`, `g_dbg_filtered`, `g_dbg_pulse_us`, `g_tick_count`.

## Testing

- **Unit tests (PC):** `make test` runs the Unity tests for `map_range()` (endpoints, clamping, reversed ranges, monotonicity) and the moving-average filter (priming, step response, noise reduction, overflow).
- **Hardware:** check the PWM registers in GDB (`PSC`, `ARR`, `CCR1`), read `GPIOA->IDR` to confirm the pin toggles, and run for 10 seconds to check that `g_tick_count` is about 1000.

## Tuning

| Symptom | Change |
|---|---|
| Servo twitches with the knob still | Raise `DEADBAND` or `FILTER_LEN` |
| Servo feels sluggish | Lower `FILTER_LEN` |
| Servo buzzes at an end | Narrow `SERVO_MIN_US` and `SERVO_MAX_US` |
| Knob direction is reversed | Swap the pot's outer wires, or reverse the output range in `map_range` |

## What I learned

Linker scripts and startup code, OpenOCD/GDB debugging, memory-mapped registers, ADC and timer configuration, PWM for servos, interrupts and low-power waiting, and testing embedded logic on a host machine.

## Possible next steps

ADC with DMA, 180 MHz PLL clock setup, and a FreeRTOS version with separate ADC and servo tasks.



