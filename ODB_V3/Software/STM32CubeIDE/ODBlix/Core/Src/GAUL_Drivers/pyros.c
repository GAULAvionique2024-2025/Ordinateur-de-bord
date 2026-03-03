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


void Pyro_Init(void) {
	HAL_GPIO_WritePin(PYROS_ARMING_PORT, PYROS_ARMING_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PYROS_FIRE_1_PORT, PYROS_FIRE_1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PYROS_FIRE_2_PORT, PYROS_FIRE_2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PYROS_FIRE_3_PORT, PYROS_FIRE_3_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PYROS_FIRE_4_PORT, PYROS_FIRE_4_PIN, GPIO_PIN_RESET);
}

void Pyro_Arming(bool arming) {
	GPIO_PinState state = arming ? GPIO_PIN_SET : GPIO_PIN_RESET;
	HAL_GPIO_WritePin(PYROS_ARMING_PORT, PYROS_ARMING_PIN, state);
	HAL_Delay(10);		// TODO: Remplace by a no-blocking delay
}

uint8_t Pyro_Fire(pyros_t pyro) {
	GPIO_TypeDef* target_port;
	uint16_t target_pin;

	switch (pyro) {
		case PYRO_1:
			target_port = PYROS_FIRE_1_PORT;
			target_pin = PYROS_FIRE_1_PIN;
			break;
		case PYRO_2:
			target_port = PYROS_FIRE_2_PORT;
			target_pin = PYROS_FIRE_2_PIN;
			break;
		case PYRO_3:
			target_port = PYROS_FIRE_3_PORT;
			target_pin = PYROS_FIRE_3_PIN;
			break;
		case PYRO_4:
			target_port = PYROS_FIRE_4_PORT;
			target_pin = PYROS_FIRE_4_PIN;
			break;
		default:
			return 0; // error
	}

	// Commande de mise à feu
	HAL_GPIO_WritePin(target_port, target_pin, GPIO_PIN_SET);
	HAL_Delay(500);
	HAL_GPIO_WritePin(target_port, target_pin, GPIO_PIN_RESET);

	return 1; // ok
}
