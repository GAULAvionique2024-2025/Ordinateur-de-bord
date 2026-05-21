/*
 * NMEA.c
 *
 * Module to parse the time, latitude/longitude, altitude and satellites 
 * from RMC and GGA NMEA sentences.
 *
 * Created on: May 12, 2024
 * Author: AudaceLol12
 *
 * Edited on: Jul 04, 2024
 * Autor: mathouqc
 *
 * Edited on: Mar 02, 2026
 * Autor: AudaceLol12
 */

#include "GAUL_Drivers/LowLevel/nmea.h"

#define NODE_TO_CMPS 51.444f

// ========================= RMC PARSING ==============================

/**
 * Validate the NMEA sentence ID is RMC ($xxRMC).
 *
 * @param nmea_sentence: pointer to sentence array.
 *
 * @retval 0 RMC sentence
 * @retval -1 Not a RMC sentence
 */
int8_t NMEA_ValidateRMC(const char *nmea_sentence) {
    if (!nmea_sentence) return -1;
    return strncmp(nmea_sentence + 3, "RMC", 3) == 0 ? 0 : -1;
}

/**
 * Parse NMEA RMC sentence ($xxRMC).
 * $GNRMC,080608.000,A,3029.461489,N,11430.072002,E,0.00,148.41,210423,,,D,V*09
 *
 * @param nmea_sentence: pointer to sentence array.
 * @param gps_data: pointer to structure to fill with parsed data.
 *
 * @retval 0 OK
 * @retval -1 ERROR
 */
int8_t NMEA_ParseRMC(nmea_t *gps_data, const char *nmea_sentence) {
    if(!gps_data || !nmea_sentence) {
        return -1; 
    }

    char copy[NMEA_MAX_RMC_LENGTH + 1];
    strncpy(copy, nmea_sentence, NMEA_MAX_RMC_LENGTH);
    copy[NMEA_MAX_RMC_LENGTH] = '\0';

    uint8_t token_idx = 0; 
    char *token = copy;
    char *next_token;

    while(token != NULL && token_idx < NMEA_MAX_TOKEN_TO_READ_RMC) {
        next_token = strchr(token, ',');
        if(next_token) {
            *next_token = '\0';
            next_token++;
        }

        if(token_idx == 1) { // TIME
            if(strlen(token) >= 6) {
                char time_raw_str[7] = {0};
                strncpy(time_raw_str, token, 6);
                gps_data->time_raw = atoi(time_raw_str);

                char hours[3] = {token[0], token[1], '\0'};
                char mins[3]  = {token[2], token[3], '\0'};
                
                gps_data->time.hours = atoi(hours);
                gps_data->time.minutes = atoi(mins);
                gps_data->time.seconds = atof(token + 4);
            } 
        } else if(token_idx == 2) { // GPS FIX
            if(token[0] == 'A') {
                gps_data->gps_fix = 1; // FIX MAVLink
            } else {
                gps_data->gps_fix = 0; // NO FIX MAVLink
                gps_data->lat = 0.0f;
                gps_data->lon = 0.0f;
                gps_data->vel = 0.0f;
                gps_data->cog = 0.0f;
                break;
            }   
        } else if(token_idx == 3) { // LATITUDE
            if(strlen(token) >= 4) {
                char degrees[3] = {token[0], token[1], '\0'};
                double deg = atof(degrees);
                double min = atof(token + 2);
                gps_data->lat = (int32_t)((deg + min / 60.0f) * 10000000.0f);
            }  
        } else if(token_idx == 4) { // LAT INDICATOR
            if(token[0] == 'S') gps_data->lat = -gps_data->lat;
        } else if(token_idx == 5) { // LONGITUDE
            if(strlen(token) >= 5) {
                char degrees[4] = {token[0], token[1], token[2], '\0'};
                double deg = atof(degrees);
                double min = atof(token + 3);
                gps_data->lon = (int32_t)((deg + min / 60.0f) * 10000000.0f);
            }  
        } else if(token_idx == 6) { // LON INDICATOR
            if(token[0] == 'W') gps_data->lon = -gps_data->lon;
        } 
        else if(token_idx == 7) { // SPEED OVER GROUND (node -> cm/s)
            if(token[0] != '\0') {
                double knots = atof(token);
                gps_data->vel = (uint16_t)(knots * NODE_TO_CMPS); 
            } else {
                gps_data->vel = 0;
            }
        } else if(token_idx == 8) { // COURSE OVER GROUND (degrees -> centi-degrees)
            if(token[0] != '\0') {
                double degrees = atof(token);
                gps_data->cog = (uint16_t)(degrees * 100.0f);
            } else {
                gps_data->cog = 0;
            }
        }

        token = next_token;
        token_idx++;
    }

    return 0;
}


// ========================= GGA PARSING ==============================

/**
 * Validate the NMEA sentence ID is GGA ($xxGGA).
 *
 * @param nmea_sentence: pointer to sentence array.
 *
 * @retval 0 GGA sentence
 * @retval -1 Not a GGA sentence
 */
int8_t NMEA_ValidateGGA(const char *nmea_sentence) {
    if (!nmea_sentence) return -1;
    return strncmp(nmea_sentence + 3, "GGA", 3) == 0 ? 0 : -1;
}

/**
 * Parse NMEA GGA sentence ($xxGGA).
 * $GNGGA,080608.000,3029.461489,N,11430.072002,E,1,08,1.0,15.2,M,0.0,M,,*xx
 *
 * @param nmea_sentence: pointer to sentence array.
 * @param gps_data: pointer to structure to fill with parsed data.
 *
 * @retval 0 OK
 * @retval -1 ERROR
 */
int8_t NMEA_ParseGGA(nmea_t *gps_data, const char *nmea_sentence) {
    if(!gps_data || !nmea_sentence) {
        return -1; 
    }

    char copy[NMEA_MAX_GGA_LENGTH + 1];
    strncpy(copy, nmea_sentence, NMEA_MAX_GGA_LENGTH);
    copy[NMEA_MAX_GGA_LENGTH] = '\0';

    uint8_t token_idx = 0; 
    char *token = copy;
    char *next_token;

    while(token != NULL && token_idx < NMEA_MAX_TOKEN_TO_READ_GGA) {
        next_token = strchr(token, ',');
        if(next_token) {
            *next_token = '\0';
            next_token++;
        }

        if(token_idx == 6) { 
            if(token[0] == '0' || token[0] == '\0') {
                gps_data->gps_fix = 0; // NO FIX MAVLink
                gps_data->satellites_nb = 0;
                break;
            } else {
            	gps_data->gps_fix = atoi(token); // FIX MAVLink
            }
        } else if(token_idx == 7) { // SATELLITES USED (00-12+)
            if(token[0] != '\0') {
                gps_data->satellites_nb = atoi(token);
            }
        } else if(token_idx == 9) { // MSL ALTITUDE (m)
            if(token[0] != '\0') {
                double alt_meters = atof(token);
                // MAVLink int32_t (mm)
                gps_data->gps_alt = (int32_t)(alt_meters * 1000.0f);
            }
        }

        token = next_token;
        token_idx++;
    }

    return 0;
}
