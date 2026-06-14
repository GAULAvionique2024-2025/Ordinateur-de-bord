/*
 * telemetry_comm.c
 *
 *  Created on: 14 juin 2026
 *      Author: gagno
 */


#include "Comm/telemetry_comm.h"


static uint8_t mavlink_tx_buffer[MAVLINK_MAX_PACKET_LEN];

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
            (int32_t)(data_temp->roll * 100.0f),
            (int32_t)(data_temp->pitch * 100.0f),
            (int32_t)(data_temp->yaw * 100.0f),
            (int32_t)(data_temp->imu_acc_x * 100.0f),
            (int32_t)(data_temp->imu_acc_y * 100.0f),
            (int32_t)(data_temp->imu_acc_z * 100.0f),
            (int32_t)(data_temp->imu_gyro_x * 100.0f),
            (int32_t)(data_temp->imu_gyro_y * 100.0f),
            (int32_t)(data_temp->imu_gyro_z * 100.0f),
            (int32_t)(data_temp->imu_mag_x * 100.0f),
            (int32_t)(data_temp->imu_mag_y * 100.0f),
            (int32_t)(data_temp->imu_mag_z * 100.0f),
            (int32_t)(data_temp->altitude_msl_m * 100.0f),
            (int32_t)(data_temp->pressure_pa * 100.0f),
            (int32_t)(data_temp->temp_celsius * 100.0f),
            (int32_t)(data_temp->highg_acc_x * 100.0f),
            (int32_t)(data_temp->highg_acc_y * 100.0f),
            (int32_t)(data_temp->highg_acc_z * 100.0f),
            data_temp->gps_fix,
            data_temp->lat,
            data_temp->lon,
            data_temp->gps_alt,
            data_temp->vel,
            data_temp->cog,
            data_temp->satellites_nb,
            (int32_t)(data_temp->imu_acc_vertical * 100.0f),
            (int32_t)(data_temp->highg_acc_vertical * 100.0f),
            (int32_t)(data_temp->kalman_z * 100.0f),
            (int32_t)(data_temp->kalman_v * 100.0f)
        );

    Telemetry_TransmitMessage(rfd_dev, &msg);
}
