/*
===============================================================================
 Name        : blink.c
 Author      : xkoupy00
 Version     : 1.0
 Description : Main definition, led toogling with delay based on buttons inc and dec
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

//simple nop based delay;
#define TICK_COUNT 500000
int sleep_time = TICK_COUNT;

int main(void) {

    SystemCoreClockUpdate();
    //init leds and buttons
    Board_Init();

    while(1) {
    	//reads the value and then sets neg of that value
		Board_LED_Toggle(0);
		Board_LED_Toggle(1);
		Board_LED_Toggle(2);

		//read buttons status and increment or decrement sleep delay
		uint8_t btn = Buttons_GetStatus();
		if(btn == BUTTONS_BUTTON_1)
		{
			sleep_time = sleep_time <=0 ? TICK_COUNT : sleep_time - 100000;
		}
		else if(btn == BUTTONS_BUTTON_2)
		{
			sleep_time = sleep_time > TICK_COUNT ? TICK_COUNT : sleep_time + 100000;
		}
		for (int i = 0; i < sleep_time; ++i) __asm volatile ("nop");

    }
    return 0 ;
}
