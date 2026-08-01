/*
 * Pyros.h
 *
 *  Created on: May 15, 2024
 *      Author: AudaceLol12
 *
 *  Edited on: Mar 02, 2026
 *  	Author: AudaceLol12
 */

#include "Drivers/pyros.h"
#include <stddef.h>

#define PYRO_RISING_TIME_MS		250
#define PYRO_THRESHOLD_CONN 	1000
#define PYRO_THRESHOLD_ARMED	1000

extern pyro_t pyros[PYRO_MAX];


pyros_state_t Pyro_Init(pyro_t *dev, system_measurements_t *measures) {
	if(!dev || !measures || dev->channel >= PYRO_MAX || !dev->fire_port || !dev->fire_pin) {
		return PYRO_INIT_ERROR; // failed
	}

	HAL_GPIO_WritePin(Pyros_CTN_GPIO_Port, Pyros_CTN_Pin, GPIO_PIN_SET);
	HAL_Delay(PYRO_RISING_TIME_MS);

	SystemMeasurements_ComputePyros(measures);
	dev->is_connected = (measures->pyro_status[dev->channel] >= PYRO_THRESHOLD_CONN);
	dev->is_fire = false;

	HAL_GPIO_WritePin(Pyros_CTN_GPIO_Port, Pyros_CTN_Pin, GPIO_PIN_RESET);

	return (dev->is_connected) ? PYRO_OK : PYRO_INIT_ERROR;
}

bool Pyro_IsArmed(system_measurements_t *measures) {
	if(!measures) {
		return false;
	}

	SystemMeasurements_ComputePyros(measures);
	bool is_pyros_armed = (measures->pyros_arming >= PYRO_THRESHOLD_ARMED);

	return is_pyros_armed;
}

pyros_state_t Pyro_Arming(system_measurements_t *measures, bool arming, bool is_test) {
	if(!measures) return PYRO_ARMING_ERROR;

	if(arming) {
		HAL_GPIO_WritePin(Pyros_CTN_GPIO_Port, Pyros_CTN_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(Pyros_Arm_GPIO_Port, Pyros_Arm_Pin, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(Pyros_Arm_GPIO_Port, Pyros_Arm_Pin, GPIO_PIN_RESET);
	}

	if(is_test) HAL_Delay(PYRO_RISING_TIME_MS);

	return (Pyro_IsArmed(measures) == arming) ? PYRO_OK : PYRO_ARMING_ERROR;
}

pyros_state_t Pyro_Fire(pyro_t *dev, system_measurements_t *measures) {
	if(!dev || !dev->fire_port || !dev->fire_pin) return PYRO_FIRE_ERROR;

	HAL_GPIO_WritePin(dev->fire_port, dev->fire_pin, GPIO_PIN_SET);
	HAL_Delay(250);
	HAL_GPIO_WritePin(dev->fire_port, dev->fire_pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(Pyros_CTN_GPIO_Port, Pyros_CTN_Pin, GPIO_PIN_SET);

	SystemMeasurements_ComputePyros(measures);
	if(measures->pyro_status[dev->channel] < PYRO_THRESHOLD_CONN) {
		dev->is_fire = true;
	}

	HAL_GPIO_WritePin(Pyros_CTN_GPIO_Port, Pyros_CTN_Pin, GPIO_PIN_RESET);

	return (dev->is_fire) ? PYRO_OK : PYRO_FIRE_ERROR;
}

pyro_t* Pyro_GetByRole(pyro_role_t role) {
	if(role >= PYROS_ROLE_MAX) return NULL;

	for(int i = 0; i < PYRO_MAX; i++) {
		if(current_config.pyro_roles[i] == role) {
			return &pyros[i];
		}
	}
	return NULL;
}
