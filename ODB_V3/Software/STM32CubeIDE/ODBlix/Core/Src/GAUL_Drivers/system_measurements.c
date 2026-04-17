/*
 * system_measurements.c
 *
 *  Created on: 2 mars 2026
 *      Author: AudaceLol12
 */

#include "GAUL_Drivers/system_measurements.h"
#include <string.h>


#define DIV_ADC_STEP 		0.00080586	// 3.3 / 4095

#define DIV_RATIO_VIN_BATT 	7.6667 		// 27k / (27k + 180k)
#define DIV_RATIO_V5_BUCK  	1.7500		// 180k / (180k + 135k)
#define DIV_RATIO_V3_BUCK	1.1111		// 180k / (180k + 20k)
#define PYROS_THRESHOLD		1800		// 1.5V


extern uint16_t adc_buffer[9];


int8_t SystemMeasurements_Init(system_measurements_t *dev) {
	if(dev == NULL || dev->hadc == NULL) {
		return -1;
	}

	dev->temperature = 0.00f;
	dev->vin_batt = 0.00f;
	dev->v5_buck = 0.00f;
	dev->v3_buck = 0.00f;
	dev->pg_v5 = (HAL_GPIO_ReadPin(dev->pg_port, dev->pg_pin) == GPIO_PIN_SET);

	memset(dev->pyro_status, 0, sizeof(dev->pyro_status));

	if(HAL_ADC_Start_DMA(dev->hadc, (uint32_t*)adc_buffer, 9) != HAL_OK) {
		return -1;
	}

	HAL_TIM_Base_Start(dev->htim);

	return 0; // success
}

void SystemMeasurements_ComputePower(system_measurements_t *dev) {
    uint16_t vin = adc_buffer[4];
    uint16_t v5  = adc_buffer[5];
    uint16_t v3  = adc_buffer[6];

    dev->vin_batt = (vin * DIV_ADC_STEP) * DIV_RATIO_VIN_BATT;
    dev->v5_buck  = (v5 * DIV_ADC_STEP) * DIV_RATIO_V5_BUCK;
    dev->v3_buck  = (v3 * DIV_ADC_STEP) * DIV_RATIO_V3_BUCK;
}

void SystemMeasurements_ComputeTemperature(system_measurements_t *dev) {
    uint16_t temp = adc_buffer[3];

    float v_temp = temp * DIV_ADC_STEP;
    dev->temperature = (v_temp - 0.40f) / 0.01953f;
}

void SystemMeasurements_ComputePyros(system_measurements_t *dev) {
    uint16_t arm = adc_buffer[0];
    uint16_t p4  = adc_buffer[1];
    uint16_t p1  = adc_buffer[2];
    uint16_t p3  = adc_buffer[7];
    uint16_t p2  = adc_buffer[8];

    dev->pyros_arming = (arm >= PYROS_THRESHOLD);

    dev->pyro_status[0] = (p1 < PYROS_THRESHOLD);
    dev->pyro_status[1] = (p2 < PYROS_THRESHOLD);
    dev->pyro_status[2] = (p3 < PYROS_THRESHOLD);
    dev->pyro_status[3] = (p4 < PYROS_THRESHOLD);
}
