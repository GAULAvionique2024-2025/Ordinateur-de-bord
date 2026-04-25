/*
 * flight_fsm.c
 *
 *  Created on: 24 avr. 2026
 *      Author: gagno
 */


#include "App/flight_fsm.h"
#include "GAUL_Drivers/pyros.h"
#include "GAUL_Drivers/hm11.h"
#include "GAUL_Drivers/rfd900x.h"
#include <string.h>

static global_state_t g_state = STATE_INIT;
static inflight_sub_t f_substate = SUB_PAD;
static uint32_t landed_timer = 0;

// Flight FSM Parameters
#define APOGEE_DROP_THRESHOLD 3.0f   // Altitude drop in meters to confirm apogee
#define MAIN_HEIGHT_THRESHOLD 450.0f // Altitude in meters for main chute deployment
#define ACCEL_LAUNCH_G        3.5f   // Acceleration in G to detect launch
#define STABLE_ALT_THRESHOLD  0.5f   // Altitude change threshold to consider stable for landing

// TODO: state condition while be based on IMU/HIGHG -> GPS and Barometer are backup for pad altitude detection and landing confirmation
void FSM_Update(odb_data *data) {
    //float current_alt = data->baro_alt - pad_altitude;

    switch(g_state) {
        case STATE_INIT:
            // TODO: HandleBuzzerDiagnostics(data);
            g_state = STATE_PREFLIGHT;
            break;

        case STATE_PREFLIGHT:
            // TODO: RFD900x_SendMAVLink(MSG_PREFLIGHT, data);
            // TODO: Mean pad altitude
            // pad_altitude = data->baro_alt;
        	// TODO: Init kalman float mean_alt, float *samples with baro
        	// TODO: update kalman predict acc_world_z with imu

            if(data->highg_acc_z > ACCEL_LAUNCH_G) {
                g_state = STATE_INFLIGHT;
                f_substate = SUB_BOOST;
                // TODO: HM11_Sleep(&hm11);
            }
            break;

        case STATE_INFLIGHT:
            // TODO: RFD900x_SendMAVLink(MSG_INFLIGHT, data);
        	// TODO: update kalman predict acc_world_z with high_h
        	// TODO: add Mach Lock logic to ignore GPS/Baro (or > 4G)
        	// TODO: change kalman R_alt dependently of altitude : < 60000ft ok, < 90000 R_alt*10, else R_alt=1000

            // Update max altitude for apogee detection
        	/*
            if(current_alt > max_alt) {
                max_alt = current_alt;
            }
            */

            // Substate logic
            switch (f_substate) {
                case SUB_PAD: f_substate = SUB_BOOST; 
                    break;
                
                case SUB_BOOST:
                    if(data->vel > 200.0f) {
                        f_substate = SUB_FAST;
                    } else if(data->highg_acc_z < 0.5f) {
                        f_substate = SUB_COAST;
                    }
                    break;

                case SUB_FAST: // MACH LOCK ACTIVATED
                    if(data->vel < 200.0f) {
                        f_substate = SUB_COAST;
                    }
                    break;

                case SUB_COAST: // Apogee detection
                	// TODO: change kalman predict acc_world_z to imu
                	/*
                    if((max_alt - data->baro_alt) > APOGEE_DROP_THRESHOLD) {
                        // TODO: Pyro_Fire(&pyro_drogue);
                        f_substate = SUB_DROGUE;
                    }
                    */
                    break;

                case SUB_DROGUE:
                	/*
                    if(data->baro_alt < MAIN_HEIGHT_THRESHOLD) {
                        // TODO: Pyro_Fire(&pyro_main);
                        f_substate = SUB_MAIN;
                    }
                    */
                    break;

                case SUB_MAIN:
                    if(fabs(data->vel) < 0.5f) {
                        if(landed_timer == 0) {
                            landed_timer = HAL_GetTick();
                        }

                        if(HAL_GetTick() - landed_timer > 10000) {
                            f_substate = SUB_LANDED;
                        }
                    } else { landed_timer = 0; }
                    break;

                case SUB_LANDED:
                    g_state = STATE_POSTFLIGHT;
                    break;
            }
            break;

        case STATE_POSTFLIGHT:
            // Low power mode, send final telemetry, etc.
            // RFD900x_Stop();
            // MS5611_Sleep(); BNO055_Sleep();
            // IdeFIX_SendGPS(data->gps_lat, data->gps_lon);
            break;
    }
    data->mission_state = (uint8_t)f_substate;
}

global_state_t FSM_GetGlobalState(void) {
    return g_state;
}
