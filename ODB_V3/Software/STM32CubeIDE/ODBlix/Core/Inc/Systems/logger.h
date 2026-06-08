/*
 * logger.h
 *
 *  Created on: 8 mai 2026
 *      Author: gagno
 */

#ifndef INC_SYSTEMS_LOGGER_H_
#define INC_SYSTEMS_LOGGER_H_


#include <odb.h>
#include "Drivers/w25q512jv.h"
#include <stdint.h>
#include <stdbool.h>

#define LOG_BUFFER_SIZE 2 // Number of stored data (2 x 118 bytes -> near to 256)


typedef enum {
    LOGGER_SUCCESS = 0,
    LOGGER_ERROR_FLASH_WRITE = -1,
    LOGGER_ERROR_FLASH_ERASE = -2,
} logger_error_t;

typedef struct {
    uint32_t magic_number;   // To identify the start of a valid header
    uint32_t flight_id;      // Incremented each time
    uint32_t time_date;
    uint32_t metadata_rsv;   // Reserved for future use (new functionality)
} logger_header_t;

typedef struct {
    uint32_t magic_number;  // To identify the start of a valid data packet
    odb_data_t data;		// The actual ODB data
} logger_data_t;

typedef struct {
    uint32_t magic_number;  // To identify the start of a valid stats packet
    odb_stats_t stats;      // The actual ODB stats
} logger_stats_t;


int8_t Logger_Init(void);

void Logger_PushData(odb_data_t *new_data);
void Logger_Task(void);
void Logger_SaveStats(const odb_stats_t *stats);
odb_data_t Logger_GetLastFlightData(void);
odb_stats_t Logger_GetLastFlightStats(odb_stats_t *stats);
bool Logger_ResetFlightHistoric(void);

int8_t Logger_Erase(void);

#endif /* INC_SYSTEMS_LOGGER_H_ */
