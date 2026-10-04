# XKboard

XKboard is a modular embedded development and teaching platform made of two independent PCBs:

| Board | Main device | On-board debug/programming | Docs |
|---|---|---|---|
| **XKboard MCU** | NXP LPC4327 (ARM Cortex-M4F + Cortex-M0 co-processor) | LPC11U35 CMSIS-DAP probe (see [known issues](#known-issues)), 10-pin JTAG/SWD header | [xkboard_mcu/README.md](xkboard_mcu/README.md) |
| **XKboard FPGA** | Lattice MachXO3D LCMXO3D-9400HC-5SG72C | FTDI FT2232HL USB-JTAG bridge, 10-pin JTAG header | [xkboard_fpga/README.md](xkboard_fpga/README.md) |

Each board works standalone and can be programmed and debugged on its own. Both boards expose
**Arduino-compatible headers** (3.3 V logic only), so standard Arduino shields can be attached.
The boards can also be stacked through the dedicated **`FPGA_CONF`** header, which carries
I2C and SPI between the two. With the boards stacked, the MCU can configure the FPGA (program
its internal flash through the MachXO3D sysCONFIG interface) and exchange data with the logic
running in it.

The project is inspired by the FITkit / Minerva boards used for teaching at FIT BUT. Unlike Minerva,
it splits the MCU and the FPGA onto separate, simpler boards. It was developed as the author's
master's thesis (see [Background](#background)).

## Repository layout

```
.
├── doc.pdf                    Master's thesis (Czech) – full technical report
├── docs/
│   ├── examples.md            Demonstration examples (MCU + FPGA + interconnect)
│   └── hardware-design.md     PCB design, manufacturing, assembly and bring-up notes
├── xkboard_mcu/               MCU board
│   ├── case/                  3D-printable enclosure (top.stl, bottom.stl)
│   ├── dps/                   Altium Designer project (schematics, PCB, libraries, outputs zip)
│   ├── firmware/              MCUXpresso examples, board library, OpenOCD/FT_PROG/FlashMagic configs
│   └── schema.pdf             Schematics
└── xkboard_fpga/              FPGA board
    ├── case/                  3D-printable enclosure (top.stl, bottom.stl)
    ├── dps/                   Altium Designer project (schematics, PCB, outputs zip)
    ├── firmware/              Lattice Diamond VHDL examples, FT_PROG template
    └── schema.pdf             Schematics
```

`dps` is the Czech abbreviation for PCB (*deska plošných spojů*).

## Getting started

1. **Pick a board.** Read the hardware overview and pinout in
   [xkboard_mcu/README.md](xkboard_mcu/README.md) or [xkboard_fpga/README.md](xkboard_fpga/README.md).
2. **Install the toolchain.**
   - MCU: [MCUXpresso IDE](https://www.nxp.com/mcuxpresso/ide) (GNU Arm Embedded toolchain included) and
     [OpenOCD](https://openocd.org/).
   - FPGA: [Lattice Diamond](https://www.latticesemi.com/latticediamond) (includes LSE, ModelSim Lattice
     Edition and the Reveal logic analyzer) and FTDI [FT_PROG](https://ftdichip.com/utilities/).
3. **Run the blink example** for your board. See [docs/examples.md](docs/examples.md#1-led-and-button-blink).
4. **Connect the boards.** Use the `FPGA_CONF` header for the I2C configuration and servo-control
   examples.

## Board interconnect

The `FPGA_CONF` header is in the same place on both boards. The FPGA board has pin headers on its
bottom side that plug into the female header on the MCU board.

| Signal | Function |
|---|---|
| `SCL`, `SDA` | I2C. Used for FPGA configuration (MachXO3D configuration port at 7-bit address `0x40`) and for user communication through the EFB I2C controllers |
| `CCLK`, `MOSI`, `MISO`, `SN` | SPI. Can be used for slave-SPI configuration of the FPGA or for user communication |
| `DONE`, `INITN` | FPGA configuration status pins |
| `GND` | Common ground |

> **Note:** On the MCU side the `FPGA_CONF` pins are not pre-assigned to these functions after reset. The
> firmware must configure the pin multiplexing (SCU) before any transfer.

Any free GPIOs (`MCU_IO` / `FPGA_IO` headers, Arduino headers) can also be used for custom protocols. The
servo-control example uses a simple 3-wire clock/data/strobe protocol.

## Known issues

- **MCU on-board debug probe:** the LPC11U35 CMSIS-DAP probe (DAPLink/SWDAP-based) is not usable in this
  revision. The JTAG TDI/TDO lines are not routed, and the LPC4327 comes up in JTAG mode by default.
  Use an external FT2232H-based JTAG adapter on the `SWD/JTAG` header instead (see
  [xkboard_mcu/README.md](xkboard_mcu/README.md#debugging)). A future revision should replace the probe,
  for example with an NXP MCU-Link-style design with full JTAG support.
- **MCU boot-mode pins** and the **FPGA reset circuit** needed rework during bring-up. See
  [docs/hardware-design.md](docs/hardware-design.md#bring-up-notes).
- The external SPI flash on the FPGA board is populated, but it is not used as a configuration source.
- Programming the FPGA internal flash over I2C from the MCU is only partially implemented. Erase, status
  and ID readout work, but there is no storage interface large enough to hold a bitstream.

## Background

This work was the basis of the master's thesis *Modular teaching platform for embedded systems and digital
circuits* (*Modulární výuková platforma pro oblast vestavěných systémů a číslicových obvodů*),
Pavel Koupý, Brno University of Technology, Faculty of Information Technology, 2021. The full report (in Czech)
is [doc.pdf](doc.pdf). The READMEs in this repository are an English technical adaptation of its practical parts.

**Keywords:** development board, ARM Cortex-M, FPGA, Arduino compatible, CMSIS-DAP on-board debug interface.

## License

See [LICENSE](LICENSE).
