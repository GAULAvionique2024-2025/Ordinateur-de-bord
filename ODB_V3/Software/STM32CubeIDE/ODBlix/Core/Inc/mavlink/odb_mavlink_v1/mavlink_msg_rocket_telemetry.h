#pragma once
// MESSAGE ROCKET_TELEMETRY PACKING

#define MAVLINK_MSG_ID_ROCKET_TELEMETRY 200


typedef struct __mavlink_rocket_telemetry_t {
 uint32_t time_boot_ms; /*<  Timestamp (ms)*/
 int32_t lat; /*<  Latitude (degE7)*/
 int32_t lon; /*<  Longitude (degE7)*/
 int32_t gps_alt; /*<  Altitude GPS (mm)*/
 float pressure_hpa; /*<  Pressure (hPa)*/
 int16_t roll; /*<  Roll (centi-deg)*/
 int16_t pitch; /*<  Pitch (centi-deg)*/
 int16_t yaw; /*<  Yaw (centi-deg)*/
 int16_t temp_celsius; /*<  Temperature (centi-deg C)*/
 int16_t acc_x; /*<  Acceleration X (centi-G)*/
 int16_t acc_y; /*<  Acceleration Y (centi-G)*/
 int16_t acc_z; /*<  Acceleration Z (centi-G)*/
 uint16_t system_states; /*<  System states*/
 uint16_t battery_mv; /*<  Battery voltage (mV)*/
 uint16_t vel; /*<  Velocity (cm/s)*/
 uint16_t cog; /*<  Course over ground (cd)*/
 uint8_t gps_fix; /*<  GPS fix type*/
 uint8_t satellites_nb; /*<  Number of satellites*/
} mavlink_rocket_telemetry_t;

#define MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN 44
#define MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN 44
#define MAVLINK_MSG_ID_200_LEN 44
#define MAVLINK_MSG_ID_200_MIN_LEN 44

#define MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC 156
#define MAVLINK_MSG_ID_200_CRC 156



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_ROCKET_TELEMETRY { \
    200, \
    "ROCKET_TELEMETRY", \
    18, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_rocket_telemetry_t, time_boot_ms) }, \
         { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_rocket_telemetry_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_rocket_telemetry_t, lon) }, \
         { "gps_alt", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_rocket_telemetry_t, gps_alt) }, \
         { "pressure_hpa", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_rocket_telemetry_t, pressure_hpa) }, \
         { "roll", NULL, MAVLINK_TYPE_INT16_T, 0, 20, offsetof(mavlink_rocket_telemetry_t, roll) }, \
         { "pitch", NULL, MAVLINK_TYPE_INT16_T, 0, 22, offsetof(mavlink_rocket_telemetry_t, pitch) }, \
         { "yaw", NULL, MAVLINK_TYPE_INT16_T, 0, 24, offsetof(mavlink_rocket_telemetry_t, yaw) }, \
         { "temp_celsius", NULL, MAVLINK_TYPE_INT16_T, 0, 26, offsetof(mavlink_rocket_telemetry_t, temp_celsius) }, \
         { "acc_x", NULL, MAVLINK_TYPE_INT16_T, 0, 28, offsetof(mavlink_rocket_telemetry_t, acc_x) }, \
         { "acc_y", NULL, MAVLINK_TYPE_INT16_T, 0, 30, offsetof(mavlink_rocket_telemetry_t, acc_y) }, \
         { "acc_z", NULL, MAVLINK_TYPE_INT16_T, 0, 32, offsetof(mavlink_rocket_telemetry_t, acc_z) }, \
         { "system_states", NULL, MAVLINK_TYPE_UINT16_T, 0, 34, offsetof(mavlink_rocket_telemetry_t, system_states) }, \
         { "battery_mv", NULL, MAVLINK_TYPE_UINT16_T, 0, 36, offsetof(mavlink_rocket_telemetry_t, battery_mv) }, \
         { "vel", NULL, MAVLINK_TYPE_UINT16_T, 0, 38, offsetof(mavlink_rocket_telemetry_t, vel) }, \
         { "cog", NULL, MAVLINK_TYPE_UINT16_T, 0, 40, offsetof(mavlink_rocket_telemetry_t, cog) }, \
         { "gps_fix", NULL, MAVLINK_TYPE_UINT8_T, 0, 42, offsetof(mavlink_rocket_telemetry_t, gps_fix) }, \
         { "satellites_nb", NULL, MAVLINK_TYPE_UINT8_T, 0, 43, offsetof(mavlink_rocket_telemetry_t, satellites_nb) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_ROCKET_TELEMETRY { \
    "ROCKET_TELEMETRY", \
    18, \
    {  { "time_boot_ms", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_rocket_telemetry_t, time_boot_ms) }, \
         { "lat", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_rocket_telemetry_t, lat) }, \
         { "lon", NULL, MAVLINK_TYPE_INT32_T, 0, 8, offsetof(mavlink_rocket_telemetry_t, lon) }, \
         { "gps_alt", NULL, MAVLINK_TYPE_INT32_T, 0, 12, offsetof(mavlink_rocket_telemetry_t, gps_alt) }, \
         { "pressure_hpa", NULL, MAVLINK_TYPE_FLOAT, 0, 16, offsetof(mavlink_rocket_telemetry_t, pressure_hpa) }, \
         { "roll", NULL, MAVLINK_TYPE_INT16_T, 0, 20, offsetof(mavlink_rocket_telemetry_t, roll) }, \
         { "pitch", NULL, MAVLINK_TYPE_INT16_T, 0, 22, offsetof(mavlink_rocket_telemetry_t, pitch) }, \
         { "yaw", NULL, MAVLINK_TYPE_INT16_T, 0, 24, offsetof(mavlink_rocket_telemetry_t, yaw) }, \
         { "temp_celsius", NULL, MAVLINK_TYPE_INT16_T, 0, 26, offsetof(mavlink_rocket_telemetry_t, temp_celsius) }, \
         { "acc_x", NULL, MAVLINK_TYPE_INT16_T, 0, 28, offsetof(mavlink_rocket_telemetry_t, acc_x) }, \
         { "acc_y", NULL, MAVLINK_TYPE_INT16_T, 0, 30, offsetof(mavlink_rocket_telemetry_t, acc_y) }, \
         { "acc_z", NULL, MAVLINK_TYPE_INT16_T, 0, 32, offsetof(mavlink_rocket_telemetry_t, acc_z) }, \
         { "system_states", NULL, MAVLINK_TYPE_UINT16_T, 0, 34, offsetof(mavlink_rocket_telemetry_t, system_states) }, \
         { "battery_mv", NULL, MAVLINK_TYPE_UINT16_T, 0, 36, offsetof(mavlink_rocket_telemetry_t, battery_mv) }, \
         { "vel", NULL, MAVLINK_TYPE_UINT16_T, 0, 38, offsetof(mavlink_rocket_telemetry_t, vel) }, \
         { "cog", NULL, MAVLINK_TYPE_UINT16_T, 0, 40, offsetof(mavlink_rocket_telemetry_t, cog) }, \
         { "gps_fix", NULL, MAVLINK_TYPE_UINT8_T, 0, 42, offsetof(mavlink_rocket_telemetry_t, gps_fix) }, \
         { "satellites_nb", NULL, MAVLINK_TYPE_UINT8_T, 0, 43, offsetof(mavlink_rocket_telemetry_t, satellites_nb) }, \
         } \
}
#endif

/**
 * @brief Pack a rocket_telemetry message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param time_boot_ms  Timestamp (ms)
 * @param lat  Latitude (degE7)
 * @param lon  Longitude (degE7)
 * @param gps_alt  Altitude GPS (mm)
 * @param pressure_hpa  Pressure (hPa)
 * @param roll  Roll (centi-deg)
 * @param pitch  Pitch (centi-deg)
 * @param yaw  Yaw (centi-deg)
 * @param temp_celsius  Temperature (centi-deg C)
 * @param acc_x  Acceleration X (centi-G)
 * @param acc_y  Acceleration Y (centi-G)
 * @param acc_z  Acceleration Z (centi-G)
 * @param system_states  System states
 * @param battery_mv  Battery voltage (mV)
 * @param vel  Velocity (cm/s)
 * @param cog  Course over ground (cd)
 * @param gps_fix  GPS fix type
 * @param satellites_nb  Number of satellites
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rocket_telemetry_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint32_t time_boot_ms, int32_t lat, int32_t lon, int32_t gps_alt, float pressure_hpa, int16_t roll, int16_t pitch, int16_t yaw, int16_t temp_celsius, int16_t acc_x, int16_t acc_y, int16_t acc_z, uint16_t system_states, uint16_t battery_mv, uint16_t vel, uint16_t cog, uint8_t gps_fix, uint8_t satellites_nb)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_int32_t(buf, 12, gps_alt);
    _mav_put_float(buf, 16, pressure_hpa);
    _mav_put_int16_t(buf, 20, roll);
    _mav_put_int16_t(buf, 22, pitch);
    _mav_put_int16_t(buf, 24, yaw);
    _mav_put_int16_t(buf, 26, temp_celsius);
    _mav_put_int16_t(buf, 28, acc_x);
    _mav_put_int16_t(buf, 30, acc_y);
    _mav_put_int16_t(buf, 32, acc_z);
    _mav_put_uint16_t(buf, 34, system_states);
    _mav_put_uint16_t(buf, 36, battery_mv);
    _mav_put_uint16_t(buf, 38, vel);
    _mav_put_uint16_t(buf, 40, cog);
    _mav_put_uint8_t(buf, 42, gps_fix);
    _mav_put_uint8_t(buf, 43, satellites_nb);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
#else
    mavlink_rocket_telemetry_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.gps_alt = gps_alt;
    packet.pressure_hpa = pressure_hpa;
    packet.roll = roll;
    packet.pitch = pitch;
    packet.yaw = yaw;
    packet.temp_celsius = temp_celsius;
    packet.acc_x = acc_x;
    packet.acc_y = acc_y;
    packet.acc_z = acc_z;
    packet.system_states = system_states;
    packet.battery_mv = battery_mv;
    packet.vel = vel;
    packet.cog = cog;
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
 * @param time_boot_ms  Timestamp (ms)
 * @param lat  Latitude (degE7)
 * @param lon  Longitude (degE7)
 * @param gps_alt  Altitude GPS (mm)
 * @param pressure_hpa  Pressure (hPa)
 * @param roll  Roll (centi-deg)
 * @param pitch  Pitch (centi-deg)
 * @param yaw  Yaw (centi-deg)
 * @param temp_celsius  Temperature (centi-deg C)
 * @param acc_x  Acceleration X (centi-G)
 * @param acc_y  Acceleration Y (centi-G)
 * @param acc_z  Acceleration Z (centi-G)
 * @param system_states  System states
 * @param battery_mv  Battery voltage (mV)
 * @param vel  Velocity (cm/s)
 * @param cog  Course over ground (cd)
 * @param gps_fix  GPS fix type
 * @param satellites_nb  Number of satellites
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rocket_telemetry_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint32_t time_boot_ms, int32_t lat, int32_t lon, int32_t gps_alt, float pressure_hpa, int16_t roll, int16_t pitch, int16_t yaw, int16_t temp_celsius, int16_t acc_x, int16_t acc_y, int16_t acc_z, uint16_t system_states, uint16_t battery_mv, uint16_t vel, uint16_t cog, uint8_t gps_fix, uint8_t satellites_nb)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_int32_t(buf, 12, gps_alt);
    _mav_put_float(buf, 16, pressure_hpa);
    _mav_put_int16_t(buf, 20, roll);
    _mav_put_int16_t(buf, 22, pitch);
    _mav_put_int16_t(buf, 24, yaw);
    _mav_put_int16_t(buf, 26, temp_celsius);
    _mav_put_int16_t(buf, 28, acc_x);
    _mav_put_int16_t(buf, 30, acc_y);
    _mav_put_int16_t(buf, 32, acc_z);
    _mav_put_uint16_t(buf, 34, system_states);
    _mav_put_uint16_t(buf, 36, battery_mv);
    _mav_put_uint16_t(buf, 38, vel);
    _mav_put_uint16_t(buf, 40, cog);
    _mav_put_uint8_t(buf, 42, gps_fix);
    _mav_put_uint8_t(buf, 43, satellites_nb);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
#else
    mavlink_rocket_telemetry_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.gps_alt = gps_alt;
    packet.pressure_hpa = pressure_hpa;
    packet.roll = roll;
    packet.pitch = pitch;
    packet.yaw = yaw;
    packet.temp_celsius = temp_celsius;
    packet.acc_x = acc_x;
    packet.acc_y = acc_y;
    packet.acc_z = acc_z;
    packet.system_states = system_states;
    packet.battery_mv = battery_mv;
    packet.vel = vel;
    packet.cog = cog;
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
 * @param time_boot_ms  Timestamp (ms)
 * @param lat  Latitude (degE7)
 * @param lon  Longitude (degE7)
 * @param gps_alt  Altitude GPS (mm)
 * @param pressure_hpa  Pressure (hPa)
 * @param roll  Roll (centi-deg)
 * @param pitch  Pitch (centi-deg)
 * @param yaw  Yaw (centi-deg)
 * @param temp_celsius  Temperature (centi-deg C)
 * @param acc_x  Acceleration X (centi-G)
 * @param acc_y  Acceleration Y (centi-G)
 * @param acc_z  Acceleration Z (centi-G)
 * @param system_states  System states
 * @param battery_mv  Battery voltage (mV)
 * @param vel  Velocity (cm/s)
 * @param cog  Course over ground (cd)
 * @param gps_fix  GPS fix type
 * @param satellites_nb  Number of satellites
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_rocket_telemetry_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint32_t time_boot_ms,int32_t lat,int32_t lon,int32_t gps_alt,float pressure_hpa,int16_t roll,int16_t pitch,int16_t yaw,int16_t temp_celsius,int16_t acc_x,int16_t acc_y,int16_t acc_z,uint16_t system_states,uint16_t battery_mv,uint16_t vel,uint16_t cog,uint8_t gps_fix,uint8_t satellites_nb)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_int32_t(buf, 12, gps_alt);
    _mav_put_float(buf, 16, pressure_hpa);
    _mav_put_int16_t(buf, 20, roll);
    _mav_put_int16_t(buf, 22, pitch);
    _mav_put_int16_t(buf, 24, yaw);
    _mav_put_int16_t(buf, 26, temp_celsius);
    _mav_put_int16_t(buf, 28, acc_x);
    _mav_put_int16_t(buf, 30, acc_y);
    _mav_put_int16_t(buf, 32, acc_z);
    _mav_put_uint16_t(buf, 34, system_states);
    _mav_put_uint16_t(buf, 36, battery_mv);
    _mav_put_uint16_t(buf, 38, vel);
    _mav_put_uint16_t(buf, 40, cog);
    _mav_put_uint8_t(buf, 42, gps_fix);
    _mav_put_uint8_t(buf, 43, satellites_nb);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
#else
    mavlink_rocket_telemetry_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.gps_alt = gps_alt;
    packet.pressure_hpa = pressure_hpa;
    packet.roll = roll;
    packet.pitch = pitch;
    packet.yaw = yaw;
    packet.temp_celsius = temp_celsius;
    packet.acc_x = acc_x;
    packet.acc_y = acc_y;
    packet.acc_z = acc_z;
    packet.system_states = system_states;
    packet.battery_mv = battery_mv;
    packet.vel = vel;
    packet.cog = cog;
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
    return mavlink_msg_rocket_telemetry_pack(system_id, component_id, msg, rocket_telemetry->time_boot_ms, rocket_telemetry->lat, rocket_telemetry->lon, rocket_telemetry->gps_alt, rocket_telemetry->pressure_hpa, rocket_telemetry->roll, rocket_telemetry->pitch, rocket_telemetry->yaw, rocket_telemetry->temp_celsius, rocket_telemetry->acc_x, rocket_telemetry->acc_y, rocket_telemetry->acc_z, rocket_telemetry->system_states, rocket_telemetry->battery_mv, rocket_telemetry->vel, rocket_telemetry->cog, rocket_telemetry->gps_fix, rocket_telemetry->satellites_nb);
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
    return mavlink_msg_rocket_telemetry_pack_chan(system_id, component_id, chan, msg, rocket_telemetry->time_boot_ms, rocket_telemetry->lat, rocket_telemetry->lon, rocket_telemetry->gps_alt, rocket_telemetry->pressure_hpa, rocket_telemetry->roll, rocket_telemetry->pitch, rocket_telemetry->yaw, rocket_telemetry->temp_celsius, rocket_telemetry->acc_x, rocket_telemetry->acc_y, rocket_telemetry->acc_z, rocket_telemetry->system_states, rocket_telemetry->battery_mv, rocket_telemetry->vel, rocket_telemetry->cog, rocket_telemetry->gps_fix, rocket_telemetry->satellites_nb);
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
    return mavlink_msg_rocket_telemetry_pack_status(system_id, component_id, _status, msg,  rocket_telemetry->time_boot_ms, rocket_telemetry->lat, rocket_telemetry->lon, rocket_telemetry->gps_alt, rocket_telemetry->pressure_hpa, rocket_telemetry->roll, rocket_telemetry->pitch, rocket_telemetry->yaw, rocket_telemetry->temp_celsius, rocket_telemetry->acc_x, rocket_telemetry->acc_y, rocket_telemetry->acc_z, rocket_telemetry->system_states, rocket_telemetry->battery_mv, rocket_telemetry->vel, rocket_telemetry->cog, rocket_telemetry->gps_fix, rocket_telemetry->satellites_nb);
}

/**
 * @brief Send a rocket_telemetry message
 * @param chan MAVLink channel to send the message
 *
 * @param time_boot_ms  Timestamp (ms)
 * @param lat  Latitude (degE7)
 * @param lon  Longitude (degE7)
 * @param gps_alt  Altitude GPS (mm)
 * @param pressure_hpa  Pressure (hPa)
 * @param roll  Roll (centi-deg)
 * @param pitch  Pitch (centi-deg)
 * @param yaw  Yaw (centi-deg)
 * @param temp_celsius  Temperature (centi-deg C)
 * @param acc_x  Acceleration X (centi-G)
 * @param acc_y  Acceleration Y (centi-G)
 * @param acc_z  Acceleration Z (centi-G)
 * @param system_states  System states
 * @param battery_mv  Battery voltage (mV)
 * @param vel  Velocity (cm/s)
 * @param cog  Course over ground (cd)
 * @param gps_fix  GPS fix type
 * @param satellites_nb  Number of satellites
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_rocket_telemetry_send(mavlink_channel_t chan, uint32_t time_boot_ms, int32_t lat, int32_t lon, int32_t gps_alt, float pressure_hpa, int16_t roll, int16_t pitch, int16_t yaw, int16_t temp_celsius, int16_t acc_x, int16_t acc_y, int16_t acc_z, uint16_t system_states, uint16_t battery_mv, uint16_t vel, uint16_t cog, uint8_t gps_fix, uint8_t satellites_nb)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN];
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_int32_t(buf, 12, gps_alt);
    _mav_put_float(buf, 16, pressure_hpa);
    _mav_put_int16_t(buf, 20, roll);
    _mav_put_int16_t(buf, 22, pitch);
    _mav_put_int16_t(buf, 24, yaw);
    _mav_put_int16_t(buf, 26, temp_celsius);
    _mav_put_int16_t(buf, 28, acc_x);
    _mav_put_int16_t(buf, 30, acc_y);
    _mav_put_int16_t(buf, 32, acc_z);
    _mav_put_uint16_t(buf, 34, system_states);
    _mav_put_uint16_t(buf, 36, battery_mv);
    _mav_put_uint16_t(buf, 38, vel);
    _mav_put_uint16_t(buf, 40, cog);
    _mav_put_uint8_t(buf, 42, gps_fix);
    _mav_put_uint8_t(buf, 43, satellites_nb);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ROCKET_TELEMETRY, buf, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC);
#else
    mavlink_rocket_telemetry_t packet;
    packet.time_boot_ms = time_boot_ms;
    packet.lat = lat;
    packet.lon = lon;
    packet.gps_alt = gps_alt;
    packet.pressure_hpa = pressure_hpa;
    packet.roll = roll;
    packet.pitch = pitch;
    packet.yaw = yaw;
    packet.temp_celsius = temp_celsius;
    packet.acc_x = acc_x;
    packet.acc_y = acc_y;
    packet.acc_z = acc_z;
    packet.system_states = system_states;
    packet.battery_mv = battery_mv;
    packet.vel = vel;
    packet.cog = cog;
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
    mavlink_msg_rocket_telemetry_send(chan, rocket_telemetry->time_boot_ms, rocket_telemetry->lat, rocket_telemetry->lon, rocket_telemetry->gps_alt, rocket_telemetry->pressure_hpa, rocket_telemetry->roll, rocket_telemetry->pitch, rocket_telemetry->yaw, rocket_telemetry->temp_celsius, rocket_telemetry->acc_x, rocket_telemetry->acc_y, rocket_telemetry->acc_z, rocket_telemetry->system_states, rocket_telemetry->battery_mv, rocket_telemetry->vel, rocket_telemetry->cog, rocket_telemetry->gps_fix, rocket_telemetry->satellites_nb);
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
static inline void mavlink_msg_rocket_telemetry_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint32_t time_boot_ms, int32_t lat, int32_t lon, int32_t gps_alt, float pressure_hpa, int16_t roll, int16_t pitch, int16_t yaw, int16_t temp_celsius, int16_t acc_x, int16_t acc_y, int16_t acc_z, uint16_t system_states, uint16_t battery_mv, uint16_t vel, uint16_t cog, uint8_t gps_fix, uint8_t satellites_nb)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, time_boot_ms);
    _mav_put_int32_t(buf, 4, lat);
    _mav_put_int32_t(buf, 8, lon);
    _mav_put_int32_t(buf, 12, gps_alt);
    _mav_put_float(buf, 16, pressure_hpa);
    _mav_put_int16_t(buf, 20, roll);
    _mav_put_int16_t(buf, 22, pitch);
    _mav_put_int16_t(buf, 24, yaw);
    _mav_put_int16_t(buf, 26, temp_celsius);
    _mav_put_int16_t(buf, 28, acc_x);
    _mav_put_int16_t(buf, 30, acc_y);
    _mav_put_int16_t(buf, 32, acc_z);
    _mav_put_uint16_t(buf, 34, system_states);
    _mav_put_uint16_t(buf, 36, battery_mv);
    _mav_put_uint16_t(buf, 38, vel);
    _mav_put_uint16_t(buf, 40, cog);
    _mav_put_uint8_t(buf, 42, gps_fix);
    _mav_put_uint8_t(buf, 43, satellites_nb);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_ROCKET_TELEMETRY, buf, MAVLINK_MSG_ID_ROCKET_TELEMETRY_MIN_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN, MAVLINK_MSG_ID_ROCKET_TELEMETRY_CRC);
#else
    mavlink_rocket_telemetry_t *packet = (mavlink_rocket_telemetry_t *)msgbuf;
    packet->time_boot_ms = time_boot_ms;
    packet->lat = lat;
    packet->lon = lon;
    packet->gps_alt = gps_alt;
    packet->pressure_hpa = pressure_hpa;
    packet->roll = roll;
    packet->pitch = pitch;
    packet->yaw = yaw;
    packet->temp_celsius = temp_celsius;
    packet->acc_x = acc_x;
    packet->acc_y = acc_y;
    packet->acc_z = acc_z;
    packet->system_states = system_states;
    packet->battery_mv = battery_mv;
    packet->vel = vel;
    packet->cog = cog;
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
 * @return  Timestamp (ms)
 */
static inline uint32_t mavlink_msg_rocket_telemetry_get_time_boot_ms(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field lat from rocket_telemetry message
 *
 * @return  Latitude (degE7)
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_lat(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

/**
 * @brief Get field lon from rocket_telemetry message
 *
 * @return  Longitude (degE7)
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_lon(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  8);
}

/**
 * @brief Get field gps_alt from rocket_telemetry message
 *
 * @return  Altitude GPS (mm)
 */
static inline int32_t mavlink_msg_rocket_telemetry_get_gps_alt(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  12);
}

/**
 * @brief Get field pressure_hpa from rocket_telemetry message
 *
 * @return  Pressure (hPa)
 */
static inline float mavlink_msg_rocket_telemetry_get_pressure_hpa(const mavlink_message_t* msg)
{
    return _MAV_RETURN_float(msg,  16);
}

/**
 * @brief Get field roll from rocket_telemetry message
 *
 * @return  Roll (centi-deg)
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_roll(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  20);
}

/**
 * @brief Get field pitch from rocket_telemetry message
 *
 * @return  Pitch (centi-deg)
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_pitch(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  22);
}

/**
 * @brief Get field yaw from rocket_telemetry message
 *
 * @return  Yaw (centi-deg)
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_yaw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  24);
}

/**
 * @brief Get field temp_celsius from rocket_telemetry message
 *
 * @return  Temperature (centi-deg C)
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_temp_celsius(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  26);
}

/**
 * @brief Get field acc_x from rocket_telemetry message
 *
 * @return  Acceleration X (centi-G)
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_acc_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  28);
}

/**
 * @brief Get field acc_y from rocket_telemetry message
 *
 * @return  Acceleration Y (centi-G)
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_acc_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  30);
}

/**
 * @brief Get field acc_z from rocket_telemetry message
 *
 * @return  Acceleration Z (centi-G)
 */
static inline int16_t mavlink_msg_rocket_telemetry_get_acc_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  32);
}

/**
 * @brief Get field system_states from rocket_telemetry message
 *
 * @return  System states
 */
static inline uint16_t mavlink_msg_rocket_telemetry_get_system_states(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  34);
}

/**
 * @brief Get field battery_mv from rocket_telemetry message
 *
 * @return  Battery voltage (mV)
 */
static inline uint16_t mavlink_msg_rocket_telemetry_get_battery_mv(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  36);
}

/**
 * @brief Get field vel from rocket_telemetry message
 *
 * @return  Velocity (cm/s)
 */
static inline uint16_t mavlink_msg_rocket_telemetry_get_vel(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  38);
}

/**
 * @brief Get field cog from rocket_telemetry message
 *
 * @return  Course over ground (cd)
 */
static inline uint16_t mavlink_msg_rocket_telemetry_get_cog(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  40);
}

/**
 * @brief Get field gps_fix from rocket_telemetry message
 *
 * @return  GPS fix type
 */
static inline uint8_t mavlink_msg_rocket_telemetry_get_gps_fix(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  42);
}

/**
 * @brief Get field satellites_nb from rocket_telemetry message
 *
 * @return  Number of satellites
 */
static inline uint8_t mavlink_msg_rocket_telemetry_get_satellites_nb(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  43);
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
    rocket_telemetry->pressure_hpa = mavlink_msg_rocket_telemetry_get_pressure_hpa(msg);
    rocket_telemetry->roll = mavlink_msg_rocket_telemetry_get_roll(msg);
    rocket_telemetry->pitch = mavlink_msg_rocket_telemetry_get_pitch(msg);
    rocket_telemetry->yaw = mavlink_msg_rocket_telemetry_get_yaw(msg);
    rocket_telemetry->temp_celsius = mavlink_msg_rocket_telemetry_get_temp_celsius(msg);
    rocket_telemetry->acc_x = mavlink_msg_rocket_telemetry_get_acc_x(msg);
    rocket_telemetry->acc_y = mavlink_msg_rocket_telemetry_get_acc_y(msg);
    rocket_telemetry->acc_z = mavlink_msg_rocket_telemetry_get_acc_z(msg);
    rocket_telemetry->system_states = mavlink_msg_rocket_telemetry_get_system_states(msg);
    rocket_telemetry->battery_mv = mavlink_msg_rocket_telemetry_get_battery_mv(msg);
    rocket_telemetry->vel = mavlink_msg_rocket_telemetry_get_vel(msg);
    rocket_telemetry->cog = mavlink_msg_rocket_telemetry_get_cog(msg);
    rocket_telemetry->gps_fix = mavlink_msg_rocket_telemetry_get_gps_fix(msg);
    rocket_telemetry->satellites_nb = mavlink_msg_rocket_telemetry_get_satellites_nb(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN? msg->len : MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN;
        memset(rocket_telemetry, 0, MAVLINK_MSG_ID_ROCKET_TELEMETRY_LEN);
    memcpy(rocket_telemetry, _MAV_PAYLOAD(msg), len);
#endif
}
