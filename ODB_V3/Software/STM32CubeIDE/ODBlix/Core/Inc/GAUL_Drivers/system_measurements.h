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
	GPIO_TypeDef        *pg_port;
	uint16_t            pg_pin;
	bool 				dma_ready;

    float 				temperature;
    uint16_t 			vin_batt;
    uint16_t 			v5_buck;
    uint16_t 			v3_buck;
    uint16_t			pg_v5;
    uint16_t 			pyros_arming;
    uint16_t 			pyro_status[4];
} system_measurements_t;


int8_t SystemMeasurements_Init(system_measurements_t *dev);

void SystemMeasurements_ComputePower(system_measurements_t *dev);
void SystemMeasurements_ComputeTemperature(system_measurements_t *dev);
void SystemMeasurements_ComputePyros(system_measurements_t *dev);

#endif /* INC_GAUL_DRIVERS_LOWLEVEL_SYSTEM_MEASUREMENTS_H_ */
