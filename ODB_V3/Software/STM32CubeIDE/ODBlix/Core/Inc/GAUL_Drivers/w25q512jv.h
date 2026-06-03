/*
 * w25q512jv.h
 *
 * Created on: 12 avr. 2026
 * Author: gagno
 */

#ifndef INC_GAUL_DRIVERS_W25Q512JV_H_
#define INC_GAUL_DRIVERS_W25Q512JV_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>

#define W25Q512_JEDEC_ID                    0xEF4020

#define W25Q512_FLASH_SIZE                  0x4000000 // 512 MBits => 64 MBytes
#define W25Q512_SECTOR_SIZE                 0x1000    // 4 Ko
#define W25Q512_BLOCK_SIZE                  0x10000   // 64 Ko
#define W25Q512_PAGE_SIZE                   0x100     // 256 octets

#define W25Q_CMD_RESET_ENABLE               0x66
#define W25Q_CMD_RESET_MEMORY               0x99
#define W25Q_CMD_READ_ID                    0x9F      // JEDEC ID
#define W25Q_CMD_WRITE_ENABLE               0x06
#define W25Q_CMD_WRITE_DISABLE              0x04

#define W25Q_CMD_READ_STATUS_REG1           0x05
#define W25Q_CMD_READ_STATUS_REG2           0x35
#define W25Q_CMD_READ_STATUS_REG3           0x15
#define W25Q_CMD_WRITE_STATUS_REG1          0x01
#define W25Q_CMD_WRITE_STATUS_REG2          0x31
#define W25Q_CMD_WRITE_STATUS_REG3          0x11

#define W25Q_CMD_ENTER_4B_MODE              0xB7
#define W25Q_CMD_EXIT_4B_MODE               0xE9

#define W25Q_CMD_FAST_READ_QUAD_IO_4B       0xEC
#define W25Q_CMD_QUAD_PAGE_PROGRAM_4B       0x34
#define W25Q_CMD_SECTOR_ERASE_4B            0x21      // Erase 4Ko
#define W25Q_CMD_BLOCK_ERASE_64K_4B         0xDC      // Erase 64Ko
#define W25Q_CMD_CHIP_ERASE                 0xC7      // Erase all

#define W25Q_CMD_DEEP_POWER_DOWN            0xB9
#define W25Q_CMD_RELEASE_POWER_DOWN         0xAB

#define W25Q_SR1_BUSY                       0x01
#define W25Q_SR1_WEL                        0x02
#define W25Q_SR2_QE                         0x02      // Quad enable (Bit 1, Status Reg 2)

#define W25Q_TIMEOUT                        5000
#define W25Q_ERASE_ALL_TIMEOUT              1000000

#define W25Q512_FLASH_SIZE_BYTE         	((uint32_t)67108864)
#define FLASH_SECTOR_SIZE_BYTE          	((uint32_t)4096)

typedef struct {
	QSPI_HandleTypeDef *hqspi;
} w25q_t;


int8_t W25Q_Init(w25q_t *dev);

int8_t W25Q_Read(w25q_t *dev, uint8_t* pData, uint32_t read_addr, uint32_t size);
int8_t W25Q_WritePage(w25q_t *dev, uint8_t* pData, uint32_t write_addr, uint32_t size);
int8_t W25Q_WritePageNoWait(w25q_t *dev, uint8_t* pData, uint32_t write_addr, uint32_t size);
int8_t W25Q_EraseSector(w25q_t *dev, uint32_t sector_addr);
int8_t W25Q_EraseBlock(w25q_t *dev, uint32_t block_addr);
int8_t W25Q_EraseChip(w25q_t *dev);
bool W25Q_IsBusy(QSPI_HandleTypeDef *hqspi);
int8_t W25Q_SetMemoryMappedMode(w25q_t *dev, bool enable);    // If enable is true, the flash will be mapped to the QSPI memory space, allowing for direct read access. If false, the flash will be in indirect mode, requiring commands to read/write.
int8_t W25Q_SetDeepPowerDown(w25q_t *dev, bool enable);       // If enable is true, the flash will enter deep power down mode, reducing power consumption. In this mode, the flash will not respond to any command except for the release from deep power down command. If false, the flash will exit deep power down mode and be ready for normal operation. Incompatible with memory mapped mode.

#endif /* INC_GAUL_DRIVERS_W25Q512JV_H_ */
