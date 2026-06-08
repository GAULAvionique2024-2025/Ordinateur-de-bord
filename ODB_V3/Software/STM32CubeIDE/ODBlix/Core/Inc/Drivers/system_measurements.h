/*
 * system_measurements.h
 *
 *  Created on: 2 mars 2026
 *      Author: AudaceLol12
 */

#ifndef INC_DRIVERS_LOWLEVEL_SYSTEM_MEASUREMENTS_H_
#define INC_DRIVERS_LOWLEVEL_SYSTEM_MEASUREMENTS_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


#define DMA_INDEX_SIZE			10		// Number of ranks

#define MAX6612MXK_MAX_TEMP_C 	150.0f
#define MAX6612MXK_MIN_TEMP_C 	-55.0f

#define VIN_BATT_MAX_MV 		24000
#define VIN_BATT_MIN_MV 		5000
#define V5_MAX_MV       		5500
#define V5_MIN_MV       		4500
#define V3_MAX_MV       		3500
#define V3_MIN_MV       		3100


typedef struct {
	ADC_HandleTypeDef 	*hadc;
	TIM_HandleTypeDef	*htim;
	GPIO_TypeDef        *pg_port;
	uint16_t            pg_pin;
	volatile bool 		dma_ready;

	float 				internal_temperature;
	float 				vdda;
	float				adc_step_compensation;

    float 				temperature;
    uint16_t 			vin_batt;
    uint16_t 			v5_buck;
    uint16_t 			v3_buck;
    uint16_t			pg_v5;
    //uint16_t 			pyros_arming;
    uint16_t 			pyro_status[4];
} system_measurements_t;


int8_t SystemMeasurements_Init(system_measurements_t *dev);

void SystemMeasurements_UpdateInternalCalibration(system_measurements_t *dev);
void SystemMeasurements_ComputePower(system_measurements_t *dev);
void SystemMeasurements_ComputeTemperature(system_measurements_t *dev);
void SystemMeasurements_ComputePyros(system_measurements_t *dev);

#endif /* INC_GAUL_DRIVERS_LOWLEVEL_SYSTEM_MEASUREMENTS_H_ */
