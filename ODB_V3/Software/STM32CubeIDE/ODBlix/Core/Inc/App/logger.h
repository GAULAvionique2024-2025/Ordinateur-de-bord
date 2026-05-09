/*
 * logger.h
 *
 *  Created on: 8 mai 2026
 *      Author: gagno
 */

#ifndef INC_APP_LOGGER_H_
#define INC_APP_LOGGER_H_


#include "GAUL_Drivers/w25q512jv.h"
#include "GAUL_Drivers/system.h"
#include <stdint.h>
#include <stdbool.h>

#define LOG_BUFFER_SIZE 2 // Number of stored data (2 x 118 bytes -> near to 256)

typedef struct {
    uint32_t magic_number;   // To identify the start of a valid header
    uint32_t flight_id;      // Incremented each time
    uint32_t time_date;
    uint32_t metadata_rsv;   // Reserved for future use (ex: new functionnality)
} flight_header_t;

void Logger_Init(void);

void Logger_PushData(odb_data_t *new_data);
void Logger_Task(void);
odb_data_t Logger_GetLastFlightData(void);

#endif /* INC_APP_LOGGER_H_ */
