/*
===============================================================================
 Name        : multifunction_shield.c
 Author      : xkoupy00
 Version     : 1.0
 Description : main definition usage of multifunction shield
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

//shiftout defs
#define LSB_FIRST true
#define LSB_LAST false
//log.levels
#define HIGH 1
#define LOW 0
//pin defs.. port, pin
#define CLK_PORT 0
#define CLK_PIN 12
#define DATA_PORT 2
#define DATA_PIN 13
#define ST_PORT 0
#define ST_PIN 2
//data for LEDs
#define CHAR_A 0x88
#define CHAR_H 0x89
#define CHAR_O 0xC0
#define CHAR_J 0xE1

#define ADR_1 0x01
#define ADR_2 0x02
#define ADR_3 0x04
#define ADR_4 0x08

//shifts oout 8 bits of data and generates clock
void shiftOut(uint8_t bit_order, uint8_t val)
{
	for(int i = 0; i < 8; i++)
	{
		if(bit_order==LSB_FIRST)
		{
			Chip_GPIO_SetPinState(LPC_GPIO_PORT,DATA_PORT, DATA_PIN, (val & (1 << i))); //datapin
		}
		else
		{
			Chip_GPIO_SetPinState(LPC_GPIO_PORT,DATA_PORT, DATA_PIN, (val & (1 << (7-i))));
		}
		Chip_GPIO_SetPinState(LPC_GPIO_PORT,CLK_PORT, CLK_PIN, HIGH); //clockpin
		Chip_GPIO_SetPinState(LPC_GPIO_PORT,CLK_PORT, CLK_PIN, LOW); //clockpin
	}
}
/* 
 *Nop based delay
 */
void delay(int sleep_time)
{
	for (int i = 0; i < sleep_time; ++i) __asm volatile ("nop");
}
/*
 *Sends char and his adress to LEDs regs
 */
void sendChar(uint8_t bit_order, uint8_t adr, uint8_t val)
{
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,ST_PORT, ST_PIN, LOW);
    delay(10);
    shiftOut(LSB_LAST, val);
    shiftOut(LSB_LAST, adr);
    delay(10);
    Chip_GPIO_SetPinState(LPC_GPIO_PORT,ST_PORT, ST_PIN, HIGH);
}

/*
 * main sets up some basic pins, set pins for com with LED display registers
 */
int main(void) {

	//init led and buttons
    Board_Init();
    //like power on LED
    Board_LED_Set(0, true);

    //set dit for specific pins
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, CLK_PORT, CLK_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, DATA_PORT, DATA_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, ST_PORT, ST_PIN);

    //make sure ST is in HIGH
    Chip_GPIO_SetPinState(LPC_GPIO_PORT,ST_PORT, ST_PIN, HIGH);
    //Shift the word "AHOJ" into shields 7-segment display
    while(1) {

        sendChar(LSB_LAST, ADR_1, CHAR_A);
        delay(1000);
        sendChar(LSB_LAST, ADR_2, CHAR_H);
        delay(1000);
        sendChar(LSB_LAST, ADR_3, CHAR_O);
        delay(1000);
        sendChar(LSB_LAST, ADR_2, CHAR_J);
        delay(1000);

        Board_LED_Toggle(1);

    }
    return 0 ;
}
