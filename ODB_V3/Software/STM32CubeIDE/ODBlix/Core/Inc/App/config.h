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

/* === Pyro Role === */
typedef enum {
    PYRO_ROLE_NONE          = 0,
    PYRO_ROLE_MAIN          = 1,
    PYRO_ROLE_DROGUE        = 2,
    PYRO_ROLE_MAIN_BACKUP   = 3,
    PYRO_ROLE_DROGUE_BACKUP = 4
} pyro_role_t;

/* === Configuration Structure === */
typedef struct {
    uint32_t magic_number;

    // Profile
    char odb_name[32];			// max 12 char
    // Stage
    uint8_t stage_role;         // 2 = BOOSTER, 3 = SUSTAINER
    uint8_t debug_mode;

    // Pyros
    uint32_t fire_attempt_delay_ms;
    uint32_t pyros_arming_failsafe_ticks;
    uint8_t min_needed_pyro_nb;
    uint8_t pyro_roles[4];

    // Phase
    float acc_z_launch_threshold;
    float boost_phase_v_threshold;
    float apogee_detect_v_threshold;
    float landing_detect_v_threshold;
    uint32_t landing_detect_threshold_ms;
    uint32_t apogee_failsafe_ticks;

    // Parachute
    float main_deploy_altitude_threshold_m;
    uint8_t drogue_fire_attempt_max_nb;
    uint8_t main_fire_attempt_max_nb;

    // Buzzer
    uint8_t enable_buzzer;
    uint16_t buzzer_report_tone_hz;

    // IdeFIX
    uint32_t idefix_frequency_hz;
} odb_config_t;
extern odb_config_t current_config;


void Config_Init(void);
int8_t Config_SaveToFlash(void);
void Config_LoadDefaults(void);
const odb_config_t* Config_Get(void);

int Config_Erase(void);

#endif /* INC_APP_CONFIG_H_ */
