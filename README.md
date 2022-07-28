# XKboard

Modular platform based on two separate PCBs. One with ARM-Cortex MCU and second with Lattice FPGA. Both board are indepent of each other and can be programmed and debugged separatly. PCB's are designed to allow interconnection of both boards and, for example, erasing and programming FLASH memory in FPGA using MCU. This work is inspired by VUT FIT internal devolopment board for students called Minerva. Different components are used and in general, both boards are siplification of one board solution like Minerva with both MCU and fpga on one PCB.This platform was the basis of my master thesis...

Another interesting aspect of this work is possibility to connect Arduino shields to both boards when used separately. One of the examples shows usage of basic shield with buttons, 7-segment display.

Both dirs xkboard_mcu and xkboard_fpga contain PCB designs in Altium Designer and some code examples based on LPCXpresso 4337 board for MCU. For Lattice FPGA there are some examples in VHDL. Just basic stuff that covers functionality of both borads.

doc.pdf is technical overview of this project in czech language. 
English wiki (translated parts of doc.pdf) - https://github.com/koupypavel/xkboard/wiki
