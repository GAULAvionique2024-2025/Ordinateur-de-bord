/*
 * system.c
 *
 *  Created on: 3 mai 2026
 *      Author: gagno
 */


// Apogee Detection & Pyros Firing logic: https://www.rocketryforum.com/threads/most-accurate-way-to-measure-velocity-accelerometer-vs-barometer-vs.157866/page-2

#include "GAUL_Drivers/system.h"
#include "App/config.h"

#include <ctype.h>


#define KALMAN_NAV_SAMPLE_NB 50

extern adxl382_t adxl382;
extern bno055_t bno055;
extern hm11_t hm11;
extern l76lm33_t l76lm33;
extern critical_led_t critical_led;
extern ms5611_t ms5611;
extern pyro_t pyro1;
extern pyro_t pyro2;
extern pyro_t pyro3;
extern pyro_t pyro4;
extern rfd900x_t rfd900x;
extern buzzer_t buzzer;
extern system_measurements_t system_measurements;
extern w25q_t w25q;
extern nexus_t nexus;

static odb_stats_t stats;
static kalman_nav_t kalman_filter;


/* === ODB === */
void ODB_Reset(odb_data *data) {
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

odb_state_t ODB_Init(odb_data *data) {
    if(!data) {
        return ODB_ERROR;
    }

    bool alimentation_fault = false;
    uint8_t error = 0;
    uint8_t warning = 0;

    ODB_Reset(data);

    uint16_t system_states = 0x0000;
    if(SystemMeasurements_Init(&system_measurements) == 0) {
        SystemMeasurements_ComputePower(&system_measurements);
        if(system_measurements.vin_batt <= VIN_BATT_MIN_MV || system_measurements.vin_batt >= VIN_BATT_MAX_MV || system_measurements.v5_buck <= V5_MIN_MV || system_measurements.v5_buck >= V5_MAX_MV || system_measurements.v3_buck <= V3_MIN_MV || system_measurements.v3_buck >= V3_MAX_MV || system_measurements.pg_v5 == false) {
            alimentation_fault = true;
            printf("Erreur : Batterie trop faible !\n");
        }

        // Pyros need to be armed for read status
        bool is_armed = Pyro_Arming(&pyro1, &system_measurements, true);
        // Verify if arming is really enabled
        if(is_armed) {
            system_states |= FLAG_PYROS_ARMED_OK;
        } else {
            error += 1;
            printf("Erreur : Armement des Pyros bloqué\n");
        }
        // Check pyros continuity
        if(Pyro_Init(&pyro1, &system_measurements) == 0) {
            system_states |= FLAG_PYRO1_CONN;
            data->pyros_connected += 1;
        } else {
            warning += 1;
            printf("Erreur : Pyro 1 déconnecté\n");
        }
        if(Pyro_Init(&pyro2, &system_measurements) == 0) {
            system_states |= FLAG_PYRO2_CONN;
            data->pyros_connected += 1;
        } else {
            warning += 1;
            printf("Erreur : Pyro 2 déconnecté\n");
        }
        if(Pyro_Init(&pyro3, &system_measurements) == 0) {
            system_states |= FLAG_PYRO3_CONN;
            data->pyros_connected += 1;
        } else {
            warning += 1;
            printf("Erreur : Pyro 3 déconnecté\n");
        }
        if(Pyro_Init(&pyro4, &system_measurements) == 0) {
            system_states |= FLAG_PYRO4_CONN;
            data->pyros_connected += 1;
        } else {
            warning += 1;
            printf("Erreur : Pyro 4 déconnecté\n");
        }

        is_armed = Pyro_Arming(&pyro1, &system_measurements, false);
        // Verify if arming is really disabled
		if(!is_armed) {
			system_states |= FLAG_PYROS_ARMED_OK;
		} else {
            system_states &= ~FLAG_PYROS_ARMED_OK;
			error += 1;
            printf("Erreur : Désarmement des Pyros bloqué\n");
		}
        // Protection
        if(data->pyros_connected < MIN_NEEDED_PYRO_NB) {
            error += 1;
            printf("Erreur : Pas assez de pyros connectés !\n");
        }

        SystemMeasurements_ComputeTemperature(&system_measurements);
        if(system_measurements.temperature < MAX6612MXK_MIN_TEMP_C || system_measurements.temperature > MAX6612MXK_MAX_TEMP_C) {
            error += 1;
            printf("Erreur : Température hors limites !\n");
        }
    } else {
        error += 1;
        printf("Erreur : Init SystemMeasurements\n");
    }

    /*
    if(BNO055_Init(&bno055) == BNO055_OK) {
        system_states |= FLAG_IMU_OK;
    } else {
        error += 1;
        printf("Erreur : Init BNO055\n");
    }
    */

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

    if(CriticalLed_Init(&critical_led) != 0) {
        warning += 1;
        printf("Erreur : Init Critical LED\n");
    }

    // Kalman filter initialization -> calculate R_static
    if((system_states & FLAG_BARO_OK) && (system_states & FLAG_HIGHG_OK)) {
        float samples[KALMAN_NAV_SAMPLE_NB];
        float sum = 0;
        float temperature, pressure;
        for(int i = 0; i < KALMAN_NAV_SAMPLE_NB; i++) { 
            MS5611_Update(&ms5611); 
            MS5611_Compute(&ms5611, &temperature, &pressure);
            samples[i] = pressure;
            sum += samples[i];
        }

        KalmanNav_Init(&kalman_filter, sum/KALMAN_NAV_SAMPLE_NB, samples, KALMAN_NAV_SAMPLE_NB);
    }

    odb_state_t odb_state = ODB_ERROR;
    if(alimentation_fault) {
        odb_state = ODB_ALIMENTATION_ERROR;
        printf("Erreur : Alimentation non conforme !\n");
    } else if(error > 0) {
        odb_state = ODB_ERROR;
        printf("Erreur : %d erreur(s) détectée(s) lors de l'initialisation du système.\n", error);
    } else {
        odb_state = ODB_OK;
    }

    // Update system states
    data->system_states = system_states;

    // Buzzer report
    Buzzer_ReportStatus(&buzzer, BUZZER_REPORT_TONE_HZ, system_measurements.vin_batt, (bool[]){(system_states & FLAG_PYRO1_CONN) != 0U, (system_states & FLAG_PYRO2_CONN) != 0U, (system_states & FLAG_PYRO3_CONN) != 0U, (system_states & FLAG_PYRO4_CONN) != 0U}, odb_state);

    return odb_state;
}

// TODO: add timestamp with RTC to all odb_stats_t data
void ODB_Update(odb_data *data) {
    if(!data) {
        return;
    }

    SystemMeasurements_ComputeTemperature(&system_measurements);
    SystemMeasurements_ComputePower(&system_measurements);
    SystemMeasurements_ComputePyros(&system_measurements);

    float temperature, pressure;
    MS5611_Update(&ms5611);
    if(MS5611_Compute(&ms5611, &temperature, &pressure) == MS5611_OK) {
    	data->pressure_hpa = pressure;
    	data->temp_celsius = temperature;
        data->altitude_msl_m = Math_ComputeAltitudeMSL(pressure);
    }

    // TODO: use real updated values
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
    /*
    if(BNO055_IsDataReady(&bno055)) {
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
    }
    */

    const float current_quat[4] = {bno055.quat.w, bno055.quat.x, bno055.quat.y, bno055.quat.z};
    if(ADXL382_ReadData(&adxl382, current_quat) == ADXL382_OK) {
      data->highg_acc_x = adxl382.acc_x;
      data->highg_acc_y = adxl382.acc_y;
      data->highg_acc_z = adxl382.acc_z;
      data->highg_acc_vertical = adxl382.acc_vertical;
    }

    // Kalman filter update with dynamic R_alt
    float raw_accel_z = data->highg_acc_vertical;
    if(fabs(raw_accel_z) < 3.5f) {
        raw_accel_z = data->imu_acc_vertical;
    }
    KalmanNav_Predict(&kalman_filter, (float)raw_accel_z);
    KalmanNav_Update(&kalman_filter, (float)data->altitude_msl_m);
    data->kalman_z = (float)kalman_filter.z;
    data->kalman_v = (float)kalman_filter.v;

    if(L76LM33_Compute(&l76lm33) == L76LM33_OK) {
      data->gps_fix         = l76lm33.gps_data.gps_fix;
      data->lat             = l76lm33.gps_data.lat;
      data->lon             = l76lm33.gps_data.lon;
      data->gps_alt         = l76lm33.gps_data.gps_alt;
      data->vel             = l76lm33.gps_data.vel;
      data->cog             = l76lm33.gps_data.cog;
      data->satellites_nb   = l76lm33.gps_data.satellites_nb;
    }

    data->time_boot_ms = HAL_GetTick();
    data->battery_mv = (uint16_t)(system_measurements.vin_batt);


    if(system_measurements.pyros_arming) {
    	stats.pyro1.fired = pyro1.is_fire;
    	stats.pyro2.fired = pyro2.is_fire;
    	stats.pyro3.fired = pyro3.is_fire;
    	stats.pyro4.fired = pyro4.is_fire;
    }

    if(data->gps_fix > 1) {
      data->system_states |= FLAG_GPS_OK;
    } else {
      data->system_states &= ~FLAG_GPS_OK;
    }

    data->event_states = ODB_SetEventStates(&stats);
}
/* =========== */

/* === TELEMETRY === */
static uint8_t mavlink_tx_buffer[MAVLINK_MAX_PACKET_LEN]; // DMA TX buffer

static void Telemetry_TransmitMessage(rfd900x_t *rfd_dev, const mavlink_message_t *msg) {
	if(!rfd_dev) return;

	uint16_t len = mavlink_msg_to_send_buffer(mavlink_tx_buffer, msg);
	RFD900x_Transmit(rfd_dev, mavlink_tx_buffer, len);
}

void Telemetry_SendRocketData(rfd900x_t *rfd_dev, const odb_modem_id_t modem_id, const odb_data *data, const uint32_t current_time_ms) {
    if(!rfd_dev || !data) return;

    mavlink_message_t msg;

    mavlink_msg_rocket_telemetry_pack(
        modem_id,
        MAVLINK_COMPONENT_ID,
        &msg,
        current_time_ms,
        data->lat,
        data->lon,
        data->gps_alt,
        data->pressure_hpa,
        (int32_t)(data->imu_gyro_x * 100.0f),
        (int32_t)(data->imu_gyro_y * 100.0f),
        (int32_t)(data->imu_gyro_z * 100.0f),
        (int32_t)(data->highg_acc_x * 100.0f),
        (int32_t)(data->highg_acc_y * 100.0f),
        (int32_t)(data->highg_acc_z * 100.0f),
        (int16_t)(data->roll * 100.0f),
        (int16_t)(data->pitch * 100.0f),
        (int16_t)(data->yaw * 100.0f),
        (int16_t)(data->temp_celsius * 100.0f),
        (int16_t)(data->imu_acc_x * 100.0f),
        (int16_t)(data->imu_acc_y * 100.0f),
        (int16_t)(data->imu_acc_z * 100.0f),
        (int16_t)(data->imu_mag_x * 10.0f),
        (int16_t)(data->imu_mag_y * 10.0f),
        (int16_t)(data->imu_mag_z * 10.0f),
        data->system_states,
        data->battery_mv,
        data->vel,
        data->cog,
        data->event_states,
        data->mission_state,
        data->gps_fix,
        data->satellites_nb
    );

    Telemetry_TransmitMessage(rfd_dev, &msg);
}

void Telemetry_SendEventLog(rfd900x_t *rfd_dev, const odb_modem_id_t modem_id, const odb_event_severity_t severity, const char *text) {
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
/* =========== */

/* === BLUETOOTH APP PACKAGING === */
void App_SendFrame(nexus_t *nexus_dev, hm11_t *hm11_dev, const odb_data *data) {
    if(!nexus_dev || !hm11_dev || !data) return;

    char buffer[512];
    snprintf(buffer, sizeof(buffer),
            "DATA,time_boot_ms=%lu,system_states=%u,event_states=%u,mission_state=%u,battery_mv=%u,roll=%.2f,pitch=%.2f,yaw=%.2f,imu_acc_x=%.2f,imu_acc_y=%.2f,imu_acc_z=%.2f,imu_gyro_x=%.2f,imu_gyro_y=%.2f,imu_gyro_z=%.2f,imu_mag_x=%.2f,imu_mag_y=%.2f,imu_mag_z=%.2f,pressure_hpa=%.2f,temp_celsius=%.2f,highg_acc_x=%.2f,highg_acc_y=%.2f,highg_acc_z=%.2f,gps_fix=%u,lat=%ld,lon=%ld,gps_alt=%ld,vel=%u,cog=%u,satellites_nb=%u\r\n",
            (unsigned long)data->time_boot_ms,
            (unsigned)data->system_states,
            (unsigned)data->event_states,
            (unsigned)data->mission_state,
            (unsigned)data->battery_mv,
            data->roll,
            data->pitch,
            data->yaw,
            data->imu_acc_x,
            data->imu_acc_y,
            data->imu_acc_z,
            data->imu_gyro_x,
            data->imu_gyro_y,
            data->imu_gyro_z,
			data->imu_mag_x,
			data->imu_mag_y,
			data->imu_mag_z,
            data->pressure_hpa,
            data->temp_celsius,
            data->highg_acc_x,
            data->highg_acc_y,
            data->highg_acc_z,
            (unsigned)data->gps_fix,
            (long)data->lat,
            (long)data->lon,
            (long)data->gps_alt,
            (unsigned)data->vel,
            (unsigned)data->cog,
            (unsigned)data->satellites_nb);

    HM11_SendString(hm11_dev, buffer);
}

void App_HandleCommands(nexus_t *nexus_dev, hm11_t *hm11_dev) {
    if(!nexus_dev || !hm11_dev) return;

    char cmd[HM11_RX_BUFFER_SIZE] = {0};
    if(!HM11_GetMessage(hm11_dev, cmd, sizeof(cmd))) {
		return;
	}

    for(int i = 0; cmd[i] && i < sizeof(cmd) - 1; i++) {
        cmd[i] = toupper((unsigned char)cmd[i]);
    }

    // System & Security
    if(strncmp(cmd, "PING", 4) == 0) {
        HM11_SendString(hm11_dev, "PONG\r\n");
    } else if(strncmp(cmd, "ARM0", 4) == 0) {
    	Pyro_Arming(&pyro1, &system_measurements, false);
        HM11_SendString(hm11_dev, "ACK: DISARMED\r\n");
        Telemetry_SendEventLog(&rfd900x, 1, MAV_SEVERITY_WARNING, "PYROS DISARMED VIA BT");
    } else if(strncmp(cmd, "ARM1", 4) == 0) {
    	Pyro_Arming(&pyro1, &system_measurements, true);
        HM11_SendString(hm11_dev, "ACK: ARMED\r\n");
        Telemetry_SendEventLog(&rfd900x, 1, MAV_SEVERITY_WARNING, "PYROS ARMED VIA BT");
    }
    // Pyros
    else if(strncmp(cmd, "P", 1) == 0 && isdigit((unsigned char)cmd[1])) {
        if(system_measurements.pyros_arming) {
            if(cmd[1] == '1') {
                Pyro_Fire(&pyro1, &system_measurements);
                // TODO: Check with system_measurements and ...
                HM11_SendString(hm11_dev, "ACK: P1 FIRED\r\n");
                Telemetry_SendEventLog(&rfd900x, 1, MAV_SEVERITY_CRITICAL, "PYRO 1 FIRED");
            } else if(cmd[1] == '2') {
                Pyro_Fire(&pyro2, &system_measurements);
                // TODO: Check with system_measurements and ...
                HM11_SendString(hm11_dev, "ACK: P2 FIRED\r\n");
                Telemetry_SendEventLog(&rfd900x, 1, MAV_SEVERITY_CRITICAL, "PYRO 2 FIRED");
            } else if(cmd[1] == '3') {
                Pyro_Fire(&pyro3, &system_measurements);
                // TODO: Check with system_measurements and ...
                HM11_SendString(hm11_dev, "ACK: P3 FIRED\r\n");
                Telemetry_SendEventLog(&rfd900x, 1, MAV_SEVERITY_CRITICAL, "PYRO 3 FIRED");
            } else if(cmd[1] == '4') {
                Pyro_Fire(&pyro4, &system_measurements);
                // TODO: Check with system_measurements and ...
                HM11_SendString(hm11_dev, "ACK: P4 FIRED\r\n");
                Telemetry_SendEventLog(&rfd900x, 1, MAV_SEVERITY_CRITICAL, "PYRO 4 FIRED");
            } else {
                HM11_SendString(hm11_dev, "ERR: UNKNOWN PYRO\r\n");
                Telemetry_SendEventLog(&rfd900x, 1, MAV_SEVERITY_CRITICAL, "UNKNOWN PYRO CMD VIA BT");
            }
        } else {
            HM11_SendString(hm11_dev, "ERR: REFUSED (NOT ARMED)\r\n");
            Telemetry_SendEventLog(&rfd900x, 1, MAV_SEVERITY_WARNING, "PYRO CMD REFUSED (NOT ARMED)");
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
    // === COMMANDE INCONNUE ===
    else {
        HM11_SendString(hm11_dev, "ERR: UNKNOWN CMD\r\n");
    }
}
/* =========== */
