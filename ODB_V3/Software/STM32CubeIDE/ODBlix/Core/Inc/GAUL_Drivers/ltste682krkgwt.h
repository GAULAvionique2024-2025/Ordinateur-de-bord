/*
 * ltste682krkgwt.h
 *
 *  Created on: 3 mars 2026
 *      Author: AudaceLol12
 */

#ifndef INC_GAUL_DRIVERS_LTSTE682KRKGWT_H_
#define INC_GAUL_DRIVERS_LTSTE682KRKGWT_H_


#include "stm32f4xx_hal.h"


typedef enum {
	GREEN,
	RED,
	ORANGE,
	NONE
} criticalLed_color_t;

void CriticalLED_SetColor(GPIO_TypeDef *port, uint16_t r_pin, uint16_t g_pin, criticalLed_color_t color);

#endif /* INC_GAUL_DRIVERS_LTSTE682KRKGWT_H_ */
