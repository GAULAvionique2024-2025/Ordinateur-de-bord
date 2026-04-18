/*
 * utils.c
 *
 *  Created on: 2 mars 2026
 *      Author: gagno
 */

#include "GAUL_Drivers/utils.h"

#include <ctype.h>


runTimer_t run_timer;

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

uint8_t ODB_SetEventStates(const odb_event_t *event_states) {
    if(!event_states) {
        return 0x00;
    }

    uint8_t packed = 0x00;
    if(event_states->pyro1_fired) {
        packed |= FLAG_PYRO1_FIRED;
    }
    if(event_states->pyro2_fired) {
        packed |= FLAG_PYRO2_FIRED;
    }
    if(event_states->pyro3_fired) {
        packed |= FLAG_PYRO3_FIRED;
    }
    if(event_states->pyro4_fired) {
        packed |= FLAG_PYRO4_FIRED;
    }
    if(event_states->apogee_detected) {
        packed |= FLAG_APOGEE_DETECTED;
    }
    if(event_states->main_deployed) {
        packed |= FLAG_MAIN_DEPLOYED;
    }
    if(event_states->drogue_deployed) {
        packed |= FLAG_DROGUE_DEPLOYED;
    }
    if(event_states->mach_lock_enabled) {
        packed |= FLAG_MACH_LOCK_ENABLED;
    }

    return packed;
}

odb_event_t ODB_GetEventStates(const odb_data *data) {
    odb_event_t event_states = {
        .pyro1_fired = false,
        .pyro2_fired = false,
        .pyro3_fired = false,
        .pyro4_fired = false,
        .apogee_detected = false,
        .main_deployed = false,
        .drogue_deployed = false,
        .mach_lock_enabled = false
    };

    event_states.pyro1_fired = (data->event_states & FLAG_PYRO1_FIRED) != 0U;
    event_states.pyro2_fired = (data->event_states & FLAG_PYRO2_FIRED) != 0U;
    event_states.pyro3_fired = (data->event_states & FLAG_PYRO3_FIRED) != 0U;
    event_states.pyro4_fired = (data->event_states & FLAG_PYRO4_FIRED) != 0U;
    event_states.apogee_detected = (data->event_states & FLAG_APOGEE_DETECTED) != 0U;
    event_states.main_deployed = (data->event_states & FLAG_MAIN_DEPLOYED) != 0U;
    event_states.drogue_deployed = (data->event_states & FLAG_DROGUE_DEPLOYED) != 0U;
    event_states.mach_lock_enabled = (data->event_states & FLAG_MACH_LOCK_ENABLED) != 0U;
    
    return event_states;
}

odb_state_t ODB_Init(odb_data *data) {
    if(!data) {
        return ODB_ERROR;
    }

    bool alimentation_fault = false;
    uint8_t error = 0;
    uint8_t warning = 0;

    ODB_Reset(data);
    data->mission_state = ODB_MISSION_STATE_PREFLIGHT;

    uint16_t system_states = 0x0000;
    if(SystemMeasurements_Init(&system_measurements) == 0) {
        SystemMeasurements_ComputePower(&system_measurements);
        if(system_measurements.vin_batt <= 5000 || system_measurements.vin_batt >= 24000 || system_measurements.v5_buck <= 4500 || system_measurements.v5_buck >= 5500 || system_measurements.v3_buck <= 3100 || system_measurements.v3_buck >= 3500 || system_measurements.pg_v5 == false) {
            alimentation_fault = true;
            printf("Erreur : Batterie trop faible !\n");
        }

        uint8_t pyros_connected = 0;
        // Pyros need to be armed for read status
        bool is_armed = Pyro_Arming(true, &system_measurements);
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
            pyros_connected += 1;
        } else {
            warning += 1;
            printf("Erreur : Pyro 1 déconnecté\n");
        }
        if(Pyro_Init(&pyro2, &system_measurements) == 0) {
            system_states |= FLAG_PYRO2_CONN;
            pyros_connected += 1;
        } else {
            warning += 1;
            printf("Erreur : Pyro 2 déconnecté\n");
        }
        if(Pyro_Init(&pyro3, &system_measurements) == 0) {
            system_states |= FLAG_PYRO3_CONN;
            pyros_connected += 1;
        } else {
            warning += 1;
            printf("Erreur : Pyro 3 déconnecté\n");
        }
        if(Pyro_Init(&pyro4, &system_measurements) == 0) {
            system_states |= FLAG_PYRO4_CONN;
            pyros_connected += 1;
        } else {
            warning += 1;
            printf("Erreur : Pyro 4 déconnecté\n");
        }

        is_armed = Pyro_Arming(true, &system_measurements);
        // Verify if arming is really disabled
		if(!is_armed) {
			system_states |= FLAG_PYROS_ARMED_OK;
		} else {
            system_states &= ~FLAG_PYROS_ARMED_OK;
			error += 1;
            printf("Erreur : Désarmement des Pyros bloqué\n");
		}
        // Protection
        if(pyros_connected == 0) {
            error += 1;
            printf("Erreur : Aucun pyro connecté !\n");
        }

        SystemMeasurements_ComputeTemperature(&system_measurements);
        if(system_measurements.temperature < -55.0f || system_measurements.temperature > 150.0f) {
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

    odb_state_t odb_state = ODB_ERROR;
    if(alimentation_fault) {
        ODB_SetMissionState(data, ODB_MISSION_STATE_ERROR);
        odb_state = ODB_ALIMENTATION_ERROR;
        printf("Erreur : Alimentation non conforme !\n");
    } else if(error > 0) {
        ODB_SetMissionState(data, ODB_MISSION_STATE_ERROR);
        odb_state = ODB_ERROR;
        printf("Erreur : %d erreur(s) détectée(s) lors de l'initialisation du système.\n", error);
    } else if(warning > 0) {
        ODB_SetMissionState(data, ODB_MISSION_STATE_READY); // Ready with warnings
        odb_state = ODB_WARNING;
        printf("Attention : %d warning(s) détectée(s) lors de l'initialisation du système.\n", warning);
    } else {
        ODB_SetMissionState(data, ODB_MISSION_STATE_READY); // All good
        odb_state = ODB_OK;
    }

    // Update system states
    data->system_states = system_states;

    return odb_state;
}

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
    }

    if(ADXL382_ReadData(&adxl382) == ADXL382_OK) {
      data->highg_acc_x = adxl382.acc_x;
      data->highg_acc_y = adxl382.acc_y;
      data->highg_acc_z = adxl382.acc_z;
    }

    if(L76LM33_Read(&l76lm33) == L76LM33_OK) {
      data->gps_fix = l76lm33.gps_data.gps_fix;
      data->lat = l76lm33.gps_data.lat;
      data->lon = l76lm33.gps_data.lon;
      data->gps_alt = l76lm33.gps_data.gps_alt;
      data->vel = l76lm33.gps_data.vel;
      data->cog = l76lm33.gps_data.cog;
      data->satellites_nb = l76lm33.gps_data.satellites_nb;
    }

    data->time_boot_ms = HAL_GetTick();
    data->battery_mv = (uint16_t)(system_measurements.vin_batt);

    odb_event_t event_states = ODB_GetEventStates(data);
    if(system_measurements.pyros_arming) {
        event_states.pyro1_fired = pyro1.is_fire;
        event_states.pyro2_fired = pyro2.is_fire;
        event_states.pyro3_fired = pyro3.is_fire;
        event_states.pyro4_fired = pyro4.is_fire;
    }

    // TODO: add real updated values
    data->roll = 0.0f;
    data->pitch = 0.0f;
    data->yaw = 0.0f;
    data->imu_acc_x = 0.0f;
    data->imu_acc_y = 0.0f;
    data->imu_acc_z = 0.0f;
    data->imu_gyro_x = 0.0f;
    data->imu_gyro_y = 0.0f;
    data->imu_gyro_z = 0.0f;

    if(data->gps_fix > 1) {
      data->system_states |= FLAG_GPS_OK;
    }

    data->event_states = ODB_SetEventStates(&event_states);
}

int8_t ODB_SetMissionState(odb_data *data, uint8_t mission_state) {
	data->mission_state = mission_state;
	return 0;
}
/* =========== */

/* === TELEMETRY === */
static uint8_t mavlink_tx_buffer[MAVLINK_MAX_PACKET_LEN]; // DMA TX buffer

static void Telemetry_TransmitMessage(rfd900x_t *rfd_dev, const mavlink_message_t *msg) {
	if(!rfd_dev) return;

	if(rfd_dev->huart->gState != HAL_UART_STATE_READY) {
		return;
	}

	uint16_t len = mavlink_msg_to_send_buffer(mavlink_tx_buffer, msg);
	RFD900x_Transmit(rfd_dev, mavlink_tx_buffer, len);
}

void Telemetry_SendRocketData(rfd900x_t *rfd_dev, const odb_modem_id_t modem_id, const odb_data *data, const uint32_t current_time_ms) {
    if(!rfd_dev || !data) return;

    // TODO: add event_states
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
        (int16_t)(data->roll * 100.0f),
        (int16_t)(data->pitch * 100.0f),
        (int16_t)(data->yaw * 100.0f),
        (int16_t)(data->temp_celsius * 100.0f),
        (int16_t)(data->imu_acc_x * 100.0f),
        (int16_t)(data->imu_acc_y * 100.0f),
        (int16_t)(data->imu_acc_z * 100.0f),
        (int32_t)(data->imu_gyro_x * 100.0f),
        (int32_t)(data->imu_gyro_y * 100.0f),
        (int32_t)(data->imu_gyro_z * 100.0f),
        (int32_t)(data->highg_acc_x * 100.0f),
        (int32_t)(data->highg_acc_y * 100.0f),
        (int32_t)(data->highg_acc_z * 100.0f),
        data->system_states,
        data->battery_mv,
        data->vel,
        data->cog,
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
            "DATA,time_boot_ms=%lu,system_states=%u,event_states=%u,mission_state=%u,battery_mv=%u,roll=%.2f,pitch=%.2f,yaw=%.2f,imu_acc_x=%.2f,imu_acc_y=%.2f,imu_acc_z=%.2f,imu_gyro_x=%.2f,imu_gyro_y=%.2f,imu_gyro_z=%.2f,pressure_hpa=%.2f,temp_celsius=%.2f,highg_acc_x=%.2f,highg_acc_y=%.2f,highg_acc_z=%.2f,gps_fix=%u,lat=%ld,lon=%ld,gps_alt=%ld,vel=%u,cog=%u,satellites_nb=%u\r\n",
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
    	Pyro_Arming(false, &system_measurements);
        HM11_SendString(hm11_dev, "ACK: DISARMED\r\n");
        Telemetry_SendEventLog(&rfd900x, 1, MAV_SEVERITY_WARNING, "PYROS DISARMED VIA BT");
    } else if(strncmp(cmd, "ARM1", 4) == 0) {
    	Pyro_Arming(true, &system_measurements);
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


/* === HELPERS === */
void RunTimer_Init(runTimer_t* dev) {
	  dev->start_time = HAL_GetTick();
	  dev->elapsed_time_ms = 0;
	  dev->elapsed_time_s = 0;
	  dev->elapsed_time_m = 0;
	  dev->elapsed_time_remaining_ms = 0;
}

void RunTimer_Update(runTimer_t *dev) {
	dev->elapsed_time_ms = HAL_GetTick() - dev->start_time;

	dev->elapsed_time_s = (dev->elapsed_time_ms / 1000) % 60;
	dev->elapsed_time_m = (dev->elapsed_time_ms / 60000);
	dev->elapsed_time_remaining_ms = dev->elapsed_time_ms % 1000;
}

int printt(const char *format, ...) {
    va_list args;
    va_start(args, format);
    RunTimer_Update(&run_timer);
    printf("[%03d:%02d:%03d] ",run_timer.elapsed_time_m, run_timer.elapsed_time_s, run_timer.elapsed_time_remaining_ms);

    int ret = vprintf(format, args);

    va_end(args);

    return ret;
}

int _write(int le, char *ptr, int len) {
    int DataIdx;
    for (DataIdx = 0; DataIdx < len; DataIdx++) {
        ITM_SendChar(*ptr++);
    }

    return len;
}

