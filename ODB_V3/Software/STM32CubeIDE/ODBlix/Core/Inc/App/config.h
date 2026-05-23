/*
 * config.h
 *
 *  Created on: 24 avr. 2026
 *      Author: gagno
 */

#ifndef INC_APP_CONFIG_H_
#define INC_APP_CONFIG_H_

#include "GAUL_Drivers/w25q512jv.h"
#include <stdint.h>

/* === Constants === */
#define GRAVITY_MS2 9.80665f
#define G_TO_MS2(g) ((GRAVITY_MS2) * (g))

/* === Flash Mapping Configuration === */
#define FLASH_CONFIG_START_ADDRESS      (W25Q512_FLASH_SIZE_BYTE - FLASH_SECTOR_SIZE_BYTE) // 0x03FFF000
#define LOGGER_MAX_ALLOWED_ADDRESS      FLASH_CONFIG_START_ADDRESS
#define LOGGER_MAGIC_HEADER             0x48454144 // HEAD
#define LOGGER_DATA_MAGIC_NUMBER	    0x44415441 // DATA
#define LOGGER_STATS_MAGIC_NUMBER	    0x53544154 // STAT
#define CONFIG_MAGIC_NUMBER  			0x434F4E46 // CONF
/* =========== */

/* === Configuration Structure === */
typedef struct {
    uint32_t magic_number;

    char odb_name[32];			// max 12 char
    uint8_t stage_role;         // 0 = BOOSTER, 1 = SUSTAINER
    uint8_t debug_mode;
    uint8_t enable_buzzer;

    float acc_z_launch_threshold;
    float boost_phase_v_threshold;
    float apogee_detect_v_threshold;
    float main_deploy_altitude_threshold_m;
    float landing_detect_v_threshold;

    uint32_t landing_detect_threshold_ms;
    uint32_t fire_attempt_delay_ms;
    uint32_t pyros_arming_failsafe_ticks;
    uint32_t apogee_failsafe_ticks;

    uint8_t min_needed_pyro_nb;
    uint8_t drogue_fire_attempt_max_nb;
    uint8_t main_fire_attempt_max_nb;
    uint16_t buzzer_report_tone_hz;
} odb_config_t;
extern odb_config_t current_config;


void Config_Init(void);
int8_t Config_SaveToFlash(void);
void Config_LoadDefaults(void);
const odb_config_t* Config_Get(void);

#endif /* INC_APP_CONFIG_H_ */
