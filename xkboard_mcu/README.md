# XKboard MCU

ARM Cortex-M development board based on the NXP **LPC4327**. It has an Arduino-compatible header,
a QSPI flash and a header for stacking with the [XKboard FPGA](../xkboard_fpga/README.md) board.

- Schematics: [schema.pdf](schema.pdf)
- Altium Designer project: [dps/](dps/) (`fitkit4.PrjPcb`; fabrication outputs in `xkboard_mcu.zip`)
- Enclosure: [case/](case/) (`top.stl`, `bottom.stl`)
- Firmware, board library and tool configs: [firmware/](firmware/README.md)

![MCU board running the blink example](https://user-images.githubusercontent.com/10815404/179639465-973eaa77-a3b1-4880-a19e-8a3a9ab6551f.png)

## Hardware

### Microcontroller

| Parameter | Value |
|---|---|
| Device | NXP LPC4327JBD144 (LQFP144) |
| Cores | ARM Cortex-M4F (with FPU) + ARM Cortex-M0 co-processor, up to 204 MHz |
| Memory | On-chip flash, 136 kB SRAM |
| Clock | 12 MHz external crystal |
| Interfaces | I2C, I2S, SPI/SSP, UART/USART, USB, SPIFI, EMC (external memory controller), ADC |
| Debug | JTAG / SWD, both cores |

The board was designed for the LPC4337. The LPC4327 was fitted because of component shortages. It is
practically identical, but has a smaller on-chip flash.

The two cores communicate through shared RAM and inter-core interrupts. The M4 is the master and sets up the M0. Two queues
in shared memory are used: `CMD_BUFFER` (M4 → M0) and `MSG_BUFFER` (M0 → M4). Each queue is
described by a start address, an end address, a read pointer and a write pointer. The queue mechanism has no overflow protection and no
error reporting. The sender must check for free space itself.

The EMC can also be used as a parallel interface to the FPGA (up to 16 bits per clock cycle), but this is
not implemented on the current interconnect.

### Debug and programming

- **On-board probe:** an LPC11U35 that runs CMSIS-DAP firmware (based on the open-source DAPLink/SWDAP design).
  It has its own micro-USB connector. **It does not work in this revision**, see [Debugging](#debugging).
- **`SWD/JTAG` header (10-pin):** connects an external probe. The board can also be powered from this header (3.3 V).

| Pin | Signal | Pin | Signal |
|---|---|---|---|
| 1 | 3.3 V | 2 | TMS |
| 3 | GND | 4 | TCK |
| 5 | GND | 6 | TDO |
| 7 | GND | 8 | TDI |
| 9 | GND | 10 | TRST |

![SWD/JTAG header](https://user-images.githubusercontent.com/10815404/179639073-b15608c1-1230-4dec-b38a-6b3b4accb078.png)

These pins default to the debug functions after reset. The MCU can therefore always be reached over JTAG,
even when the application has reconfigured all other interfaces.

### Power

- Two micro-USB connectors: **debug** (probe) and **MCU I/O** (LPC4327 USB0).
- **TPS77733DR** LDO, 3.3 V / 750 mA. Supplies the LPC4327 and the 3.3 V inner plane.
- **TC1017** LDO, 3.3 V / 150 mA. Supplies the LPC11U35 probe.
- The debug USB port supplies both regulators. The MCU I/O USB port supplies only the LPC4327 side, so the
  probe stays off.
- The Arduino `5V` pin is connected directly to USB VBUS. Arduino `VIN` feeds the 3.3 V regulator through a removable
  jumper. The two USB supplies are also separated by a jumper.

### Peripherals

- **S25FL128S** 128 Mbit QSPI NOR flash on the SPIFI interface (P3_3–P3_8), up to 52 MB/s. The MCU can
  boot from it, and it can store FPGA configuration images.
- 3 user LEDs and 2 user buttons:

| Component | Pin | GPIO | Board API |
|---|---|---|---|
| LED | P6_7 | GPIO5[15] | `Board_LED_*(0)` |
| LED | P6_9 | GPIO3[5] | `Board_LED_*(1)` |
| LED | P6_11 | GPIO3[7] | `Board_LED_*(2)` |
| Button SW3 | P6_3 | GPIO3[2] | `BUTTONS_BUTTON_1` |
| Button SW4 | P6_6 | GPIO0[5] | `BUTTONS_BUTTON_2` |

### Pinout

**Arduino headers and `FPGA_CONF`**. All I/O is **3.3 V only**. Applying 5 V logic can damage the LPC4327.
The Arduino analog pins A0–A5 are labeled `ADC0`–`ADC5`. The servo example reads A0/A1 as ADC0 channels 0/1.

![Arduino-compatible headers and FPGA_CONF](https://user-images.githubusercontent.com/10815404/179639063-c54b35b8-d236-4e18-9c96-ac6863a9853b.png)

The `FPGA_CONF` labels (`SCL`, `SDA`, `CCLK`, `MOSI`, `MISO`, `SN`, `DONE`, `INITN`) describe the intended
use. The pins are **not** pre-configured for these functions, so set the pin multiplexing in firmware
before use.

**`MCU_IO` header**. Spare MCU pins. Pins 3/4 (P2_0/P2_1) are USART0, which the UART ISP bootloader uses.

| Pin | Signal | Pin | Signal |
|---|---|---|---|
| 1 | P9_6 | 2 | P9_5 |
| 3 | P2_0 (U0_TXD) | 4 | P2_1 (U0_RXD) |
| 5 | P2_2 | 6 | P2_3 |
| 7 | P2_4 | 8 | P2_5 |
| 9 | P2_6 | 10 | P1_0 |
| 11 | P1_3 | 12 | P1_4 |
| 13 | P1_5 | 14 | P1_6 |
| 15 | P1_7 | 16 | P1_8 |
| 17 | P1_9 | 18 | P1_10 |
| 19 | P1_11 | 20 | P1_12 |

![MCU_IO header](https://user-images.githubusercontent.com/10815404/179639069-ea8def5d-62c8-4ebc-b388-0c12ac1521b5.png)

### Boot modes

The boot source is selected by the levels of P2_9, P2_8, P1_2 and P1_1 at reset. If all four pins are low,
the MCU starts the USART0 ISP bootloader.

| Boot mode | P2_9 | P2_8 | P1_2 | P1_1 | Notes |
|---|---|---|---|---|---|
| USART0 ISP | L | L | L | L | UART ISP on P2_0 / P2_1 (`MCU_IO` pins 3/4) |
| SPIFI | L | L | L | H | On-board QSPI flash |
| EMC 8-bit | L | L | H | L | Parallel external memory (16-bit also supported) |
| USB0 ISP | L | H | L | H | USB connector next to R31; requires the 12 MHz oscillator |
| SPI (SSP0) | L | H | H | H | P3_3 SCK, P3_6 SSEL, P3_7 MISO, P3_8 MOSI |
| USART3 ISP | H | L | L | L | |

## Software

### Toolchain

- **MCUXpresso IDE** (NXP, Eclipse-based). It generates the Makefiles, linker scripts and startup code and builds with
  the **GNU Arm Embedded** toolchain (`arm-none-eabi-gcc`, binutils, GDB).
- **LPCOpen** for LPC43xx: the `lpc_chip_43xx` chip library. The examples are based on the LPCXpresso4337 package
  [firmware/examples/lpcopen_3_02_lpcxpresso4337.zip](firmware/examples/lpcopen_3_02_lpcxpresso4337.zip).
- **xkboard_mcu_4327**: the board support library for this board. It implements the generic LPCOpen
  `board_api.h` interface in `board.c` / `board.h` (clock setup, pin muxing, debug UART, LEDs, buttons, I2C,
  SSP, UART, GPIO interrupts).

![MCUXpresso IDE](https://user-images.githubusercontent.com/10815404/179640694-4e530ca1-e2d7-4cc9-84dd-6506a039efbb.png)

![Library dependencies](https://user-images.githubusercontent.com/10815404/179640427-d811343a-9b56-4d17-ada2-177a844a60f8.png)

The board library sets up the SCU pin multiplexing and the peripheral clocks, so applications do not write
these registers bit by bit. Main board API functions:

| Function | Purpose |
|---|---|
| `Board_SystemInit()` | Clock and pin-mux setup, called from `SystemInit()` before `main()` |
| `Board_Init()` | Debug UART (USART0), GPIO defaults, LEDs, buttons |
| `Board_LED_Set(n, state)`, `Board_LED_Toggle(n)`, `Board_LED_Test(n)` | User LEDs 0–2 |
| `Board_Buttons_Init()`, `Buttons_GetStatus()` | User buttons (`BUTTONS_BUTTON_1`, `BUTTONS_BUTTON_2`) |
| `Board_I2C_Init(id)`, `Board_SSP_Init(pSSP)`, `Board_UART_Init(pUART)` | Peripheral pin muxing |
| `DEBUGSTR()`, `DEBUGOUT()` | Debug output on USART0 |

### Creating a project

1. Import `lpc_chip_43xx` (from the LPCOpen zip) and
   [firmware/examples/10_xkboard_mcu_4327](firmware/examples/10_xkboard_mcu_4327) into the MCUXpresso workspace.
2. **File → New → Project → LPCOpen – C Project**, target LPC4337/LPC43xx (M4 core).
3. Select `lpc_chip_43xx` as the chip library and `xkboard_mcu_4327` as the board library. The IDE builds both as
   static libraries and links them into the application.
4. Build with **Project → Build**. The IDE runs `make` with `arm-none-eabi-gcc`.

### Flashing

- **External JTAG + OpenOCD/GDB** (recommended): see [Debugging](#debugging). Use the GDB `load` command or OpenOCD
  `flash write_image`.
- **UART ISP**: pull all boot pins low, connect a USB-UART adapter to `MCU_IO` pins 3/4 (P2_0/P2_1) and program
  with **FlashMagic**. A project template is in
  [firmware/flashmagic_prj/lpc4327.fmx](firmware/flashmagic_prj/lpc4327.fmx).
- **USB ISP**: select the USB0 boot mode (requires the 12 MHz oscillator).
- **Drag-and-drop**: CMSIS-DAP/DAPLink mass-storage programming. This needs a working on-board probe, which this
  revision does not have.

### Debugging

The on-board LPC11U35 probe cannot debug the LPC4327 in this revision:

- **DAPLink** firmware supports SWD only. The LPC4327 starts in JTAG mode and must first be switched to SWD over JTAG, so a
  pure SWD probe cannot connect.
- **IBDAP** (JTAG-capable CMSIS-DAP) needs TDI/TDO from the LPC11U35 (pins 18 and 27), which are not routed. A
  hand-wired attempt with 33 Ω series resistors did not work.
- **Black Magic Probe** firmware can run on a PC (hosted build) with an FT2232H MPSSE adapter. It works, but it is
  redundant because OpenOCD supports the FT2232H directly.

**Working setup: external FT2232H(L) JTAG adapter.** A simple FT2232HL breakout board (FT2232HL, configuration
EEPROM, crystal) is connected to the `SWD/JTAG` header. It can also power the board with 3.3 V. The adapter
needs no firmware. Only its EEPROM has to be programmed once with **FT_PROG** using the template
[firmware/ft_prog/FTDIJTAG.xml](firmware/ft_prog/FTDIJTAG.xml).

```bash
openocd -f xkboard_mcu/firmware/openocd_config/dp_busblaster.cfg -f xkboard_mcu/firmware/openocd_config/lpc4327.cfg
```

| File | Content |
|---|---|
| `dp_busblaster.cfg` | FT2232H adapter (Bus Blaster-compatible wiring), channel A, JTAG transport |
| `openocd-usb.cfg` | Alternative FTDI layout matched by the device description `FTDIJTAG` (as programmed by the FT_PROG template) |
| `lpc4327.cfg` | LPC4327 target, Cortex-M4 TAP, 100 kHz adapter clock, 128 kB work area at `0x10000000` |

To debug the Cortex-M0 core as well, use the stock OpenOCD `target/lpc4350.cfg`. It declares both TAPs (M4 and M0).

OpenOCD starts a GDB server (port 3333 by default) and a Telnet console (port 4444). Connect from MCUXpresso
(a GDB hardware-debugging configuration) or from the command line:

```bash
arm-none-eabi-gdb Debug/10_1_blink.axf -ex "target extended-remote :3333" -ex "monitor reset halt" -ex load
```

**pyOCD** is a Python alternative for CMSIS-DAP probes. LPC4327 support needs the Keil MDK5 device pack. This
project does not use it.

## Firmware examples

See [firmware/README.md](firmware/README.md) and [docs/examples.md](../docs/examples.md).
