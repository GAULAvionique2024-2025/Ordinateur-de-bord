/*
 * system_measurements.h
 *
 *  Created on: 2 mars 2026
 *      Author: AudaceLol12
 */

#ifndef INC_GAUL_DRIVERS_LOWLEVEL_SYSTEM_MEASUREMENTS_H_
#define INC_GAUL_DRIVERS_LOWLEVEL_SYSTEM_MEASUREMENTS_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


typedef struct {
	ADC_HandleTypeDef 	*hadc;
	TIM_HandleTypeDef	*htim;

    float 				temperature;
    float 				vin_batt;
    float 				v5_buck;
    float 				v3_buck;
    bool 				pyros_arming;
    bool 				pyro_status[4];
} system_measurements_t;


int8_t SystemMeasurements_Init(system_measurements_t *dev);

void SystemMeasurements_Update(system_measurements_t *dev);

#endif /* INC_GAUL_DRIVERS_LOWLEVEL_SYSTEM_MEASUREMENTS_H_ */
