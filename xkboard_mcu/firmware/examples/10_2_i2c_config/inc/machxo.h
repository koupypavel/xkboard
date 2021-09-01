/*
 * machxo.h
 *
 *  Created on: 17. 7. 2021
 *      Author: pavel
 */

#ifndef MACHXO_H_
#define MACHXO_H_
#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif
#define MACHXO_I2C_ADDR       0x40

#define DBG_MSG               if (_verbose) Serial
// Erase Flag Enumeration
enum {
    MACHXO_ERASE_SRAM = (1 << 16),
    MACHXO_ERASE_FEATURE_ROW = (1 << 17),
    MACHXO_ERASE_CONFIG_FLASH = (1 << 18),
    MACHXO_ERASE_UFM = (1 << 19),
};

    //void begin(uint8_t verbose = 0);
    void cmd_readDeviceID(uint8_t *obuf, uint8_t *ibuf_obuf_size);
    void cmd_readUserCode(uint8_t *obuf, uint8_t *ibuf_obuf_size);
    void cmd_readStatus(uint8_t *obuf, uint8_t *ibuf_obuf_size);
    void cmd_readFeatureBits(uint8_t *obuf, uint8_t *ibuf_obuf_size);
    void cmd_readFeatureRow(uint8_t *obuf, uint8_t *ibuf_obuf_size);
    void cmd_readOTPFuses(uint8_t *obuf, uint8_t *ibuf_obuf_size);
    void cmd_readFlash(uint8_t *obuf, uint8_t *ibuf_obuf_size);
    void cmd_readUFM(uint8_t *obuf, uint8_t *ibuf_obuf_size);
    void cmd_eraseUFM(uint8_t *obuf, uint8_t *ibuf_obuf_size);
    void cmd_erase(uint8_t *obuf, uint8_t *ibuf_obuf_size, uint32_t flags);
    void cmd_enableConfigTransparent(uint8_t *obuf, uint8_t *ibuf_obuf_size);
    void cmd_enableConfigOffline(uint8_t *obuf, uint8_t *ibuf_obuf_size);
    void cmd_isBusy(uint8_t *obuf, uint8_t *ibuf_obuf_size);
    void cmd_resetConfigAddress(uint8_t *obuf, uint8_t *ibuf_obuf_size);
    void cmd_resetUFMAddress(uint8_t *obuf, uint8_t *ibuf_obuf_size);
    void cmd_setConfigAddress(uint8_t *obuf, uint8_t *ibuf_obuf_size, uint32_t page);
    void cmd_setUFMAddress(uint8_t *obuf, uint8_t *ibuf_obuf_size, uint32_t page);
    void cmd_programDone(uint8_t *obuf, uint8_t *ibuf_obuf_size);
    void cmd_refresh(uint8_t *obuf, uint8_t *ibuf_obuf_size);
    void cmd_wakeup(uint8_t *obuf, uint8_t *ibuf_obuf_size);

#endif /* MACHXO_H_ */
