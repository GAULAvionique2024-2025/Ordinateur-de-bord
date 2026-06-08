/*
 * ltste682krkgwt.h
 *
 *  Created on: 3 mars 2026
 *      Author: AudaceLol12
 */

#ifndef INC_DRIVERS_LTSTE682KRKGWT_H_
#define INC_DRIVERS_LTSTE682KRKGWT_H_


#include "stm32f4xx_hal.h"
#include <stdbool.h>


typedef enum {
	GREEN,
	RED,
	ORANGE,
	NONE
} critical_led_color_t;

typedef struct {
	GPIO_TypeDef *r_port;
	GPIO_TypeDef *g_port;
	uint16_t r_pin;
	uint16_t g_pin;

	bool is_active;
	critical_led_color_t current_color;
} critical_led_t;

int8_t CriticalLed_Init(critical_led_t *led);

void CriticalLED_SetColor(critical_led_t *led, critical_led_color_t color);

#endif /* INC_DRIVERS_LTSTE682KRKGWT_H_ */
