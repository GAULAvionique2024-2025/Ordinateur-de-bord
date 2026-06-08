/*
 * ltste682krkgwt.c
 *
 *  Created on: 3 mars 2026
 *      Author: AudaceLol12
 */


#include "Drivers/ltste682krkgwt.h"


void CriticalLED_SetColor(critical_led_t *led, critical_led_color_t color) {
	if (!led) return;

	switch(color) {
		case GREEN:
			HAL_GPIO_WritePin(led->g_port, led->g_pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(led->r_port, led->r_pin, GPIO_PIN_SET);
			led->is_active = true;
			led->current_color = GREEN;
			break;
		case RED:
			HAL_GPIO_WritePin(led->g_port, led->g_pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(led->r_port, led->r_pin, GPIO_PIN_RESET);
			led->is_active = true;
			led->current_color = RED;
			break;
		case ORANGE:
			HAL_GPIO_WritePin(led->g_port, led->g_pin, GPIO_PIN_RESET);
			HAL_GPIO_WritePin(led->r_port, led->r_pin, GPIO_PIN_RESET);
			led->is_active = true;
			led->current_color = ORANGE;
			break;
		case NONE:
			HAL_GPIO_WritePin(led->g_port, led->g_pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(led->r_port, led->r_pin, GPIO_PIN_SET);
			led->is_active = false;
			led->current_color = NONE;
			break;
		default :
			break;
	}
}

int8_t CriticalLed_Init(critical_led_t *led) {
	if(!led) {
		return -1;
	}

	CriticalLED_SetColor(led, NONE);
	led->is_active = false;

	return 0;
}
