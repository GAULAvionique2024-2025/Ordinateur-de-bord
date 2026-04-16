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
            "DATA,time_boot_ms=%lu,system_states=%u,battery_mv=%u,roll=%.2f,pitch=%.2f,yaw=%.2f,imu_acc_x=%.2f,imu_acc_y=%.2f,imu_acc_z=%.2f,imu_gyro_x=%.2f,imu_gyro_y=%.2f,imu_gyro_z=%.2f,pressure_hpa=%.2f,temp_celsius=%.2f,highg_acc_x=%.2f,highg_acc_y=%.2f,highg_acc_z=%.2f,gps_fix=%u,lat=%ld,lon=%ld,gps_alt=%ld,vel=%u,cog=%u,satellites_nb=%u\r\n",
            (unsigned long)data->time_boot_ms,
            (unsigned)data->system_states,
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
    	Pyro_Arming(false);
        HM11_SendString(hm11_dev, "ACK: DISARMED\r\n");
        Telemetry_SendEventLog(&rfd900x, 1, MAV_SEVERITY_WARNING, "PYROS DISARMED VIA BT");
    } else if(strncmp(cmd, "ARM1", 4) == 0) {
    	Pyro_Arming(true);
        HM11_SendString(hm11_dev, "ACK: ARMED\r\n");
        Telemetry_SendEventLog(&rfd900x, 1, MAV_SEVERITY_WARNING, "PYROS ARMED VIA BT");
    }
    // Pyros
    else if(strncmp(cmd, "P", 1) == 0 && isdigit((unsigned char)cmd[1])) {
        if(system_measurements.pyros_arming) {
            if(cmd[1] == '1') {
                Pyro_Fire(&pyro1);
                // TODO: Check with system_measurements and ...
                HM11_SendString(hm11_dev, "ACK: P1 FIRED\r\n");
                Telemetry_SendEventLog(&rfd900x, 1, MAV_SEVERITY_CRITICAL, "PYRO 1 FIRED");
            } else if(cmd[1] == '2') {
                Pyro_Fire(&pyro2);
                // TODO: Check with system_measurements and ...
                HM11_SendString(hm11_dev, "ACK: P2 FIRED\r\n");
                Telemetry_SendEventLog(&rfd900x, 1, MAV_SEVERITY_CRITICAL, "PYRO 2 FIRED");
            } else if(cmd[1] == '3') {
                Pyro_Fire(&pyro3);
                // TODO: Check with system_measurements and ...
                HM11_SendString(hm11_dev, "ACK: P3 FIRED\r\n");
                Telemetry_SendEventLog(&rfd900x, 1, MAV_SEVERITY_CRITICAL, "PYRO 3 FIRED");
            } else if(cmd[1] == '4') {
                Pyro_Fire(&pyro4);
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

