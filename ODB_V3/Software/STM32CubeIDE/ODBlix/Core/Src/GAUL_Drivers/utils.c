/*
 * utils.c
 *
 *  Created on: 2 mars 2026
 *      Author: gagno
 */

#include "GAUL_Drivers/utils.h"


runTimer_t run_timer;

//extern adxl382_t adxl382;
extern bno055_t bno055;
extern hm11_t hm11;
extern l76lm33_t l76lm33;
extern critical_led_t critical_led;
//extern ms5611_t ms5611;
extern pyro_t pyro1;
extern pyro_t pyro2;
extern pyro_t pyro3;
extern pyro_t pyro4;
extern rfd900x_t rfd900x;
extern buzzer_t buzzer;
extern system_measurements_t system_measurements;



static void Telemetry_TransmitMessage(rfd900x_t *rfd_dev, mavlink_message_t *msg) {
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t len = mavlink_msg_to_send_buffer(buffer, msg);
    RFD900x_Transmit(rfd_dev, buffer, len);
}

void Telemetry_SendRocketData(rfd900x_t *rfd_dev, odb_modem_id_t modem_id, odb_data *data, uint32_t current_time_ms) {
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

void Telemetry_SendEventLog(rfd900x_t *rfd_dev, odb_modem_id_t modem_id, odb_event_severity_t severity, const char *text) {
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

