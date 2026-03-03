/*
 * NMEA.h
 *
 *  Created on: May 12, 2024
 *      Author: gagnon
 *
 *  Edited on: Jul 4, 2024
 *      Autor: mathouqc
 *
 *  Edited on: Mar 02, 2026
 *      Autor: AudaceLol12
 */

#ifndef INC_GAUL_DRIVERS_LOWLEVEL_NMEA_H
#define INC_GAUL_DRIVERS_LOWLEVEL_NMEA_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#define NMEA_MAX_TOKEN_TO_READ 	7
#define NMEA_MAX_RMC_LENGTH 	90

typedef enum {
	NMEA_ERROR = -1,
	NMEA_OK    =  0
} nmea_state_t;

typedef struct {
    uint8_t hours;      // Hours when GPS fix acquired (two digits)
    uint8_t minutes;    // Minutes when GPS fix acquired (two digits)
    float seconds;      // Seconds when GPS fix acquired
} nmea_time_t;

typedef struct {
	nmea_time_t time;        // Time when GPS fix acquired
    uint32_t time_raw;
    uint8_t fix;         // 1: GPS Fix, 0: No GPS Fix
    float latitude;     // Latitude in Decimal Degrees
    float longitude;    // Longitude in Decimal Degrees
} nmea_t;

nmea_state_t NMEA_ValidateRMC(const char *nmea_sentence);
nmea_state_t NMEA_ParseRMC(nmea_t *gps_data, const char *nmea_sentence);

#endif /* INC_GAUL_DRIVERS_LOWLEVEL_NMEA_H */
