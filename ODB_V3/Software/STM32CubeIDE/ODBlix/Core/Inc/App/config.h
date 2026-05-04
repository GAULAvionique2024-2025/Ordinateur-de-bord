/*
 * config.h
 *
 *  Created on: 24 avr. 2026
 *      Author: gagno
 */

#ifndef INC_APP_CONFIG_H_
#define INC_APP_CONFIG_H_

#include "GAUL_Drivers/utils.h"
#include <math.h>



#define G_TO_MS2(g) ((GRAVITY_MS2) * (g))
#define ALT_M_TO_HPA(h) (1013.25f * powf((1.0f - ((h) / 44330.0f)), 5.255f))

// TODO: add main deployement altitude, apogee delay, buzzer frequency report routine
// TODO: move main.c configuration object declarations here
#define BUZZER_REPORT_TONE_HZ               500                     // Frequency in Hz for buzzer report tone
#define ACC_Z_LAUNCH_THRESHOLD              (G_TO_MS2(3.5f))        // 3.5g in m/s^2
#define BOOST_PHASE_V_THRESHOLD             200.0f                  // Velocity threshold in m/s to consider end of boost phase and enable Mach lock
#define APOGEE_DETECT_V_THRESHOLD           0.0f                    // Apogee detection velocity threshold in m/s (450m in hPa)
#define MAIN_DEPLOY_ALTITUDE_THRESHOLD      (ALT_M_TO_HPA(450.0f))  // Altitude threshold in hpa to consider main deployment (450m in hPa)
#define LANDING_DETECT_V_THRESHOLD          0.5f                    // Velocity threshold in m/s to consider landing detection
#define LANDING_DETECT_TIME_THRESHOLD_MS    10000                   // Time threshold in ms to consider landing detection after velocity is below threshold

#endif /* INC_APP_CONFIG_H_ */
