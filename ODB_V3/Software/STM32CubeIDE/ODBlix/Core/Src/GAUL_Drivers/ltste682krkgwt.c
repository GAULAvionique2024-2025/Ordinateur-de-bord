/*
 * ltste682krkgwt.c
 *
 *  Created on: 3 mars 2026
 *      Author: AudaceLol12
 */


#include "GAUL_Drivers/ltste682krkgwt.h"


int8_t CriticalLed_Init(critical_led_t *led) {
	if (!led) {
		return -1;
	}

	HAL_GPIO_WritePin(led->port, led->g_pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(led->port, led->r_pin, GPIO_PIN_SET);
	led->current_color = NONE;

	return 0;
}

void CriticalLED_SetColor(critical_led_t *led, critical_led_color_t color) {
	if (!led) return;

	switch(color) {
		case GREEN:
			HAL_GPIO_WritePin(led->port, led->g_pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(led->port, led->r_pin, GPIO_PIN_SET);
			break;
		case RED:
			HAL_GPIO_WritePin(led->port, led->g_pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(led->port, led->r_pin, GPIO_PIN_RESET);
			break;
		case ORANGE:
			HAL_GPIO_WritePin(led->port, led->g_pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(led->port, led->r_pin, GPIO_PIN_RESET);
			break;
		case NONE:
			HAL_GPIO_WritePin(led->port, led->g_pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(led->port, led->r_pin, GPIO_PIN_SET);
			break;
		default :
			break;
	}
}
