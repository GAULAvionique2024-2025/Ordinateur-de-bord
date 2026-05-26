/*
 * system.c
 *
 *  Created on: 3 mai 2026
 *      Author: gagno
 */


// Apogee Detection & Pyros Firing logic: https://www.rocketryforum.com/threads/most-accurate-way-to-measure-velocity-accelerometer-vs-barometer-vs.157866/page-2

#include "GAUL_Drivers/system.h"
#include "App/config.h"
#include "App/profiler.h"

#include <ctype.h>


#define KALMAN_NAV_SAMPLE_NB 50

extern adxl382_t adxl382;
extern bno055_t bno055;
extern hm11_t hm11;
extern l76lm33_t l76lm33;
extern pyro_t pyros[4];
extern critical_led_t critical_led;
extern ms5611_t ms5611;
extern rfd900x_t rfd900x;
extern buzzer_t buzzer;
extern system_measurements_t system_measurements;
extern w25q_t w25q;

static kalman_nav_t kalman_filter;


/* === ODB === */
void ODB_Reset(odb_data_t *data, odb_stats_t *stats) {
    if(!data) {
        return;
    }

    data->time_boot_ms = 0;
    data->system_states = 0x0000; // All flags cleared (components not OK)
    data->event_states = 0x00;    // All flags cleared (events not occurred)
    data->battery_mv = 0;
    data->roll = 0.0f;
    data->pitch = 0.0f;
    data->yaw = 0.0f;
    data->imu_acc_x = 0.0f;
    data->imu_acc_y = 0.0f;
    data->imu_acc_z = 0.0f;
    data->imu_gyro_x = 0.0f;
    data->imu_gyro_y = 0.0f;
    data->imu_gyro_z = 0.0f;
    data->imu_mag_x = 0.0f;
    data->imu_mag_y = 0.0f;
    data->imu_mag_z = 0.0f;
    data->altitude_msl_m = 0.0f;
    data->pressure_hpa = 0.0f;
    data->temp_celsius = 0.0f;
    data->highg_acc_x = 0.0f;
    data->highg_acc_y = 0.0f;
    data->highg_acc_z = 0.0f;
    data->gps_fix = 0;
    data->lat = 0;
    data->lon = 0;
    data->gps_alt = 0;
    data->vel = 0;
    data->cog = 0;
    data->satellites_nb = 0;
    data->imu_acc_vertical = 0.0f;
    data->highg_acc_vertical = 0.0f;
    data->kalman_z = 0.0f;
    data->kalman_v = 0.0f;

    stats->pyro1.fired = false;
    stats->pyro1.time_ms = 0;
    stats->pyro2.fired = false;
    stats->pyro2.time_ms = 0;
    stats->pyro3.fired = false;
    stats->pyro3.time_ms = 0;
    stats->pyro4.fired = false;
    stats->pyro4.time_ms = 0;
    stats->mach_lock.fired = false;
    stats->mach_lock.time_ms = 0;
    stats->max_altitude_gps.valid = false;
    stats->max_altitude_gps.value = 0.0f;
    stats->max_altitude_gps.time_ms = 0;
    stats->max_altitude_baro.valid = false;
    stats->max_altitude_baro.value = 0.0f;
    stats->max_altitude_baro.time_ms = 0;
    stats->max_altitude_kalman.valid = false;
    stats->max_altitude_kalman.value = 0.0f;
    stats->max_altitude_kalman.time_ms = 0;
    stats->apogee.valid = false;
    stats->apogee.value = 0.0f;
    stats->apogee.time_ms = 0;
    stats->main_deploy.valid = false;
    stats->main_deploy.value = 0.0f;
    stats->main_deploy.time_ms = 0;
    stats->drogue_deploy.valid = false;
    stats->drogue_deploy.value = 0.0f;
    stats->drogue_deploy.time_ms = 0;
    stats->max_ascend_speed.valid = false;
    stats->max_ascend_speed.value = 0.0f;
    stats->max_ascend_speed.time_ms = 0;
    stats->max_ascend_accel.valid = false;
    stats->max_ascend_accel.value = 0.0f;
    stats->max_ascend_accel.time_ms = 0;
    stats->max_descend_speed.valid = false;
    stats->max_descend_speed.value = 0.0f;
    stats->max_descend_speed.time_ms = 0;
    stats->max_descend_accel.valid = false;
    stats->max_descend_accel.value = 0.0f;
    stats->max_descend_accel.time_ms = 0;
    stats->flight_time_ms = 0;
}

uint8_t ODB_SetEventStates(const odb_stats_t *stats) {
    if(!stats) {
        return 0x00;
    }

    uint8_t packed = 0x00;
    if(stats->pyro1.fired) {
        packed |= FLAG_PYRO1_FIRED;
    }
    if(stats->pyro2.fired) {
        packed |= FLAG_PYRO2_FIRED;
    }
    if(stats->pyro3.fired) {
        packed |= FLAG_PYRO3_FIRED;
    }
    if(stats->pyro4.fired) {
        packed |= FLAG_PYRO4_FIRED;
    }
    if(stats->mach_lock.fired) {
        packed |= FLAG_MACH_LOCK_ENABLED;
    } else {
        packed &= ~FLAG_MACH_LOCK_ENABLED;
    }
    if(stats->apogee.valid) {
        packed |= FLAG_APOGEE_DETECTED;
    }
    if(stats->main_deploy.valid) {
        packed |= FLAG_MAIN_DEPLOYED;
    }
    if(stats->drogue_deploy.valid) {
        packed |= FLAG_DROGUE_DEPLOYED;
    }

    return packed;
}

// TODO: add behavior with debug_mode (ifdef printf, ect...)
odb_state_t ODB_Init(odb_data_t *data, odb_stats_t *stats) {
    if(!data) {
        return ODB_ERROR;
    }

    bool alimentation_fault = false;
    uint8_t error = 0;
    uint8_t warning = 0;

    ODB_Reset(data, stats);

    // Load configuration from flash
    Config_Init();
    Config_Get();

    uint16_t system_states = 0x0000;
    if(SystemMeasurements_Init(&system_measurements) == 0) {
        SystemMeasurements_ComputePower(&system_measurements);
        if(system_measurements.vin_batt <= VIN_BATT_MIN_MV || system_measurements.vin_batt >= VIN_BATT_MAX_MV || system_measurements.v5_buck <= V5_MIN_MV || system_measurements.v5_buck >= V5_MAX_MV || system_measurements.v3_buck <= V3_MIN_MV || system_measurements.v3_buck >= V3_MAX_MV || system_measurements.pg_v5 == false) {
            alimentation_fault = true;
            printf("Erreur : Batterie trop faible !\n");
        }

        uint8_t pyros_connected = 0;
		const uint32_t FLAG_PYRO_CONN[4] = {FLAG_PYRO1_CONN, FLAG_PYRO2_CONN, FLAG_PYRO3_CONN, FLAG_PYRO4_CONN};
		const char* role_names[] = {"NONE", "MAIN", "DROGUE", "MAIN_BACKUP", "DROGUE_BACKUP"};
		bool is_armed = Pyro_Arming(&pyros[0], &system_measurements, true);
		if(is_armed) {
			system_states |= FLAG_PYROS_ARMED_OK;
		} else {
			error += 1;
			printf("Erreur : Armement des Pyros bloque\n");
		}

		for(int i = 0; i < 4; i++) {
			pyro_role_t role = (pyro_role_t)current_config.pyro_roles[i];
			int8_t init_res = Pyro_Init(&pyros[i], &system_measurements);
			if(role != PYRO_ROLE_NONE) {
				if(init_res == 0) {
					system_states |= FLAG_PYRO_CONN[i];
					pyros_connected++;
				} else {
					warning += 1;
					printf("Erreur : Pyro %d (%s) deconnecte\n", i + 1, role_names[role]);
				}
			}
		}

		is_armed = Pyro_Arming(&pyros[0], &system_measurements, false);
		if(!is_armed) {
			system_states |= FLAG_PYROS_ARMED_OK;
		} else {
			system_states &= ~FLAG_PYROS_ARMED_OK;
			error += 1;
			printf("Erreur : Desarmement des Pyros bloque\n");
		}

		// Protection
		if(pyros_connected < current_config.min_needed_pyro_nb) {
			error += 1;
			printf("Erreur : Pas assez de pyros connectes ! (%d/%d)\n", pyros_connected, current_config.min_needed_pyro_nb);
		}

        SystemMeasurements_ComputeTemperature(&system_measurements);
        if(system_measurements.temperature < MAX6612MXK_MIN_TEMP_C || system_measurements.temperature > MAX6612MXK_MAX_TEMP_C) {
            error += 1;
            printf("Erreur : Temperature hors limites !\n");
        }
    } else {
        error += 1;
        printf("Erreur : Init SystemMeasurements\n");
    }

    if(BNO055_Init(&bno055) == BNO055_OK) {
        system_states |= FLAG_IMU_OK;
    } else {
        error += 1;
        printf("Erreur : Init BNO055\n");
    }

    if(MS5611_Init(&ms5611, OSR1024, OSR1024) == MS5611_OK) {
        system_states |= FLAG_BARO_OK;
    } else {
        error += 1;
        printf("Erreur : Init MS5611\n");
    }

    if(ADXL382_Init(&adxl382) == ADXL382_OK) {
        system_states |= FLAG_HIGHG_OK;
    } else {
        error += 1;
        printf("Erreur : Init ADXL382\n");
    }

    if(L76LM33_Init(&l76lm33) == L76LM33_OK) {
        system_states |= FLAG_GPS_OK;
    } else {
        error += 1;
        printf("Erreur : Init L76LM33\n");
    }

    if(RFD900x_Init(&rfd900x) == RFD_OK) {
        system_states |= FLAG_RADIO_OK;
    } else {
        error += 1;
        printf("Erreur : Init RFD900x\n");
    }

    if(W25Q_Init(&w25q) == 0) {
        system_states |= FLAG_FLASH_OK;
    } else {
        warning += 1;
        printf("Erreur : Init W25Q\n");
    }
    
    if(HM11_Init(&hm11) == HM11_OK) {
    	system_states |= FLAG_BT_OK;
    } else {
    	warning += 1;
    	printf("Erreur : HM-11 ne repond pas.\n");
    }

    // Kalman filter initialization -> calculate R_static
    if((system_states & FLAG_BARO_OK) && (system_states & FLAG_HIGHG_OK)) {
        float samples[KALMAN_NAV_SAMPLE_NB];
        float sum = 0;
        float temperature, pressure, alt;
        for(int i = 0; i < KALMAN_NAV_SAMPLE_NB; i++) { 
            MS5611_Update(&ms5611); 
            HAL_Delay(10);
            MS5611_Compute(&ms5611, &temperature, &pressure);
            alt = Math_ComputeAltitudeMSL(pressure);
			samples[i] = alt;
			sum += samples[i];
        }

        KalmanNav_Init(&kalman_filter, sum/KALMAN_NAV_SAMPLE_NB, samples, KALMAN_NAV_SAMPLE_NB);
    }

    if(CriticalLed_Init(&critical_led) != 0) {
		warning += 1;
		printf("Erreur : Init Critical LED\n");
	}
    // Sensors Init End

    odb_state_t odb_state = ODB_ERROR;
    if(alimentation_fault) {
        odb_state = ODB_ALIMENTATION_ERROR;
        printf("Erreur : Alimentation non conforme !\n");
    } else if(error > 0) {
        odb_state = ODB_ERROR;
        printf("Erreur : %d erreur(s) detectee(s) lors de l'initialisation du systeme.\n", error);
    } else {
        odb_state = ODB_OK;
    }

    // Update system states
    data->system_states = system_states;

    // Buzzer report
    if(current_config.enable_buzzer) {
        Buzzer_ReportStatus(&buzzer, current_config.buzzer_report_tone_hz, system_measurements.vin_batt, (bool[]){(system_states & FLAG_PYRO1_CONN) != 0U, (system_states & FLAG_PYRO2_CONN) != 0U, (system_states & FLAG_PYRO3_CONN) != 0U, (system_states & FLAG_PYRO4_CONN) != 0U}, odb_state);
    }

    //Scheduler_AddTask(ODB_Update, 100);
    //...

    return odb_state;
}

// TODO: add timestamp with RTC to all odb_stats_t data
void ODB_Update(odb_data_t *data, odb_stats_t *stats) {
    if(!data) {
        return;
    }

    SystemMeasurements_ComputeTemperature(&system_measurements);
    SystemMeasurements_ComputePower(&system_measurements);
    SystemMeasurements_ComputePyros(&system_measurements);

    float temperature, pressure;
    Profiler_StartTask(PROFILE_TASK_BARO);
    MS5611_Update(&ms5611);
    if(MS5611_Compute(&ms5611, &temperature, &pressure) == MS5611_OK) {
    	data->pressure_hpa = pressure;
    	data->temp_celsius = temperature;
        data->altitude_msl_m = Math_ComputeAltitudeMSL(pressure);
    }
    Profiler_StopTask(PROFILE_TASK_BARO);

    Profiler_StartTask(PROFILE_TASK_IMU);
    /*
    if(BNO055_IsDataReady(&bno055)) {

    }
    */
    if(BNO055_ReadAllData(&bno055) == BNO055_OK) {
		data->imu_acc_x = bno055.acc_x;
		data->imu_acc_y = bno055.acc_y;
		data->imu_acc_z = bno055.acc_z;
		data->imu_gyro_x = bno055.gyro_x;
		data->imu_gyro_y = bno055.gyro_y;
		data->imu_gyro_z = bno055.gyro_z;
		data->imu_mag_x = bno055.mag_x;
		data->imu_mag_y = bno055.mag_y;
		data->imu_mag_z = bno055.mag_z;

		BNO055_ComputeEulerAngles(&bno055);
		data->roll = bno055.euler_angles.roll;
		data->pitch = bno055.euler_angles.pitch;
		data->yaw = bno055.euler_angles.yaw;

		BNO055_ComputeVerticalAcc(&bno055);
		data->imu_acc_vertical = bno055.acc_vertical;
	}
    BNO055_ReadTemperature(&bno055);
    Profiler_StopTask(PROFILE_TASK_IMU);

    const float current_quat[4] = {bno055.quat.w, bno055.quat.x, bno055.quat.y, bno055.quat.z};
    Profiler_StartTask(PROFILE_TASK_HIGHG);
    if(ADXL382_ReadData(&adxl382, current_quat) == ADXL382_OK) {
      data->highg_acc_x = adxl382.acc_x;
      data->highg_acc_y = adxl382.acc_y;
      data->highg_acc_z = adxl382.acc_z;
      data->highg_acc_vertical = adxl382.acc_vertical;
    }
    Profiler_StopTask(PROFILE_TASK_HIGHG);

    // Kalman filter update with dynamic R_alt
    float raw_accel_z = data->highg_acc_vertical;
    Profiler_StartTask(PROFILE_TASK_KALMAN);
    if(fabs(raw_accel_z) < 3.5f) {
        raw_accel_z = data->imu_acc_vertical;
    }
    KalmanNav_Predict(&kalman_filter, (float)raw_accel_z);
    KalmanNav_Update(&kalman_filter, (float)data->altitude_msl_m);
    data->kalman_z = (float)kalman_filter.z;
    data->kalman_v = (float)kalman_filter.v;
    Profiler_StopTask(PROFILE_TASK_KALMAN);

    Profiler_StartTask(PROFILE_TASK_GPS);
    if(L76LM33_Compute(&l76lm33) == L76LM33_OK) {
      data->gps_fix         = l76lm33.gps_data.gps_fix;
      data->lat             = l76lm33.gps_data.lat;
      data->lon             = l76lm33.gps_data.lon;
      data->gps_alt         = l76lm33.gps_data.gps_alt;
      data->vel             = l76lm33.gps_data.vel;
      data->cog             = l76lm33.gps_data.cog;
      data->satellites_nb   = l76lm33.gps_data.satellites_nb;
    }
    Profiler_StopTask(PROFILE_TASK_GPS);

    data->time_boot_ms = HAL_GetTick();
    data->battery_mv = (uint16_t)(system_measurements.vin_batt);


    if(system_measurements.pyros_arming) {
		stats->pyro1.fired = pyros[0].is_fire;
		stats->pyro2.fired = pyros[1].is_fire;
		stats->pyro3.fired = pyros[2].is_fire;
		stats->pyro4.fired = pyros[3].is_fire;
	}

    if(data->gps_fix > 1) {
      data->system_states |= FLAG_GPS_OK;
    } else {
      data->system_states &= ~FLAG_GPS_OK;
    }

    data->event_states = ODB_SetEventStates(stats);
}

int8_t ODB_SetMissionState(odb_data_t *data, uint8_t mission_state) {
    if(!data) {
        return ODB_ERROR;
    }

    data->mission_state = mission_state;
    return ODB_OK;
}

uint8_t ODB_GetPyroStates(const odb_data_t *data) {
    if(!data) {
        return 0x00;
    }

    uint8_t pyro_continuity = 0;
    if(data->system_states & FLAG_PYRO1_CONN) {
        pyro_continuity++;
    }
    if(data->system_states & FLAG_PYRO2_CONN) {
        pyro_continuity++;
    }
    if(data->system_states & FLAG_PYRO3_CONN) {
        pyro_continuity++;
    }
    if(data->system_states & FLAG_PYRO4_CONN) {
        pyro_continuity++;
    }

    return pyro_continuity;
}
/* =========== */

/* === TELEMETRY === */
static uint8_t mavlink_tx_buffer[MAVLINK_MAX_PACKET_LEN]; // DMA TX buffer

static void Telemetry_TransmitMessage(rfd900x_t *rfd_dev, const mavlink_message_t *msg) {
	if(!rfd_dev) return;

	uint16_t len = mavlink_msg_to_send_buffer(mavlink_tx_buffer, msg);
	rfd900x_state_t err = RFD900x_Transmit(rfd_dev, mavlink_tx_buffer, len);
	if(err == RFD_BUSY) {
		// TODO: add missing transmit counter
	}
}

void Telemetry_SendRocketData(rfd900x_t *rfd_dev, const mavlink_modem_id_t modem_id, odb_data_t *data, const uint32_t current_time_ms) {
    if(!rfd_dev || !data) return;

    static uint32_t last_send_time = 0;
    uint32_t current_time = HAL_GetTick();
    if((current_time - last_send_time) >= TELEMETRY_DELAY_REFRESH_MS) {
		odb_data_t *data_temp = data;
		mavlink_message_t msg;
		mavlink_msg_rocket_telemetry_pack(
				modem_id,
				MAVLINK_COMPONENT_ID,
				&msg,
				current_time_ms,
				data_temp->system_states,
				data_temp->event_states,
				data_temp->mission_state,
				data_temp->battery_mv,
				data_temp->roll,
				data_temp->pitch,
				data_temp->yaw,
				data_temp->imu_acc_x,
				data_temp->imu_acc_y,
				data_temp->imu_acc_z,
				data_temp->imu_gyro_x,
				data_temp->imu_gyro_y,
				data_temp->imu_gyro_z,
				data_temp->imu_mag_x,
				data_temp->imu_mag_y,
				data_temp->imu_mag_z,
				data_temp->altitude_msl_m,
				data_temp->pressure_hpa,
				data_temp->temp_celsius,
				data_temp->highg_acc_x,
				data_temp->highg_acc_y,
				data_temp->highg_acc_z,
				data_temp->gps_fix,
				data_temp->lat,
				data_temp->lon,
				data_temp->gps_alt,
				data_temp->vel,
				data_temp->cog,
				data_temp->satellites_nb,
				data_temp->imu_acc_vertical,
				data_temp->highg_acc_vertical,
				data_temp->kalman_z,
				data_temp->kalman_v
			);

		Telemetry_TransmitMessage(rfd_dev, &msg);

		last_send_time = current_time;
    }
}

/*
void Telemetry_SendEventLog(rfd900x_t *rfd_dev, const mavlink_modem_id_t modem_id, const mavlink_event_severity_t severity, const char *text) {
    if (!rfd_dev || !text || text[0] == '\0' || strlen(text) > 50) return;

    mavlink_message_t msg;
    mavlink_msg_statustext_pack(
        modem_id,
        MAVLINK_COMPONENT_ID,
        &msg,
        severity,
        text,
        0,
        0
    );

    Telemetry_TransmitMessage(rfd_dev, &msg);
}
*/
/* =========== */

/* === BLUETOOTH APP PACKAGING === */
void App_SendFrame(hm11_t *hm11_dev, const odb_data_t *data) {
    if(!hm11_dev || !data) return;

    static uint32_t last_send_time = 0;
    uint32_t current_time = HAL_GetTick();
    if((current_time - last_send_time) >= APP_DELAY_REFRESH_MS) {
		static char buffer[512];
		snprintf(buffer, sizeof(buffer),
				"DATA,time_boot_ms=%lu,system_states=%u,event_states=%u,mission_state=%u,battery_mv=%u,"
				"roll=%ld,pitch=%ld,yaw=%ld,imu_acc_x=%ld,imu_acc_y=%ld,imu_acc_z=%ld,imu_gyro_x=%ld,imu_gyro_y=%ld,imu_gyro_z=%ld,imu_mag_x=%ld,imu_mag_y=%ld,imu_mag_z=%ld,imu_acc_vertical=%ld,"
				"pressure_hpa=%ld,altitude_msl_m=%ld,temp_celsius=%ld,"
				"highg_acc_x=%ld,highg_acc_y=%ld,highg_acc_z=%ld,highg_acc_vertical=%ld,"
				"gps_fix=%u,lat=%ld,lon=%ld,gps_alt=%ld,vel=%u,cog=%u,satellites_nb=%u,"
				"kalman_z=%ld,kalman_v=%ld,pyro_roles=%u:%u:%u:%u\r\n",
				(unsigned long)data->time_boot_ms,           // (ms) - /1
				(unsigned)data->system_states,                // (bitfield) - /1
				(unsigned)data->event_states,                 // (bitfield) - /1
				(unsigned)data->mission_state,                // (state) - /1
				(unsigned)data->battery_mv,                   // (mV) - /1
				(long)(data->roll * 100.0f),                  // (deg) - /100
				(long)(data->pitch * 100.0f),                 // (deg) - /100
				(long)(data->yaw * 100.0f),                   // (deg) - /100
				(long)(data->imu_acc_x * 100.0f),             // (m/s^2) - /100
				(long)(data->imu_acc_y * 100.0f),             // (m/s^2) - /100
				(long)(data->imu_acc_z * 100.0f),             // (m/s^2) - /100
				(long)(data->imu_gyro_x * 100.0f),            // (deg/s) - /100
				(long)(data->imu_gyro_y * 100.0f),            // (deg/s) - /100
				(long)(data->imu_gyro_z * 100.0f),            // (deg/s) - /100
				(long)(data->imu_mag_x * 100.0f),             // (uT) - /100
				(long)(data->imu_mag_y * 100.0f),             // (uT) - /100
				(long)(data->imu_mag_z * 100.0f),             // (uT) - /100
				(long)(data->imu_acc_vertical * 100.0f),      // (m/s^2) - /100
				(long)(data->pressure_hpa * 100.0f),          // (hPa) - /100
				(long)(data->altitude_msl_m * 100.0f),        // (m) - /100
				(long)(data->temp_celsius * 100.0f),          // (°C) - /100
				(long)(data->highg_acc_x * 100.0f),           // (m/s^2) - /100
				(long)(data->highg_acc_y * 100.0f),           // (m/s^2) - /100
				(long)(data->highg_acc_z * 100.0f),           // (m/s^2) - /100
				(long)(data->highg_acc_vertical * 100.0f),    // (m/s^2) - /100
				(unsigned)data->gps_fix,                      // (fix) - /1
				(long)data->lat,                              // (degE7) - /10000000
				(long)data->lon,                              // (degE7) - /10000000
				(long)data->gps_alt,                          // (m) - /1000
				(unsigned)data->vel,                          // (m/s) - /100
				(unsigned)data->cog,                          // (deg) - /100
				(unsigned)data->satellites_nb,                // (count) - /1
				(long)(data->kalman_z * 100.0f),              // (m) - /100
				(long)(data->kalman_v * 100.0f),              // (m/s) - /100
				(unsigned)current_config.pyro_roles[0],       // (role) - Pyro 1
				(unsigned)current_config.pyro_roles[1],       // (role) - Pyro 2
				(unsigned)current_config.pyro_roles[2],       // (role) - Pyro 3
				(unsigned)current_config.pyro_roles[3]);      // (role) - Pyro 4

		HM11_SendString(hm11_dev, buffer);

		last_send_time = current_time;
    }
}

// TODO: use APP_DELAY_REFRESH_MS
void App_HandleCommands(hm11_t *hm11_dev) {
    if(!hm11_dev) return;

    char cmd[HM11_RX_BUFFER_SIZE] = {0};
    if(!HM11_GetMessage(hm11_dev, cmd, sizeof(cmd))) {
		return;
	}

    // Upper case
    for(int i = 0; cmd[i] && i < sizeof(cmd) - 1; i++) {
        if(cmd[i] == '=') break;
        cmd[i] = toupper((unsigned char)cmd[i]);
    }

    // Hello ODB
    if(strncmp(cmd, "HELLO", 5) == 0) {
		const odb_config_t *actual_config = Config_Get();

		static char tx_buf[512];
		snprintf(tx_buf, sizeof(tx_buf),
			"VER:%s\r\n"
			"CFG:NAME=%s\r\n"
			"CFG:ROLE=%u\r\n"
			"CFG:DEBUG=%u\r\n"
			"CFG:BUZZER=%u\r\n"
			"CFG:MIN_PYRO=%u\r\n"
			"CFG:MAX_DROGUE=%u\r\n"
			"CFG:MAX_MAIN=%u\r\n"
			"CFG:ACC_LAUNCH=%.2f\r\n"
			"CFG:V_BOOST=%.2f\r\n"
			"CFG:V_APOGEE=%.2f\r\n"
			"CFG:ALT_MAIN=%.2f\r\n"
			"CFG:V_LAND=%.2f\r\n"
			"CFG:TONE=%u\r\n"
			"CFG:T_LAND=%lu\r\n"
			"CFG:DELAY_FIRE=%lu\r\n"
			"CFG:FAIL_ARM=%lu\r\n"
			"CFG:FAIL_APOGEE=%lu\r\n"
			"CFG:PYRO_ROLE=0,%u\r\n"
			"CFG:PYRO_ROLE=1,%u\r\n"
			"CFG:PYRO_ROLE=2,%u\r\n"
			"CFG:PYRO_ROLE=3,%u\r\n",
			ODB_BLE_FRAME_VERSION,
			actual_config->odb_name,
			actual_config->stage_role,
			actual_config->debug_mode,
			actual_config->enable_buzzer,
			actual_config->min_needed_pyro_nb,
			actual_config->drogue_fire_attempt_max_nb,
			actual_config->main_fire_attempt_max_nb,
			actual_config->acc_z_launch_threshold,
			actual_config->boost_phase_v_threshold,
			actual_config->apogee_detect_v_threshold,
			actual_config->main_deploy_altitude_threshold_m,
			actual_config->landing_detect_v_threshold,
			actual_config->buzzer_report_tone_hz,
			actual_config->landing_detect_threshold_ms,
			actual_config->fire_attempt_delay_ms,
			actual_config->pyros_arming_failsafe_ticks,
			actual_config->apogee_failsafe_ticks,
			actual_config->pyro_roles[0],
			actual_config->pyro_roles[1],
			actual_config->pyro_roles[2],
			actual_config->pyro_roles[3]
		);

		HM11_SendString(hm11_dev, tx_buf);
	}
    // System & Security
    else if(strncmp(cmd, "PING", 4) == 0) {
        HM11_SendString(hm11_dev, "ACK: PONG\r\n");
        if(!critical_led.is_active) {
        	CriticalLED_SetColor(&critical_led, GREEN);
        } else {
        	CriticalLED_SetColor(&critical_led, NONE);
        }
    }
    // Pyros
    // TODO: Check with system_measurements and ...
    else if(strncmp(cmd, "ARM0", 4) == 0) {
    	Pyro_Arming(&pyros[0], &system_measurements, false);
        HM11_SendString(hm11_dev, "ACK: DISARMED\r\n");
    } else if(strncmp(cmd, "ARM1", 4) == 0) {
    	Pyro_Arming(&pyros[0], &system_measurements, true);
        HM11_SendString(hm11_dev, "ACK: ARMED\r\n");
    } else if(strncmp(cmd, "P", 1) == 0 && isdigit((unsigned char)cmd[1])) {
        if(system_measurements.pyros_arming) {
        	int pyro_idx = cmd[1] - '1';
			if(pyro_idx >= 0 && pyro_idx < 4) {
				Pyro_Fire(&pyros[pyro_idx], &system_measurements);

				static char ack_buf[24];
				snprintf(ack_buf, sizeof(ack_buf), "ACK: P%d FIRED\r\n", pyro_idx + 1);
				HM11_SendString(hm11_dev, ack_buf);
			} else {
				HM11_SendString(hm11_dev, "ERR: UNKNOWN PYRO\r\n");
			}
		} else {
			HM11_SendString(hm11_dev, "ERR: REFUSED (NOT ARMED)\r\n");
		}
    }
    // Tests
    else if(strncmp(cmd, "TEST", 4) == 0) {
        if(cmd[4] == '1') {
            HM11_SendString(hm11_dev, "RES: TEST1 OK\r\n");
        } else if(cmd[4] == '2') {
            HM11_SendString(hm11_dev, "RES: TEST2 OK\r\n");
        } else if(cmd[4] == '3') {
            HM11_SendString(hm11_dev, "RES: TEST3 OK\r\n");
        } else {
            HM11_SendString(hm11_dev, "ERR: UNKNOWN TEST\r\n");
        }
    }
    // Calibrations
    else if(strncmp(cmd, "CALIB", 5) == 0) {
        if(cmd[5] == '1') {
            HM11_SendString(hm11_dev, "RES: CALIB1 DONE\r\n");
        } else if(cmd[5] == '2') {
            HM11_SendString(hm11_dev, "RES: CALIB2 DONE\r\n");
        } else {
            HM11_SendString(hm11_dev, "ERR: UNKNOWN CALIB\r\n");
        }
    }
    // Configurations
    else if(strncmp(cmd, "CFG:", 4) == 0) {
        float float_val = 0.0f;
        uint32_t uint32_val = 0;
        int int_val = 0;
        char str_val[32] = {0};
        bool success = false;        

        // Apply new config
        if(strncmp(cmd, "CFG:APPLY", 9) == 0) {
            if(Config_SaveToFlash() == 0) {
            	HM11_SendString(hm11_dev, "ACK: CONFIG SAVED, REBOOTING\r\n");
            	NVIC_SystemReset();

            	return;
            }

            HM11_SendString(hm11_dev, "ACK: FAILED CONFIG SAVE, REBOOTING\r\n");
        }
        // Reset conf
        else if(strncmp(cmd, "CFG:RESET", 9) == 0) {
        	Config_LoadDefaults();
        	Config_SaveToFlash();
        	HM11_SendString(hm11_dev, "ACK: CONFIG SAVED, REBOOTING\r\n");
			NVIC_SystemReset();

			return;
        }
        // ODB name
        else if(sscanf(cmd, "CFG:NAME=%31[^\r\n]", str_val) == 1) {
            strncpy(current_config.odb_name, str_val, sizeof(current_config.odb_name) - 1);
            current_config.odb_name[sizeof(current_config.odb_name) - 1] = '\0';
            HM11_SendString(hm11_dev, "ACK: NAME UPDATED\r\n");
            success = true;
        }
        // Rocket role : 2 = Booster, 3 = Sustainer
        else if(sscanf(cmd, "CFG:ROLE=%d", &int_val) == 1) {
            current_config.stage_role = (uint8_t)int_val;
            HM11_SendString(hm11_dev, "ACK: ROLE UPDATED\r\n");
            success = true;
        }
        // Debug mode : 0 = disabled, 1 = enabled
        else if(sscanf(cmd, "CFG:DEBUG=%d", &int_val) == 1) {
            current_config.debug_mode = (uint8_t)int_val;
            HM11_SendString(hm11_dev, "ACK: DEBUG UPDATED\r\n");
            success = true;
        }
        // Enable buzzer : 0 = disabled, 1 = enabled
        else if(sscanf(cmd, "CFG:BUZZER=%d", &int_val) == 1) {
            current_config.enable_buzzer = (uint8_t)int_val;
            HM11_SendString(hm11_dev, "ACK: BUZZER UPDATED\r\n");
            success = true;
        }
        // Minimum number of pyros needed for launch validation
        else if(sscanf(cmd, "CFG:MIN_PYRO=%d", &int_val) == 1) {
            current_config.min_needed_pyro_nb = (uint8_t)int_val;
            HM11_SendString(hm11_dev, "ACK: MIN_PYRO UPDATED\r\n");
            success = true;
        }
        // Maximum number of attempts to fire the drogue
        else if(sscanf(cmd, "CFG:MAX_DROGUE=%d", &int_val) == 1) {
            current_config.drogue_fire_attempt_max_nb = (uint8_t)int_val;
            HM11_SendString(hm11_dev, "ACK: MAX_DROGUE UPDATED\r\n");
            success = true;
        }
        // Maximum number of attempts to fire the main
        else if(sscanf(cmd, "CFG:MAX_MAIN=%d", &int_val) == 1) {
            current_config.main_fire_attempt_max_nb = (uint8_t)int_val;
            HM11_SendString(hm11_dev, "ACK: MAX_MAIN UPDATED\r\n");
            success = true;
        }
        // Threshold of vertical acceleration in g to validate the launch phase
        else if(sscanf(cmd, "CFG:ACC_LAUNCH=%f", &float_val) == 1) {
            current_config.acc_z_launch_threshold = float_val;
            HM11_SendString(hm11_dev, "ACK: ACC_LAUNCH UPDATED\r\n");
            success = true;
        }
        // Threshold of velocity for validating the boost phase
        else if(sscanf(cmd, "CFG:V_BOOST=%f", &float_val) == 1) {
            current_config.boost_phase_v_threshold = float_val;
            HM11_SendString(hm11_dev, "ACK: V_BOOST UPDATED\r\n");
            success = true;
        }
        // Threshold of velocity to validate apogee
        else if(sscanf(cmd, "CFG:V_APOGEE=%f", &float_val) == 1) {
            current_config.apogee_detect_v_threshold = float_val;
            HM11_SendString(hm11_dev, "ACK: V_APOGEE UPDATED\r\n");
            success = true;
        }
        // Altitude of main parachute deployment in meters
        else if(sscanf(cmd, "CFG:ALT_MAIN=%f", &float_val) == 1) {
            current_config.main_deploy_altitude_threshold_m = float_val;
            HM11_SendString(hm11_dev, "ACK: ALT_MAIN UPDATED\r\n");
            success = true;
        }
        // Threshold of velocity to validate landing
        else if(sscanf(cmd, "CFG:V_LAND=%f", &float_val) == 1) {
            current_config.landing_detect_v_threshold = float_val;
            HM11_SendString(hm11_dev, "ACK: V_LAND UPDATED\r\n");
            success = true;
        }
        // Frequency of the buzzer tone in Hz
        else if(sscanf(cmd, "CFG:TONE=%d", &int_val) == 1) {
            current_config.buzzer_report_tone_hz = (uint32_t)int_val;
            HM11_SendString(hm11_dev, "ACK: TONE UPDATED\r\n");
            success = true;
        }
        // Time threshold for landing detection in milliseconds
        else if(sscanf(cmd, "CFG:T_LAND=%lu", &uint32_val) == 1) {
            current_config.landing_detect_threshold_ms = uint32_val;
            HM11_SendString(hm11_dev, "ACK: T_LAND UPDATED\r\n");
            success = true;
        }
        // Time delay between pyros firing attempts in milliseconds
        else if(sscanf(cmd, "CFG:DELAY_FIRE=%lu", &uint32_val) == 1) {
            current_config.fire_attempt_delay_ms = uint32_val;
            HM11_SendString(hm11_dev, "ACK: DELAY_FIRE UPDATED\r\n");
            success = true;
        }
        // Maximum safety time for pyrotechnic arming in ticks
        else if(sscanf(cmd, "CFG:FAIL_ARM=%lu", &uint32_val) == 1) {
            current_config.pyros_arming_failsafe_ticks = uint32_val;
            HM11_SendString(hm11_dev, "ACK: FAIL_ARM UPDATED\r\n");
            success = true;
        }
        // Maximum safety time for apogee detection in ticks
        else if(sscanf(cmd, "CFG:FAIL_APOGEE=%lu", &uint32_val) == 1) {
            current_config.apogee_failsafe_ticks = uint32_val;
            HM11_SendString(hm11_dev, "ACK: FAIL_APOGEE UPDATED\r\n");
            success = true;
        }
        // Pyro role
        // id: 0
        // role: 0=NONE, 1=MAIN, 2=DROGUE, 3=MAIN_BACKUP, 4=DROGUE_BACKUP
        else if(strncmp(cmd, "CFG:PYRO_ROLE=", 14) == 0) {
			int pyro_id = -1, pyro_role = -1;
			if(sscanf(cmd, "CFG:PYRO_ROLE=%d,%d", &pyro_id, &pyro_role) == 2) {
				if(pyro_id >= 0 && pyro_id < 4 && pyro_role >= 0 && pyro_role <= 4) {
					current_config.pyro_roles[pyro_id] = (uint8_t)pyro_role;
					HM11_SendString(hm11_dev, "ACK: PYRO_ROLE UPDATED\r\n");
					success = true;
				} else {
					HM11_SendString(hm11_dev, "ERR: INVALID PYRO ID OR ROLE\r\n");
				}
			}
		}

        if(!success) {
            HM11_SendString(hm11_dev, "ERR: UNKNOWN CONFIG PARAMETER\r\n");
        }
    }
    // Unknown command
    else {
        HM11_SendString(hm11_dev, "ERR: UNKNOWN CMD\r\n");
    }
}
/* =========== */
