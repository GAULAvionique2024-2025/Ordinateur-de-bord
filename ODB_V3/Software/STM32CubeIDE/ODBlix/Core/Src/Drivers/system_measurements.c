/*
 * system_measurements.c
 *
 *  Created on: 2 mars 2026
 *      Author: AudaceLol12
 */

#include "Drivers/system_measurements.h"
#include <string.h>


// Calibration
#define ADC_THERMAL_DRIFT_PPM  	0.00003f

// Ranks DMA
#define IDX_PYROS_ARMED			0
#define IDX_PYRO4				1
#define IDX_PYRO1				2
#define IDX_TEMP  				3
#define IDX_VIN_BATT  			4
#define IDX_V5_BUCK   			5
#define IDX_V3_BUCK   			6
#define IDX_PYRO3				7
#define IDX_PYRO2				8
#define IDX_TEMP_INT  			9
#define IDX_VREFINT   			10

#define NB_WAIT_CYCLE			10		// Number of ADC/DMA acquisition to wait before keep measures
#define TIMEOUT_WAIT_CYCLE_MS	100

#define DIV_MULT_VIN_BATT 		207		// 180k + 27k
#define DIV_DIV_VIN_BATT 		27		// 27k
#define DIV_MULT_V5_BUCK  		315		// 180k + 135k
#define DIV_DIV_V5_BUCK  		180		// 180k
#define DIV_MULT_V3_BUCK		200		// 180k + 20k
#define DIV_DIV_V3_BUCK			180		// 180k


extern volatile uint16_t adc_buffer[DMA_INDEX_SIZE];


static uint16_t SystemMeasurements_GetCompensatedVoltage(uint16_t raw_adc, system_measurements_t *dev) {
    float voltage = (float)raw_adc * dev->adc_step_compensation;
    float delta_temp = dev->internal_temperature - TEMPSENSOR_CAL1_TEMP;
    float thermal_correction_factor = 1.0f - (ADC_THERMAL_DRIFT_PPM * delta_temp);

    return (uint16_t)(voltage * thermal_correction_factor * 1000.0f); // mV
}


int8_t SystemMeasurements_Init(system_measurements_t *dev) {
	if(dev == NULL || dev->hadc == NULL) {
		return -1;
	}

	dev->dma_ready = false;
	dev->internal_temperature = 0.0f;
	dev->vdda = 3.3f;
	dev->temperature = 0.00f;
	dev->vin_batt = 0.0f;
	dev->v5_buck = 0.0f;
	dev->v3_buck = 0.0f;
	dev->pg_v5 = (HAL_GPIO_ReadPin(dev->pg_port, dev->pg_pin) == GPIO_PIN_SET);
	memset(dev->pyro_status, 0, sizeof(dev->pyro_status));

	// Start DMA
	if(HAL_ADC_Start_DMA(dev->hadc, (uint32_t*)adc_buffer, DMA_INDEX_SIZE) != HAL_OK) {
		return -1;
	}

	// Start trigger
	HAL_TIM_Base_Start(dev->htim);
	HAL_Delay(200);

	// Wait multiple acquisition before keep them
	for(uint8_t i = 0; i < NB_WAIT_CYCLE; i++) {
		dev->dma_ready = false;
		uint32_t start_tick = HAL_GetTick();

		while(!dev->dma_ready) {
			if((HAL_GetTick() - start_tick) > TIMEOUT_WAIT_CYCLE_MS) {
				HAL_TIM_Base_Stop(dev->htim);
				HAL_ADC_Stop_DMA(dev->hadc);
				return -1;
			}
		}
	}

	SystemMeasurements_UpdateInternalCalibration(dev);

	return 0; // success
}

void SystemMeasurements_UpdateInternalCalibration(system_measurements_t *dev) {
    uint16_t vref_raw = adc_buffer[IDX_VREFINT];
    uint16_t temp_raw = adc_buffer[IDX_TEMP_INT];

    dev->vdda = 3.3f * (float)(*VREFINT_CAL_ADDR) / (float)vref_raw;
    dev->adc_step_compensation = dev->vdda / 4095.0f;

    uint32_t temp_raw_compensated = ((uint32_t)temp_raw * (uint32_t)(*VREFINT_CAL_ADDR)) / vref_raw;

    int32_t delta_temp_raw = (int32_t)temp_raw_compensated - (int32_t)(*TEMPSENSOR_CAL1_ADDR);
    int32_t delta_cal_raw  = (int32_t)(*TEMPSENSOR_CAL2_ADDR) - (int32_t)(*TEMPSENSOR_CAL1_ADDR);

    dev->internal_temperature = ((float)delta_temp_raw * (TEMPSENSOR_CAL2_TEMP - TEMPSENSOR_CAL1_TEMP)) / (float)delta_cal_raw + TEMPSENSOR_CAL1_TEMP;
}

void SystemMeasurements_ComputePower(system_measurements_t *dev) {
    uint16_t vin = SystemMeasurements_GetCompensatedVoltage(adc_buffer[IDX_VIN_BATT], dev);
    uint16_t v5  = SystemMeasurements_GetCompensatedVoltage(adc_buffer[IDX_V5_BUCK], dev);
    uint16_t v3  = SystemMeasurements_GetCompensatedVoltage(adc_buffer[IDX_V3_BUCK], dev);

    dev->vin_batt = (uint16_t)(((uint32_t)vin * DIV_MULT_VIN_BATT) / DIV_DIV_VIN_BATT);
	dev->v5_buck  = (uint16_t)(((uint32_t)v5  * DIV_MULT_V5_BUCK)  / DIV_DIV_V5_BUCK);
	dev->v3_buck  = (uint16_t)(((uint32_t)v3  * DIV_MULT_V3_BUCK)  / DIV_DIV_V3_BUCK);
}

void SystemMeasurements_ComputeTemperature(system_measurements_t *dev) {
    uint16_t temp = SystemMeasurements_GetCompensatedVoltage(adc_buffer[IDX_TEMP], dev);

    dev->temperature = ((float)temp - 400.0f) / 19.53f;
}

void SystemMeasurements_ComputePyros(system_measurements_t *dev) {
    uint16_t arm = SystemMeasurements_GetCompensatedVoltage(adc_buffer[IDX_PYROS_ARMED], dev);
    uint16_t p4  = SystemMeasurements_GetCompensatedVoltage(adc_buffer[IDX_PYRO4], dev);
    uint16_t p1  = SystemMeasurements_GetCompensatedVoltage(adc_buffer[IDX_PYRO1], dev);
    uint16_t p3  = SystemMeasurements_GetCompensatedVoltage(adc_buffer[IDX_PYRO3], dev);
    uint16_t p2  = SystemMeasurements_GetCompensatedVoltage(adc_buffer[IDX_PYRO2], dev);

    dev->pyros_arming 	= arm;
    dev->pyro_status[0] = p1;
    dev->pyro_status[1] = p2;
    dev->pyro_status[2] = p3;
    dev->pyro_status[3] = p4;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	extern system_measurements_t system_measurements;
	if(hadc == system_measurements.hadc) {
		system_measurements.dma_ready = true;
	}
}
