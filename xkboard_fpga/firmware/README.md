# XKboard FPGA – firmware

| Directory | Content |
|---|---|
| [examples/](examples/) | Lattice Diamond VHDL projects (open the `.ldf` file) |
| [ft_prog/](ft_prog/) | `ftdi.xml`: FT_PROG EEPROM template for the on-board FT2232HL, so that Diamond detects it as a programming cable |

## Examples

The numbering follows chapter 10 of the thesis. Full descriptions are in [docs/examples.md](../../docs/examples.md).

| Project | Description | Key files |
|---|---|---|
| `10_1_blink` | LEDs D1–D3 blink at 1/5/10/20 Hz, selected by S1/S2. Includes a Reveal analyzer setup and a ModelSim project | `top.vhd`, `Blinky.lpf`, `rev_test.rvl`, `sim_blinky/` |
| `10_2_i2c_config` | EFB with I2C enabled, so the configuration port stays reachable from the MCU in user mode | `efb_i2c_conf.vhd` / `.ipx`, `i2c_config.lpf` |
| `10_3_multifunc_shield` | Drives the multi-function shield 7-segment display ("AHOJ") through 74HC595 shift registers | `top.vhd`, `multi_shield.lpf` |
| `10_4_servo_control` | Receives 8-bit servo commands from the MCU and generates three 50 Hz PWM outputs | `top.vhd`, `servo_control.lpf` |

Target device in all projects: **LCMXO3D-9400HC-5SG72C**, 12 MHz clock on pin 69.

See [../README.md](../README.md#software) for the Diamond flow, programming and Reveal debugging.
