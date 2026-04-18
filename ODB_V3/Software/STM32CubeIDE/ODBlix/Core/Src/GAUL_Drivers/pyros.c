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
int8_t Pyro_Init(pyro_t *dev) {
	if(!dev) return -1; // failed

	// TODO: Check with system_measurements and ...
	dev->is_connected = true;
	dev->is_fire = false;

	return (dev->is_connected == true) ? 0 : -1; // 0 = success, -1 = failed
}

bool Pyro_Arming(bool arming) {
	GPIO_PinState state = arming ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(PYROS_ARMING_PORT, PYROS_ARMING_PIN, state);

	return true;
}

bool Pyro_Fire(pyro_t *dev) {
	if(!dev) return false;

	HAL_GPIO_WritePin(dev->port, dev->pin, GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(dev->port, dev->pin, GPIO_PIN_RESET);

	// TODO: Check with system_measurements and ...
	dev->is_fire = true;

	return true;
}
