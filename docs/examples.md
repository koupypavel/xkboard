# Demonstration examples

The examples are numbered as in chapter 10 of the thesis ([doc.pdf](../doc.pdf)). Each one has an MCU
implementation and/or an FPGA implementation:

| # | Example | MCU project | FPGA project | Boards connected |
|---|---|---|---|---|
| 1 | [LED and button blink](#1-led-and-button-blink) | `10_1_blink` | `10_1_blink` | no |
| 2 | [FPGA configuration over I2C](#2-fpga-configuration-over-i2c) | `10_2_i2c_config` | `10_2_i2c_config` | yes (`FPGA_CONF`) |
| 3 | [Arduino multi-function shield](#3-arduino-multi-function-shield) | `10_3_multifunction_shield` | `10_3_multifunc_shield` | no |
| 4 | [Servo arm control](#4-servo-arm-control) | `10_4_servo_control` | `10_4_servo_control` | yes (GPIO) |

MCU projects are in [xkboard_mcu/firmware/examples](../xkboard_mcu/firmware/examples/README.md) (MCUXpresso). FPGA
projects are in [xkboard_fpga/firmware/examples](../xkboard_fpga/firmware/README.md) (Lattice Diamond).

The MCU examples build on the NXP LPCOpen examples for LPC43xx and the `xkboard_mcu_4327` board library. The FPGA
I2C example is based on the Lattice EFB usage and I2C configuration application notes.

---

## 1. LED and button blink

The embedded "hello world". It checks the toolchain, the programming path and the basic I/O on each board.

### MCU

[`10_1_blink/src/blink.c`](../xkboard_mcu/firmware/examples/10_1_blink/src/blink.c)

- `Board_Init()` sets up the debug UART (USART0), the GPIO defaults, the LEDs and the buttons.
- The three LEDs (P6_7, P6_9, P6_11) are toggled with `Board_LED_Toggle(0..2)` in a busy-wait loop.
- SW3 / SW4 (P6_3, P6_6), read through `Buttons_GetStatus()`, shorten or lengthen the delay and so change the
  blink rate.
- Load and debug the build through OpenOCD + GDB (see [MCU debugging](../xkboard_mcu/README.md#debugging)).

![MCU board blinking](https://user-images.githubusercontent.com/10815404/179639465-973eaa77-a3b1-4880-a19e-8a3a9ab6551f.png)

### FPGA

[`10_1_blink/top.vhd`](../xkboard_fpga/firmware/examples/10_1_blink/top.vhd), constraints in `Blinky.lpf`

Four counters divide the 12 MHz clock (pin 69) into 1, 5, 10 and 20 Hz toggle signals. A multiplexer
controlled by buttons S1/S2 (pins 26/27) selects one of them and drives all three LEDs D1–D3 (pins 28/30/31):

| S1 | S2 | Blink rate |
|---|---|---|
| 0 | 0 | 1 Hz |
| 0 | 1 | 5 Hz |
| 1 | 0 | 10 Hz |
| 1 | 1 | 20 Hz |

![FPGA blink circuit](https://user-images.githubusercontent.com/10815404/179639561-d0aed662-b3d1-4bd7-9b61-333b41a6445d.png)

The project includes a Reveal configuration (`rev_test.rvl`, `rev_analyzer.rva`) and a ModelSim project
(`sim_blinky/`). The capture below shows S1 being pressed and the 10 Hz source being selected:

![Reveal capture](https://user-images.githubusercontent.com/10815404/179639668-17c1b79c-0558-48f0-b297-d8ca0b92b29e.png)

---

## 2. FPGA configuration over I2C

The MCU (I2C master) talks to the MachXO3D configuration port (I2C slave) through the `FPGA_CONF` header.
The FPGA side needs no user logic for this, because the sysCONFIG I2C port is part of the configuration
engine and also responds while the device is unconfigured.

### MCU

[`10_2_i2c_config`](../xkboard_mcu/firmware/examples/10_2_i2c_config): `src/i2c_config.c` (main loop),
`src/machxo.c` / `inc/machxo.h` (command set)

- I2C master is interrupt-driven (NVIC). Results are printed on USART0.
- `machxo.c` only builds the sysCONFIG command buffers. The main loop sends them over I2C to the 7-bit address
  `0x40` (`MACHXO_I2C_ADDR`).
- `printDetails()` reads the device ID, user code, Feature Row, feature bits and status register.

| Function | sysCONFIG operation |
|---|---|
| `cmd_readDeviceID`, `cmd_readUserCode`, `cmd_readStatus` | Identification and status |
| `cmd_readFeatureRow`, `cmd_readFeatureBits`, `cmd_readOTPFuses` | Feature Row / fuses |
| `cmd_enableConfigOffline`, `cmd_enableConfigTransparent` | Enter configuration mode |
| `cmd_erase(flags)` | Erase `MACHXO_ERASE_SRAM`, `_FEATURE_ROW`, `_CONFIG_FLASH`, `_UFM` |
| `cmd_isBusy` | Busy-flag polling |
| `cmd_resetConfigAddress`, `cmd_setConfigAddress`, `cmd_readFlash` | CFG flash access |
| `cmd_resetUFMAddress`, `cmd_setUFMAddress`, `cmd_readUFM`, `cmd_eraseUFM` | UFM access |
| `cmd_programDone`, `cmd_refresh`, `cmd_wakeup` | Finish programming and reload configuration |

Programming sequence:

```
cmd_enableConfigOffline()                       // stop user logic
cmd_erase(MACHXO_ERASE_CONFIG_FLASH | MACHXO_ERASE_UFM)
while (cmd_isBusy()) ;                          // poll until the erase finishes
<program pages from the bitstream source>
cmd_programDone()
cmd_refresh()                                   // load the new configuration
```

**Status:** identification, status readout and erase work. The page-programming step is not complete, because
the MCU has no interface yet for a bitstream-sized source such as SD card, USB mass storage or a UART upload. Possible sources:
the on-board QSPI flash, a hex stream over USART0 from a PC, or an SD card on an Arduino shield.

### FPGA

[`10_2_i2c_config`](../xkboard_fpga/firmware/examples/10_2_i2c_config): EFB instance `efb_i2c_conf` generated
with IPexpress

The FPGA accepts I2C configuration commands if either:

1. it has no valid configuration (CFG0/CFG1 erased or a new device), so it never enters user mode, or
2. the running design keeps the I2C configuration port enabled: `I2C_PORT` enabled in *Spreadsheet View →
   Global Preferences*. Diamond sets this automatically when an EFB with I2C is instantiated.

EFB settings:

- `wb_clk_i` must be at least **7.5× the I2C bus clock**. The default I2C clock is 100 kHz with 7-bit addressing.
- 7-bit address `aaaaabb`: the upper 5 bits are user-configurable (default `10000`). The lower 2 bits select the
  target: `00` configuration port (`0x40`), `01` primary user I2C, `10` secondary user I2C.
- Primary/secondary user I2C is reached from user logic through the EFB **Wishbone** slave. A Wishbone
  master (state machine or soft-core) is required. This is not needed for configuration.

---

## 3. Arduino multi-function shield

Uses a common "Multi-function Shield" on both boards, to show that the Arduino-compatible headers work.

![Multi-function shield](https://user-images.githubusercontent.com/10815404/179639697-300e8e71-b2d8-4756-bb8b-48c77599a62d.png)

Shield resources (Arduino pin numbering):

| Resource | Arduino pin |
|---|---|
| LEDs D1–D4 | D10–D13 |
| Buttons S1–S3 | A1–A3 |
| Potentiometer | A0 |
| Passive buzzer (needs a PWM tone) | D3 |
| 74HC595 latch / strobe (`st`) | D4 |
| 74HC595 clock | D7 |
| 74HC595 data | D8 |

**4-digit 7-segment display protocol.** The display is driven by two daisy-chained 74HC595 shift registers. Each
update sends **two bytes** MSB first, framed by the latch line:

1. Pull `st` low.
2. Shift out the **segment byte** (active low; for example `A`=`0x88`, `H`=`0x89`, `O`=`0xC0`, `J`=`0xE1`).
3. Shift out the **digit-select byte** (`0x01`, `0x02`, `0x04`, `0x08`).
4. Pull `st` high to latch.

Only one digit is lit at a time. Multiplexing fast enough makes all four digits appear lit at once. Both
implementations show the word `AHOJ`.

### MCU

[`10_3_multifunction_shield/src/main.c`](../xkboard_mcu/firmware/examples/10_3_multifunction_shield/src/main.c)

Uses plain GPIO bit-banging with an Arduino-style `shiftOut()`: for each bit, set the data line, then pulse the clock.
Clock is GPIO0[12], data GPIO2[13], latch GPIO0[2]. The potentiometer can be read with the ADC, which is not
possible on the FPGA board.

### FPGA

[`10_3_multifunc_shield/top.vhd`](../xkboard_fpga/firmware/examples/10_3_multifunc_shield/top.vhd), constraints in
`multi_shield.lpf` (clock pin 40, data 44, st 53; LEDs 16/36/20/19; buttons 63/59/58)

A state machine (`CURR_STATE`) sends the two bytes bit by bit. Each bit takes two states: data set with clock high,
then clock low to shift. A 2-bit counter drives multiplexers that select the letter and the digit address.

![Shift protocol simulation](https://user-images.githubusercontent.com/10815404/179639812-2bfcf4b4-5bda-4727-a487-a8bbc323b988.png)

![Shield on the FPGA board](https://user-images.githubusercontent.com/10815404/179639875-078eb8f3-5601-4a51-85ae-2317d8efc852.png)

---

## 4. Servo arm control

A 3-DOF 3D-printed arm with **MG995** servos, controlled from a **Joystick Shield V1.A**. Each board does the part it is
suited for: the MCU reads the joystick with its ADC, and the FPGA generates the three PWM signals with counters.

### Wiring

![Servo example wiring](https://user-images.githubusercontent.com/10815404/179640031-b0a7d984-756a-4c0c-bcc8-2710f8164ec1.png)

- The joystick shield is plugged into the MCU board (axes on A0/A1).
- The MCU and FPGA are connected by 3 wires: clock, data, strobe.
- The MG995 needs 5 V supply and 5 V PWM levels. Both boards use 3.3 V logic, so the servos use an
  **external 5 V supply** and a **level shifter**.

### Command format

One 8-bit command per servo, sent MSB first and framed by the strobe:

```
 7   6   5   4   3   2   1   0
[ servo addr ][   position    ]
  001 = A, 010 = B, 011 = C
```

The position is the 5 least-significant bits of the 8-bit ADC sample (the 3 MSBs are overwritten by the
address). This is coarse, but enough to show the function.

### MCU

[`10_4_servo_control/src/main.c`](../xkboard_mcu/firmware/examples/10_4_servo_control/src/main.c)

- ADC0 channels 0 and 1 (A0, A1) read by polling (`pool_ADC_val(channel)`), 8-bit result (~13 mV/LSB at 3.3 V).
- The joystick has two axes. The third servo is driven by a counter that the two on-board buttons increment or decrement.
- Clock GPIO0[15] (P1_20), data GPIO0[6] (P3_6), strobe GPIO5[10] (P3_7).

### FPGA

[`10_4_servo_control/top.vhd`](../xkboard_fpga/firmware/examples/10_4_servo_control/top.vhd), constraints in
`servo_control.lpf` (clock_i 20, data_i 35, st_i 36; servo_a/b/c 44/40/38)

- **Clock divider:** 12 MHz / 187 ≈ 64 kHz PWM tick.
- **PWM:** 50 Hz period = 20 ms = 1280 ticks. Pulse width ≈ 0.5–1.5 ms (≈ 32–96 ticks). Centre ≈ 1 ms = 64 ticks.
  Exact limits vary between servo clones.
- **Receiver:** a state machine (`idle`, `data_0`…`data_7`, `parse_cmd`) is started by the strobe, shifts in 8 bits
  on the clock, decodes the address, and updates that servo's compare value.

![Servo arm driven by both boards](https://user-images.githubusercontent.com/10815404/179640103-07d4eb6a-3fb2-4d9d-bf73-9d996ec0c2c3.png)
