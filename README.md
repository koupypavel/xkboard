# xkboard

Modular platform based on two separate PCB. One with ARM-Cortex MCU and second with Lattice FPGA. Both board are indepent of each other and can be programmed and debugged as separate or combined. PCB's are design in way that allows connecting both boards and for example erase and program FLASH memory in FPGA using MCU. This work is based on VUT FIT's internal devolopment board for students called Minerva. Different components are used and in general both boards are siplification of one board solution like Minerva, where you can find both MCU and FPGA on one PCB. 

Another "plus" of this work is possibility to connect Arduino shields to both boards whe used separately

xkboard_mcu directory contains pcb design and some code examples based on LPCXpresso 4337 board. 
xkboard_fpga same for FPGA
