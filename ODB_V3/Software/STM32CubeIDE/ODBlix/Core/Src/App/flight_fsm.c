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
#include "App/logger.h"

#include <stdint.h>


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
extern odb_stats_t flight_stats;
extern system_measurements_t system_measurements;
extern TIM_HandleTypeDef htim5;
extern pyro_t pyros[4];

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
            if(ODB_GetPyroStates(&flight_data) >= current_config.min_needed_pyro_nb && fabs(flight_data.kalman_v) < current_config.landing_detect_v_threshold) {
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
                        flight_data.event_states |= FLAG_MACH_LOCK_ENABLED;
                        current_substate = SUB_FAST;
                    }
                    break;

                case SUB_FAST:
                    // Wait for fast ascent detection
                	if(flight_data.kalman_v < current_config.boost_phase_v_threshold) {
						flight_data.event_states &= ~FLAG_MACH_LOCK_ENABLED;
						current_substate = SUB_COAST;
					}
                    break;

                case SUB_COAST:
                    // Wait for apogee detection
                	if(current_config.stage_role == 3 && flight_data.highg_acc_z > current_config.acc_z_launch_threshold) {
                		// cyclic inflight substate for sustainer
						current_substate = SUB_BOOST;
						break;
					}

                	/* WINDOWED FAILSAFE LOGIC
					 * 1. Nominal apogee detection : velocity below threshold after a reasonable flight duration (to avoid early detection during boost or fast phase)
					 * 2. Failsafe timeout : if apogee not detected after a maximum time
					 */
					bool mach_lock_enabled = ((flight_data.event_states & FLAG_MACH_LOCK_ENABLED) != 0U);
                	bool nominal_apogee = (flight_data.kalman_v < current_config.apogee_detect_v_threshold) && (flight_duration > current_config.pyros_arming_failsafe_ticks);
					bool failsafe_timeout = (flight_duration > current_config.apogee_failsafe_ticks);

					if(mach_lock_enabled) {
						// Turbulence window: keep estimation running but forbid deployment transition.
						break;
					}

					if(nominal_apogee || failsafe_timeout) {
						current_substate = SUB_DROGUE;
						fire_timer = 0;
						fire_attempt_count = 0;
						backup_active = false;

						// Arming Drogue
						pyro_t *drogue = Pyro_GetByRole(PYRO_ROLE_DROGUE);
						if(drogue) {
							Pyro_Arming(&system_measurements, true);
						}

						pyro_t *drogue_backup = Pyro_GetByRole(PYRO_ROLE_DROGUE_BACKUP);
						if(drogue_backup) {
							Pyro_Arming(&system_measurements, true);
						}
					}
					break;

				case SUB_DROGUE:
				{
					pyro_t *drogue = Pyro_GetByRole(PYRO_ROLE_DROGUE);
					pyro_t *drogue_backup = Pyro_GetByRole(PYRO_ROLE_DROGUE_BACKUP);
					// Wait for drogue deployment detection
					if(HAL_GetTick() - fire_timer >= current_config.fire_attempt_delay_ms) {
						if(!backup_active) {
							if(drogue != NULL && fire_attempt_count < current_config.drogue_fire_attempt_max_nb) {
								Pyro_Fire(drogue, &system_measurements);
								fire_attempt_count++;
								fire_timer = HAL_GetTick();
							} else {
								backup_active = (drogue_backup != NULL && drogue_backup->is_connected);
								fire_attempt_count = 0;
								fire_timer = HAL_GetTick();
							}
						} else {
							if(drogue_backup != NULL && fire_attempt_count < current_config.drogue_fire_attempt_max_nb) {
								Pyro_Fire(drogue_backup, &system_measurements);
								fire_attempt_count++;
								fire_timer = HAL_GetTick();
							}
						}
					}

					if(flight_data.kalman_z <= current_config.main_deploy_altitude_threshold_m) {
						current_substate = SUB_MAIN;
						fire_timer = 0;
						fire_attempt_count = 0;
						backup_active = false;

						// Arming Main
						pyro_t *main_pyro = Pyro_GetByRole(PYRO_ROLE_MAIN);
						if(main_pyro) {
							Pyro_Arming(&system_measurements, true);
						}

						pyro_t *main_backup = Pyro_GetByRole(PYRO_ROLE_MAIN_BACKUP);
						if(main_backup) {
							Pyro_Arming(&system_measurements, true);
						}
					}
					break;
				}

				case SUB_MAIN:
				{
					pyro_t *main_pyro = Pyro_GetByRole(PYRO_ROLE_MAIN);
					pyro_t *main_backup = Pyro_GetByRole(PYRO_ROLE_MAIN_BACKUP);

					// Wait for main deployment detection
					if(HAL_GetTick() - fire_timer >= current_config.fire_attempt_delay_ms) {
						if(!backup_active) {
							if(main_pyro != NULL && fire_attempt_count < current_config.main_fire_attempt_max_nb) {
								Pyro_Fire(main_pyro, &system_measurements);
								fire_attempt_count++;
								fire_timer = HAL_GetTick();
							} else {
								backup_active = (main_backup != NULL && main_backup->is_connected);
								fire_attempt_count = 0;
								fire_timer = HAL_GetTick();
							}
						} else {
							if(main_backup != NULL && fire_attempt_count < current_config.main_fire_attempt_max_nb) {
								Pyro_Fire(main_backup, &system_measurements);
								fire_attempt_count++;
								fire_timer = HAL_GetTick();
							}
						}
					}

					if(fabs(flight_data.kalman_v) < current_config.landing_detect_v_threshold) {
						if(landing_timer == 0) landing_timer = HAL_GetTick();
						if(HAL_GetTick() - landing_timer > current_config.landing_detect_threshold_ms) {
							current_substate = SUB_LANDED;
							current_global_state = STATE_POSTFLIGHT;

							Pyro_Arming(&system_measurements, false);
						}
					} else {
						landing_timer = 0;
					}
					break;
				}

				case SUB_LANDED:
					ODB_SetMissionState(&flight_data, STATE_POSTFLIGHT);
					current_global_state = STATE_POSTFLIGHT;
					break;
			}
			break;

		case STATE_POSTFLIGHT:
			// Handle landing timer conclusion, put in low power mode and handle IdeFIX communication
			ODB_SetMissionState(&flight_data, STATE_POSTFLIGHT);
			Logger_SaveStats(&flight_stats); // TODO: save only one time (not in loop)
			break;
	}
}

// Callback for failsafe timer (TIM5)
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if(htim->Instance == TIM5) {
		bool mach_lock_enabled = ((flight_data.event_states & FLAG_MACH_LOCK_ENABLED) != 0U);
		if(current_global_state == STATE_INFLIGHT && current_substate < SUB_DROGUE && !mach_lock_enabled) {
			current_substate = SUB_DROGUE;
			// Pyro_Fire(&pyro1, &system_measurements);
		}
	}
}
