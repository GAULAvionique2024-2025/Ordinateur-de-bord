/*
 * config.c
 *
 *  Created on: 19 mai 2026
 *      Author: gagno
 */


#include "App/config.h"
#include <string.h>
#include <stdio.h>

extern w25q_t w25q;
odb_config_t current_config;

// Default configuration
const odb_config_t default_config = {
    .magic_number = CONFIG_MAGIC_NUMBER,

    // Profile
    .odb_name = "ODB_1",

    // Stage
    .stage_role = 2,
    .debug_mode = 1,

    // Pyros
    .fire_attempt_delay_ms = 250,
    .pyros_arming_failsafe_ticks = 40000,
    .min_needed_pyro_nb = 2,
    .pyro_roles = {
        PYRO_ROLE_MAIN,          // Pyro 1
        PYRO_ROLE_DROGUE,        // Pyro 2
        PYRO_ROLE_MAIN_BACKUP,   // Pyro 3
        PYRO_ROLE_DROGUE_BACKUP  // Pyro 4
    },

    // Phase
    .acc_z_launch_threshold = 29.42f, 				// m/s2 (3.0G)
    .boost_phase_v_threshold = 100.0f,				// m/s
    .apogee_detect_v_threshold = -2.0f,				// m/s
    .landing_detect_v_threshold = 0.5f,				// m/s
    .landing_detect_threshold_ms = 10000,			// ms
    .apogee_failsafe_ticks = 60000,					// tick

    // Parachute
    .main_deploy_altitude_threshold_m = 450.0f,
    .drogue_fire_attempt_max_nb = 3,
    .main_fire_attempt_max_nb = 3,

    // Buzzer
    .enable_buzzer = 0,
    .buzzer_report_tone_hz = 500,

	// IdeFIX
	.idefix_frequency_hz = 433,
};

void Config_Init(void) {
    odb_config_t temp_config;
    if(W25Q_Read(&w25q, (uint8_t*)&temp_config, FLASH_CONFIG_START_ADDRESS, sizeof(odb_config_t)) == 0) {
    	// Check magic number
        if(temp_config.magic_number == CONFIG_MAGIC_NUMBER) {
            memcpy(&current_config, &temp_config, sizeof(odb_config_t));
            return; // success
        }
    }

    // No valid config found in flash, load defaults and save to flash
    Config_LoadDefaults();
    Config_SaveToFlash();
}

int8_t Config_SaveToFlash(void) {
	if(W25Q_EraseSector(&w25q, FLASH_CONFIG_START_ADDRESS) != 0) {
    	return -1; // failed
    }

    if(W25Q_WritePage(&w25q, (uint8_t*)&current_config, FLASH_CONFIG_START_ADDRESS, sizeof(odb_config_t)) != 0) {
        return -1; // failed
    }

    return 0; // success
}

void Config_LoadDefaults(void) {
    memcpy(&current_config, &default_config, sizeof(odb_config_t));
}

const odb_config_t* Config_Get(void) {
    return &current_config;
}
