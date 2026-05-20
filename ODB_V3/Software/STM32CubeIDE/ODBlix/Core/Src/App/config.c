/*
 * config.c
 *
 *  Created on: 19 mai 2026
 *      Author: gagno
 */


#include "App/config.h"
#include "GAUL_Drivers/w25q512jv.h"
#include <string.h>
#include <stdio.h>

extern w25q_t w25q;
odb_config_t current_config;

// Default configuration
const odb_config_t default_config = {
    .magic_number = CONFIG_MAGIC_NUMBER,
    .odb_name = "ODB_1",
    .stage_role = 0,
    .debug_mode = 1,
    .enable_buzzer = 0,

    .acc_z_launch_threshold = G_TO_MS2(3.5f),
    .boost_phase_v_threshold = 200.0f,
    .apogee_detect_v_threshold = -2.0f,
    .main_deploy_altitude_threshold_m = 450.0f,
    .landing_detect_v_threshold = 0.5f,

    .landing_detect_threshold_ms = 10000,
    .fire_attempt_delay_ms = 250,
    .pyros_arming_failsafe_ticks = 40000,
    .apogee_failsafe_ticks = 60000,

    .min_needed_pyro_nb = 2,
    .drogue_fire_attempt_max_nb = 3,
    .main_fire_attempt_max_nb = 3,
    .buzzer_report_tone_hz = 500
};

void Config_LoadDefaults(void) {
    memcpy(&current_config, &default_config, sizeof(odb_config_t));
}

void Config_Init(void) {
    odb_config_t temp_config;
    if(W25Q_Read(&w25q, (uint8_t*)&temp_config, FLASH_CONFIG_START_ADDRESS, sizeof(odb_config_t)) == 0) {
    	// Check magic number
        if(temp_config.magic_number == CONFIG_MAGIC_NUMBER) {
            memcpy(&current_config, &temp_config, sizeof(odb_config_t));
            printf("Config: Loaded from Flash successfully.\r\n");
            return;
        }
    }
    printf("Config: Flash corrupted or empty. Loading default values.\r\n");
    Config_LoadDefaults();
    Config_SaveToFlash();
}

void Config_SaveToFlash(void) {
    W25Q_EraseSector(&w25q, FLASH_CONFIG_START_ADDRESS);

    if(W25Q_WritePage(&w25q, (uint8_t*)&current_config, FLASH_CONFIG_START_ADDRESS, sizeof(odb_config_t)) == 0) {
        printf("Config: Save to Flash successful.\r\n");
    } else {
        printf("Config: ERROR saving to Flash.\r\n");
    }
}

const odb_config_t* Config_Get(void) {
    return &current_config;
}
