/*
 * system.h
 *
 *  Created on: 3 mai 2026
 *      Author: gagno
 */

#ifndef INC_ODB_H_
#define INC_ODB_H_


#include "Drivers/adxl382.h"
#include "Drivers/bno055.h"
#include "Drivers/hm11.h"
#include "Drivers/idefix.h"
#include "Drivers/l76lm33.h"
#include "Drivers/ltste682krkgwt.h"
#include "Utils/dwt.h"
#include "Drivers/LowLevel/kalman_nav.h"
#include "Drivers/ms5611.h"
#include "Drivers/pyros.h"
#include "Drivers/rfd900x.h"
#include "Drivers/smtb0927twr.h"
#include "Drivers/system_measurements.h"
#include "Drivers/w25q512jv.h"
#include "mavlink/odb_mavlink_v1/mavlink.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <assert.h>
#include <Protocols/odb_protocol.h>


typedef enum {
    ODB_OK                  =  0,
    ODB_WARNING             = -1,
    ODB_ALIMENTATION_ERROR  = -2,
    ODB_ERROR               = -3,
} odb_state_t;

// TODO: add functions to set system states and event states based on sensors data and pyros state to convert them into packed bitfields for telemetry transmission
odb_state_t ODB_Init(odb_data_t *data, odb_stats_t *stats);
void ODB_Reset(odb_data_t *data, odb_stats_t *stats);
void ODB_Update(odb_data_t *data, odb_stats_t *stats);
uint16_t ODB_SetEventStates(const odb_stats_t *stats);
int8_t ODB_SetMissionState(odb_data_t *data, uint8_t mission_state);
uint8_t ODB_GetPyroStates(const odb_data_t *data);
/* =========== */

/* === TELEMETRY === */
void Telemetry_SendRocketData(rfd900x_t *rfd_dev, const mavlink_modem_id_t modem_id, odb_data_t *data, const uint32_t current_time_ms);
//void Telemetry_SendEventLog(rfd900x_t *rfd_dev, const mavlink_modem_id_t modem_id, const mavlink_event_severity_t severity, const char *text);
/* =========== */

/* === BEACON INTEGRATION === */
void Beacon_SendCoordinates(idefix_t *idefix_dev, const int32_t lat_e7, const int32_t lon_e7);
void Beacon_SetFrequency(idefix_t *idefix_dev);
/* =========== */

#endif /* INC_ODB_H_ */
