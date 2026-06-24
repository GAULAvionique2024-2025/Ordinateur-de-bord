/*
 * fatfs_sd.c
 *
 *  Created on: Jul 2, 2020
 *      Author: meh
 *
 *  Edited on: Mar 02, 2026
 *      Autor: AudaceLol12
 */


#define TRUE  1
#define FALSE 0
#define bool BYTE

#include "stm32f4xx_hal.h"
#include "main.h"

#include "Drivers/LowLevel/fatfs_sd.h"

extern SPI_HandleTypeDef hspi5;

static volatile DSTATUS Stat = STA_NOINIT;
static uint8_t CardType;

static void SELECT(void) {
	HAL_GPIO_WritePin(SD_SPI5_CS_GPIO_Port, SD_SPI5_CS_Pin, GPIO_PIN_RESET);
}

static void DESELECT(void) {
	HAL_GPIO_WritePin(SD_SPI5_CS_GPIO_Port, SD_SPI5_CS_Pin, GPIO_PIN_SET);
}

static void SPI_TxByte(uint8_t data) {
    HAL_SPI_Transmit(&hspi5, &data, 1, HAL_MAX_DELAY);
}

static uint8_t SPI_RxByte(void) {
    uint8_t dummy = 0xFF;
    uint8_t data;
    HAL_SPI_TransmitReceive(&hspi5, &dummy, &data, 1, HAL_MAX_DELAY);
    return data;
}

static uint8_t SD_ReadyWait(void) {
    uint8_t res;
    uint32_t tickstart = HAL_GetTick();
    do {
        res = SPI_RxByte();
    } while ((res != 0xFF) && ((HAL_GetTick() - tickstart) < 500));
    return res;
}

static void SD_PowerOn(void) {
    uint8_t cmd_arg[6];
    DESELECT();
    for(int i = 0; i < 10; i++) SPI_TxByte(0xFF);
    SELECT();
    cmd_arg[0] = (CMD0 | 0x40);
    cmd_arg[1] = 0; cmd_arg[2] = 0; cmd_arg[3] = 0; cmd_arg[4] = 0;
    cmd_arg[5] = 0x95;
    for (int i = 0; i < 6; i++) SPI_TxByte(cmd_arg[i]);
    while (SPI_RxByte() != 0x01);
    DESELECT();
    SPI_TxByte(0XFF);
}

static bool SD_RxDataBlock(BYTE *buff, UINT btr) {
    uint8_t token;
    uint32_t tickstart = HAL_GetTick();
    do {
        token = SPI_RxByte();
    } while((token == 0xFF) && ((HAL_GetTick() - tickstart) < 200));

    if(token != 0xFE) return 0; // Erreur

    HAL_SPI_Receive(&hspi5, buff, btr, HAL_MAX_DELAY);
    SPI_RxByte();
    SPI_RxByte();
    return 1;
}

static bool SD_TxDataBlock(const BYTE *buff, BYTE token) {
    uint8_t resp;
    if (SD_ReadyWait() != 0xFF) return 0;
    SPI_TxByte(token);

    if (token != 0xFD) {
        HAL_SPI_Transmit(&hspi5, (uint8_t*)buff, 512, HAL_MAX_DELAY);
        SPI_RxByte();
        SPI_RxByte();

        while (1) {
            resp = SPI_RxByte();
            if ((resp & 0x1F) == 0x05) break;
        }
        while (SPI_RxByte() == 0);
    }
    return ((resp & 0x1F) == 0x05) ? 1 : 0;
}

static BYTE SD_SendCmd(BYTE cmd, DWORD arg) {
    uint8_t crc, res;
    if (SD_ReadyWait() != 0xFF) return 0xFF;
    SPI_TxByte(cmd);
    SPI_TxByte((BYTE) (arg >> 24));
    SPI_TxByte((BYTE) (arg >> 16));
    SPI_TxByte((BYTE) (arg >> 8));
    SPI_TxByte((BYTE) arg);
    crc = 0x01;
    if (cmd == CMD0) crc = 0x95;
    if (cmd == CMD8) crc = 0x87;
    SPI_TxByte(crc);
    if (cmd == CMD12) SPI_RxByte();

    uint8_t n = 10;
    do {
        res = SPI_RxByte();
    } while ((res & 0x80) && --n);
    return res;
}

DSTATUS SD_disk_initialize(BYTE drv) {
    uint8_t n, type, ocr[4];
    if(drv) return STA_NOINIT;
    SD_PowerOn();
    SELECT();
    type = 0;
    if (SD_SendCmd(CMD0, 0) == 1) {
        uint32_t tickstart = HAL_GetTick();
        if (SD_SendCmd(CMD8, 0x1AA) == 1) {
            for (n = 0; n < 4; n++) ocr[n] = SPI_RxByte();
            if (ocr[2] == 0x01 && ocr[3] == 0xAA) {
                do {
                    if (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 1UL << 30) == 0) break;
                } while (HAL_GetTick() - tickstart < 1000);
                if ((HAL_GetTick() - tickstart < 1000) && SD_SendCmd(CMD58, 0) == 0) {
                    for (n = 0; n < 4; n++) ocr[n] = SPI_RxByte();
                    type = (ocr[0] & 0x40) ? 6 : 2;
                }
            }
        } else {
            type = (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 0) <= 1) ? 2 : 1;
            do {
                if (type == 2) { if (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 0) == 0) break; }
                else { if (SD_SendCmd(CMD1, 0) == 0) break; }
            } while (HAL_GetTick() - tickstart < 1000);
            if ((HAL_GetTick() - tickstart >= 1000) || SD_SendCmd(CMD16, 512) != 0) type = 0;
        }
    }
    CardType = type;
    DESELECT();
    SPI_RxByte();
    if (type) Stat &= ~STA_NOINIT;
    else Stat = STA_NOINIT;
    return Stat;
}

DSTATUS SD_disk_status(BYTE drv) {
    if (drv) return STA_NOINIT;
    return Stat;
}

DRESULT SD_disk_read(BYTE pdrv, BYTE* buff, DWORD sector, UINT count) {
    if (pdrv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;
    if (!(CardType & 4)) sector *= 512;
    SELECT();
    if (count == 1) {
        if ((SD_SendCmd(CMD17, sector) == 0) && SD_RxDataBlock(buff, 512)) count = 0;
    } else {
        if (SD_SendCmd(CMD18, sector) == 0) {
            do {
                if (!SD_RxDataBlock(buff, 512)) break;
                buff += 512;
            } while (--count);
            SD_SendCmd(CMD12, 0);
        }
    }
    DESELECT();
    SPI_RxByte();
    return count ? RES_ERROR : RES_OK;
}

DRESULT SD_disk_write(BYTE pdrv, const BYTE* buff, DWORD sector, UINT count) {
    if (pdrv || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;
    if (!(CardType & 4)) sector *= 512;
    SELECT();
    if (count == 1) {
        if ((SD_SendCmd(CMD24, sector) == 0) && SD_TxDataBlock(buff, 0xFE)) count = 0;
    } else {
        if (CardType & 2) { SD_SendCmd(CMD55, 0); SD_SendCmd(CMD23, count); }
        if (SD_SendCmd(CMD25, sector) == 0) {
            do {
                if(!SD_TxDataBlock(buff, 0xFC)) break;
                buff += 512;
            } while (--count);
            if(!SD_TxDataBlock(0, 0xFD)) count = 1;
        }
    }
    DESELECT();
    SPI_RxByte();
    return count ? RES_ERROR : RES_OK;
}

DRESULT SD_disk_ioctl(BYTE drv, BYTE ctrl, void *buff) {
    DRESULT res = RES_ERROR;
    BYTE n, csd[16];
    WORD csize;

    if (drv) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    SELECT();
    switch (ctrl) {
        case GET_SECTOR_COUNT:
            if ((SD_SendCmd(CMD9, 0) == 0) && SD_RxDataBlock(csd, 16)) {
                if ((csd[0] >> 6) == 1) {
                    csize = csd[9] + ((WORD) csd[8] << 8) + 1;
                    *(DWORD*) buff = (DWORD) csize << 10;
                } else {
                    n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
                    csize = (csd[8] >> 6) + ((WORD) csd[7] << 2) + ((WORD) (csd[6] & 3) << 10) + 1;
                    *(DWORD*) buff = (DWORD) csize << (n - 9);
                }
                res = RES_OK;
            }
            break;

        case GET_SECTOR_SIZE:
            *(WORD*) buff = 512;
            res = RES_OK;
            break;

        case CTRL_SYNC:
            if (SD_ReadyWait() == 0xFF) {
                res = RES_OK;
            }
            break;

        default:
            res = RES_PARERR;
    }
    DESELECT();
    SPI_RxByte();

    return res;
}
