/*
 * RFD900.h
 *
 *  Created on: Feb 19, 2024
 *      Author: gagno
 *
 *  Edited on: Mar 02, 2026
 *  	Author: AudaceLol12
 */

#ifndef INC_GAUL_DRIVERS_RFD900X_H_
#define INC_GAUL_DRIVERS_RFD900X_H_

#include "stm32f4xx_hal.h"
#include "mavlink/common/mavlink.h"

#include <stdint.h>

// TODO: move to global struct
// state
#define ODB_STATE_PREFLIGHT     0  // 00
#define ODB_STATE_READY         1  // 01
#define ODB_STATE_INFLIGHT      2  // 10
#define ODB_STATE_POSTFLIGHT    3  // 11
#define MASK_STATUS_ODB         (3 << 14) // Reset state

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

#define MAV_COMPONENT_ID 		1

typedef enum {
    RFD900X_MODEM_BOOSTER       = 1,
    RFD900X_MODEM_SUSTAINER     = 2,
} rfd900x_modem_id_t;

typedef struct {
    UART_HandleTypeDef  *huart;
    rfd900x_modem_id_t  modem_id;
    uint8_t             component_id;   // main=1
} RFD900x_HandleTypeDef;

// TODO: move to global struct
typedef struct {
	// Status
	uint16_t	system_states;
	uint16_t	battery_mv;
	// IMU
    float 		roll;
    float 		pitch;
    float 		yaw;
    // Pressure & Temp
    float   	pressure_hpa;
    float   	temp_celsius;	// 2 decimals
    // High-G
    float 		acc_x;
    float 		acc_y;
    float 		acc_z;
    // GPS
    uint8_t     gps_fix;
    int32_t 	lat;			// Latitude
    int32_t 	lon;       		// Longitude
    int32_t 	gps_alt;        // Altitude based on GPS
    uint8_t 	satellites_nb;
} odb_mavlink_data;

int8_t RFD900x_Init(RFD900x_HandleTypeDef *hrfd, UART_HandleTypeDef *huart, rfd900x_modem_id_t modem_id, uint8_t component_id);

void RFD900x_Send(RFD900x_HandleTypeDef *hrfd, odb_mavlink_data *data, uint32_t current_time_ms);
void RFD900x_Send_EventLog(RFD900x_HandleTypeDef *hrfd, uint8_t severity, const char *log);

#endif /* INC_GAUL_DRIVERS_RFD900X_H_ */
