/*
 * MachXO3D.cpp
 *
 *  Created on: 21. 6. 2021
 *      Author: xkoupy00
 */
#include "machxo.h"
/*
 *
 */
void cmd_readDeviceID(uint8_t *obuf, uint8_t *ibuf_obuf_size) {
	obuf[0] = 0xE0;
	obuf[1] = 0x00;
	obuf[2] = 0x00;
	obuf[3] = 0x00;
	ibuf_obuf_size[0] = 0x04;
	ibuf_obuf_size[1] = 0x04;
}
/*
 *
 */
void cmd_readUserCode(uint8_t *obuf, uint8_t *ibuf_obuf_size) {

	obuf[0] = 0xC0;
	obuf[1] = 0x00;
	obuf[2] = 0x00;
	obuf[3] = 0x00;
	ibuf_obuf_size[0] = 0x04;
	ibuf_obuf_size[1] = 0x04;
}
/*
 *
 */
void cmd_readStatus(uint8_t *obuf, uint8_t *ibuf_obuf_size) {
	obuf[0] = 0x3C;
	obuf[1] = 0x00;
	obuf[2] = 0x00;
	obuf[3] = 0x00;
	ibuf_obuf_size[0] = 0x04;
	ibuf_obuf_size[1] = 0x04;
}
/*
 *
 */
void cmd_readFeatureBits(uint8_t *obuf, uint8_t *ibuf_obuf_size) {

	obuf[0] = 0xFB;
	obuf[1] = 0x00;
	obuf[2] = 0x00;
	obuf[3] = 0x00;
	ibuf_obuf_size[0] = 0x04;
	ibuf_obuf_size[1] = 0x02;
}
/*
 *
 */
void cmd_readFeatureRow(uint8_t *obuf, uint8_t *ibuf_obuf_size) {
	obuf[0] = 0xE7;
	obuf[1] = 0x00;
	obuf[2] = 0x00;
	obuf[3] = 0x00;
	ibuf_obuf_size[0] = 0x04;
	ibuf_obuf_size[1] = 0x08;
}
/*
 *
 */
void cmd_readOTPFuses(uint8_t *obuf, uint8_t *ibuf_obuf_size) {
	obuf[0] = 0xFA;
	obuf[1] = 0x00;
	obuf[2] = 0x00;
	obuf[3] = 0x00;
	ibuf_obuf_size[0] = 0x04;
	ibuf_obuf_size[1] = 0x01;
}
/*
 *
 */
void cmd_readFlash(uint8_t *obuf, uint8_t *ibuf_obuf_size) {
	obuf[0] = 0x73;
	obuf[1] = 0x00;
	obuf[2] = 0x00;
	obuf[3] = 0x00;
	ibuf_obuf_size[0] = 0x04;
	ibuf_obuf_size[1] = 16;
}
/*
 *
 */
void cmd_readUFM(uint8_t *obuf, uint8_t *ibuf_obuf_size) {
	obuf[0] = 0xCA;
	obuf[1] = 0x00;
	obuf[2] = 0x00;
	obuf[3] = 0x00;
	ibuf_obuf_size[0] = 0x04;
	ibuf_obuf_size[1] = 16;
}
/*
 *
 */
void cmd_eraseUFM(uint8_t *obuf, uint8_t *ibuf_obuf_size) {
	obuf[0] = 0xCB;
	obuf[1] = 0x00;
	obuf[2] = 0x00;
	obuf[3] = 0x00;
	ibuf_obuf_size[0] = 0x04;
	ibuf_obuf_size[1] = 0x00;
}
/*
 *
 */
void cmd_erase(uint8_t *obuf, uint8_t *ibuf_obuf_size, uint32_t flags) {
	obuf[0] = 0x0E;
	obuf[1] = (0x0F & (flags >> 16));
	obuf[2] = 0x00;
	obuf[3] = 0x00;
	ibuf_obuf_size[0] = 0x04;
	ibuf_obuf_size[1] = 0x00;
}
/*
 *
 */
void cmd_enableConfigTransparent(uint8_t *obuf, uint8_t *ibuf_obuf_size) {
	obuf[0] = 0x74;
	obuf[1] = 0x08;
	obuf[2] = 0x00;
	ibuf_obuf_size[0] = 0x03;
	ibuf_obuf_size[1] = 0x00;

}
/*
 *
 */
void cmd_enableConfigOffline(uint8_t *obuf, uint8_t *ibuf_obuf_size) {
	obuf[0] = 0xC6;
	obuf[1] = 0x08;
	obuf[2] = 0x00;
	ibuf_obuf_size[0] = 0x03;
	ibuf_obuf_size[1] = 0x00;
}
/*
 *
 */
void cmd_isBusy(uint8_t *obuf, uint8_t *ibuf_obuf_size) {
	obuf[0] = 0xF0;
	obuf[1] = 0x00;
	obuf[2] = 0x00;
	obuf[3] = 0x00;
	ibuf_obuf_size[0] = 0x04;
	ibuf_obuf_size[1] = 0x01;
}
/*
 *
 */
void cmd_resetConfigAddress(uint8_t *obuf, uint8_t *ibuf_obuf_size) {
	obuf[0] = 0x46;
	obuf[1] = 0x00;
	obuf[2] = 0x00;
	obuf[3] = 0x00;
	ibuf_obuf_size[0] = 0x04;
	ibuf_obuf_size[1] = 0x00;
}
/*
 *
 */
void cmd_resetUFMAddress(uint8_t *obuf, uint8_t *ibuf_obuf_size) {
	obuf[0] = 0x47;
	obuf[1] = 0x00;
	obuf[2] = 0x00;
	obuf[3] = 0x00;
	ibuf_obuf_size[0] = 0x04;
	ibuf_obuf_size[1] = 0x00;
}
/*
 *
 */
void cmd_setConfigAddress(uint8_t *obuf, uint8_t *ibuf_obuf_size, uint32_t page) {
	obuf[0] = 0xB4;
	obuf[1] = 0x00;
	obuf[2] = 0x00;
	obuf[3] = 0x00;
	obuf[4] = 0x00;
	obuf[5] = 0x00;
	obuf[6] = (page >> 8) & 0xFF;
	obuf[7] = (page) & 0xFF;
	ibuf_obuf_size[0] = 0x08;
	ibuf_obuf_size[1] = 0x00;
}
/*
 *
 */
void cmd_setUFMAddress(uint8_t *obuf, uint8_t *ibuf_obuf_size, uint32_t page) {
	obuf[0] = 0xB4;
	obuf[1] = 0x00;
	obuf[2] = 0x00;
	obuf[3] = 0x00;
	obuf[4] = 0x40;
	obuf[5] = 0x00;
	obuf[6] = (page >> 8) & 0xFF;
	obuf[7] = (page) & 0xFF;
	ibuf_obuf_size[0] = 0x08;
	ibuf_obuf_size[1] = 0x00;
}
/*
 *
 */
void cmd_programPage(uint8_t *ibuf_obuf_size) {
	ibuf_obuf_size[0] = 0x20;
	ibuf_obuf_size[1] = 0x00;
	//waitBusy(); // a 200us delay is also acceptable, should not be needed with I2C
}
/*
 *
 */
void cmd_programDone(uint8_t *obuf, uint8_t *ibuf_obuf_size) {
	obuf[0] = 0x5E;
	obuf[1] = 0x00;
	obuf[2] = 0x00;
	obuf[3] = 0x00;
	ibuf_obuf_size[0] = 0x04;
	ibuf_obuf_size[1] = 0x00;
}
/*
 *
 */
void cmd_refresh(uint8_t *obuf, uint8_t *ibuf_obuf_size) {
	obuf[0] = 0x79;
	obuf[1] = 0x00;
	obuf[2] = 0x00;
	ibuf_obuf_size[0] = 0x03;
	ibuf_obuf_size[1] = 0x00;
}
/*
 *
 */
void cmd_wakeup(uint8_t *obuf, uint8_t *ibuf_obuf_size) {
	obuf[0] = 0xFF;
	obuf[1] = 0xFF;
	obuf[2] = 0xFF;
	obuf[3] = 0xFF;
	ibuf_obuf_size[0] = 0x04;
	ibuf_obuf_size[1] = 0x00;
}

/*void cmd_loadHex(FatFile hexFile) {
	int byteCnt = 0;
	//  FatFile hexFile = fatfs->open(fileName);
	int pageCnt = 0;
	char nextChr;
	char hexByteStr[3] = { 0, 0, 0 };
	uint8_t pageBuf[20];
	pageBuf[0] = 0x70;
	pageBuf[1] = 0x00;
	pageBuf[2] = 0x00;
	pageBuf[3] = 0x01;
	resetConfigAddress();
	while (hexFile.available()) {
		nextChr = hexFile.read();
		if (!isHexadecimalDigit(nextChr)) {
			byteCnt = 0;
		} else {
			if (byteCnt > 15) {
				DBG_MSG.println("too many hex digits");
			} else {
				if (hexFile.available()) {
					hexByteStr[0] = nextChr;
					hexByteStr[1] = hexFile.read();
					pageBuf[byteCnt + 4] = strtoul(hexByteStr, NULL, 16);
					byteCnt += 1;
					if (byteCnt == 16) {
						programPage(pageBuf);
						pageCnt += 1;
					}
				} else {
					DBG_MSG.println("uneven number of hex digits");
				}
			}
		}
	}
	if (byteCnt == 16) {
		byteCnt = 0;
	}
	hexFile.close();
	return (byteCnt);
}
*/
