/*
 * system.h
 *
 *  Created on: 3 mai 2026
 *      Author: gagno
 */

#ifndef INC_GAUL_DRIVERS_SYSTEM_H_
#define INC_GAUL_DRIVERS_SYSTEM_H_

#include "stm32f4xx_hal.h"
#include "bno055.h"
#include "hm11.h"
#include "ms5611.h"
#include "adxl382.h"
#include "l76lm33.h"
#include "ltste682krkgwt.h"
#include "pyros.h"
#include "rfd900x.h"
#include "smtb0927twr.h"
#include "system_measurements.h"
#include "w25q512jv.h"
#include "mavlink/odb_mavlink_v1/mavlink.h"
#include "Nexus/nexus.h"
#include "GAUL_Drivers/LowLevel/kalman_nav.h"
#include "GAUL_Drivers/LowLevel/dwt.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>


/* === ODB === */
// Init system_states
/*
 * [15] : IdeFIX communication OK
 * [14] : Bluetooth module OK
 * [13] : Flash memory OK
 * [12] : SD card OK
 * [11] : Temperature sensor OK
 * [10] : High-G accelerometer OK
 * [9]  : GPS OK
 * [8]  : Barometer OK
 * [7]  : IMU OK
 * [6]  : Radio OK
 * [5]  : Pyros armed OK
 * [4]  : Pyro arming module OK
 * [3]  : Pyro 1 connected
 * [2]  : Pyro 2 connected
 * [1]  : Pyro 3 connected
 * [0]  : Pyro 4 connected
*/
#define FLAG_IDEFIX_OK                  (1 << 14)
#define FLAG_BT_OK                      (1 << 13)
#define FLAG_FLASH_OK                   (1 << 12)
#define FLAG_SD_OK                      (1 << 11)
#define FLAG_TEMP_OK                    (1 << 10)
#define FLAG_HIGHG_OK                   (1 << 9)
#define FLAG_GPS_OK                     (1 << 8)
#define FLAG_BARO_OK                    (1 << 7)
#define FLAG_IMU_OK                     (1 << 6)
#define FLAG_RADIO_OK                   (1 << 5)
#define FLAG_PYROS_ARMED_OK             (1 << 4)
#define FLAG_PYRO1_CONN                 (1 << 3)
#define FLAG_PYRO2_CONN                 (1 << 2)
#define FLAG_PYRO3_CONN                 (1 << 1)
#define FLAG_PYRO4_CONN                 (1 << 0)
// Events states
/*
 * [7]  : Mach lock enabled
 * [6]  : Drogue deployed
 * [5]  : Main deployed
 * [4]  : Apogee detected
 * [3]  : Pyro 4 fired
 * [2]  : Pyro 3 fired
 * [1]  : Pyro 2 fired
 * [0]  : Pyro 1 fired
*/
#define FLAG_PYRO1_FIRED                (1 << 0)
#define FLAG_PYRO2_FIRED                (1 << 1)
#define FLAG_PYRO3_FIRED                (1 << 2)
#define FLAG_PYRO4_FIRED                (1 << 3)
#define FLAG_APOGEE_DETECTED            (1 << 4)
#define FLAG_MAIN_DEPLOYED              (1 << 5)
#define FLAG_DROGUE_DEPLOYED            (1 << 6)
#define FLAG_MACH_LOCK_ENABLED          (1 << 7)

typedef enum {
    ODB_OK                  =  0,
    ODB_WARNING             = -1,
    ODB_ALIMENTATION_ERROR  = -2,
    ODB_ERROR               = -3,
} odb_state_t;

// TODO: maybe use RTC + POSIX timestamps instead of time since boot
// pyro event stucture with time of event and pyro number
typedef struct {
    bool fired;
    uint32_t time_ms;
} pyro_event_t;

// metrics structure with value and time of occurrence
typedef struct {
    bool valid;
    float value;
    uint32_t time_ms;
} metric_t;

// states of the events for statistics reporting
typedef struct {
    pyro_event_t pyro1;				// in event state
    pyro_event_t pyro2;				// in event state
    pyro_event_t pyro3;				// in event state
    pyro_event_t pyro4;				// in event state
    pyro_event_t mach_lock;			// in event state

    metric_t max_altitude_gps;      // internal event for POSTFLIGHT reporting (save in FLASH/SD)
    metric_t max_altitude_baro;     // internal event for POSTFLIGHT reporting (save in FLASH/SD)
    metric_t max_altitude_kalman;   // internal event for POSTFLIGHT reporting (save in FLASH/SD)

    metric_t apogee;				// in event state
    metric_t main_deploy;			// in event state
    metric_t drogue_deploy;			// in event state

    metric_t max_ascend_speed;      // internal event for POSTFLIGHT reporting (save in FLASH/SD)
    metric_t max_ascend_accel;      // internal event for POSTFLIGHT reporting (save in FLASH/SD)
    metric_t max_descend_speed;     // internal event for POSTFLIGHT reporting (save in FLASH/SD)
    metric_t max_descend_accel;     // internal event for POSTFLIGHT reporting (save in FLASH/SD)

    uint32_t flight_time_ms;        // internal event for POSTFLIGHT reporting (save in FLASH/SD)
} odb_stats_t;

// Main ODB data structure to be sent via telemetry
typedef struct __attribute__((packed)) {
    // Status
    uint32_t    time_boot_ms;       // Timestamp since system boot in milliseconds (ms)
    uint16_t    system_states;      // Current system/component states -> linked with odb_stats_t
    uint8_t     event_states;       // Current events states (pyros fired, apogee detected, etc.) -> linked with odb_stats_t
    uint8_t     mission_state;      // Mission state (preflight, inflight, postflight) -> Linked with FSM
    uint16_t    battery_mv;         // Main battery voltage in millivolts (mV)
    // IMU (Attitude & Rates)
    float       roll;               // Roll angle in degrees (converted to cdeg for MAVLink) -> Linked with BNO055
    float       pitch;              // Pitch angle in degrees (converted to cdeg for MAVLink) -> Linked with BNO055
    float       yaw;                // Yaw angle in degrees between -180 and 180 (converted to cdeg for MAVLink) -> Linked with BNO055
    float       imu_acc_x;          // IMU Acceleration X in m/s^2 (converted to cm/s^2 for MAVLink) -> Linked with BNO055
    float       imu_acc_y;          // IMU Acceleration Y in m/s^2 (converted to cm/s^2 for MAVLink) -> Linked with BNO055
    float       imu_acc_z;          // IMU Acceleration Z in m/s^2 (converted to cm/s^2 for MAVLink) -> Linked with BNO055
    float       imu_gyro_x;         // IMU Angular rate X in deg/s (converted to cdeg/s for MAVLink) -> Linked with BNO055
    float       imu_gyro_y;         // IMU Angular rate Y in deg/s (converted to cdeg/s for MAVLink) -> Linked with BNO055
    float       imu_gyro_z;         // IMU Angular rate Z in deg/s (converted to cdeg/s for MAVLink) -> Linked with BNO055
    float		imu_mag_x;			// IMU Magnetometer X in uT (converted to cuT for MAVLink) -> Linked with BNO055
    float		imu_mag_y;			// IMU Magnetometer Y in uT (converted to cuT for MAVLink) -> Linked with BNO055
    float		imu_mag_z;			// IMU Magnetometer Z in uT (converted to cuT for MAVLink) -> Linked with BNO055
    // Pressure & Temp
    float		altitude_msl_m;		// Altitude in meters (m) from barometer referenced with the sea level (MLS)-> Linked with MS5611
    float       pressure_hpa;       // Atmospheric pressure in hectopascals (hPa) -> Linked with MS5611
    float       temp_celsius;       // Board or environment temperature in Celsius (°C) -> Linked with MAX6612MXK
    // High-G Acclerometer
    float       highg_acc_x;        // High-G Acceleration X in m/s^2 (converted to cm/s^2 for MAVLink) -> Linked with ADXL382
    float       highg_acc_y;        // High-G Acceleration Y in m/s^2 (converted to cm/s^2 for MAVLink) -> Linked with ADXL382
    float       highg_acc_z;        // High-G Acceleration Z in m/s^2 (converted to cm/s^2 for MAVLink) -> Linked with ADXL382
    // GPS
    uint8_t     gps_fix;            // 1 = Active fix, 0 = Void/No fix -> Linked with L76LM33
    int32_t     lat;                // Latitude in degE7 (MAVLink format: deg * 10^7) -> Linked with L76LM33
    int32_t     lon;                // Longitude in degE7 (MAVLink format: deg * 10^7) -> Linked with L76LM33
    int32_t     gps_alt;            // Altitude (MSL) based on GPS in millimeters (mm) -> Linked with L76LM33
    uint16_t    vel;                // Ground velocity in centimeters per second (cm/s) -> Linked with L76LM33
    uint16_t    cog;                // Course Over Ground in centi-degrees (cdeg) -> Linked with L76LM33
    uint8_t     satellites_nb;      // Number of satellites used for the fix -> Linked with L76LM33

    // Statistics
    float       imu_acc_vertical;   // Vertical acceleration (World frame) from IMU in m/s^2 -> Linked with BNO055
    float       highg_acc_vertical; // Vertical acceleration (World frame) from High-G Accelerometer in m/s^2 -> Linked with ADXL382
    float       kalman_z;           // filtered altitude from Kalman filter in m (converted to cm for MAVLink)
    float       kalman_v;           // filtered velocity from Kalman filter in m/s (converted to cm/s for MAVLink)

    // TOTAL 116
    // Empty data to reach 128 bytes
    uint8_t padding[12];
} odb_data_t;
//_Static_assert(sizeof(odb_data_t) == 128, "odb_data_t_size_error");

// TODO: add functions to set system states and event states based on sensors data and pyros state to convert them into packed bitfields for telemetry transmission
odb_state_t ODB_Init(odb_data_t *data);
void ODB_Reset(odb_data_t *data);
void ODB_Update(odb_data_t *data);
uint8_t ODB_SetEventStates(const odb_stats_t *stats);
int8_t ODB_SetMissionState(odb_data_t *data, uint8_t mission_state);
uint8_t ODB_GetPyroStates(const odb_data_t *data);
/* =========== */

/* === TELEMETRY === */

void Telemetry_SendRocketData(rfd900x_t *rfd_dev, const mavlink_modem_id_t modem_id, odb_data_t *data, const uint32_t current_time_ms);
//void Telemetry_SendEventLog(rfd900x_t *rfd_dev, const mavlink_modem_id_t modem_id, const mavlink_event_severity_t severity, const char *text);
/* =========== */

/* === BLUETOOTH APP === */
void App_SendFrame(nexus_t *nexus_dev, hm11_t *hm11_dev, const odb_data_t *data);
void App_HandleCommands(nexus_t *nexus_dev, hm11_t *hm11_dev);
/* =========== */

#endif /* INC_GAUL_DRIVERS_SYSTEM_H_ */
