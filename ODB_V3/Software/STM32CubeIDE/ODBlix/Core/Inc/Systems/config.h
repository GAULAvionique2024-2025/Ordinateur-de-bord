/*
 * config.h
 *
 *  Created on: 24 avr. 2026
 *      Author: gagno
 */

#ifndef INC_SYSTEMS_CONFIG_H_
#define INC_SYSTEMS_CONFIG_H_

#include "Drivers/w25q512jv.h"
#include "Protocols/config_protocol.h"
#include "Utils/utils.h"
#include <stdint.h>

/* === Constants === */
#define EXPECTED_CONFIG_COUNT 22

/* === Flash Mapping Configuration === */
#define FLASH_CONFIG_START_ADDRESS      (W25Q512_FLASH_SIZE_BYTE - FLASH_SECTOR_SIZE_BYTE) // 0x03FFF000
#define LOGGER_MAX_ALLOWED_ADDRESS      FLASH_CONFIG_START_ADDRESS
#define LOGGER_MAGIC_HEADER             0x48454144 // HEAD
#define LOGGER_DATA_MAGIC_NUMBER	    0x44415441 // DATA
#define LOGGER_STATS_MAGIC_NUMBER	    0x53544154 // STAT
#define CONFIG_MAGIC_NUMBER  			0x434F4E46 // CONF
/* =========== */

extern odb_config_t current_config;

void Config_Init(void);
int8_t Config_SaveToFlash(void);
void Config_LoadDefaults(void);
const odb_config_t* Config_Get(void);
void Config_ResetTransaction(void);

int Config_Erase(void);

#endif /* INC_SYSTEMS_CONFIG_H_ */
