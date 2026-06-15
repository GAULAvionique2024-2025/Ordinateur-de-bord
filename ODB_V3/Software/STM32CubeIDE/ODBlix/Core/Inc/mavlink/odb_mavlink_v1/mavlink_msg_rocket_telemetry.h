#pragma once
// MESSAGE ROCKET_TELEMETRY PACKING

#define MAVLINK_MSG_ID_ROCKET_TELEMETRY 1350


typedef struct __mavlink_rocket_telemetry_t {
 uint32_t time_boot_ms; /*< [ms] Timestamp since system boot in milliseconds.*/
 int32_t lat; /*< [degE7] Latitude in degrees * 1E7.*/
 int32_t lon; /*< [degE7] Longitude in degrees * 1E7.*/
 int32_t gps_alt; /*< [mm] Altitude (MSL) from GPS.*/
 int32_t altitude_msl_cm; /*< [cm] Altitude (MSL) from barometer.*/
 int32_t kalman_z; /*< [cm] Filtered altitude from Kalman filter.*/
 int32_t kalman_v; /*< [cm/s] Filtered velocity from Kalman filter.*/
 int32_t imu_gyro_x; /*< [cdeg/s] IMU Gyro X.*/
 int32_t imu_gyro_y; /*< [cdeg/s] IMU Gyro Y.*/
 int32_t imu_gyro_z; /*< [cdeg/s] IMU Gyro Z.*/
 int32_t highg_acc_x; /*< [cm/s/s] High-G Acceleration X.*/
 int32_t highg_acc_y; /*< [cm/s/s] High-G Acceleration Y.*/
 int32_t highg_acc_z; /*< [cm/s/s] High-G Acceleration Z.*/
 int32_t highg_acc_vertical; /*< [cm/s/s] Vertical High-G acceleration.*/
 int32_t imu_acc_vertical; /*< [cm/s/s] Vertical IMU acceleration.*/
 int16_t roll; /*< [cdeg] Roll angle in centi-degrees.*/
 int16_t pitch; /*< [cdeg] Pitch angle in centi-degrees.*/
 int16_t yaw; /*< [cdeg] Yaw angle in centi-degrees.*/
 int16_t imu_acc_x; /*< [cm/s/s] IMU Acceleration X.*/
 int16_t imu_acc_y; /*< [cm/s/s] IMU Acceleration Y.*/
 int16_t imu_acc_z; /*< [cm/s/s] IMU Acceleration Z.*/
 int16_t imu_mag_x; /*< [cuT] IMU Mag X in centi-microTesla.*/
 int16_t imu_mag_y; /*< [cuT] IMU Mag Y in centi-microTesla.*/
 int16_t imu_mag_z; /*< [cuT] IMU Mag Z in centi-microTesla.*/
 uint16_t pressure_hpa; /*< [hPa] Pressure in hectopascals.*/
 int16_t temp_celsius; /*< [cdegC] Temperature in centi-degrees Celsius.*/
 uint16_t system_states; /*<  Current system state bitfield.*/
 uint16_t event_states; /*<  Current events states.*/
 uint16_t battery_mv; /*< [mV] Battery voltage in millivolts.*/
 uint16_t vel; /*< [cm/s] Ground speed.*/
 uint16_t cog; /*< [cdeg] Course over ground.*/
 uint8_t mission_state; /*<  Mission state.*/
 uint8_t gps_fix; /*<  GPS fix type.*/
 uint8_t satellites_nb; /*<  Number of visible satellites.*/
} mavlink_rocket_telemetry_t;

#define MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN 95
#define MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN 95
#define MAVLINK_MSG_ID_1350_LEN 95
#define MAVLINK_MSG_ID_1350_MIN_LEN 95

#define MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC 94
#define MAVLINK_MSG_ID_1350_CRC 94



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_ROCKET_TELEMETRY { \
    1350, \
    "ROCKET_TELEMETRY", \
    34, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_rocket_telemetry_t, time_boot_ms) }, \
         { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_rocket_telemetry_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_rocket_telemetry_t, lon) }, \
         { "gps_alt", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_rocket_telemetry_t, gps_alt) }, \
         { "altitude_msl_cm", NULL, MAVLINK_TYPE_INT32_T, 0, 16, offsetof(mavlink_rocket_telemetry_t, altitude_msl_cm) }, \
         { "kalman_z", NULL, MAVLINK_TYPE_INT32_T, 0, 20, offsetof(mavlink_rocket_telemetry_t, kalman_z) }, \
         { "kalman_v", NULL, MAVLINK_TYPE_INT32_T, 0, 24, offsetof(mavlink_rocket_telemetry_t, kalman_v) }, \
         { "imu_gyro_x", NULL, MAVLINK_TYPE_INT32_T, 0, 28, offsetof(mavlink_rocket_telemetry_t, imu_gyro_x) }, \
         { "imu_gyro_y", NULL, MAVLINK_TYPE_INT32_T, 0, 32, offsetof(mavlink_rocket_telemetry_t, imu_gyro_y) }, \
         { "imu_gyro_z", NULL, MAVLINK_TYPE_INT32_T, 0, 36, offsetof(mavlink_rocket_telemetry_t, imu_gyro_z) }, \
         { "highg_acc_x", NULL, MAVLINK_TYPE_INT32_T, 0, 40, offsetof(mavlink_rocket_telemetry_t, highg_acc_x) }, \
         { "highg_acc_y", NULL, MAVLINK_TYPE_INT32_T, 0, 44, offsetof(mavlink_rocket_telemetry_t, highg_acc_y) }, \
         { "highg_acc_z", NULL, MAVLINK_TYPE_INT32_T, 0, 48, offsetof(mavlink_rocket_telemetry_t, highg_acc_z) }, \
         { "highg_acc_vertical", NULL, MAVLINK_TYPE_INT32_T, 0, 52, offsetof(mavlink_rocket_telemetry_t, highg_acc_vertical) }, \
         { "imu_acc_vertical", NULL, MAVLINK_TYPE_INT32_T, 0, 56, offsetof(mavlink_rocket_telemetry_t, imu_acc_vertical) }, \
         { "roll", NULL, MAVLINK_TYPE_INT16_T, 0, 60, offsetof(mavlink_rocket_telemetry_t, roll) }, \
         { "pitch", NULL, MAVLINK_TYPE_INT16_T, 0, 62, offsetof(mavlink_rocket_telemetry_t, pitch) }, \
         { "yaw", NULL, MAVLINK_TYPE_INT16_T, 0, 64, offsetof(mavlink_rocket_telemetry_t, yaw) }, \
         { "imu_acc_x", NULL, MAVLINK_TYPE_INT16_T, 0, 66, offsetof(mavlink_rocket_telemetry_t, imu_acc_x) }, \
         { "imu_acc_y", NULL, MAVLINK_TYPE_INT16_T, 0, 68, offsetof(mavlink_rocket_telemetry_t, imu_acc_y) }, \
         { "imu_acc_z", NULL, MAVLINK_TYPE_INT16_T, 0, 70, offsetof(mavlink_rocket_telemetry_t, imu_acc_z) }, \
         { "imu_mag_x", NULL, MAVLINK_TYPE_INT16_T, 0, 72, offsetof(mavlink_rocket_telemetry_t, imu_mag_x) }, \
         { "imu_mag_y", NULL, MAVLINK_TYPE_INT16_T, 0, 74, offsetof(mavlink_rocket_telemetry_t, imu_mag_y) }, \
         { "imu_mag_z", NULL, MAVLINK_TYPE_INT16_T, 0, 76, offsetof(mavlink_rocket_telemetry_t, imu_mag_z) }, \
         { "pressure_hpa", NULL, MAVLINK_TYPE_UINT16_T, 0, 78, offsetof(mavlink_rocket_telemetry_t, pressure_hpa) }, \
         { "temp_celsius", NULL, MAVLINK_TYPE_INT16_T, 0, 80, offsetof(mavlink_rocket_telemetry_t, temp_celsius) }, \
         { "system_states", NULL, MAVLINK_TYPE_UINT16_T, 0, 82, offsetof(mavlink_rocket_telemetry_t, system_states) }, \
         { "event_states", NULL, MAVLINK_TYPE_UINT16_T, 0, 84, offsetof(mavlink_rocket_telemetry_t, event_states) }, \
         { "battery_mv", NULL, MAVLINK_TYPE_UINT16_T, 0, 86, offsetof(mavlink_rocket_telemetry_t, battery_mv) }, \
         { "vel", NULL, MAVLINK_TYPE_UINT16_T, 0, 88, offsetof(mavlink_rocket_telemetry_t, vel) }, \
         { "cog", NULL, MAVLINK_TYPE_UINT16_T, 0, 90, offsetof(mavlink_rocket_telemetry_t, cog) }, \
         { "mission_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 92, offsetof(mavlink_rocket_telemetry_t, mission_state) }, \
         { "gps_fix", NULL, MAVLINK_TYPE_UINT8_T, 0, 93, offsetof(mavlink_rocket_telemetry_t, gps_fix) }, \
         { "satellites_nb", NULL, MAVLINK_TYPE_UINT8_T, 0, 94, offsetof(mavlink_rocket_telemetry_t, satellites_nb) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_ROCKET_TELEMETRY { \
    "ROCKET_TELEMETRY", \
    34, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_rocket_telemetry_t, time_boot_ms) }, \
         { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_rocket_telemetry_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_rocket_telemetry_t, lon) }, \
         { "gps_alt", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_rocket_telemetry_t, gps_alt) }, \
         { "altitude_msl_cm", NULL, MAVLINK_TYPE_INT32_T, 0, 16, offsetof(mavlink_rocket_telemetry_t, altitude_msl_cm) }, \
         { "kalman_z", NULL, MAVLINK_TYPE_INT32_T, 0, 20, offsetof(mavlink_rocket_telemetry_t, kalman_z) }, \
         { "kalman_v", NULL, MAVLINK_TYPE_INT32_T, 0, 24, offsetof(mavlink_rocket_telemetry_t, kalman_v) }, \
         { "imu_gyro_x", NULL, MAVLINK_TYPE_INT32_T, 0, 28, offsetof(mavlink_rocket_telemetry_t, imu_gyro_x) }, \
         { "imu_gyro_y", NULL, MAVLINK_TYPE_INT32_T, 0, 32, offsetof(mavlink_rocket_telemetry_t, imu_gyro_y) }, \
         { "imu_gyro_z", NULL, MAVLINK_TYPE_INT32_T, 0, 36, offsetof(mavlink_rocket_telemetry_t, imu_gyro_z) }, \
         { "highg_acc_x", NULL, MAVLINK_TYPE_INT32_T, 0, 40, offsetof(mavlink_rocket_telemetry_t, highg_acc_x) }, \
         { "highg_acc_y", NULL, MAVLINK_TYPE_INT32_T, 0, 44, offsetof(mavlink_rocket_telemetry_t, highg_acc_y) }, \
         { "highg_acc_z", NULL, MAVLINK_TYPE_INT32_T, 0, 48, offsetof(mavlink_rocket_telemetry_t, highg_acc_z) }, \
         { "highg_acc_vertical", NULL, MAVLINK_TYPE_INT32_T, 0, 52, offsetof(mavlink_rocket_telemetry_t, highg_acc_vertical) }, \
         { "imu_acc_vertical", NULL, MAVLINK_TYPE_INT32_T, 0, 56, offsetof(mavlink_rocket_telemetry_t, imu_acc_vertical) }, \
         { "roll", NULL, MAVLINK_TYPE_INT16_T, 0, 60, offsetof(mavlink_rocket_telemetry_t, roll) }, \
         { "pitch", NULL, MAVLINK_TYPE_INT16_T, 0, 62, offsetof(mavlink_rocket_telemetry_t, pitch) }, \
         { "yaw", NULL, MAVLINK_TYPE_INT16_T, 0, 64, offsetof(mavlink_rocket_telemetry_t, yaw) }, \
         { "imu_acc_x", NULL, MAVLINK_TYPE_INT16_T, 0, 66, offsetof(mavlink_rocket_telemetry_t, imu_acc_x) }, \
         { "imu_acc_y", NULL, MAVLINK_TYPE_INT16_T, 0, 68, offsetof(mavlink_rocket_telemetry_t, imu_acc_y) }, \
         { "imu_acc_z", NULL, MAVLINK_TYPE_INT16_T, 0, 70, offsetof(mavlink_rocket_telemetry_t, imu_acc_z) }, \
         { "imu_mag_x", NULL, MAVLINK_TYPE_INT16_T, 0, 72, offsetof(mavlink_rocket_telemetry_t, imu_mag_x) }, \
         { "imu_mag_y", NULL, MAVLINK_TYPE_INT16_T, 0, 74, offsetof(mavlink_rocket_telemetry_t, imu_mag_y) }, \
         { "imu_mag_z", NULL, MAVLINK_TYPE_INT16_T, 0, 76, offsetof(mavlink_rocket_telemetry_t, imu_mag_z) }, \
         { "pressure_hpa", NULL, MAVLINK_TYPE_UINT16_T, 0, 78, offsetof(mavlink_rocket_telemetry_t, pressure_hpa) }, \
         { "temp_celsius", NULL, MAVLINK_TYPE_INT16_T, 0, 80, offsetof(mavlink_rocket_telemetry_t, temp_celsius) }, \
         { "system_states", NULL, MAVLINK_TYPE_UINT16_T, 0, 82, offsetof(mavlink_rocket_telemetry_t, system_states) }, \
         { "event_states", NULL, MAVLINK_TYPE_UINT16_T, 0, 84, offsetof(mavlink_rocket_telemetry_t, event_states) }, \
         { "battery_mv", NULL, MAVLINK_TYPE_UINT16_T, 0, 86, offsetof(mavlink_rocket_telemetry_t, battery_mv) }, \
         { "vel", NULL, MAVLINK_TYPE_UINT16_T, 0, 88, offsetof(mavlink_rocket_telemetry_t, vel) }, \
         { "cog", NULL, MAVLINK_TYPE_UINT16_T, 0, 90, offsetof(mavlink_rocket_telemetry_t, cog) }, \
         { "mission_state", NULL, MAVLINK_TYPE_UINT8_T, 0, 92, offsetof(mavlink_rocket_telemetry_t, mission_state) }, \
         { "gps_fix", NULL, MAVLINK_TYPE_UINT8_T, 0, 93, offsetof(mavlink_rocket_telemetry_t, gps_fix) }, \
         { "satellites_nb", NULL, MAVLINK_TYPE_UINT8_T, 0, 94, offsetof(mavlink_rocket_telemetry_t, satellites_nb) }, \
         } \
}
#endif

/**
 * @brief Pack a rocket_telemetry message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp since system boot in milliseconds.
 * @param lat [degE7] Latitude in degrees * 1E7.
 * @param lon [degE7] Longitude in degrees * 1E7.
 * @param gps_alt [mm] Altitude (MSL) from GPS.
 * @param altitude_msl_cm [cm] Altitude (MSL) from barometer.
 * @param kalman_z [cm] Filtered altitude from Kalman filter.
 * @param kalman_v [cm/s] Filtered velocity from Kalman filter.
 * @param imu_gyro_x [cdeg/s] IMU Gyro X.
 * @param imu_gyro_y [cdeg/s] IMU Gyro Y.
 * @param imu_gyro_z [cdeg/s] IMU Gyro Z.
 * @param highg_acc_x [cm/s/s] High-G Acceleration X.
 * @param highg_acc_y [cm/s/s] High-G Acceleration Y.
 * @param highg_acc_z [cm/s/s] High-G Acceleration Z.
 * @param highg_acc_vertical [cm/s/s] Vertical High-G acceleration.
 * @param imu_acc_vertical [cm/s/s] Vertical IMU acceleration.
 * @param roll [cdeg] Roll angle in centi-degrees.
 * @param pitch [cdeg] Pitch angle in centi-degrees.
 * @param yaw [cdeg] Yaw angle in centi-degrees.
 * @param imu_acc_x [cm/s/s] IMU Acceleration X.
 * @param imu_acc_y [cm/s/s] IMU Acceleration Y.
 * @param imu_acc_z [cm/s/s] IMU Acceleration Z.
 * @param imu_mag_x [cuT] IMU Mag X in centi-microTesla.
 * @param imu_mag_y [cuT] IMU Mag Y in centi-microTesla.
 * @param imu_mag_z [cuT] IMU Mag Z in centi-microTesla.
 * @param pressure_hpa [hPa] Pressure in hectopascals.
 * @param temp_celsius [cdegC] Temperature in centi-degrees Celsius.
 * @param system_states  Current system state bitfield.
 * @param event_states  Current events states.
 * @param battery_mv [mV] Battery voltage in millivolts.
 * @param vel [cm/s] Ground speed.
 * @param cog [cdeg] Course over ground.
 * @param mission_state  Mission state.
 * @param gps_fix  GPS fix type.
 * @param satellites_nb  Number of visible satellites.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rocket_telemetry_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, int32_t lat, int32_t lon, int32_t gps_alt, int32_t altitude_msl_cm, int32_t kalman_z, int32_t kalman_v, int32_t imu_gyro_x, int32_t imu_gyro_y, int32_t imu_gyro_z, int32_t highg_acc_x, int32_t highg_acc_y, int32_t highg_acc_z, int32_t highg_acc_vertical, int32_t imu_acc_vertical, int16_t roll, int16_t pitch, int16_t yaw, int16_t imu_acc_x, int16_t imu_acc_y, int16_t imu_acc_z, int16_t imu_mag_x, int16_t imu_mag_y, int16_t imu_mag_z, uint16_t pressure_hpa, int16_t temp_celsius, uint16_t system_states, uint16_t event_states, uint16_t battery_mv, uint16_t vel, uint16_t cog, uint8_t mission_state, uint8_t gps_fix, uint8_t satellites_nb)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_int32_t(buf, 12, gps_alt);
    _mav_put_int32_t(buf, 16, altitude_msl_cm);
    _mav_put_int32_t(buf, 20, kalman_z);
    _mav_put_int32_t(buf, 24, kalman_v);
    _mav_put_int32_t(buf, 28, imu_gyro_x);
    _mav_put_int32_t(buf, 32, imu_gyro_y);
    _mav_put_int32_t(buf, 36, imu_gyro_z);
    _mav_put_int32_t(buf, 40, highg_acc_x);
    _mav_put_int32_t(buf, 44, highg_acc_y);
    _mav_put_int32_t(buf, 48, highg_acc_z);
    _mav_put_int32_t(buf, 52, highg_acc_vertical);
    _mav_put_int32_t(buf, 56, imu_acc_vertical);
    _mav_put_int16_t(buf, 60, roll);
    _mav_put_int16_t(buf, 62, pitch);
    _mav_put_int16_t(buf, 64, yaw);
    _mav_put_int16_t(buf, 66, imu_acc_x);
    _mav_put_int16_t(buf, 68, imu_acc_y);
    _mav_put_int16_t(buf, 70, imu_acc_z);
    _mav_put_int16_t(buf, 72, imu_mag_x);
    _mav_put_int16_t(buf, 74, imu_mag_y);
    _mav_put_int16_t(buf, 76, imu_mag_z);
    _mav_put_uint16_t(buf, 78, pressure_hpa);
    _mav_put_int16_t(buf, 80, temp_celsius);
    _mav_put_uint16_t(buf, 82, system_states);
    _mav_put_uint16_t(buf, 84, event_states);
    _mav_put_uint16_t(buf, 86, battery_mv);
    _mav_put_uint16_t(buf, 88, vel);
    _mav_put_uint16_t(buf, 90, cog);
    _mav_put_uint8_t(buf, 92, mission_state);
    _mav_put_uint8_t(buf, 93, gps_fix);
    _mav_put_uint8_t(buf, 94, satellites_nb);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
#else
    mavlink_rocket_telemetry_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.gps_alt = gps_alt;
    packet.altitude_msl_cm = altitude_msl_cm;
    packet.kalman_z = kalman_z;
    packet.kalman_v = kalman_v;
    packet.imu_gyro_x = imu_gyro_x;
    packet.imu_gyro_y = imu_gyro_y;
    packet.imu_gyro_z = imu_gyro_z;
    packet.highg_acc_x = highg_acc_x;
    packet.highg_acc_y = highg_acc_y;
    packet.highg_acc_z = highg_acc_z;
    packet.highg_acc_vertical = highg_acc_vertical;
    packet.imu_acc_vertical = imu_acc_vertical;
    packet.roll = roll;
    packet.pitch = pitch;
    packet.yaw = yaw;
    packet.imu_acc_x = imu_acc_x;
    packet.imu_acc_y = imu_acc_y;
    packet.imu_acc_z = imu_acc_z;
    packet.imu_mag_x = imu_mag_x;
    packet.imu_mag_y = imu_mag_y;
    packet.imu_mag_z = imu_mag_z;
    packet.pressure_hpa = pressure_hpa;
    packet.temp_celsius = temp_celsius;
    packet.system_states = system_states;
    packet.event_states = event_states;
    packet.battery_mv = battery_mv;
    packet.vel = vel;
    packet.cog = cog;
    packet.mission_state = mission_state;
    packet.gps_fix = gps_fix;
    packet.satellites_nb = satellites_nb;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ROCKET_TELEMETRY;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC);
}

/**
 * @brief Pack a rocket_telemetry message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms [ms] Timestamp since system boot in milliseconds.
 * @param lat [degE7] Latitude in degrees * 1E7.
 * @param lon [degE7] Longitude in degrees * 1E7.
 * @param gps_alt [mm] Altitude (MSL) from GPS.
 * @param altitude_msl_cm [cm] Altitude (MSL) from barometer.
 * @param kalman_z [cm] Filtered altitude from Kalman filter.
 * @param kalman_v [cm/s] Filtered velocity from Kalman filter.
 * @param imu_gyro_x [cdeg/s] IMU Gyro X.
 * @param imu_gyro_y [cdeg/s] IMU Gyro Y.
 * @param imu_gyro_z [cdeg/s] IMU Gyro Z.
 * @param highg_acc_x [cm/s/s] High-G Acceleration X.
 * @param highg_acc_y [cm/s/s] High-G Acceleration Y.
 * @param highg_acc_z [cm/s/s] High-G Acceleration Z.
 * @param highg_acc_vertical [cm/s/s] Vertical High-G acceleration.
 * @param imu_acc_vertical [cm/s/s] Vertical IMU acceleration.
 * @param roll [cdeg] Roll angle in centi-degrees.
 * @param pitch [cdeg] Pitch angle in centi-degrees.
 * @param yaw [cdeg] Yaw angle in centi-degrees.
 * @param imu_acc_x [cm/s/s] IMU Acceleration X.
 * @param imu_acc_y [cm/s/s] IMU Acceleration Y.
 * @param imu_acc_z [cm/s/s] IMU Acceleration Z.
 * @param imu_mag_x [cuT] IMU Mag X in centi-microTesla.
 * @param imu_mag_y [cuT] IMU Mag Y in centi-microTesla.
 * @param imu_mag_z [cuT] IMU Mag Z in centi-microTesla.
 * @param pressure_hpa [hPa] Pressure in hectopascals.
 * @param temp_celsius [cdegC] Temperature in centi-degrees Celsius.
 * @param system_states  Current system state bitfield.
 * @param event_states  Current events states.
 * @param battery_mv [mV] Battery voltage in millivolts.
 * @param vel [cm/s] Ground speed.
 * @param cog [cdeg] Course over ground.
 * @param mission_state  Mission state.
 * @param gps_fix  GPS fix type.
 * @param satellites_nb  Number of visible satellites.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rocket_telemetry_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_boot_ms, int32_t lat, int32_t lon, int32_t gps_alt, int32_t altitude_msl_cm, int32_t kalman_z, int32_t kalman_v, int32_t imu_gyro_x, int32_t imu_gyro_y, int32_t imu_gyro_z, int32_t highg_acc_x, int32_t highg_acc_y, int32_t highg_acc_z, int32_t highg_acc_vertical, int32_t imu_acc_vertical, int16_t roll, int16_t pitch, int16_t yaw, int16_t imu_acc_x, int16_t imu_acc_y, int16_t imu_acc_z, int16_t imu_mag_x, int16_t imu_mag_y, int16_t imu_mag_z, uint16_t pressure_hpa, int16_t temp_celsius, uint16_t system_states, uint16_t event_states, uint16_t battery_mv, uint16_t vel, uint16_t cog, uint8_t mission_state, uint8_t gps_fix, uint8_t satellites_nb)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_int32_t(buf, 12, gps_alt);
    _mav_put_int32_t(buf, 16, altitude_msl_cm);
    _mav_put_int32_t(buf, 20, kalman_z);
    _mav_put_int32_t(buf, 24, kalman_v);
    _mav_put_int32_t(buf, 28, imu_gyro_x);
    _mav_put_int32_t(buf, 32, imu_gyro_y);
    _mav_put_int32_t(buf, 36, imu_gyro_z);
    _mav_put_int32_t(buf, 40, highg_acc_x);
    _mav_put_int32_t(buf, 44, highg_acc_y);
    _mav_put_int32_t(buf, 48, highg_acc_z);
    _mav_put_int32_t(buf, 52, highg_acc_vertical);
    _mav_put_int32_t(buf, 56, imu_acc_vertical);
    _mav_put_int16_t(buf, 60, roll);
    _mav_put_int16_t(buf, 62, pitch);
    _mav_put_int16_t(buf, 64, yaw);
    _mav_put_int16_t(buf, 66, imu_acc_x);
    _mav_put_int16_t(buf, 68, imu_acc_y);
    _mav_put_int16_t(buf, 70, imu_acc_z);
    _mav_put_int16_t(buf, 72, imu_mag_x);
    _mav_put_int16_t(buf, 74, imu_mag_y);
    _mav_put_int16_t(buf, 76, imu_mag_z);
    _mav_put_uint16_t(buf, 78, pressure_hpa);
    _mav_put_int16_t(buf, 80, temp_celsius);
    _mav_put_uint16_t(buf, 82, system_states);
    _mav_put_uint16_t(buf, 84, event_states);
    _mav_put_uint16_t(buf, 86, battery_mv);
    _mav_put_uint16_t(buf, 88, vel);
    _mav_put_uint16_t(buf, 90, cog);
    _mav_put_uint8_t(buf, 92, mission_state);
    _mav_put_uint8_t(buf, 93, gps_fix);
    _mav_put_uint8_t(buf, 94, satellites_nb);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
#else
    mavlink_rocket_telemetry_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.gps_alt = gps_alt;
    packet.altitude_msl_cm = altitude_msl_cm;
    packet.kalman_z = kalman_z;
    packet.kalman_v = kalman_v;
    packet.imu_gyro_x = imu_gyro_x;
    packet.imu_gyro_y = imu_gyro_y;
    packet.imu_gyro_z = imu_gyro_z;
    packet.highg_acc_x = highg_acc_x;
    packet.highg_acc_y = highg_acc_y;
    packet.highg_acc_z = highg_acc_z;
    packet.highg_acc_vertical = highg_acc_vertical;
    packet.imu_acc_vertical = imu_acc_vertical;
    packet.roll = roll;
    packet.pitch = pitch;
    packet.yaw = yaw;
    packet.imu_acc_x = imu_acc_x;
    packet.imu_acc_y = imu_acc_y;
    packet.imu_acc_z = imu_acc_z;
    packet.imu_mag_x = imu_mag_x;
    packet.imu_mag_y = imu_mag_y;
    packet.imu_mag_z = imu_mag_z;
    packet.pressure_hpa = pressure_hpa;
    packet.temp_celsius = temp_celsius;
    packet.system_states = system_states;
    packet.event_states = event_states;
    packet.battery_mv = battery_mv;
    packet.vel = vel;
    packet.cog = cog;
    packet.mission_state = mission_state;
    packet.gps_fix = gps_fix;
    packet.satellites_nb = satellites_nb;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ROCKET_TELEMETRY;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
#endif
}

/**
 * @brief Pack a rocket_telemetry message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param time_boot_ms [ms] Timestamp since system boot in milliseconds.
 * @param lat [degE7] Latitude in degrees * 1E7.
 * @param lon [degE7] Longitude in degrees * 1E7.
 * @param gps_alt [mm] Altitude (MSL) from GPS.
 * @param altitude_msl_cm [cm] Altitude (MSL) from barometer.
 * @param kalman_z [cm] Filtered altitude from Kalman filter.
 * @param kalman_v [cm/s] Filtered velocity from Kalman filter.
 * @param imu_gyro_x [cdeg/s] IMU Gyro X.
 * @param imu_gyro_y [cdeg/s] IMU Gyro Y.
 * @param imu_gyro_z [cdeg/s] IMU Gyro Z.
 * @param highg_acc_x [cm/s/s] High-G Acceleration X.
 * @param highg_acc_y [cm/s/s] High-G Acceleration Y.
 * @param highg_acc_z [cm/s/s] High-G Acceleration Z.
 * @param highg_acc_vertical [cm/s/s] Vertical High-G acceleration.
 * @param imu_acc_vertical [cm/s/s] Vertical IMU acceleration.
 * @param roll [cdeg] Roll angle in centi-degrees.
 * @param pitch [cdeg] Pitch angle in centi-degrees.
 * @param yaw [cdeg] Yaw angle in centi-degrees.
 * @param imu_acc_x [cm/s/s] IMU Acceleration X.
 * @param imu_acc_y [cm/s/s] IMU Acceleration Y.
 * @param imu_acc_z [cm/s/s] IMU Acceleration Z.
 * @param imu_mag_x [cuT] IMU Mag X in centi-microTesla.
 * @param imu_mag_y [cuT] IMU Mag Y in centi-microTesla.
 * @param imu_mag_z [cuT] IMU Mag Z in centi-microTesla.
 * @param pressure_hpa [hPa] Pressure in hectopascals.
 * @param temp_celsius [cdegC] Temperature in centi-degrees Celsius.
 * @param system_states  Current system state bitfield.
 * @param event_states  Current events states.
 * @param battery_mv [mV] Battery voltage in millivolts.
 * @param vel [cm/s] Ground speed.
 * @param cog [cdeg] Course over ground.
 * @param mission_state  Mission state.
 * @param gps_fix  GPS fix type.
 * @param satellites_nb  Number of visible satellites.
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rocket_telemetry_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,int32_t lat,int32_t lon,int32_t gps_alt,int32_t altitude_msl_cm,int32_t kalman_z,int32_t kalman_v,int32_t imu_gyro_x,int32_t imu_gyro_y,int32_t imu_gyro_z,int32_t highg_acc_x,int32_t highg_acc_y,int32_t highg_acc_z,int32_t highg_acc_vertical,int32_t imu_acc_vertical,int16_t roll,int16_t pitch,int16_t yaw,int16_t imu_acc_x,int16_t imu_acc_y,int16_t imu_acc_z,int16_t imu_mag_x,int16_t imu_mag_y,int16_t imu_mag_z,uint16_t pressure_hpa,int16_t temp_celsius,uint16_t system_states,uint16_t event_states,uint16_t battery_mv,uint16_t vel,uint16_t cog,uint8_t mission_state,uint8_t gps_fix,uint8_t satellites_nb)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_int32_t(buf, 12, gps_alt);
    _mav_put_int32_t(buf, 16, altitude_msl_cm);
    _mav_put_int32_t(buf, 20, kalman_z);
    _mav_put_int32_t(buf, 24, kalman_v);
    _mav_put_int32_t(buf, 28, imu_gyro_x);
    _mav_put_int32_t(buf, 32, imu_gyro_y);
    _mav_put_int32_t(buf, 36, imu_gyro_z);
    _mav_put_int32_t(buf, 40, highg_acc_x);
    _mav_put_int32_t(buf, 44, highg_acc_y);
    _mav_put_int32_t(buf, 48, highg_acc_z);
    _mav_put_int32_t(buf, 52, highg_acc_vertical);
    _mav_put_int32_t(buf, 56, imu_acc_vertical);
    _mav_put_int16_t(buf, 60, roll);
    _mav_put_int16_t(buf, 62, pitch);
    _mav_put_int16_t(buf, 64, yaw);
    _mav_put_int16_t(buf, 66, imu_acc_x);
    _mav_put_int16_t(buf, 68, imu_acc_y);
    _mav_put_int16_t(buf, 70, imu_acc_z);
    _mav_put_int16_t(buf, 72, imu_mag_x);
    _mav_put_int16_t(buf, 74, imu_mag_y);
    _mav_put_int16_t(buf, 76, imu_mag_z);
    _mav_put_uint16_t(buf, 78, pressure_hpa);
    _mav_put_int16_t(buf, 80, temp_celsius);
    _mav_put_uint16_t(buf, 82, system_states);
    _mav_put_uint16_t(buf, 84, event_states);
    _mav_put_uint16_t(buf, 86, battery_mv);
    _mav_put_uint16_t(buf, 88, vel);
    _mav_put_uint16_t(buf, 90, cog);
    _mav_put_uint8_t(buf, 92, mission_state);
    _mav_put_uint8_t(buf, 93, gps_fix);
    _mav_put_uint8_t(buf, 94, satellites_nb);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
#else
    mavlink_rocket_telemetry_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.gps_alt = gps_alt;
    packet.altitude_msl_cm = altitude_msl_cm;
    packet.kalman_z = kalman_z;
    packet.kalman_v = kalman_v;
    packet.imu_gyro_x = imu_gyro_x;
    packet.imu_gyro_y = imu_gyro_y;
    packet.imu_gyro_z = imu_gyro_z;
    packet.highg_acc_x = highg_acc_x;
    packet.highg_acc_y = highg_acc_y;
    packet.highg_acc_z = highg_acc_z;
    packet.highg_acc_vertical = highg_acc_vertical;
    packet.imu_acc_vertical = imu_acc_vertical;
    packet.roll = roll;
    packet.pitch = pitch;
    packet.yaw = yaw;
    packet.imu_acc_x = imu_acc_x;
    packet.imu_acc_y = imu_acc_y;
    packet.imu_acc_z = imu_acc_z;
    packet.imu_mag_x = imu_mag_x;
    packet.imu_mag_y = imu_mag_y;
    packet.imu_mag_z = imu_mag_z;
    packet.pressure_hpa = pressure_hpa;
    packet.temp_celsius = temp_celsius;
    packet.system_states = system_states;
    packet.event_states = event_states;
    packet.battery_mv = battery_mv;
    packet.vel = vel;
    packet.cog = cog;
    packet.mission_state = mission_state;
    packet.gps_fix = gps_fix;
    packet.satellites_nb = satellites_nb;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_ROCKET_TELEMETRY;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC);
}

/**
 * @brief Encode a rocket_telemetry struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param rocket_telemetry C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_rocket_telemetry_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_rocket_telemetry_t* rocket_telemetry)
{
    return mavlink_msg_rocket_telemetry_pack(system_id, component_id, msg, rocket_telemetry->time_boot_ms, rocket_telemetry->lat, rocket_telemetry->lon, rocket_telemetry->gps_alt, rocket_telemetry->altitude_msl_cm, rocket_telemetry->kalman_z, rocket_telemetry->kalman_v, rocket_telemetry->imu_gyro_x, rocket_telemetry->imu_gyro_y, rocket_telemetry->imu_gyro_z, rocket_telemetry->highg_acc_x, rocket_telemetry->highg_acc_y, rocket_telemetry->highg_acc_z, rocket_telemetry->highg_acc_vertical, rocket_telemetry->imu_acc_vertical, rocket_telemetry->roll, rocket_telemetry->pitch, rocket_telemetry->yaw, rocket_telemetry->imu_acc_x, rocket_telemetry->imu_acc_y, rocket_telemetry->imu_acc_z, rocket_telemetry->imu_mag_x, rocket_telemetry->imu_mag_y, rocket_telemetry->imu_mag_z, rocket_telemetry->pressure_hpa, rocket_telemetry->temp_celsius, rocket_telemetry->system_states, rocket_telemetry->event_states, rocket_telemetry->battery_mv, rocket_telemetry->vel, rocket_telemetry->cog, rocket_telemetry->mission_state, rocket_telemetry->gps_fix, rocket_telemetry->satellites_nb);
}

/**
 * @brief Encode a rocket_telemetry struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param rocket_telemetry C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_rocket_telemetry_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_rocket_telemetry_t* rocket_telemetry)
{
    return mavlink_msg_rocket_telemetry_pack_chan(system_id, component_id, chan, msg, rocket_telemetry->time_boot_ms, rocket_telemetry->lat, rocket_telemetry->lon, rocket_telemetry->gps_alt, rocket_telemetry->altitude_msl_cm, rocket_telemetry->kalman_z, rocket_telemetry->kalman_v, rocket_telemetry->imu_gyro_x, rocket_telemetry->imu_gyro_y, rocket_telemetry->imu_gyro_z, rocket_telemetry->highg_acc_x, rocket_telemetry->highg_acc_y, rocket_telemetry->highg_acc_z, rocket_telemetry->highg_acc_vertical, rocket_telemetry->imu_acc_vertical, rocket_telemetry->roll, rocket_telemetry->pitch, rocket_telemetry->yaw, rocket_telemetry->imu_acc_x, rocket_telemetry->imu_acc_y, rocket_telemetry->imu_acc_z, rocket_telemetry->imu_mag_x, rocket_telemetry->imu_mag_y, rocket_telemetry->imu_mag_z, rocket_telemetry->pressure_hpa, rocket_telemetry->temp_celsius, rocket_telemetry->system_states, rocket_telemetry->event_states, rocket_telemetry->battery_mv, rocket_telemetry->vel, rocket_telemetry->cog, rocket_telemetry->mission_state, rocket_telemetry->gps_fix, rocket_telemetry->satellites_nb);
}

/**
 * @brief Encode a rocket_telemetry struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param rocket_telemetry C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_rocket_telemetry_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_rocket_telemetry_t* rocket_telemetry)
{
    return mavlink_msg_rocket_telemetry_pack_status(system_id, component_id, _status, msg,  rocket_telemetry->time_boot_ms, rocket_telemetry->lat, rocket_telemetry->lon, rocket_telemetry->gps_alt, rocket_telemetry->altitude_msl_cm, rocket_telemetry->kalman_z, rocket_telemetry->kalman_v, rocket_telemetry->imu_gyro_x, rocket_telemetry->imu_gyro_y, rocket_telemetry->imu_gyro_z, rocket_telemetry->highg_acc_x, rocket_telemetry->highg_acc_y, rocket_telemetry->highg_acc_z, rocket_telemetry->highg_acc_vertical, rocket_telemetry->imu_acc_vertical, rocket_telemetry->roll, rocket_telemetry->pitch, rocket_telemetry->yaw, rocket_telemetry->imu_acc_x, rocket_telemetry->imu_acc_y, rocket_telemetry->imu_acc_z, rocket_telemetry->imu_mag_x, rocket_telemetry->imu_mag_y, rocket_telemetry->imu_mag_z, rocket_telemetry->pressure_hpa, rocket_telemetry->temp_celsius, rocket_telemetry->system_states, rocket_telemetry->event_states, rocket_telemetry->battery_mv, rocket_telemetry->vel, rocket_telemetry->cog, rocket_telemetry->mission_state, rocket_telemetry->gps_fix, rocket_telemetry->satellites_nb);
}

/**
 * @brief Send a rocket_telemetry message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms [ms] Timestamp since system boot in milliseconds.
 * @param lat [degE7] Latitude in degrees * 1E7.
 * @param lon [degE7] Longitude in degrees * 1E7.
 * @param gps_alt [mm] Altitude (MSL) from GPS.
 * @param altitude_msl_cm [cm] Altitude (MSL) from barometer.
 * @param kalman_z [cm] Filtered altitude from Kalman filter.
 * @param kalman_v [cm/s] Filtered velocity from Kalman filter.
 * @param imu_gyro_x [cdeg/s] IMU Gyro X.
 * @param imu_gyro_y [cdeg/s] IMU Gyro Y.
 * @param imu_gyro_z [cdeg/s] IMU Gyro Z.
 * @param highg_acc_x [cm/s/s] High-G Acceleration X.
 * @param highg_acc_y [cm/s/s] High-G Acceleration Y.
 * @param highg_acc_z [cm/s/s] High-G Acceleration Z.
 * @param highg_acc_vertical [cm/s/s] Vertical High-G acceleration.
 * @param imu_acc_vertical [cm/s/s] Vertical IMU acceleration.
 * @param roll [cdeg] Roll angle in centi-degrees.
 * @param pitch [cdeg] Pitch angle in centi-degrees.
 * @param yaw [cdeg] Yaw angle in centi-degrees.
 * @param imu_acc_x [cm/s/s] IMU Acceleration X.
 * @param imu_acc_y [cm/s/s] IMU Acceleration Y.
 * @param imu_acc_z [cm/s/s] IMU Acceleration Z.
 * @param imu_mag_x [cuT] IMU Mag X in centi-microTesla.
 * @param imu_mag_y [cuT] IMU Mag Y in centi-microTesla.
 * @param imu_mag_z [cuT] IMU Mag Z in centi-microTesla.
 * @param pressure_hpa [hPa] Pressure in hectopascals.
 * @param temp_celsius [cdegC] Temperature in centi-degrees Celsius.
 * @param system_states  Current system state bitfield.
 * @param event_states  Current events states.
 * @param battery_mv [mV] Battery voltage in millivolts.
 * @param vel [cm/s] Ground speed.
 * @param cog [cdeg] Course over ground.
 * @param mission_state  Mission state.
 * @param gps_fix  GPS fix type.
 * @param satellites_nb  Number of visible satellites.
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_rocket_telemetry_send(mavlink_channel_t chan, uint32_t time_boot_ms, int32_t lat, int32_t lon, int32_t gps_alt, int32_t altitude_msl_cm, int32_t kalman_z, int32_t kalman_v, int32_t imu_gyro_x, int32_t imu_gyro_y, int32_t imu_gyro_z, int32_t highg_acc_x, int32_t highg_acc_y, int32_t highg_acc_z, int32_t highg_acc_vertical, int32_t imu_acc_vertical, int16_t roll, int16_t pitch, int16_t yaw, int16_t imu_acc_x, int16_t imu_acc_y, int16_t imu_acc_z, int16_t imu_mag_x, int16_t imu_mag_y, int16_t imu_mag_z, uint16_t pressure_hpa, int16_t temp_celsius, uint16_t system_states, uint16_t event_states, uint16_t battery_mv, uint16_t vel, uint16_t cog, uint8_t mission_state, uint8_t gps_fix, uint8_t satellites_nb)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_int32_t(buf, 12, gps_alt);
    _mav_put_int32_t(buf, 16, altitude_msl_cm);
    _mav_put_int32_t(buf, 20, kalman_z);
    _mav_put_int32_t(buf, 24, kalman_v);
    _mav_put_int32_t(buf, 28, imu_gyro_x);
    _mav_put_int32_t(buf, 32, imu_gyro_y);
    _mav_put_int32_t(buf, 36, imu_gyro_z);
    _mav_put_int32_t(buf, 40, highg_acc_x);
    _mav_put_int32_t(buf, 44, highg_acc_y);
    _mav_put_int32_t(buf, 48, highg_acc_z);
    _mav_put_int32_t(buf, 52, highg_acc_vertical);
    _mav_put_int32_t(buf, 56, imu_acc_vertical);
    _mav_put_int16_t(buf, 60, roll);
    _mav_put_int16_t(buf, 62, pitch);
    _mav_put_int16_t(buf, 64, yaw);
    _mav_put_int16_t(buf, 66, imu_acc_x);
    _mav_put_int16_t(buf, 68, imu_acc_y);
    _mav_put_int16_t(buf, 70, imu_acc_z);
    _mav_put_int16_t(buf, 72, imu_mag_x);
    _mav_put_int16_t(buf, 74, imu_mag_y);
    _mav_put_int16_t(buf, 76, imu_mag_z);
    _mav_put_uint16_t(buf, 78, pressure_hpa);
    _mav_put_int16_t(buf, 80, temp_celsius);
    _mav_put_uint16_t(buf, 82, system_states);
    _mav_put_uint16_t(buf, 84, event_states);
    _mav_put_uint16_t(buf, 86, battery_mv);
    _mav_put_uint16_t(buf, 88, vel);
    _mav_put_uint16_t(buf, 90, cog);
    _mav_put_uint8_t(buf, 92, mission_state);
    _mav_put_uint8_t(buf, 93, gps_fix);
    _mav_put_uint8_t(buf, 94, satellites_nb);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ROCKET_TELEMETRY, buf, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC);
#else
    mavlink_rocket_telemetry_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.gps_alt = gps_alt;
    packet.altitude_msl_cm = altitude_msl_cm;
    packet.kalman_z = kalman_z;
    packet.kalman_v = kalman_v;
    packet.imu_gyro_x = imu_gyro_x;
    packet.imu_gyro_y = imu_gyro_y;
    packet.imu_gyro_z = imu_gyro_z;
    packet.highg_acc_x = highg_acc_x;
    packet.highg_acc_y = highg_acc_y;
    packet.highg_acc_z = highg_acc_z;
    packet.highg_acc_vertical = highg_acc_vertical;
    packet.imu_acc_vertical = imu_acc_vertical;
    packet.roll = roll;
    packet.pitch = pitch;
    packet.yaw = yaw;
    packet.imu_acc_x = imu_acc_x;
    packet.imu_acc_y = imu_acc_y;
    packet.imu_acc_z = imu_acc_z;
    packet.imu_mag_x = imu_mag_x;
    packet.imu_mag_y = imu_mag_y;
    packet.imu_mag_z = imu_mag_z;
    packet.pressure_hpa = pressure_hpa;
    packet.temp_celsius = temp_celsius;
    packet.system_states = system_states;
    packet.event_states = event_states;
    packet.battery_mv = battery_mv;
    packet.vel = vel;
    packet.cog = cog;
    packet.mission_state = mission_state;
    packet.gps_fix = gps_fix;
    packet.satellites_nb = satellites_nb;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ROCKET_TELEMETRY, (const char *)&packet, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC);
#endif
}

/**
 * @brief Send a rocket_telemetry message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_rocket_telemetry_send_struct(mavlink_channel_t chan, const mavlink_rocket_telemetry_t* rocket_telemetry)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_rocket_telemetry_send(chan, rocket_telemetry->time_boot_ms, rocket_telemetry->lat, rocket_telemetry->lon, rocket_telemetry->gps_alt, rocket_telemetry->altitude_msl_cm, rocket_telemetry->kalman_z, rocket_telemetry->kalman_v, rocket_telemetry->imu_gyro_x, rocket_telemetry->imu_gyro_y, rocket_telemetry->imu_gyro_z, rocket_telemetry->highg_acc_x, rocket_telemetry->highg_acc_y, rocket_telemetry->highg_acc_z, rocket_telemetry->highg_acc_vertical, rocket_telemetry->imu_acc_vertical, rocket_telemetry->roll, rocket_telemetry->pitch, rocket_telemetry->yaw, rocket_telemetry->imu_acc_x, rocket_telemetry->imu_acc_y, rocket_telemetry->imu_acc_z, rocket_telemetry->imu_mag_x, rocket_telemetry->imu_mag_y, rocket_telemetry->imu_mag_z, rocket_telemetry->pressure_hpa, rocket_telemetry->temp_celsius, rocket_telemetry->system_states, rocket_telemetry->event_states, rocket_telemetry->battery_mv, rocket_telemetry->vel, rocket_telemetry->cog, rocket_telemetry->mission_state, rocket_telemetry->gps_fix, rocket_telemetry->satellites_nb);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ROCKET_TELEMETRY, (const char *)rocket_telemetry, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC);
#endif
}

#if MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by reusing
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_rocket_telemetry_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, int32_t lat, int32_t lon, int32_t gps_alt, int32_t altitude_msl_cm, int32_t kalman_z, int32_t kalman_v, int32_t imu_gyro_x, int32_t imu_gyro_y, int32_t imu_gyro_z, int32_t highg_acc_x, int32_t highg_acc_y, int32_t highg_acc_z, int32_t highg_acc_vertical, int32_t imu_acc_vertical, int16_t roll, int16_t pitch, int16_t yaw, int16_t imu_acc_x, int16_t imu_acc_y, int16_t imu_acc_z, int16_t imu_mag_x, int16_t imu_mag_y, int16_t imu_mag_z, uint16_t pressure_hpa, int16_t temp_celsius, uint16_t system_states, uint16_t event_states, uint16_t battery_mv, uint16_t vel, uint16_t cog, uint8_t mission_state, uint8_t gps_fix, uint8_t satellites_nb)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_int32_t(buf, 12, gps_alt);
    _mav_put_int32_t(buf, 16, altitude_msl_cm);
    _mav_put_int32_t(buf, 20, kalman_z);
    _mav_put_int32_t(buf, 24, kalman_v);
    _mav_put_int32_t(buf, 28, imu_gyro_x);
    _mav_put_int32_t(buf, 32, imu_gyro_y);
    _mav_put_int32_t(buf, 36, imu_gyro_z);
    _mav_put_int32_t(buf, 40, highg_acc_x);
    _mav_put_int32_t(buf, 44, highg_acc_y);
    _mav_put_int32_t(buf, 48, highg_acc_z);
    _mav_put_int32_t(buf, 52, highg_acc_vertical);
    _mav_put_int32_t(buf, 56, imu_acc_vertical);
    _mav_put_int16_t(buf, 60, roll);
    _mav_put_int16_t(buf, 62, pitch);
    _mav_put_int16_t(buf, 64, yaw);
    _mav_put_int16_t(buf, 66, imu_acc_x);
    _mav_put_int16_t(buf, 68, imu_acc_y);
    _mav_put_int16_t(buf, 70, imu_acc_z);
    _mav_put_int16_t(buf, 72, imu_mag_x);
    _mav_put_int16_t(buf, 74, imu_mag_y);
    _mav_put_int16_t(buf, 76, imu_mag_z);
    _mav_put_uint16_t(buf, 78, pressure_hpa);
    _mav_put_int16_t(buf, 80, temp_celsius);
    _mav_put_uint16_t(buf, 82, system_states);
    _mav_put_uint16_t(buf, 84, event_states);
    _mav_put_uint16_t(buf, 86, battery_mv);
    _mav_put_uint16_t(buf, 88, vel);
    _mav_put_uint16_t(buf, 90, cog);
    _mav_put_uint8_t(buf, 92, mission_state);
    _mav_put_uint8_t(buf, 93, gps_fix);
    _mav_put_uint8_t(buf, 94, satellites_nb);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ROCKET_TELEMETRY, buf, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC);
#else
    mavlink_rocket_telemetry_t *packet = (mavlink_rocket_telemetry_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->lat = lat;
    packet->lon = lon;
    packet->gps_alt = gps_alt;
    packet->altitude_msl_cm = altitude_msl_cm;
    packet->kalman_z = kalman_z;
    packet->kalman_v = kalman_v;
    packet->imu_gyro_x = imu_gyro_x;
    packet->imu_gyro_y = imu_gyro_y;
    packet->imu_gyro_z = imu_gyro_z;
    packet->highg_acc_x = highg_acc_x;
    packet->highg_acc_y = highg_acc_y;
    packet->highg_acc_z = highg_acc_z;
    packet->highg_acc_vertical = highg_acc_vertical;
    packet->imu_acc_vertical = imu_acc_vertical;
    packet->roll = roll;
    packet->pitch = pitch;
    packet->yaw = yaw;
    packet->imu_acc_x = imu_acc_x;
    packet->imu_acc_y = imu_acc_y;
    packet->imu_acc_z = imu_acc_z;
    packet->imu_mag_x = imu_mag_x;
    packet->imu_mag_y = imu_mag_y;
    packet->imu_mag_z = imu_mag_z;
    packet->pressure_hpa = pressure_hpa;
    packet->temp_celsius = temp_celsius;
    packet->system_states = system_states;
    packet->event_states = event_states;
    packet->battery_mv = battery_mv;
    packet->vel = vel;
    packet->cog = cog;
    packet->mission_state = mission_state;
    packet->gps_fix = gps_fix;
    packet->satellites_nb = satellites_nb;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ROCKET_TELEMETRY, (const char *)packet, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC);
#endif
}
#endif

#endif

// MESSAGE ROCKET_TELEMETRY UNPACKING


/**
 * @brief Get field time_boot_ms from rocket_telemetry message
 *
 * @return [ms] Timestamp since system boot in milliseconds.
 */
static inline uint32_t mavlink_msg_rocket_telemetry_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field lat from rocket_telemetry message
 *
 * @return [degE7] Latitude in degrees * 1E7.
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_lat(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field lon from rocket_telemetry message
 *
 * @return [degE7] Longitude in degrees * 1E7.
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_lon(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  8);
}

/**
 * @brief Get field gps_alt from rocket_telemetry message
 *
 * @return [mm] Altitude (MSL) from GPS.
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_gps_alt(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  12);
}

/**
 * @brief Get field altitude_msl_cm from rocket_telemetry message
 *
 * @return [cm] Altitude (MSL) from barometer.
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_altitude_msl_cm(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  16);
}

/**
 * @brief Get field kalman_z from rocket_telemetry message
 *
 * @return [cm] Filtered altitude from Kalman filter.
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_kalman_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  20);
}

/**
 * @brief Get field kalman_v from rocket_telemetry message
 *
 * @return [cm/s] Filtered velocity from Kalman filter.
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_kalman_v(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  24);
}

/**
 * @brief Get field imu_gyro_x from rocket_telemetry message
 *
 * @return [cdeg/s] IMU Gyro X.
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_imu_gyro_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  28);
}

/**
 * @brief Get field imu_gyro_y from rocket_telemetry message
 *
 * @return [cdeg/s] IMU Gyro Y.
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_imu_gyro_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  32);
}

/**
 * @brief Get field imu_gyro_z from rocket_telemetry message
 *
 * @return [cdeg/s] IMU Gyro Z.
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_imu_gyro_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  36);
}

/**
 * @brief Get field highg_acc_x from rocket_telemetry message
 *
 * @return [cm/s/s] High-G Acceleration X.
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_highg_acc_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  40);
}

/**
 * @brief Get field highg_acc_y from rocket_telemetry message
 *
 * @return [cm/s/s] High-G Acceleration Y.
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_highg_acc_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  44);
}

/**
 * @brief Get field highg_acc_z from rocket_telemetry message
 *
 * @return [cm/s/s] High-G Acceleration Z.
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_highg_acc_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  48);
}

/**
 * @brief Get field highg_acc_vertical from rocket_telemetry message
 *
 * @return [cm/s/s] Vertical High-G acceleration.
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_highg_acc_vertical(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  52);
}

/**
 * @brief Get field imu_acc_vertical from rocket_telemetry message
 *
 * @return [cm/s/s] Vertical IMU acceleration.
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_imu_acc_vertical(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  56);
}

/**
 * @brief Get field roll from rocket_telemetry message
 *
 * @return [cdeg] Roll angle in centi-degrees.
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_roll(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  60);
}

/**
 * @brief Get field pitch from rocket_telemetry message
 *
 * @return [cdeg] Pitch angle in centi-degrees.
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_pitch(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  62);
}

/**
 * @brief Get field yaw from rocket_telemetry message
 *
 * @return [cdeg] Yaw angle in centi-degrees.
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_yaw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  64);
}

/**
 * @brief Get field imu_acc_x from rocket_telemetry message
 *
 * @return [cm/s/s] IMU Acceleration X.
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_imu_acc_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  66);
}

/**
 * @brief Get field imu_acc_y from rocket_telemetry message
 *
 * @return [cm/s/s] IMU Acceleration Y.
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_imu_acc_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  68);
}

/**
 * @brief Get field imu_acc_z from rocket_telemetry message
 *
 * @return [cm/s/s] IMU Acceleration Z.
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_imu_acc_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  70);
}

/**
 * @brief Get field imu_mag_x from rocket_telemetry message
 *
 * @return [cuT] IMU Mag X in centi-microTesla.
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_imu_mag_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  72);
}

/**
 * @brief Get field imu_mag_y from rocket_telemetry message
 *
 * @return [cuT] IMU Mag Y in centi-microTesla.
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_imu_mag_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  74);
}

/**
 * @brief Get field imu_mag_z from rocket_telemetry message
 *
 * @return [cuT] IMU Mag Z in centi-microTesla.
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_imu_mag_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  76);
}

/**
 * @brief Get field pressure_hpa from rocket_telemetry message
 *
 * @return [hPa] Pressure in hectopascals.
 */
static inline uint16_t mavlink_msg_rocket_telemetry_get_pressure_hpa(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  78);
}

/**
 * @brief Get field temp_celsius from rocket_telemetry message
 *
 * @return [cdegC] Temperature in centi-degrees Celsius.
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_temp_celsius(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  80);
}

/**
 * @brief Get field system_states from rocket_telemetry message
 *
 * @return  Current system state bitfield.
 */
static inline uint16_t mavlink_msg_rocket_telemetry_get_system_states(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  82);
}

/**
 * @brief Get field event_states from rocket_telemetry message
 *
 * @return  Current events states.
 */
static inline uint16_t mavlink_msg_rocket_telemetry_get_event_states(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  84);
}

/**
 * @brief Get field battery_mv from rocket_telemetry message
 *
 * @return [mV] Battery voltage in millivolts.
 */
static inline uint16_t mavlink_msg_rocket_telemetry_get_battery_mv(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  86);
}

/**
 * @brief Get field vel from rocket_telemetry message
 *
 * @return [cm/s] Ground speed.
 */
static inline uint16_t mavlink_msg_rocket_telemetry_get_vel(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  88);
}

/**
 * @brief Get field cog from rocket_telemetry message
 *
 * @return [cdeg] Course over ground.
 */
static inline uint16_t mavlink_msg_rocket_telemetry_get_cog(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  90);
}

/**
 * @brief Get field mission_state from rocket_telemetry message
 *
 * @return  Mission state.
 */
static inline uint8_t mavlink_msg_rocket_telemetry_get_mission_state(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  92);
}

/**
 * @brief Get field gps_fix from rocket_telemetry message
 *
 * @return  GPS fix type.
 */
static inline uint8_t mavlink_msg_rocket_telemetry_get_gps_fix(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  93);
}

/**
 * @brief Get field satellites_nb from rocket_telemetry message
 *
 * @return  Number of visible satellites.
 */
static inline uint8_t mavlink_msg_rocket_telemetry_get_satellites_nb(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  94);
}

/**
 * @brief Decode a rocket_telemetry message into a struct
 *
 * @param msg The message to decode
 * @param rocket_telemetry C-struct to decode the message contents into
 */
static inline void mavlink_msg_rocket_telemetry_decode(const mavlink_message_t* msg, mavlink_rocket_telemetry_t* rocket_telemetry)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    rocket_telemetry->time_boot_ms = mavlink_msg_rocket_telemetry_get_time_boot_ms(msg);
    rocket_telemetry->lat = mavlink_msg_rocket_telemetry_get_lat(msg);
    rocket_telemetry->lon = mavlink_msg_rocket_telemetry_get_lon(msg);
    rocket_telemetry->gps_alt = mavlink_msg_rocket_telemetry_get_gps_alt(msg);
    rocket_telemetry->altitude_msl_cm = mavlink_msg_rocket_telemetry_get_altitude_msl_cm(msg);
    rocket_telemetry->kalman_z = mavlink_msg_rocket_telemetry_get_kalman_z(msg);
    rocket_telemetry->kalman_v = mavlink_msg_rocket_telemetry_get_kalman_v(msg);
    rocket_telemetry->imu_gyro_x = mavlink_msg_rocket_telemetry_get_imu_gyro_x(msg);
    rocket_telemetry->imu_gyro_y = mavlink_msg_rocket_telemetry_get_imu_gyro_y(msg);
    rocket_telemetry->imu_gyro_z = mavlink_msg_rocket_telemetry_get_imu_gyro_z(msg);
    rocket_telemetry->highg_acc_x = mavlink_msg_rocket_telemetry_get_highg_acc_x(msg);
    rocket_telemetry->highg_acc_y = mavlink_msg_rocket_telemetry_get_highg_acc_y(msg);
    rocket_telemetry->highg_acc_z = mavlink_msg_rocket_telemetry_get_highg_acc_z(msg);
    rocket_telemetry->highg_acc_vertical = mavlink_msg_rocket_telemetry_get_highg_acc_vertical(msg);
    rocket_telemetry->imu_acc_vertical = mavlink_msg_rocket_telemetry_get_imu_acc_vertical(msg);
    rocket_telemetry->roll = mavlink_msg_rocket_telemetry_get_roll(msg);
    rocket_telemetry->pitch = mavlink_msg_rocket_telemetry_get_pitch(msg);
    rocket_telemetry->yaw = mavlink_msg_rocket_telemetry_get_yaw(msg);
    rocket_telemetry->imu_acc_x = mavlink_msg_rocket_telemetry_get_imu_acc_x(msg);
    rocket_telemetry->imu_acc_y = mavlink_msg_rocket_telemetry_get_imu_acc_y(msg);
    rocket_telemetry->imu_acc_z = mavlink_msg_rocket_telemetry_get_imu_acc_z(msg);
    rocket_telemetry->imu_mag_x = mavlink_msg_rocket_telemetry_get_imu_mag_x(msg);
    rocket_telemetry->imu_mag_y = mavlink_msg_rocket_telemetry_get_imu_mag_y(msg);
    rocket_telemetry->imu_mag_z = mavlink_msg_rocket_telemetry_get_imu_mag_z(msg);
    rocket_telemetry->pressure_hpa = mavlink_msg_rocket_telemetry_get_pressure_hpa(msg);
    rocket_telemetry->temp_celsius = mavlink_msg_rocket_telemetry_get_temp_celsius(msg);
    rocket_telemetry->system_states = mavlink_msg_rocket_telemetry_get_system_states(msg);
    rocket_telemetry->event_states = mavlink_msg_rocket_telemetry_get_event_states(msg);
    rocket_telemetry->battery_mv = mavlink_msg_rocket_telemetry_get_battery_mv(msg);
    rocket_telemetry->vel = mavlink_msg_rocket_telemetry_get_vel(msg);
    rocket_telemetry->cog = mavlink_msg_rocket_telemetry_get_cog(msg);
    rocket_telemetry->mission_state = mavlink_msg_rocket_telemetry_get_mission_state(msg);
    rocket_telemetry->gps_fix = mavlink_msg_rocket_telemetry_get_gps_fix(msg);
    rocket_telemetry->satellites_nb = mavlink_msg_rocket_telemetry_get_satellites_nb(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN? msg->len : MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN;
        memset(rocket_telemetry, 0, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
    memcpy(rocket_telemetry, _MAV_PAYLOAD(msg), len);
#endif
}
