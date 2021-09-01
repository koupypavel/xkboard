/*
===============================================================================
 Name        : servo_control.c
 Author      : xkoupy00
 Version     : 1.0
 Description : Main file with init func for used ADC and cmd interface. Sends x ,y, z to FPGA.
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

#define LSB_FIRST 1
#define LSB_LAST 0
#define HIGH 1
#define LOW 0
#define bitRead(val, index)  (((val) >> (index)) & 0x01)
//#define digitalWrite(port, pin, level) Chip_GPIO_SetPinState(LPC_GPIO_PORT, port, pin, (bool) level)

#define CLK_PORT 0
#define CLK_PIN 15
#define DATA_PORT 0
#define DATA_PIN 6
#define ST_PORT 5
#define ST_PIN 10

static ADC_CLOCK_SETUP_T ADCSetup;

/*
 * Init pins for cmd interface
 */
static void init_cmd_bus()
{
	Chip_SCU_PinMuxSet(0x3, 6, (SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | SCU_MODE_FUNC0));
	Chip_SCU_PinMuxSet(0x3, 7, (SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | SCU_MODE_FUNC4));
	Chip_SCU_PinMuxSet(0x1, 20, (SCU_MODE_INBUFF_EN | SCU_MODE_PULLUP | SCU_MODE_FUNC0));
}
/*
 * shifts out 8 bit of data and generate clock
 */
void shiftOut(uint8_t bit_order, uint8_t val)
{
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,CLK_PORT, CLK_PIN, HIGH); //clockpin
	Chip_GPIO_SetPinState(LPC_GPIO_PORT,CLK_PORT, CLK_PIN, LOW); //clockpin
	for (int i = 0; i < 10; ++i) __asm volatile ("nop");
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
/* Polling routine for ADC example */
static uint16_t  pool_ADC_val(uint8_t axis)
{
	uint8_t dataADC;

	Chip_ADC_SetStartMode(LPC_ADC0, ADC_START_NOW, ADC_TRIGGERMODE_RISING);
	/* Waiting for A/D conversion complete */
	while (Chip_ADC_ReadStatus(LPC_ADC0, axis, ADC_DR_DONE_STAT) != SET) {}
	/* Read ADC value */
	Chip_ADC_ReadByte(LPC_ADC0, axis, &dataADC);
	return dataADC;
}

int main(void) {

    SystemCoreClockUpdate();
    Board_Init();
    init_cmd_bus();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);

    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, CLK_PORT, CLK_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, DATA_PORT, DATA_PIN);
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, ST_PORT, ST_PIN);

	Chip_ADC_Init(LPC_ADC0, &ADCSetup);
	Chip_ADC_SetBurstCmd(LPC_ADC0, DISABLE);
	Chip_ADC_EnableChannel(LPC_ADC0, ADC_CH0, ENABLE);

	Chip_ADC_EnableChannel(LPC_ADC0, ADC_CH1, ENABLE);

	uint8_t x = 0;
	uint8_t y = 0;
	uint8_t z = 0;
    //
    while(1) {
        //checkout adc0 for x and adc1 for y joystick
    	 x = pool_ADC_val(ADC_CH0);

 		uint8_t btn = Buttons_GetStatus();
 		if(btn == BUTTONS_BUTTON_1)
 		{
 			z = z <=0 ? 0 : z - 10;
 		}
 		else if(btn == BUTTONS_BUTTON_2)
 		{
 			z = z > 256 ? 256 : z + 10;
 		}
		  	//send x and y to fpga
	    Chip_GPIO_SetPinState(LPC_GPIO_PORT,ST_PORT, ST_PIN, LOW);
	    for (int i = 0; i < 10; ++i) __asm volatile ("nop");
	    x &= ~(1UL << 7);
	    x &= ~(1UL << 6);
	    x &= ~(1UL << 5);
	    x |= 1 << 5; //001xxxxx
	    shiftOut(LSB_LAST, x);
	    for (int i = 0; i < 10; ++i) __asm volatile ("nop");
	    Chip_GPIO_SetPinState(LPC_GPIO_PORT,ST_PORT, ST_PIN, HIGH);
	    for (int i = 0; i < 1000; ++i) __asm volatile ("nop");
	    ////send y
	   // y = pool_ADC_val(ADC_CH1);
	    Chip_GPIO_SetPinState(LPC_GPIO_PORT,ST_PORT, ST_PIN, LOW);
	    for (int i = 0; i < 10; ++i) __asm volatile ("nop");
	    y &= ~(1UL << 7);
	    y &= ~(1UL << 6);
	    y &= ~(1UL << 5);
	    y |= 1 << 6; //010xxxxx
	    shiftOut(LSB_LAST, y);
	    for (int i = 0; i < 10; ++i) __asm volatile ("nop");
	    Chip_GPIO_SetPinState(LPC_GPIO_PORT,ST_PORT, ST_PIN, HIGH);
	    for (int i = 0; i < 1000; ++i) __asm volatile ("nop");
	    ////send z
	    Chip_GPIO_SetPinState(LPC_GPIO_PORT,ST_PORT, ST_PIN, LOW);
	    for (int i = 0; i < 10; ++i) __asm volatile ("nop");
	    z &= ~(1UL << 7);
	    z &= ~(1UL << 6);
	    z &= ~(1UL << 5);
	    z |= 1 << 6; //010xxxxx
	    z |= 1 << 5; //001xxxxx
	    shiftOut(LSB_LAST, z);
	    for (int i = 0; i < 10; ++i) __asm volatile ("nop");
	    Chip_GPIO_SetPinState(LPC_GPIO_PORT,ST_PORT, ST_PIN, HIGH);


	    for (int i = 0; i < 1000; ++i) __asm volatile ("nop");
		Board_LED_Toggle(1);
    }
    return 0 ;
}
