/*
 * xkboard.c
 *
 *  Created on: 13. 5. 2021
 *      Author: xkoupy00
 */
#include <board.h>


/* System configuration variables used by chip driver */
const uint32_t ExtRateIn = 0;
const uint32_t OscRateIn = 12000000;


static const io_port_t gpioLEDBits[] = {{3, 5}, {5, 15}, {3, 7}};

static void Board_LED_Init()
{
	uint32_t idx;

	for (idx = 0; idx < (sizeof(gpioLEDBits) / sizeof(io_port_t)); ++idx) {
		/* Set pin direction and init to off */
		Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, gpioLEDBits[idx].port, gpioLEDBits[idx].pin);
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, gpioLEDBits[idx].port, gpioLEDBits[idx].pin, (bool) true);
	}
}
/* Initialize debug output via UART for board */
void Board_Debug_Init(void)
{
#if defined(DEBUG_UART)
	Board_UART_Init(DEBUG_UART);

	Chip_UART_Init(DEBUG_UART);
	Chip_UART_SetBaudFDR(DEBUG_UART, 115200);
	Chip_UART_ConfigData(DEBUG_UART, UART_LCR_WLEN8 | UART_LCR_SBS_1BIT | UART_LCR_PARITY_DIS);

	/* Enable UART Transmit */
	Chip_UART_TXEnable(DEBUG_UART);
#endif
}

/* Set up and initialize all required blocks and functions related to the board hardware
 *
 */
void Board_Init(void)
{
	/* Sets up DEBUG UART */
	//DEBUGINIT();

	/* Initializes GPIO */
	Board_SetupGPIO();

	/* Initialize LEDs */
	Board_LED_Init();

	/* Initialize Buttons */
	Board_Buttons_Init();
}


void Board_UART_Init(LPC_USART_T *pUART)
{
	Chip_SCU_PinMuxSet(0x2, 0, (SCU_MODE_INACT | SCU_MODE_FUNC2));					/* P2,0 : UART0_TXD */
	Chip_SCU_PinMuxSet(0x2, 1, (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC0));/* P2.1 : UART0_RXD */
}

void Board_SSP_Init(LPC_SSP_T *pSSP)
{
	if (pSSP == LPC_SSP0) {
		//Chip_SCU_PinMuxSet(0x3, 1, (SCU_PINIO_FAST | SCU_MODE_FUNC2));  /* P3.6 => SSEL0 */
		Chip_SCU_PinMuxSet(0x3, 3, (SCU_PINIO_FAST | SCU_MODE_FUNC2));  /* P3.3 => SCk0 */

		Chip_SCU_PinMuxSet(0x3, 7, (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC5)); /* P3.8 => MOSI0 */
		Chip_SCU_PinMuxSet(0x3, 6, (SCU_MODE_INACT | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC5)); /* P3.7 => MISO0 */

        Chip_SCU_PinMuxSet(0x3, 1, (SCU_MODE_INACT | SCU_MODE_FUNC4));  /* P3.1 => SSEL0  PORT 5 BIT 8*/
        Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 5, 8);
        Chip_GPIO_SetPinState(LPC_GPIO_PORT, 5, 8, (bool) true);
	}
	else {
		return;
	}
}
void Board_I2C_Init(I2C_ID_T id)
{
	if (id == I2C1) {
		/* Configure pin function for I2C1*/
		Chip_SCU_PinMuxSet(0x2, 3, (SCU_MODE_ZIF_DIS | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC1));		/* P2.3 : I2C1_SDA */
		Chip_SCU_PinMuxSet(0x2, 4, (SCU_MODE_ZIF_DIS | SCU_MODE_INBUFF_EN | SCU_MODE_FUNC1));		/* P2.4 : I2C1_SCL */
	}
	else {
		Chip_SCU_I2C0PinConfig(I2C0_STANDARD_FAST_MODE);
	}
}

void Board_SetupGPIO(void)
{
	Chip_GPIO_Init(LPC_GPIO_PORT);
}


/* Sends a character on the UART */
void Board_UARTPutChar(char ch)
{
#if defined(DEBUG_UART)
	/* Wait for space in FIFO */
	while ((Chip_UART_ReadLineStatus(DEBUG_UART) & UART_LSR_THRE) == 0) {}
	Chip_UART_SendByte(DEBUG_UART, (uint8_t) ch);
#endif
}

/* Gets a character from the UART, returns EOF if no character is ready */
int Board_UARTGetChar(void)
{
#if defined(DEBUG_UART)
	if (Chip_UART_ReadLineStatus(DEBUG_UART) & UART_LSR_RDR) {
		return (int) Chip_UART_ReadByte(DEBUG_UART);
	}
#endif
	return EOF;
}

/* Outputs a string on the debug UART */
void Board_UARTPutSTR(const char *str)
{
#if defined(DEBUG_UART)
	while (*str != '\0') {
		Board_UARTPutChar(*str++);
	}
#endif
}


void Board_LED_Set(uint8_t LEDNumber, bool On)
{
	if (LEDNumber < (sizeof(gpioLEDBits) / sizeof(io_port_t)))
		Chip_GPIO_SetPinState(LPC_GPIO_PORT, gpioLEDBits[LEDNumber].port, gpioLEDBits[LEDNumber].pin, (bool) !On);
}

bool Board_LED_Test(uint8_t LEDNumber)
{
	if (LEDNumber < (sizeof(gpioLEDBits) / sizeof(io_port_t)))
		return (bool) !Chip_GPIO_GetPinState(LPC_GPIO_PORT, gpioLEDBits[LEDNumber].port, gpioLEDBits[LEDNumber].pin);

	return false;
}

void Board_LED_Toggle(uint8_t LEDNumber)
{
	Board_LED_Set(LEDNumber, !Board_LED_Test(LEDNumber));
}

void Board_Buttons_Init(void)
{
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, BUTTONS_BUTTON1_GPIO_PORT_NUM, BUTTONS_BUTTON1_GPIO_BIT_NUM);	// input
	Chip_GPIO_SetPinDIRInput(LPC_GPIO_PORT, BUTTONS_BUTTON2_GPIO_PORT_NUM, BUTTONS_BUTTON2_GPIO_BIT_NUM);	// input
}

/*
 *
 */
uint32_t Buttons_GetStatus(void)
{
	uint8_t ret = 0;
	if (Chip_GPIO_GetPinState(LPC_GPIO_PORT, BUTTONS_BUTTON1_GPIO_PORT_NUM, BUTTONS_BUTTON1_GPIO_BIT_NUM) == 0)
	{
		//Board_UARTPutChar('a');
		ret |= BUTTONS_BUTTON_1;
	}
	else if(Chip_GPIO_GetPinState(LPC_GPIO_PORT, BUTTONS_BUTTON2_GPIO_PORT_NUM, BUTTONS_BUTTON2_GPIO_BIT_NUM) == 0)
	{
		//Board_UARTPutChar('b');
		ret |= BUTTONS_BUTTON_2;
	}
	return ret;
}

