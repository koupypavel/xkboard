# Hardware design, manufacturing and bring-up

This document covers how the two boards were designed, manufactured, assembled and brought up. For the
board-level descriptions, see [xkboard_mcu/README.md](../xkboard_mcu/README.md) and
[xkboard_fpga/README.md](../xkboard_fpga/README.md).

## Design concept

- **Two standalone boards** instead of one combined MCU+FPGA board, as in FITkit/Minerva. Each board is simpler
  and cheaper, and can be used alone. Stacked through `FPGA_CONF`, they form a more capable system.
- **On-board programming/debug adapters**, so that no vendor probe is needed: a CMSIS-DAP probe on the MCU board
  and an FT2232HL USB-JTAG bridge on the FPGA board. Standard 10-pin JTAG headers are kept as a fallback.
- **Arduino-compatible headers** on both boards give access to the existing shield ecosystem
  (3.3 V logic only).
- The **FPGA board** handles high-bandwidth peripherals and parallel hardware acceleration. The **MCU board**
  provides ready-made peripherals (USB, ADC, UART, …) that would otherwise have to be built in the FPGA.

## Schematic capture (Altium Designer)

Both projects use a **hierarchical** schematic. A top-level sheet shows every sub-sheet as a block and connects
them through ports, harnesses and buses.

| MCU board (`xkboard_mcu/dps`) | FPGA board (`xkboard_fpga/dps`) |
|---|---|
| `arm_board.SchDoc` (top) | `fpga_board.SchDoc` (top) |
| `mcu_core.SchDoc`: LPC4327, crystal, decoupling | `fpga_core.SchDoc`: MachXO3D, oscillator, decoupling |
| `mcu_debug.SchDoc`: LPC11U35 probe, JTAG/SWD header | `fpga_config.SchDoc`: FT2232HL, EEPROM, JTAG header |
| `mcu_headers.SchDoc`: Arduino, `FPGA_CONF`, `MCU_IO` | `fpga_headers.SchDoc`: Arduino, `FPGA_CONF`, `FPGA_IO` |
| `mcu_peripheral.SchDoc`, `mcu_io.SchDoc`: QSPI flash, LEDs, buttons, USB | `fpga_peripherals.SchDoc`: SPI flash, LEDs, buttons |
| `mcu_power.SchDoc`: LDOs, USB power | `fpga_power.SchDoc`: LDO, jumpers |

Connectivity uses wires, net labels, buses, signal harnesses (`*.Harness`), sheet ports and power ports.
Component symbols and footprints come from the Celestial Altium Library, SnapEDA and distributor models
(Mouser). Always check downloaded footprints against the datasheet of the part you actually buy, because
auto-generated models are sometimes wrong or out of date.

## PCB layout

| Parameter | Value |
|---|---|
| Stack-up | 4 layers, FR4, 1.6 mm |
| Inner layers | GND and 3.3 V planes, reached through power vias |
| Smallest passives | 0603 (hand-solderable) |
| IC packages | LQFP (LPC4327), QFN (MachXO3D, LPC11U35, FT2232HL), SOP |
| Fabrication | Gatema POOL prototype service, Gerber output |

Layout guidelines:

- Keep the **placement** close to the schematic arrangement. Use Altium cross-select and cross-probe to place parts
  group by group.
- **EMC:** minimise current-loop area and trace length, separate high- and low-frequency sections, do not
  use faster parts than needed, protect power and I/O lines against ESD and transients.
- **Grounding:** connect each ground pin to the GND plane by the shortest possible path (multi-point grounding, suitable for digital circuits).
- **Decoupling:** place local (decoupling) capacitors as close to each IC power pin as possible. Add bulk capacitors per
  power domain and bypass filtering at the power input.
- **Routing:** route the critical nets by hand first: clocks, USB pairs, QSPI and power. Use the
  autorouter only as an aid. Optimal routing is NP-hard, so autorouters use heuristics, and they handle DDR and
  analog nets poorly.

Fabrication outputs (Gerber, drill files, BOM) are generated with the `*.OutJob` files and archived in
`dps/xkboard_mcu.zip` and `dps/xkboard_fpga.zip`.

Unit reminder: 1 mil = 0.0254 mm, 1 mm ≈ 39.37 mil, 1 in = 25.4 mm.

## Assembly

Both boards are designed for hand assembly:

- 0603 passives and LQFP parts: soldering iron with a fine conical tip. Use a hoof or flat tip for drag-soldering LQFP
  pins.
- QFN parts with an exposed thermal pad: solder paste and hot air.
- Order: bottom side first (decoupling capacitors, flash). Then populate the top side **block by block, testing each block
  as it is fitted**.

The most common failure is misalignment of fine-pitch parts. The 144-pin LPC4327 is especially prone to shorts that
cannot be seen by eye. Use a multimeter for supply rails and shorts, and a logic analyzer or oscilloscope for
crystals and buses (JTAG, SPI). A Saleae logic analyzer was used during bring-up.

## Bring-up notes

### MCU board

1. Fit the **debug probe** (LPC11U35 + TC1017 LDO) first. When connected over micro-USB it should enumerate as a USB
   mass-storage device, which accepts a firmware binary.
2. Fit the **3.3 V supply** (TPS77733) and the **LPC4327**. Then try to connect over JTAG.
3. Fixes found during bring-up:
   - The **boot-mode pins** were wired incorrectly and had to be corrected (see the boot-mode table in the
     [MCU README](../xkboard_mcu/README.md#boot-modes)).
   - The **on-board probe** cannot be used: the LPC4327 starts in JTAG mode, and the probe's TDI/TDO lines are not routed.
     Use an external FT2232H JTAG adapter on the `SWD/JTAG` header.

### FPGA board

1. Fit the **FT2232HL** first. Over USB it should enumerate as an unknown device until the FTDI drivers are
   installed, then as a serial/JTAG device. Program its EEPROM with FT_PROG
   ([ftdi.xml](../xkboard_fpga/firmware/ft_prog/ftdi.xml)).
2. Close the **`VCC_CORE`** and **`VCC_0`** jumpers to power the FPGA.
3. Fix found during bring-up: the **reset circuit** held the FPGA in reset permanently and had to be reworked.

## Ideas for the next revision

- Replace the MCU-board debug probe with a design that has full JTAG support, for example based on NXP MCU-Link.
- Complete the FPGA bitstream path from the MCU (QSPI flash, SD card or USB as the source).
- Use the FPGA-board SPI flash as a configuration source.
- Add more examples and an online, install-free way to try them.
