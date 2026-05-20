/*
 * flight_fsm.c
 *
 *  Created on: 24 avr. 2026
 *      Author: gagno
 */

#include "App/flight_fsm.h"
#include "stm32f4xx_hal.h"
#include "GAUL_Drivers/system.h"
#include "GAUL_Drivers/utils.h"
#include "App/config.h"
#include "App/scheduler.h"

#include <stdint.h>


#define STABILITY_CHECK_THRESHOLD 0.5f // Vertical speed to consider the rocket stable on the ground during preflight checks (m/s)

typedef enum {
    STATE_PREFLIGHT,
    STATE_ARMED,
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

extern odb_data_t flight_data;
extern system_measurements_t system_measurements;
extern TIM_HandleTypeDef htim5;
extern pyro_t pyro1;
extern pyro_t pyro2;
extern pyro_t pyro3;
extern pyro_t pyro4;

global_state_t current_global_state = STATE_PREFLIGHT;
volatile inflight_substate_t current_substate = SUB_BOOST;

static uint32_t fire_timer = 0;
static uint32_t flight_duration = 0;
static uint32_t landing_timer = 0;
static uint8_t fire_attempt_count = 0;
static bool backup_active = false;

// TODO: check pyro number to adapt target attempt
void FSM_Update(void) {
    flight_duration = __HAL_TIM_GET_COUNTER(&htim5); // Failsafe apogee timeout
    switch(current_global_state) {
        case STATE_PREFLIGHT:
            // Security : Continuity pyros and stability check
            if(ODB_GetPyroStates(&flight_data) >= current_config.min_needed_pyro_nb && fabs(flight_data.kalman_v) < STABILITY_CHECK_THRESHOLD) {
                ODB_SetMissionState(&flight_data, STATE_ARMED);
                current_global_state = STATE_ARMED;
            }
            break;

        case STATE_ARMED:
            if(flight_data.highg_acc_z > current_config.acc_z_launch_threshold) {
                ODB_SetMissionState(&flight_data, STATE_INFLIGHT);
                Scheduler_RemoveTask("BTRx");
                Scheduler_RemoveTask("BTTx");
                //HM11_Sleep(&hm11);
                HAL_TIM_Base_Start_IT(&htim5);      // Start timer to measure time since launch
                __HAL_TIM_SET_COUNTER(&htim5, 0);   // Reset timer counter
                current_global_state = STATE_INFLIGHT;
                current_substate = SUB_BOOST;
            }
            break;

        case STATE_INFLIGHT:
            // Handle substate transitions based on events
            switch(current_substate) {
                case SUB_BOOST:
                    // Wait for boost phase detection
                    if(flight_data.kalman_v >= current_config.boost_phase_v_threshold) {
                        flight_data.system_states |= FLAG_MACH_LOCK_ENABLED;
                        current_substate = SUB_FAST;
                    }
                    break;

                case SUB_FAST:
                    // Wait for fast ascent detection
                    if(flight_data.kalman_v < current_config.boost_phase_v_threshold) {
                        flight_data.system_states &= ~FLAG_MACH_LOCK_ENABLED;
                        current_substate = SUB_COAST;
                    }
                    break;

                case SUB_COAST:
                    // Wait for apogee detection
                    /** 
                     * WINDOWED FAILSAFE LOGIC 
                     * 1. Nominal apogee detection : velocity below threshold after a reasonable flight duration (to avoid early detection during boost or fast phase)
                     * 2. Failsafe timeout : if apogee not detected after a maximum time
                     */
                    bool nominal_apogee = (flight_data.kalman_v < current_config.apogee_detect_v_threshold) && (flight_duration > current_config.pyros_arming_failsafe_ticks);
                    bool failsafe_timeout = (flight_duration > current_config.apogee_failsafe_ticks);

                    if(nominal_apogee || failsafe_timeout) {
                        current_substate = SUB_DROGUE;
                        fire_timer = 0; 
                        fire_attempt_count = 0;
                        backup_active = false;
                        
                        Pyro_Arming(&pyro1, &system_measurements, true);
                        Pyro_Arming(&pyro2, &system_measurements, true);
                    }
                    break;

                case SUB_DROGUE:
                    // Wait for drogue deployment detection
                    if(HAL_GetTick() - fire_timer >= current_config.fire_attempt_delay_ms) {
                        if(!backup_active) {
                            if(fire_attempt_count < current_config.drogue_fire_attempt_max_nb) {
                                Pyro_Fire(&pyro1, &system_measurements);
                                fire_attempt_count++;
                                fire_timer = HAL_GetTick();
                            } else {
                                backup_active = flight_data.system_states & FLAG_PYRO2_CONN;
                                fire_attempt_count = 0;
                            }
                        } else if(fire_attempt_count < current_config.drogue_fire_attempt_max_nb) {
                            Pyro_Fire(&pyro2, &system_measurements);
                            fire_attempt_count++;
                            fire_timer = HAL_GetTick();
                        }
                    }

                    if(flight_data.kalman_z <= current_config.main_deploy_altitude_threshold_m) {
                        current_substate = SUB_MAIN;
                        fire_timer = 0;
                        fire_attempt_count = 0;
                        backup_active = false;

                        Pyro_Arming(&pyro3, &system_measurements, true);
                        Pyro_Arming(&pyro4, &system_measurements, true);
                    }
                    break;

                case SUB_MAIN:
                    // Wait for main deployment detection
                    if(HAL_GetTick() - fire_timer >= current_config.fire_attempt_delay_ms) {
                        if(!backup_active) {
                            if(fire_attempt_count < current_config.main_fire_attempt_max_nb) {
                                Pyro_Fire(&pyro3, &system_measurements);
                                fire_attempt_count++;
                                fire_timer = HAL_GetTick();
                            } else {
                                backup_active = flight_data.system_states & FLAG_PYRO4_CONN;
                                fire_attempt_count = 0;
                            }
                        } else if(fire_attempt_count < current_config.main_fire_attempt_max_nb) {
                            Pyro_Fire(&pyro4, &system_measurements);
                            fire_attempt_count++;
                            fire_timer = HAL_GetTick();
                        }
                    }

                    if(fabs(flight_data.kalman_v) < current_config.landing_detect_v_threshold) {
                        if(landing_timer == 0) landing_timer = HAL_GetTick();
                        if(HAL_GetTick() - landing_timer > current_config.landing_detect_threshold_ms) {
                            current_substate = SUB_LANDED;
                            current_global_state = STATE_POSTFLIGHT;
                            Pyro_Arming(&pyro1, &system_measurements, false);
                            Pyro_Arming(&pyro2, &system_measurements, false);
                            Pyro_Arming(&pyro3, &system_measurements, false);
                            Pyro_Arming(&pyro4, &system_measurements, false);
                        }
                    } else {
                        landing_timer = 0;
                    }
                    break;

                case SUB_LANDED:
                    ODB_SetMissionState(&flight_data, STATE_POSTFLIGHT);
                    current_global_state = STATE_POSTFLIGHT;
                    break;
            }
            break;

        case STATE_POSTFLIGHT:
            // Handle landing timer conclusion, put in low power mode and handle IdeFIX communication
            ODB_SetMissionState(&flight_data, STATE_POSTFLIGHT);
            break;
    }
}

// Callback for failsafe timer (TIM5)
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if(htim->Instance == TIM5) {
        if(current_global_state == STATE_INFLIGHT && current_substate < SUB_DROGUE) {
            current_substate = SUB_DROGUE;
            // Pyro_Fire(&pyro1, &system_measurements);
        }
    }
}
