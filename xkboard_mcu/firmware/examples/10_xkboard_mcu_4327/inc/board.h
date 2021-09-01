/*
 * xkboard.h
 *
 *  Created on: 13. 5. 2021
 *      Author: xkoupy00
 */

#ifndef BOARD_H_
#define BOARD_H_

#include "chip.h"
/* board_api.h is included at the bottom of this file after DEBUG setup */

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup BOARD_XKBOARD_MCU_4327 LPC4327 XKBoard MCU board support software API functions
 * @ingroup LPCOPEN_43XX_BOARD_XKBOARD_4327
 * The board support software API functions provide some simple abstracted
 * functions used across multiple LPCOpen board examples. See @ref BOARD_COMMON_API
 * for the functions defined by this board support layer.<br>
 * @{
 */

/** @defgroup BOARD_XKBOARD_MCU_4327_OPTIONS BOARD: LPC4327 XKBoard MCU board options
 * This board has options that configure its operation at build-time.<br>
 * @{
 */

/** Define DEBUG_ENABLE to enable IO via the DEBUGSTR, DEBUGOUT, and
    DEBUGIN macros. If not defined, DEBUG* functions will be optimized
	out of the code at build time.
 */
#define DEBUG_ENABLE

/** Define DEBUG_SEMIHOSTING along with DEBUG_ENABLE to enable IO support
    via semihosting. You may need to use a C library that supports
	semihosting with this option.
 */
//#define DEBUG_SEMIHOSTING

/** Board UART used for debug output and input using the DEBUG* macros. This
    is also the port used for Board_UARTPutChar, Board_UARTGetChar, and
	Board_UARTPutSTR functions. */
#define DEBUG_UART LPC_USART0
/**
 * @}
 */

/* Board name */
#define BOARD_XKBOARD_MCU

#define DEBUG_UART LPC_USART0

typedef struct {
	uint8_t port;
	uint8_t pin;
} io_port_t;

#define BUTTONS_BUTTON1_GPIO_PORT_NUM   3
#define BUTTONS_BUTTON1_GPIO_BIT_NUM    2
#define BUTTONS_BUTTON2_GPIO_PORT_NUM   0
#define BUTTONS_BUTTON2_GPIO_BIT_NUM    5
#define LED1_GPIO_PORT_NUM              5
#define LED1_GPIO_BIT_NUM               15
#define LED2_GPIO_PORT_NUM              3
#define LED2_GPIO_BIT_NUM               5
#define LED3_GPIO_PORT_NUM              3
#define LED3_GPIO_BIT_NUM               7

#define BUTTONS_BUTTON_1     0x01
#define BUTTONS_BUTTON_2     0x02
/**
 * @brief	Sets up board specific I2C interface
 * @param	id	: I2C Peripheral ID (I2C0, I2C1)
 * @return	Nothing
 */
void Board_I2C_Init(I2C_ID_T id);

/**
 * @brief	Sets up I2C Fast Plus mode
 * @param	id	: Must always be I2C0
 * @return	Nothing
 * @note	This function must be called before calling
 *          Chip_I2C_SetClockRate() to set clock rates above
 *          normal range 100KHz to 400KHz. Only I2C0 supports
 *          this mode.
 */
STATIC INLINE void Board_I2C_EnableFastPlus(I2C_ID_T id)
{
	Chip_SCU_I2C0PinConfig(I2C0_FAST_MODE_PLUS);
}

/**
 * @brief	Disable I2C Fast Plus mode and enables default mode
 * @param	id	: Must always be I2C0
 * @return	Nothing
 * @sa		Board_I2C_EnableFastPlus()
 */
STATIC INLINE void Board_I2C_DisableFastPlus(I2C_ID_T id)
{
	Chip_SCU_I2C0PinConfig(I2C0_STANDARD_FAST_MODE);
}

/**
 * @brief	Initializes board specific GPIO Interrupt
 * @return	Nothing
 */
void Board_GPIO_Int_Init(void);

/**
 * @brief	Initialize pin muxing for SSP interface
 * @param	pSSP	: Pointer to SSP interface to initialize
 * @return	Nothing
 */
void Board_SSP_Init(LPC_SSP_T *pSSP);


/**
 * @brief	Initialize pin muxing for a UART
 * @param	pUART	: Pointer to UART register block for UART pins to init
 * @return	Nothing
 */
void Board_UART_Init(LPC_USART_T *pUART);

/**
 * @brief	Initialize button(s) interface on board
 * @return	Nothing
 */
void Board_Buttons_Init(void);

/**
 * @brief	Returns button(s) state on board
 * @return	Returns BUTTONS_BUTTON1 if button1 is pressed
 */
uint32_t Buttons_GetStatus (void);
/**
 * @brief	Initialize GPIO of board
 * @return	Nothing
 */
void Board_SetupGPIO(void);

/**
 * @}
 */

#include "board_api.h"

#ifdef __cplusplus
}
#endif

#endif /* BOARD_H_ */
