/*
 * config.h
 *
 *  Created on: 24 avr. 2026
 *      Author: gagno
 */

#ifndef INC_SYSTEMS_CONFIG_H_
#define INC_SYSTEMS_CONFIG_H_

#include "Drivers/w25q512jveim.h"
#include "Protocols/config_protocol.h"
#include "Utils/utils.h"
#include <stdint.h>

/* === Constants === */
#define EXPECTED_CONFIG_COUNT 	22

/* === Flash Mapping Configuration === */
#define FLASH_CONFIG_START_ADDRESS      (W25Q512_FLASH_SIZE_BYTE - FLASH_SECTOR_SIZE_BYTE) // 0x03FFF000
#define LOGGER_MAX_ALLOWED_ADDRESS      FLASH_CONFIG_START_ADDRESS
#define LOGGER_MAGIC_HEADER             0x48454144 // HEAD
#define LOGGER_DATA_MAGIC_NUMBER	    0x44415441 // DATA
#define LOGGER_STATS_MAGIC_NUMBER	    0x53544154 // STAT
#define CONFIG_MAGIC_NUMBER  			0x434F4E46 // CONF
/* =========== */

/* === Limits === */
/* === Stage Limits --- */
#define CONFIG_STAGE_ROLE_MIN			(2U)
#define CONFIG_STAGE_ROLE_MAX			(3U)
/* --- Pyro Limits --- */
#define CONFIG_PYRO_NEEDED_MIN			(1U)
#define CONFIG_PYRO_NEEDED_MAX			(4U)
#define CONFIG_PYRO_ATTEMPTS_MIN		(1U)
/* --- Acceleration & Thresholds --- */
#define CONFIG_ACC_Z_LAUNCH_MIN_MS2		(0.0f)
#define CONFIG_ACC_Z_LAUNCH_MAX_MS2		(39.24f)
/* --- Altitude Thresholds --- */
#define CONFIG_MAIN_DEPLOY_ALT_MIN_M	(50.0f)
/* --- Velocity Thresholds --- */
#define CONFIG_APOGEE_DETECT_V_MAX_MS	(0.0f)
/* =========== */

typedef enum {
    CONFIG_VALID_OK = 0,
    CONFIG_ERR_MAGIC_NUMBER = -1,
    CONFIG_ERR_VERSION = -2,
    CONFIG_ERR_STAGE_ROLE = -3,
	CONFIG_ERR_PROFILE_AXIS = -4,
    CONFIG_ERR_TIMING_CONFLICT = -5,
    CONFIG_ERR_PYRO_LIMITS = -6,
	CONFIG_ERR_PYRO_ROLE = -7,
    CONFIG_ERR_THRESHOLDS = -8
} config_error_t;

extern odb_config_t current_config;

void Config_Init(void);
int8_t Config_SaveToFlash(void);
void Config_LoadDefaults(void);
const odb_config_t* Config_Get(void);
void Config_ResetTransaction(void);

int Config_Erase(void);

#endif /* INC_SYSTEMS_CONFIG_H_ */
