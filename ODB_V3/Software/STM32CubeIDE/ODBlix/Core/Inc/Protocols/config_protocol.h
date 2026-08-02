/*
 * app_protocol.h
 *
 *  Created on: 6 juin 2026
 *      Author: gagno
 */

#ifndef INC_PROTOCOLS_CONFIG_PROTOCOL_H_
#define INC_PROTOCOLS_CONFIG_PROTOCOL_H_

#include <stdint.h>

/* === Protocol Versioning === */
#define CONFIG_PROTOCOL_VERSION_MAJOR 1
#define CONFIG_PROTOCOL_VERSION_MINOR 0

/* === Pyro Role === */
#define PYRO_ROLES(X)			\
    X(PYRO_ROLE_NONE)    		\
    X(PYRO_ROLE_MAIN)  			\
    X(PYRO_ROLE_DROGUE)    		\
    X(PYRO_ROLE_MAIN_BACKUP)	\
	X(PYRO_ROLE_DROGUE_BACKUP)

#define AS_ENUM(NAME) NAME,
#define AS_STRING(NAME) #NAME,

typedef enum {
	PYRO_ROLES(AS_ENUM)
	PYROS_ROLE_MAX
} pyro_role_t;

static const char* const PYRO_ROLES_LOOKUP[] = {
	PYRO_ROLES(AS_STRING)
};

/* === Configuration Structure === */
typedef struct __attribute__((packed)) {
	uint32_t 	magic_number;
	uint32_t 	crc32;

	uint8_t  	version_major;
	uint8_t  	version_minor;
	uint16_t 	payload_size;

    // Profile
    char 		odb_name[32];				// max 12 char
    // Stage
    uint8_t 	stage_role;         		// 2 = BOOSTER, 3 = SUSTAINER
    uint8_t 	debug_mode;

    // Pyros
    uint32_t 	fire_attempt_delay_ms;
    uint32_t 	pyros_arming_failsafe_ms;
    uint8_t 	min_needed_pyro_nb;
    uint8_t 	pyro_roles[4];

    // Phase
    float 		acc_z_launch_threshold;
    float 		boost_phase_v_threshold;
    float 		apogee_detect_v_threshold;
    float 		landing_detect_v_threshold;
    uint32_t 	landing_detect_threshold_ms;
    uint32_t 	apogee_failsafe_ms;

    // Parachute
    float 		main_deploy_altitude_threshold_m;
    uint8_t 	drogue_fire_attempt_max_nb;
    uint8_t 	main_fire_attempt_max_nb;

    // Buzzer
    uint8_t 	enable_buzzer;
    uint16_t 	buzzer_report_tone_hz;

    // IdeFIX
    uint32_t 	idefix_frequency_hz;

    // TOTAL 92
} odb_config_t;
#define CONFIG_DATA_SIZE sizeof(odb_config_t)

#endif /* INC_PROTOCOLS_CONFIG_PROTOCOL_H_ */
