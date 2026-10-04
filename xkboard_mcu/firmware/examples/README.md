# XKboard MCU – examples

MCUXpresso IDE projects for the LPC4327 and the board support library. The numbering follows chapter 10 of the
thesis. All examples are described in [docs/examples.md](../../../docs/examples.md).

| Project | Description |
|---|---|
| `10_1_blink` | LED blink, buttons change the rate. The first test of a new board |
| `10_2_i2c_config` | I2C master talking to the MachXO3D configuration port (sysCONFIG commands in `machxo.c`) |
| `10_3_multifunction_shield` | 7-segment display, LEDs and buttons on the Arduino multi-function shield |
| `10_4_servo_control` | Joystick read with the ADC, servo commands sent to the FPGA board |
| `10_xkboard_mcu_4327` | Board support library (`board.c`, `board.h`, `board_api.h`, `board_sysinit.c`) |
| `lpcopen_3_02_lpcxpresso4337.zip` | NXP LPCOpen v3.02 package for LPCXpresso4337 (provides `lpc_chip_43xx` and vendor examples) |

## Importing into MCUXpresso

1. **File → Import → General → Existing Projects into Workspace**, then select this directory.
2. Import `lpc_chip_43xx` from `lpcopen_3_02_lpcxpresso4337.zip` (**Import project(s) from file system** → archive).
3. Build `lpc_chip_43xx` and `10_xkboard_mcu_4327`, then the example project.
