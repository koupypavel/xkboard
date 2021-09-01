/*
 ===============================================================================
 Name        : i2c_config.c
 Author      : xkoupy00
 Version     : 1.0
 Description : main definition example of config com with FPGA. Prints detils about FGPA 
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

#include "machxo.h"

static void printHex(char * str, uint8_t *arr, uint8_t size)
{
	Board_UARTPutSTR(str);
}
#define SPEED_100KHZ         100000
#define SPEED_400KHZ         400000


static I2CM_XFER_T i2cmXferRec;

#define DEBUGHEXPRINT(str, arr, size) printHex(str, arr, size)

/* Initialize the I2C bus */
static void i2c_app_init(I2C_ID_T id, int speed)
{
	Board_I2C_Init(id);
	/* Initialize I2C */
	Chip_I2C_Init(id);
	Chip_I2C_SetClockRate(id, speed);
	/* Set default mode to interrupt */
	Chip_I2C_SetMasterEventHandler(id, Chip_I2C_EventHandler);
	/* Set I2C mode to interrupt */
	if (id == I2C0) {
		NVIC_EnableIRQ(I2C0_IRQn);
	} else {
		NVIC_EnableIRQ(I2C1_IRQn);
	}

}

/* Function to wait for I2CM transfer completion */
static void WaitForI2cXferComplete(I2CM_XFER_T *xferRecPtr)
{
	/* Test for still transferring data */
	while (xferRecPtr->status == I2CM_STATUS_BUSY) {
		/* Sleep until next interrupt */
		__WFI();
	}
}
/*
 *
 */
static void SetupXferRecAndExecute(uint8_t devAddr, uint8_t *txBuffPtr,
		uint16_t txSize, uint8_t *rxBuffPtr, uint16_t rxSize) {
	/* Setup I2C transfer record */
	i2cmXferRec.slaveAddr = devAddr;
	i2cmXferRec.options = 0;
	i2cmXferRec.status = 0;
	i2cmXferRec.txSz = txSize;
	i2cmXferRec.rxSz = rxSize;
	i2cmXferRec.txBuff = txBuffPtr;
	i2cmXferRec.rxBuff = rxBuffPtr;
	Chip_I2CM_Xfer(LPC_I2C0, &i2cmXferRec);

	/* Wait for transfer completion */
	WaitForI2cXferComplete(&i2cmXferRec);
}
/*
 *
 */
void clearBuffers(uint8_t* buf, uint8_t* buf1)
{
    for (int i =0 ; i < 8; i++ )
    {
    	buf[i] = 0x00;
    	buf1[i] = 0x00;
    }
}
/*
 *
 */
void printDetails() {
	uint8_t txBuf[8] = {0x00, 0x00, 0x00 , 0x00 ,0x00, 0x00, 0x00, 0x00};
	uint8_t rxBuf[8] = {0x00, 0x00, 0x00 , 0x00 ,0x00, 0x00, 0x00, 0x00};
	uint8_t inbuf_outbuf_sizes[2] = {0x00, 0x00};

	cmd_readDeviceID(txBuf, inbuf_outbuf_sizes);
	SetupXferRecAndExecute(MACHXO_I2C_ADDR, txBuf, inbuf_outbuf_sizes[0], rxBuf, inbuf_outbuf_sizes[1]);
	DEBUGHEXPRINT("Device ID:  ", rxBuf, inbuf_outbuf_sizes[1]);

	clearBuffers(txBuf, rxBuf);
	cmd_readUserCode(txBuf, inbuf_outbuf_sizes);
	SetupXferRecAndExecute(MACHXO_I2C_ADDR, txBuf, inbuf_outbuf_sizes[0], rxBuf, inbuf_outbuf_sizes[1]);
	DEBUGHEXPRINT("User Code:  ", rxBuf, inbuf_outbuf_sizes[1]);

	clearBuffers(txBuf, rxBuf);
	cmd_readFeatureRow(txBuf, inbuf_outbuf_sizes);
	SetupXferRecAndExecute(MACHXO_I2C_ADDR, txBuf, inbuf_outbuf_sizes[0], rxBuf, inbuf_outbuf_sizes[1]);
	DEBUGHEXPRINT("Feature Row:  ", rxBuf, inbuf_outbuf_sizes[1]);

	clearBuffers(txBuf, rxBuf);
	cmd_readFeatureBits(txBuf, inbuf_outbuf_sizes);
	SetupXferRecAndExecute(MACHXO_I2C_ADDR, txBuf, inbuf_outbuf_sizes[0], rxBuf, inbuf_outbuf_sizes[1]);
	DEBUGHEXPRINT("Feature Bits:  ", rxBuf, inbuf_outbuf_sizes[1]);

	clearBuffers(txBuf, rxBuf);
	cmd_readStatus(txBuf, inbuf_outbuf_sizes);
	SetupXferRecAndExecute(MACHXO_I2C_ADDR, txBuf, inbuf_outbuf_sizes[0], rxBuf, inbuf_outbuf_sizes[1]);
	DEBUGHEXPRINT("Status:  ", rxBuf, inbuf_outbuf_sizes[1]);
}
/*
 *
 */
void configDevice(uint8_t *obuf, uint8_t *ibuf_obuf_size) {

	//cmd_enableConfigOffline();
	//SetupXferRecAndExecute(I2C_ADDR_7BIT, txBuf, inbuf_outbuf_sizes[0], rxBuf, inbuf_outbuf_sizes[1]);
	DEBUGOUT("Erasing Configuration and UFM");
	//cmd_erase(MACHXO_ERASE_CONFIG_FLASH | MACHXO_ERASE_UFM);

	DEBUGOUT("Waiting for erase to complete");
	//cmd_waitBusy();
	//SetupXferRecAndExecute(I2C_ADDR_7BIT, txBuf, inbuf_outbuf_sizes[0], rxBuf, inbuf_outbuf_sizes[1]);
	//DEBUGOUT("Loading ");
	//cmd_loadHex(file);
	//SetupXferRecAndExecute(I2C_ADDR_7BIT, txBuf, inbuf_outbuf_sizes[0], rxBuf, inbuf_outbuf_sizes[1]);
	// file.close();

	DEBUGOUT("Programming DONE status bit");
	//cmd_programDone();
	//SetupXferRecAndExecute(I2C_ADDR_7BIT, txBuf, inbuf_outbuf_sizes[0], rxBuf, inbuf_outbuf_sizes[1]);
	DEBUGOUT("Refreshing image");
	//cmd_refresh();
	//SetupXferRecAndExecute(I2C_ADDR_7BIT, txBuf, inbuf_outbuf_sizes[0], rxBuf, inbuf_outbuf_sizes[1]);
	DEBUGOUT("Loaded ");
}
uint32_t waitBusy() {
	uint8_t txBuf[8] = {0x00, 0x00, 0x00 , 0x00 ,0x00, 0x00, 0x00, 0x00};
	uint8_t rxBuf[8] = {0x00, 0x00, 0x00 , 0x00 ,0x00, 0x00, 0x00, 0x00};
	uint8_t inbuf_outbuf_sizes[2] = {0x00, 0x00};
	cmd_isBusy(txBuf, inbuf_outbuf_sizes);
	uint32_t waitCnt = 0;
	while ((rxBuf[0] & 0x80) ? 1 : 0) {
		SetupXferRecAndExecute(MACHXO_I2C_ADDR, txBuf, inbuf_outbuf_sizes[0], rxBuf, inbuf_outbuf_sizes[1]);
		waitCnt += 1;
	}
	return (waitCnt);
}

void deviceErase() {
	uint8_t txBuf[8] = {0x00, 0x00, 0x00 , 0x00 ,0x00, 0x00, 0x00, 0x00};
	uint8_t rxBuf[8] = {0x00, 0x00, 0x00 , 0x00 ,0x00, 0x00, 0x00, 0x00};
	uint8_t inbuf_outbuf_sizes[2] = {0x00, 0x00};
	cmd_erase(txBuf, inbuf_outbuf_sizes,MACHXO_ERASE_CONFIG_FLASH | MACHXO_ERASE_UFM);
	SetupXferRecAndExecute(MACHXO_I2C_ADDR, txBuf, inbuf_outbuf_sizes[0], rxBuf, inbuf_outbuf_sizes[1]);
	//DEBUGHEXPRINT("Erasing...");
	waitBusy();
	//DEBUGHEXPRINT("Erased Configuration and UFM");
}


/**
 * @brief	Handle I2C0 interrupt by calling I2CM interrupt transfer handler
 * @return	Nothing
 */
void I2C0_IRQHandler(void) {
	/* Call I2CM ISR function with the I2C device and transfer rec */
	Chip_I2CM_XferHandler(LPC_I2C0, &i2cmXferRec);
}

int main(void) {

	SystemCoreClockUpdate();

    Board_Init();

    // TODO: insert code here
	i2c_app_init(I2C0, SPEED_100KHZ);

	printDetails();
	// Force the counter to be placed into memory
	volatile static int i = 0;
	// Enter an infinite loop, just incrementing a counter
	while (1) {
		i++;
		// "Dummy" NOP to allow source level single
		// stepping of tight while() loop
		__asm volatile ("nop");
	}
	return 0;
}
