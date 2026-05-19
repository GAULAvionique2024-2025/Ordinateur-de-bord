/*
 * config.h
 *
 *  Created on: 24 avr. 2026
 *      Author: gagno
 */

#ifndef INC_APP_CONFIG_H_
#define INC_APP_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Includes
// ============================================================================

#include <math.h>

#include "GAUL_Drivers/utils.h"
#include "GAUL_Drivers/l76lm33.h"
#include "GAUL_Drivers/rfd900x.h"

// ============================================================================
// Utility Macros
// ============================================================================

#define G_TO_MS2(g) \
    ((GRAVITY_MS2) * (g))

#define ALT_M_TO_HPA(h) \
	(1013.25f * powf((1.0f - ((h) / 44330.0f)), 5.255f))

// ============================================================================
// Rocket Role Selection
// ============================================================================

#define ROCKET_ROLE_BOOSTER        	0
#define ROCKET_ROLE_SUSTAINER      	1

// -----------------------------------------------------------------------------
// Active Configuration
// -----------------------------------------------------------------------------

#define ROCKET_ROLE                	ROCKET_ROLE_BOOSTER
#define DEBUG_MODE 					1

// ============================================================================
// Rocket Configuration
// ============================================================================
#if ROCKET_ROLE == ROCKET_ROLE_SUSTAINER

    #define CONFIG_GPS_PROFILE   	L76_FLIGHT_PROFILE_100K
    #define CONFIG_MODEM_ID      	ODB_MODEM_SUSTAINER

#else

    #define CONFIG_GPS_PROFILE   	L76_FLIGHT_PROFILE_30K
    #define CONFIG_MODEM_ID      	MAVLINK_MODEM_BOOSTER

#endif

typedef struct {
    l76_flight_profile_t gps_profile;
    mavlink_modem_id_t modem_id;
} rocket_config_t;
extern const rocket_config_t rocket_config;

// ============================================================================
// Flight Detection Thresholds
// ============================================================================

#define ACC_Z_LAUNCH_THRESHOLD				(G_TO_MS2(3.5f))
#define BOOST_PHASE_V_THRESHOLD             200.0f
#define APOGEE_DETECT_V_THRESHOLD           -2.0f

#define MAIN_DEPLOY_ALTITUDE_THRESHOLD_M	450.0f
#define MAIN_DEPLOY_ALTITUDE_THRESHOLD_HPA 	\
    (ALT_M_TO_HPA(450.0f))

#define LANDING_DETECT_V_THRESHOLD          0.5f
#define LANDING_DETECT_TIME_THRESHOLD_MS    10000

// ============================================================================
// Pyrotechnics Configuration
// ============================================================================

#define MIN_NEEDED_PYRO_NB                  2

#define DROGUE_FIRE_ATTEMPT_MAX_NB          3
#define MAIN_FIRE_ATTEMPT_MAX_NB            3

#define FIRE_ATTEMPT_DELAY_MS               250

#define PYROS_ARMING_FAILSAFE_TICKS         40000 // tick = ms
#define APOGEE_FAILSAFE_TICKS               60000 // tick = ms

// ============================================================================
// Audio / User Feedback
// ============================================================================

#define ENABLE_BUZZER                       0
#define BUZZER_REPORT_TONE_HZ               500

// ============================================================================

#ifdef __cplusplus
}
#endif

#endif /* INC_APP_CONFIG_H_ */
