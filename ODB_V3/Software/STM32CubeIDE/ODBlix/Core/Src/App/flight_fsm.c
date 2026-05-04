/*
 * flight_fsm.c
 *
 *  Created on: 24 avr. 2026
 *      Author: gagno
 */

#include "App/flight_fsm.h"
#include "GAUL_Drivers/system.h"
#include "GAUL_Drivers/utils.h"
#include "App/config.h"

#include <stdint.h>


typedef enum {
    STATE_INIT,
    STATE_PREFLIGHT,
    STATE_INFLIGHT,
    STATE_POSTFLIGHT
} global_state_t;

typedef enum {
    SUB_BOOST,
    SUB_FAST,
    SUB_COAST,
    SUB_DROGUE,
    SUB_MAIN,
    SUB_LANDED
} inflight_substate_t;

extern odb_data flight_data;

volatile global_state_t current_global_state = STATE_INIT;
volatile inflight_substate_t current_substate = SUB_BOOST;
volatile uint32_t landing_timer = 0;

void FSM_Update(void) {
    switch(current_global_state) {
        case STATE_INIT:
            // Wait for ODB_Init to complete and set mission state to PREFLIGHT
            odb_state_t odb_state = ODB_Init(&flight_data);
            if(odb_state == ODB_OK || odb_state == ODB_WARNING) {
                ODB_SetMissionState(&flight_data, STATE_PREFLIGHT);
                current_global_state = STATE_PREFLIGHT;
            } else {
                // Handle initialization error
            }
            break;
        case STATE_PREFLIGHT:
            // Wait for launch detection
            if(flight_data.highg_acc_z > ACC_Z_LAUNCH_THRESHOLD) {
                //HM11_Sleep(&hm11);
                current_global_state = STATE_INFLIGHT;
                current_substate = SUB_BOOST;
            }
            // Handle launch abort conditions
            if(flight_data.kalman_v < APOGEE_DETECT_V_THRESHOLD) {
                current_global_state = STATE_INFLIGHT;
                current_substate = SUB_COAST;
            }
            break;
        case STATE_INFLIGHT:
            // Handle substate transitions based on events
            switch(current_substate) {
                case SUB_BOOST:
                    // Wait for boost phase detection
                    if(flight_data.kalman_v >= BOOST_PHASE_V_THRESHOLD) {
                        flight_data.system_states |= FLAG_MACH_LOCK_ENABLED;
                        current_substate = SUB_FAST;
                    }
                    break;
                case SUB_FAST:
                    // Wait for fast ascent detection
                    if(flight_data.kalman_v < BOOST_PHASE_V_THRESHOLD) {
                        flight_data.system_states &= ~FLAG_MACH_LOCK_ENABLED;
                        current_substate = SUB_COAST;
                    }
                    break;
                case SUB_COAST:
                    // Wait for apogee detection
                    if(flight_data.pressure_hpa < APOGEE_DETECT_V_THRESHOLD) {
                        //Pyro_Fire(PYRO_DROGUE);
                        current_substate = SUB_DROGUE;
                    }
                    break;
                case SUB_DROGUE:
                    // Wait for drogue deployment detection
                    if(flight_data.kalman_v <= MAIN_DEPLOY_ALTITUDE_THRESHOLD) {
                        //Pyro_Fire(PYRO_MAIN);
                        current_substate = SUB_MAIN;
                    }
                    break;
                case SUB_MAIN:
                    // Wait for main deployment detection
                    if(fabs(flight_data.kalman_v) < LANDING_DETECT_V_THRESHOLD) {
                        if(landing_timer == 0) landing_timer = HAL_GetTick();
                        if(HAL_GetTick() - landing_timer > LANDING_DETECT_TIME_THRESHOLD_MS) {
                            current_substate = SUB_LANDED;
                            current_global_state = STATE_POSTFLIGHT;
                        }
                    } else {
                        landing_timer = 0; // Reset
                    }
                    break;
                case SUB_LANDED:
                    // Wait for landing detection
                    current_global_state = STATE_POSTFLIGHT;
                    break;
            }
            break;
        case STATE_POSTFLIGHT:
            // Handle landing timer, put in low power mode and handle IdeFIX communication
            break;
    }
}
