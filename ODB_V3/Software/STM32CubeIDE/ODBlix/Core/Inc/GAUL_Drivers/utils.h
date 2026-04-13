/*
 * utils.h
 *
 *  Created on: 2 mars 2026
 *      Author: AudaceLol12
 */

#ifndef INC_GAUL_DRIVERS_UTILS_H_
#define INC_GAUL_DRIVERS_UTILS_H_

#include "bno055.h"
#include "hm11.h"
#include "ms5611.h"
#include "stm32f4xx_hal.h"
#include "adxl382.h"
#include "bno055.h"
#include "l76lm33.h"
#include "ltste682krkgwt.h"
#include "pyros.h"
#include "rfd900x.h"
#include "smtb0927twr.h"
#include "system_measurements.h"
#include "mavlink/odb_mavlink_v1/mavlink.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>



typedef struct {
	uint32_t  	start_time;
	uint32_t  	elapsed_time_ms;
	uint8_t		elapsed_time_s;
	uint16_t  	elapsed_time_m;
	uint16_t  	elapsed_time_remaining_ms;
} runTimer_t;

// state
#define ODB_STATE_PREFLIGHT     0  			// 00
#define ODB_STATE_READY         1  			// 01
#define ODB_STATE_INFLIGHT      2  			// 10
#define ODB_STATE_POSTFLIGHT    3  			// 11
#define MASK_STATUS_ODB         (3 << 14) 	// Reset state

#define FLAG_STATUS_ODB(state)  (((state) & 3) << 14)
#define FLAG_PYROS_CONN_OK      (1 << 13)
#define FLAG_GPS_OK             (1 << 12)
#define FLAG_BARO_OK            (1 << 11)
#define FLAG_IMU_OK             (1 << 10)
#define FLAG_HIGHG_OK           (1 << 9)
#define FLAG_RADIO_OK           (1 << 8)
#define FLAG_TEMP_OK            (1 << 7)
#define FLAG_SD_OK              (1 << 6)
#define FLAG_IDEFIX_OK          (1 << 5)
#define FLAG_PYROS_ARMED        (1 << 4)
#define FLAG_PYRO1_FIRED        (1 << 3)
#define FLAG_PYRO2_FIRED        (1 << 2)
#define FLAG_PYRO3_FIRED        (1 << 1)
#define FLAG_PYRO4_FIRED        (1 << 0)

/* === ODB DATA === */
typedef struct {
    // Status
    uint32_t    time_boot_ms;   // Timestamp since system boot in milliseconds (ms)
    uint16_t    system_states;  // Current system state or flight phase (bitmask/enum)
    uint16_t    battery_mv;     // Main battery voltage in millivolts (mV)
    // IMU (Attitude & Rates)
    float       roll;           // Roll angle in degrees (converted to cdeg for MAVLink)
    float       pitch;          // Pitch angle in degrees (converted to cdeg for MAVLink)
    float       yaw;            // Yaw angle in degrees between -180 and 180 (converted to cdeg for MAVLink)
    float       imu_acc_x;      // IMU Acceleration X in m/s^2 (converted to cm/s^2 for MAVLink)
    float       imu_acc_y;      // IMU Acceleration Y in m/s^2 (converted to cm/s^2 for MAVLink)
    float       imu_acc_z;      // IMU Acceleration Z in m/s^2 (converted to cm/s^2 for MAVLink)
    float       imu_gyro_x;     // IMU Angular rate X in deg/s (converted to cdeg/s for MAVLink)
    float       imu_gyro_y;     // IMU Angular rate Y in deg/s (converted to cdeg/s for MAVLink)
    float       imu_gyro_z;     // IMU Angular rate Z in deg/s (converted to cdeg/s for MAVLink)
    // Pressure & Temp
    float       pressure_hpa;   // Atmospheric pressure in hectopascals (hPa)
    float       temp_celsius;   // Board or environment temperature in Celsius (°C)
    // High-G IMU
    float       highg_acc_x;    // High-G Acceleration X in m/s^2 (converted to cm/s^2 for MAVLink)
    float       highg_acc_y;    // High-G Acceleration Y in m/s^2 (converted to cm/s^2 for MAVLink)
    float       highg_acc_z;    // High-G Acceleration Z in m/s^2 (converted to cm/s^2 for MAVLink)
    // GPS
    uint8_t     gps_fix;        // 1 = Active fix, 0 = Void/No fix
    int32_t     lat;            // Latitude in degE7 (MAVLink format: deg * 10^7)
    int32_t     lon;            // Longitude in degE7 (MAVLink format: deg * 10^7)
    int32_t     gps_alt;        // Altitude (MSL) based on GPS in millimeters (mm)
    uint16_t    vel;            // Ground velocity in centimeters per second (cm/s)
    uint16_t    cog;            // Course Over Ground in centi-degrees (cdeg)
    uint8_t     satellites_nb;  // Number of satellites used for the fix
} odb_data;
/* =========== */

/* === TELEMETRY === */
#define MAVLINK_COMPONENT_ID 1 // air_1 = 1, air_2 = 2

typedef enum {
    ODB_MODEM_BOOSTER       = 1,
    ODB_MODEM_SUSTAINER     = 2,
} odb_modem_id_t;

typedef enum {
    EVENT_SEVERITY_EMERGENCY 	= MAV_SEVERITY_EMERGENCY,
	EVENT_SEVERITY_CRITICAL 	= MAV_SEVERITY_CRITICAL,
	EVENT_SEVERITY_WARNING 		= MAV_SEVERITY_WARNING,
	EVENT_SEVERITY_INFO			= MAV_SEVERITY_INFO,
} odb_event_severity_t;

void Telemetry_SendRocketData(rfd900x_t *rfd_dev, odb_modem_id_t modem_id, odb_data *data, uint32_t current_time_ms);
void Telemetry_SendEventLog(rfd900x_t *rfd_dev, odb_modem_id_t modem_id, odb_event_severity_t severity, const char *text);
/* =========== */

/* === APP DEBUG - BLUETOOTH === */
#define APP_BLE_NAME "ODB_1"
/* =========== */

/* === HELPERS === */
void RunTimer_Init(runTimer_t *dev);
void RunTimer_Update(runTimer_t *dev);
int printt(const char *format, ...);
int _write(int le, char *ptr, int len);

#endif /* INC_GAUL_DRIVERS_UTILS_H_ */
