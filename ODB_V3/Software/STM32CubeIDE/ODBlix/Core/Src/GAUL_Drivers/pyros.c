/*
 * Pyros.h
 *
 *  Created on: May 15, 2024
 *      Author: AudaceLol12
 *
 *  Edited on: Mar 02, 2026
 *  	Author: AudaceLol12
 */

#include "GAUL_Drivers/pyros.h"
#include <stddef.h>
#include <stdbool.h>


#define DMA_RISING_TIME_MS 	10000
#define PYRO_THRESHOLD 		75

extern pyro_t pyros[PYRO_MAX];
extern bool is_pyros_armed;

bool is_pa_an_active = false;

/*
 * PA_AN is configured in push-pull mode to power senses and check pyros continuity without arming them
 * TODO: Fix Kicad schematic to add a dedicated pin to power senses to keep PA_An the sense of arming continuity
 * TODO: Remove HAL_Delay
 */
int8_t Pyro_Init(pyro_t *dev, system_measurements_t *measures) {
	if(!dev || !measures || dev->channel >= PYRO_MAX || !dev->fire_port || !dev->fire_pin) {
		return -1; // failed
	}

	if(!is_pa_an_active) {
		HAL_GPIO_WritePin(PA_An_GPIO_Port, PA_An_Pin, GPIO_PIN_SET);
		HAL_Delay(DMA_RISING_TIME_MS);
		is_pa_an_active = true;
	}

	SystemMeasurements_ComputePyros(measures);

	dev->is_connected = (measures->pyro_status[dev->channel] >= PYRO_THRESHOLD);
	dev->is_fire = false;

	return (dev->is_connected) ? 0 : -1;
}

bool Pyro_Arming(system_measurements_t *measures, bool arming) {
	// Define in main.h (generated in IOC)
	if (arming) {
		HAL_GPIO_WritePin(PA_An_GPIO_Port, PA_An_Pin, GPIO_PIN_RESET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(Pyros_Arm_GPIO_Port, Pyros_Arm_Pin, GPIO_PIN_SET);
		is_pyros_armed = true;
	} else {
		HAL_GPIO_WritePin(Pyros_Arm_GPIO_Port, Pyros_Arm_Pin, GPIO_PIN_RESET);
		HAL_Delay(10);
		HAL_GPIO_WritePin(PA_An_GPIO_Port, PA_An_Pin, GPIO_PIN_SET);
		is_pyros_armed = false;
	}

	SystemMeasurements_ComputePyros(measures);

	return arming;
}

bool Pyro_Fire(pyro_t *dev, system_measurements_t *measures) {
	if(!dev || !dev->fire_port) return false;

	HAL_GPIO_WritePin(dev->fire_port, dev->fire_pin, GPIO_PIN_SET);

	HAL_Delay(250);

	SystemMeasurements_ComputePyros(measures);

	if(measures->pyro_status[dev->channel] < PYRO_THRESHOLD) {
		dev->is_fire = true;
	}

	HAL_GPIO_WritePin(dev->fire_port, dev->fire_pin, GPIO_PIN_RESET);

	return true;
}

pyro_t* Pyro_GetByRole(pyro_role_t role) {
	for (int i = 0; i < PYRO_MAX; i++) {
		if (current_config.pyro_roles[i] == role) {
			return &pyros[i];
		}
	}
	return NULL;
}
