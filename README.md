# XKboard

Modular platform based on two separate circuit boards. One with an ARM-Cortex MCU and the other with a Lattice FPGA. Both boards are independent of each other and can be programmed and debugged separately. The circuit boards are designed to allow interconnection of the two boards and, for example, erasing and programming FLASH memory in the FPGA using the MCU. This work is inspired by an internal FIT BUT development board for students called Minerva. Different components are used and in general both boards are a siplification of a single board solution like Minerva with both MCU and fpga on one PCB. this platform was the basis of my master thesis.

Another interesting aspect of this thesis is the possibility of connecting Arduino shields to both boards when used separately. One example shows the usege of a basic shield with buttons and a 7segment display.

Both dirs xkboard_mcu and xkboard_fpga contain circuit board designs in Altium Designer and some code examples based on the LPCXpresso 4337 board for the MCU. For Lattice FPGAs there are a few examples in VHDL. Just the basic stuff that covers the functionality of both borads.



Modular platform based on two separate PCBs. One with ARM-Cortex MCU and second with Lattice FPGA. Both board are indepent of each other and can be programmed and debugged separatly. PCB's are designed to allow interconnection of both boards and, for example, erasing and programming FLASH memory in FPGA using MCU. This work is inspired by VUT FIT internal devolopment board for students called Minerva. Different components are used and in general, both boards are siplification of one board solution like Minerva with both MCU and fpga on one PCB.This platform was the basis of my master thesis...

Another interesting aspect of this work is possibility to connect Arduino shields to both boards when used separately. One of the examples shows usage of basic shield with buttons, 7-segment display.

Both dirs xkboard_mcu and xkboard_fpga contain PCB designs in Altium Designer and some code examples based on LPCXpresso 4337 board for MCU. For Lattice FPGA there are some examples in VHDL. Just basic stuff that covers functionality of both borads.

doc.pdf is technical overview of this project in czech language. 
English wiki (translated parts of doc.pdf) - https://github.com/koupypavel/xkboard/wiki
