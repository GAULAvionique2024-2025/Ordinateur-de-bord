/*
 * RFD900.c
 *
 *  Created on: Feb 19, 2024
 *      Author: gagno
 *
 *  Edited on: Mar 02, 2026
 *  	Author: AudaceLol12
 */

#include "GAUL_Drivers/rfd900x.h"

#include <string.h>


static void MAVLink_Transmit(RFD900x_HandleTypeDef *hrfd, mavlink_message_t *msg) {
    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    uint16_t len = mavlink_msg_to_send_buffer(buffer, msg);

    HAL_UART_Transmit(hrfd->huart, buffer, len, HAL_MAX_DELAY);
}

int8_t RFD900x_Init(RFD900x_HandleTypeDef *hrfd, UART_HandleTypeDef *huart, rfd900x_modem_id_t modem_id, uint8_t component_id) {
	if (!hrfd || !huart || modem_id < 1 || component_id == 0) return -1;

    hrfd->huart = huart;
    hrfd->modem_id = modem_id;
    hrfd->component_id = component_id;

    return 0; // success
}

void RFD900x_Send(RFD900x_HandleTypeDef *hrfd, odb_mavlink_data *data, uint32_t current_time_ms) {
    mavlink_message_t msg;
    uint64_t time_usec = (uint64_t)current_time_ms * 1000;

    /// Heartbeat
    mavlink_msg_heartbeat_pack(hrfd->modem_id, hrfd->component_id, &msg, MAV_TYPE_ROCKET, MAV_AUTOPILOT_INVALID, MAV_MODE_FLAG_CUSTOM_MODE_ENABLED, 0, MAV_STATE_ACTIVE);
    MAVLink_Transmit(hrfd, &msg);

    // Status
    mavlink_msg_named_value_int_pack(hrfd->modem_id, hrfd->component_id, &msg, current_time_ms, "STATES", data->system_states);
    MAVLink_Transmit(hrfd, &msg);

    // IMU
    mavlink_msg_attitude_pack(hrfd->modem_id, hrfd->component_id, &msg, current_time_ms, data->roll, data->pitch, data->yaw, 0, 0, 0);
    MAVLink_Transmit(hrfd, &msg);

    // Pressure & Temp
    mavlink_msg_scaled_pressure_pack(hrfd->modem_id, hrfd->component_id, &msg, current_time_ms, data->pressure_hpa, 0, (int16_t)(data->temp_celsius * 100.0f), 0);
    MAVLink_Transmit(hrfd, &msg);

    // High-G
    mavlink_msg_highres_imu_pack(hrfd->modem_id, hrfd->component_id, &msg, time_usec, data->acc_x, data->acc_y, data->acc_z, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    MAVLink_Transmit(hrfd, &msg);

    // GPS
    mavlink_msg_gps_raw_int_pack(hrfd->modem_id, hrfd->component_id, &msg, time_usec, data->gps_fix, data->lat, data->lon, data->gps_alt, 65535, 65535, data->vel, data->cog, data->satellites_nb, 0, 0, 0, 0, 0, 0);
    MAVLink_Transmit(hrfd, &msg);
}

void RFD900x_Send_EventLog(RFD900x_HandleTypeDef *hrfd, uint8_t severity, const char *log) {
    if(strlen(log) > 50) return;

    mavlink_message_t msg;
    mavlink_msg_statustext_pack(hrfd->modem_id, hrfd->component_id, &msg, severity, log, 0, 0);

    MAVLink_Transmit(hrfd, &msg);
}
