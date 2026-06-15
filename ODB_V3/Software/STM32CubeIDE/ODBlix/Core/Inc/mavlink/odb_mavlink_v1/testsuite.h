/** @file
 *    @brief MAVLink comm protocol testsuite generated from odb_mavlink_v1.xml
 *    @see https://mavlink.io/en/
 */
#pragma once
#ifndef ODB_MAVLINK_V1_TESTSUITE_H
#define ODB_MAVLINK_V1_TESTSUITE_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MAVLINK_TEST_ALL
#define MAVLINK_TEST_ALL
static void mavlink_test_common(uint8_t, uint8_t, mavlink_message_t *last_msg);
static void mavlink_test_odb_mavlink_v1(uint8_t, uint8_t, mavlink_message_t *last_msg);

static void mavlink_test_all(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_common(system_id, component_id, last_msg);
    mavlink_test_odb_mavlink_v1(system_id, component_id, last_msg);
}
#endif

#include "../common/testsuite.h"


static void mavlink_test_rocket_telemetry(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
    mavlink_status_t *status = mavlink_get_channel_status(MAVLINK_COMM_0);
        if ((status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) && MAVLINK_MSG_ID_ROCKET_TELEMETRY >= 256) {
            return;
        }
#endif
    mavlink_message_t msg;
        uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
        uint16_t i;
    mavlink_rocket_telemetry_t packet_in = {
        963497464,963497672,963497880,963498088,963498296,963498504,963498712,963498920,963499128,963499336,963499544,963499752,963499960,963500168,963500376,20355,20459,20563,20667,20771,20875,20979,21083,21187,21291,21395,21499,21603,21707,21811,21915,25,92,159
    };
    mavlink_rocket_telemetry_t packet1, packet2;
        memset(&packet1, 0, sizeof(packet1));
        packet1.time_boot_ms = packet_in.time_boot_ms;
        packet1.lat = packet_in.lat;
        packet1.lon = packet_in.lon;
        packet1.gps_alt = packet_in.gps_alt;
        packet1.altitude_msl_cm = packet_in.altitude_msl_cm;
        packet1.kalman_z = packet_in.kalman_z;
        packet1.kalman_v = packet_in.kalman_v;
        packet1.imu_gyro_x = packet_in.imu_gyro_x;
        packet1.imu_gyro_y = packet_in.imu_gyro_y;
        packet1.imu_gyro_z = packet_in.imu_gyro_z;
        packet1.highg_acc_x = packet_in.highg_acc_x;
        packet1.highg_acc_y = packet_in.highg_acc_y;
        packet1.highg_acc_z = packet_in.highg_acc_z;
        packet1.highg_acc_vertical = packet_in.highg_acc_vertical;
        packet1.imu_acc_vertical = packet_in.imu_acc_vertical;
        packet1.roll = packet_in.roll;
        packet1.pitch = packet_in.pitch;
        packet1.yaw = packet_in.yaw;
        packet1.imu_acc_x = packet_in.imu_acc_x;
        packet1.imu_acc_y = packet_in.imu_acc_y;
        packet1.imu_acc_z = packet_in.imu_acc_z;
        packet1.imu_mag_x = packet_in.imu_mag_x;
        packet1.imu_mag_y = packet_in.imu_mag_y;
        packet1.imu_mag_z = packet_in.imu_mag_z;
        packet1.pressure_hpa = packet_in.pressure_hpa;
        packet1.temp_celsius = packet_in.temp_celsius;
        packet1.system_states = packet_in.system_states;
        packet1.event_states = packet_in.event_states;
        packet1.battery_mv = packet_in.battery_mv;
        packet1.vel = packet_in.vel;
        packet1.cog = packet_in.cog;
        packet1.mission_state = packet_in.mission_state;
        packet1.gps_fix = packet_in.gps_fix;
        packet1.satellites_nb = packet_in.satellites_nb;
        
        
#ifdef MAVLINK_STATUS_FLAG_OUT_MAVLINK1
        if (status->flags & MAVLINK_STATUS_FLAG_OUT_MAVLINK1) {
           // cope with extensions
           memset(MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN + (char *)&packet1, 0, sizeof(packet1)-MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN);
        }
#endif
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rocket_telemetry_encode(system_id, component_id, &msg, &packet1);
    mavlink_msg_rocket_telemetry_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rocket_telemetry_pack(system_id, component_id, &msg , packet1.time_boot_ms , packet1.lat , packet1.lon , packet1.gps_alt , packet1.altitude_msl_cm , packet1.kalman_z , packet1.kalman_v , packet1.imu_gyro_x , packet1.imu_gyro_y , packet1.imu_gyro_z , packet1.highg_acc_x , packet1.highg_acc_y , packet1.highg_acc_z , packet1.highg_acc_vertical , packet1.imu_acc_vertical , packet1.roll , packet1.pitch , packet1.yaw , packet1.imu_acc_x , packet1.imu_acc_y , packet1.imu_acc_z , packet1.imu_mag_x , packet1.imu_mag_y , packet1.imu_mag_z , packet1.pressure_hpa , packet1.temp_celsius , packet1.system_states , packet1.event_states , packet1.battery_mv , packet1.vel , packet1.cog , packet1.mission_state , packet1.gps_fix , packet1.satellites_nb );
    mavlink_msg_rocket_telemetry_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rocket_telemetry_pack_chan(system_id, component_id, MAVLINK_COMM_0, &msg , packet1.time_boot_ms , packet1.lat , packet1.lon , packet1.gps_alt , packet1.altitude_msl_cm , packet1.kalman_z , packet1.kalman_v , packet1.imu_gyro_x , packet1.imu_gyro_y , packet1.imu_gyro_z , packet1.highg_acc_x , packet1.highg_acc_y , packet1.highg_acc_z , packet1.highg_acc_vertical , packet1.imu_acc_vertical , packet1.roll , packet1.pitch , packet1.yaw , packet1.imu_acc_x , packet1.imu_acc_y , packet1.imu_acc_z , packet1.imu_mag_x , packet1.imu_mag_y , packet1.imu_mag_z , packet1.pressure_hpa , packet1.temp_celsius , packet1.system_states , packet1.event_states , packet1.battery_mv , packet1.vel , packet1.cog , packet1.mission_state , packet1.gps_fix , packet1.satellites_nb );
    mavlink_msg_rocket_telemetry_decode(&msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

        memset(&packet2, 0, sizeof(packet2));
        mavlink_msg_to_send_buffer(buffer, &msg);
        for (i=0; i<mavlink_msg_get_send_buffer_length(&msg); i++) {
            comm_send_ch(MAVLINK_COMM_0, buffer[i]);
        }
    mavlink_msg_rocket_telemetry_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);
        
        memset(&packet2, 0, sizeof(packet2));
    mavlink_msg_rocket_telemetry_send(MAVLINK_COMM_1 , packet1.time_boot_ms , packet1.lat , packet1.lon , packet1.gps_alt , packet1.altitude_msl_cm , packet1.kalman_z , packet1.kalman_v , packet1.imu_gyro_x , packet1.imu_gyro_y , packet1.imu_gyro_z , packet1.highg_acc_x , packet1.highg_acc_y , packet1.highg_acc_z , packet1.highg_acc_vertical , packet1.imu_acc_vertical , packet1.roll , packet1.pitch , packet1.yaw , packet1.imu_acc_x , packet1.imu_acc_y , packet1.imu_acc_z , packet1.imu_mag_x , packet1.imu_mag_y , packet1.imu_mag_z , packet1.pressure_hpa , packet1.temp_celsius , packet1.system_states , packet1.event_states , packet1.battery_mv , packet1.vel , packet1.cog , packet1.mission_state , packet1.gps_fix , packet1.satellites_nb );
    mavlink_msg_rocket_telemetry_decode(last_msg, &packet2);
        MAVLINK_ASSERT(memcmp(&packet1, &packet2, sizeof(packet1)) == 0);

#ifdef MAVLINK_HAVE_GET_MESSAGE_INFO
    MAVLINK_ASSERT(mavlink_get_message_info_by_name("ROCKET_TELEMETRY") != NULL);
    MAVLINK_ASSERT(mavlink_get_message_info_by_id(MAVLINK_MSG_ID_ROCKET_TELEMETRY) != NULL);
#endif
}

static void mavlink_test_odb_mavlink_v1(uint8_t system_id, uint8_t component_id, mavlink_message_t *last_msg)
{
    mavlink_test_rocket_telemetry(system_id, component_id, last_msg);
}

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // ODB_MAVLINK_V1_TESTSUITE_H
