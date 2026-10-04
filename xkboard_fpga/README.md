# XKboard FPGA

FPGA development board based on the Lattice **MachXO3D** family. It has an on-board USB-JTAG bridge, an
Arduino-compatible header, and a header for stacking with the [XKboard MCU](../xkboard_mcu/README.md) board.

- Schematics: [schema.pdf](schema.pdf)
- Altium Designer project: [dps/](dps/) (`fitkit4_fpga.PrjPcb`; fabrication outputs in `xkboard_fpga.zip`)
- Enclosure: [case/](case/) (`top.stl`, `bottom.stl`)
- VHDL examples and FT_PROG template: [firmware/](firmware/README.md)

The board design draws on FITkit 3 (Minerva), Spartan Edge Accelerator and Arty S7. Lattice was chosen over
Xilinx because the MachXO3D can be configured over open interfaces (I2C, SPI, JTAG) without a vendor-specific
programming cable.

## Hardware

### FPGA

| Parameter | Value |
|---|---|
| Device | Lattice LCMXO3D-9400HC-5SG72C |
| Logic | 9400 LUT4s, EBR block RAM, distributed RAM |
| Package | QFN72 with exposed pad, 58 user I/O |
| Supply | 2.5–3.3 V (core and I/O banks; 3.3 V on this board) |
| Non-volatile memory | Internal configuration flash (CFG0/CFG1) and user flash (UFM) |
| Hard IP | EFB: 2× I2C, SPI, timer/counter, UFM access, sysCONFIG via Wishbone. ESB: AES-128/256, SHA-256/HMAC, ECC, TRNG. sysCLOCK PLL |
| Clock | 12 MHz oscillator on pin 69 |

#### Architecture notes (MachXO3D)

- **PFU** (Programmable Functional Unit): 4 slices, each with 2 LUT4s and 2 registers. Slice modes are Logic (LUT4/LUT5),
  Ripple (2-bit add/sub, counters, comparators, multipliers with fast carry FCI/FCO), RAM (16×4 distributed
  single-port RAM) and ROM.
- **sysIO** buffers support LVTTL, LVCMOS33/25/18/15/12, I3C, LVDS and MIPI, depending on the bank VCCIO.
- **Clocking:** 8 primary clock inputs (`PCLK[T/C][bank]_[0..2]`). They can be used as general I/O when not
  needed as clock inputs.
- **EFB / ESB** blocks are accessed from user logic over a **Wishbone** bus. User logic must implement a
  Wishbone master: either a custom state machine, or a soft-core such as Mico8/Mico32. Lattice does not officially support
  those cores on MachXO3D (Mico8 supports MachXO3L).

**Internal flash map:** `CFG0` and `CFG1` hold the primary and secondary bitstreams. They are followed by
the user flash sectors `UFM0`–`UFM3`. A bitstream larger than a CFG sector may extend into the UFM sector
directly after it. The **Feature Row** is erased and programmed separately. It enables or disables the
configuration ports (I2C, SPI, JTAG) and the dedicated pins `PROGRAMN`, `INITN` and `DONE`.

**Configuration ports:** IEEE 1149.1 JTAG, self-download (from internal flash), master/slave SPI, dual boot,
I2C and Wishbone. All use the **sysCONFIG** command set. The device switches between **User**, **Offline**
and **Transparent** configuration modes. Offline mode must be entered before reprogramming, so that the
running logic is disabled while the flash or SRAM is rewritten.

### Debug and configuration

- **FTDI FT2232HL** USB 2.0 high-speed bridge. Channel A runs in MPSSE mode as a JTAG adapter for Lattice
  Diamond Programmer and the Reveal analyzer. Channel B is free (UART/FIFO/MPSSE). Program the EEPROM once with
  **FT_PROG** using [firmware/ft_prog/ftdi.xml](firmware/ft_prog/ftdi.xml) so that Diamond detects the
  cable.
- **`JTAG` header (10-pin)** for an external JTAG cable:

| Pin | Signal | Pin | Signal |
|---|---|---|---|
| 1 | 3.3 V | 2 | TMS |
| 3 | GND | 4 | TCK |
| 5 | GND | 6 | TDO |
| 7 | GND | 8 | TDI |
| 9 | GND | 10 | TRST |

![JTAG header](https://user-images.githubusercontent.com/10815404/179638946-165ff923-c8c1-48ff-964c-87695e90a4f5.png)

- **`FPGA_CONF`** header (bottom side). The XKboard MCU can configure the FPGA over I2C or SPI through it.
  See the [root README](../README.md#board-interconnect).

### Power

- 5 V from micro-USB → **NCP1117DT33T5G** LDO, 3.3 V / 1 A, distributed on an inner plane.
- **The `VCC_CORE` and `VCC_0` jumpers must be closed** to power the FPGA core and bank 0.

### Peripherals

- **S25FL128S** 128 Mbit SPI flash. It is intended for configuration images or user data, for example shared with the MCU.
  It is populated, but it is not used as a configuration source in the current designs.
- 3 user LEDs and 2 user buttons:

| Component | FPGA pin |
|---|---|
| LED D1 | 28 |
| LED D2 | 30 |
| LED D3 | 31 |
| Button S1 | 26 |
| Button S2 | 27 |
| 12 MHz clock | 69 |

### Pinout

**Arduino headers and `FPGA_CONF`**. **3.3 V logic only.** The FPGA has no ADC, so the Arduino analog pins are
plain digital I/O.

![Arduino-compatible headers and FPGA_CONF](https://user-images.githubusercontent.com/10815404/179638879-40bc7fa6-3084-4822-9dcc-f157f1cc59bc.png)

**`FPGA_IO` header**. Spare FPGA I/O (pins named by the MachXO3D pad name).

| Pin | Signal | Pin | Signal |
|---|---|---|---|
| 1 | 3.3 V | 2 | 3.3 V |
| 3 | 3.3 V | 4 | PL29B |
| 5 | PB7B | 6 | PL30A |
| 7 | PL25A | 8 | PL30B |
| 9 | GND | 10 | GND |
| 11 | PL25B | 12 | PL30C |
| 13 | PL27A | 14 | PR5A |
| 15 | GND | 16 | GND |
| 17 | PL27B | 18 | PT23A |
| 19 | PL29A | 20 | PT23B |

![FPGA_IO header](https://user-images.githubusercontent.com/10815404/179638927-0f44e568-9d8b-4785-b46d-0b84c2260a21.png)

## Software

### Toolchain

**Lattice Diamond** (3.12 was used) covers the whole flow: HDL entry, IP configuration (IPexpress),
synthesis (LSE), place and route, timing analysis, bitstream generation, programming and on-chip debugging.
Unlike the MCU flow, a practical IDE-free (Makefile-only) flow is not available.

![Lattice Diamond](https://user-images.githubusercontent.com/10815404/179640848-b9ce7dab-ccab-4c63-82f4-dbf61926f4f2.png)

Useful views:

- **Spreadsheet View**: pin assignment and I/O settings (I/O standard, pull mode, slew rate). *Global
  Preferences* holds sysCONFIG options such as `I2C_PORT` and `SLAVE_SPI_PORT`.
- **Netlist View / Netlist Analyzer**: the synthesized design as a schematic of primitives.
- **Floorplan / Physical View**: placement relative to the package. Supports cross-probing between views.
- **Timing Analysis View**.
- **IPexpress**: configurable IP blocks (EFB, FIFOs, adders, PLL, power controller, …).

### Creating a project

1. **File → New → Project** (New Project wizard).
2. Select the device: MachXO3D, **LCMXO3D-9400HC**, speed grade **5**, package **QFN72** (`-5SG72C`).
3. Add the VHDL/Verilog sources. All other files are optional: the `.lpf` constraints file, synthesis
   constraints, Reveal `.rvl` files and IP `.ipx` files.
4. Assign pins in Spreadsheet View, or copy the `LOCATE COMP` lines from an example `.lpf`.

### Simulation

The simulation wizard creates a **ModelSim Lattice Edition** project from the design. ModelSim is installed with Diamond.

### Synthesis and implementation

The **Process** pane runs the flow in order:

1. **Synthesize Design**: LSE (Lattice Synthesis Engine) infers ROM/RAM, FSMs and arithmetic and writes an NGD
   netlist. Supported languages: VHDL-87/93 (`numeric_bit`, `numeric_std`, `std_logic_1164`,
   `std_logic_arith`, `std_logic_signed/unsigned`, `math_real`) and Verilog-95/2001.
2. **Map Design** and **Place & Route Design**, using the constraints in the `.lpf`.
3. **Export Files**: JEDEC (`.jed`) or bitstream (`.bit`, binary or hex).

### Programming

Open **Tools → Programmer**. Diamond detects the FT2232HL as a USB cable. Select the device (family, part, package)
and an access mode:

| Target | Operation | Persistence |
|---|---|---|
| SRAM | *SRAM Fast Configuration* | Lost at power-off. Use for quick tests and debugging |
| Internal flash (CFG0/CFG1) | *FLASH Erase, Program, Verify* | Non-volatile. Loaded into SRAM at power-up (self-download mode) |
| External SPI flash | — | Populated, not implemented |

The FPGA can also be configured from the XKboard MCU over I2C. See the
[I2C configuration example](../docs/examples.md#2-fpga-configuration-over-i2c).

### Debugging with Reveal

FPGA designs are debugged with an embedded logic analyzer rather than a software debugger:

1. **Reveal Inserter**: select the signals to sample, the trigger signals and the buffer depth. Diamond inserts the
   analyzer core and a JTAG hub into the design. Re-run the flow afterwards.
2. **Reveal Analyzer**: connects through the FT2232HL JTAG bridge, configures the trigger conditions, and
   shows the captured samples on a timeline. Captures can be exported as `.vcd`, for example for ModelSim.

> Simulate before running on hardware. A bad pin assignment or a short to external circuitry can damage
> the FPGA despite its protection features.

## Firmware examples

See [firmware/README.md](firmware/README.md) and [docs/examples.md](../docs/examples.md).
