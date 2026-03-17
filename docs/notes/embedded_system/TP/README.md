# BeagleBone linear slide control project

This package contains a complete reference implementation for the TP5-style project:
- state machine: STOP / START / RUN / IDLE
- pthread + mutex shared status
- PWM motor command
- eQEP position feedback
- PID position control
- limit-switch protection
- homing sequence
- stronger anti-windup
- LIS331HH I2C accelerometer driver integration

## Files
- `main.c`
- `mode.h` / `mode.c`
- `gpio.h` / `gpio.c`
- `pwm.h` / `pwm.c`
- `eqep.h` / `eqep.c`
- `lis331hh.h` / `lis331hh.c`
- `Makefile`

## Important adjustments before running on your board
1. Check the PWM sysfs path in `pwm.h`
2. Check the GPIO numbers used for the two limit switches in `gpio.h`
3. Recalibrate `COUNT_TO_METER` in `mode.c`
4. Check that the eQEP sysfs path matches your kernel/device-tree setup
5. Check the LIS331HH I2C bus and slave address in `lis331hh.h`

## Build
```bash
make
```

## Run
```bash
./slide_ctrl
```
