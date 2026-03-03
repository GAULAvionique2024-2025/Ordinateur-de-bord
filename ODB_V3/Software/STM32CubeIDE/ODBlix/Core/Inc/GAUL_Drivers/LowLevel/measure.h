/*
 * telemetry.h
 *
 *  Created on: 2 mars 2026
 *      Author: AudaceLol12
 */

#ifndef INC_GAUL_DRIVERS_LOWLEVEL_MEASURE_H_
#define INC_GAUL_DRIVERS_LOWLEVEL_MEASURE_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


#define DIV_RATIO_VIN_BATT 	7.6667 		// 27k / (27k + 180k)
#define DIV_RATIO_V5_BUCK  	1.7500		// 180k / (180k + 135k)
#define DIV_RATIO_V3_BUCK	1.1111		// 180k / (180k + 20k)
#define PYROS_THRESHOLD		1800		// 1.5V


typedef struct {
    float temperature;
    float vin_batt;
    float V5_buck;
    float V3_buck;
    bool pyros_arming;
    bool pyro_status[4];
} system_measurements_t;


void Update_System_Measurements(system_measurements_t *dev);

#endif /* INC_GAUL_DRIVERS_LOWLEVEL_MEASURE_H_ */
