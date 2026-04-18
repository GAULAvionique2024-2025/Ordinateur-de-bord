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

// TODO: Pyros need to be armed for read status
int8_t Pyro_Init(pyro_t *dev, system_measurements_t *measures) {
	if(!dev || !measures || dev->channel < 0 || dev->channel > 3) return -1; // failed

	HAL_Delay(100);

	SystemMeasurements_ComputePyros(measures);
	if(measures->pyro_status[dev->channel] >= PYRO_THRESHOLD) {
		dev->is_connected = true;
	}
	dev->is_fire = false;

	return (dev->is_connected) ? 0 : -1; // 0 = success, -1 = failed
}

bool Pyro_Arming(bool arming, system_measurements_t *measures) {
	GPIO_PinState state = arming ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(PYROS_ARMING_PORT, PYROS_ARMING_PIN, state);

	SystemMeasurements_ComputePyros(measures);

	return (measures->pyros_arming >= ARM_THRESHOLD);
}

bool Pyro_Fire(pyro_t *dev, system_measurements_t *measures) {
	if(!dev) return false;

	HAL_GPIO_WritePin(dev->port, dev->pin, GPIO_PIN_SET);
	HAL_Delay(500);

	SystemMeasurements_ComputePyros(measures);
	if(measures->pyro_status[dev->channel] < PYRO_THRESHOLD) {
		dev->is_fire = true;
	}
	// TODO: Maybe add while
	HAL_GPIO_WritePin(dev->port, dev->pin, GPIO_PIN_RESET);

	return true;
}
