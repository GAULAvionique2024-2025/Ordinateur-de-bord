/*
 * Pyros.h
 *
 *  Created on: May 15, 2024
 *      Author: AudaceLol12
 *
 *  Edited on: Mar 02, 2026
 *  	Author: AudaceLol12
 */

#ifndef INC_GAUL_DRIVERS_PYROS_H_
#define INC_GAUL_DRIVERS_PYROS_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>


#define PYROS_ARMING_PORT	GPIOF
#define PYROS_ARMING_PIN	13

typedef enum {
	PYRO_1 = 0,
	PYRO_2 = 1,
	PYRO_3 = 2,
	PYRO_4 = 3
} pyro_channel_t;

typedef struct {
	GPIO_TypeDef 	*port;
	uint16_t		pin;

	bool			is_connected;
	bool			is_fire;
} pyro_t;

int8_t Pyro_Init(pyro_t *dev);

void Pyro_Arming(bool arming);
uint8_t Pyro_Fire(pyro_t *dev);

#endif /* INC_GAUL_DRIVERS_PYROS_H_ */
