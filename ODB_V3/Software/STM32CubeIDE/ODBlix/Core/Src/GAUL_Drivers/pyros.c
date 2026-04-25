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

// Pyros need to be armed for read status
int8_t Pyro_Init(pyro_t *dev, system_measurements_t *measures) {
	if(!dev || !measures || dev->channel < 0 || dev->channel > 3 || !dev->fire_port || !dev->arm_port) return -1; // failed

	HAL_Delay(50);
	SystemMeasurements_ComputePyros(measures);

	if(measures->pyro_status[dev->channel] >= PYRO_THRESHOLD) {
		dev->is_connected = true;
	}
	dev->is_fire = false;

	return (dev->is_connected) ? 0 : -1; // 0 = success, -1 = failed
}

// TODO: maybe add verification later (not working now injected ADC DMA)
bool Pyro_Arming(pyro_t *dev, system_measurements_t *measures, bool arming) {
	GPIO_PinState state = arming ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(dev->arm_port, dev->arm_pin, state);

	SystemMeasurements_ComputePyros(measures);

	return arming;
}

bool Pyro_Fire(pyro_t *dev, system_measurements_t *measures) {
	if(!dev) return false;

	HAL_GPIO_WritePin(dev->fire_port, dev->fire_pin, GPIO_PIN_SET);
	HAL_Delay(500);

	SystemMeasurements_ComputePyros(measures);
	if(measures->pyro_status[dev->channel] < PYRO_THRESHOLD) {
		dev->is_fire = true;
	}

	HAL_GPIO_WritePin(dev->fire_port, dev->fire_pin, GPIO_PIN_RESET);

	return true;
}
