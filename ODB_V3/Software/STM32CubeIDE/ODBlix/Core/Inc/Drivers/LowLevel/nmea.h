/*
 * NMEA.h
 *
 * Created on: May 12, 2024
 * Author: gagnon
 *
 * Edited on: Jul 4, 2024
 * Autor: mathouqc
 *
 * Edited on: Mar 02, 2026
 * Autor: AudaceLol12
 */

#ifndef INC_DRIVERS_LOWLEVEL_NMEA_H
#define INC_DRIVERS_LOWLEVEL_NMEA_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>


typedef struct {
    uint8_t hours;      // Hours when GPS fix acquired (two digits)
    uint8_t minutes;    // Minutes when GPS fix acquired (two digits)
    float seconds;      // Seconds when GPS fix acquired
} nmea_time_t;

typedef struct {
    uint32_t    time_raw;       // HHMMSS raw format
    nmea_time_t time;           // Time when GPS fix acquired
    uint8_t     gps_fix;        // 1 = Active fix, 0 = Void/No fix
    int32_t     lat;            // Latitude in degE7 (MAVLink format: deg * 10^7)
    int32_t     lon;            // Longitude in degE7 (MAVLink format: deg * 10^7)
    int32_t     gps_alt;        // Altitude based on GPS (Mavlink format: mm)
    uint16_t    vel;            // Vitesse au sol en cm/s
    uint16_t    cog;            // Cap au sol en centi-degrés
    uint8_t     satellites_nb;  // Number of satellites used for the fix
} nmea_t;

int8_t NMEA_ValidateRMC(const char *nmea_sentence);
int8_t NMEA_ParseRMC(nmea_t *gps_data, const char *nmea_sentence);

int8_t NMEA_ValidateGGA(const char *nmea_sentence);
int8_t NMEA_ParseGGA(nmea_t *gps_data, const char *nmea_sentence);

#endif /* INC_GAUL_DRIVERS_LOWLEVEL_NMEA_H */
