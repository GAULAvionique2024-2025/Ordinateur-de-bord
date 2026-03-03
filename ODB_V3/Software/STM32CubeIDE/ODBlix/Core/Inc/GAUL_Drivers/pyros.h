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
#define PYROS_FIRE_1_PORT	GPIOF
#define PYROS_FIRE_1_PIN	14
#define PYROS_FIRE_2_PORT	GPIOF
#define PYROS_FIRE_2_PIN	15
#define PYROS_FIRE_3_PORT	GPIOG
#define PYROS_FIRE_3_PIN	0
#define PYROS_FIRE_4_PORT	GPIOG
#define PYROS_FIRE_4_PIN	1


typedef enum {
	PYRO_1 = 0,
	PYRO_2 = 1,
	PYRO_3 = 2,
	PYRO_4 = 3
} pyros_t;


void Pyro_Init(void);

void Pyro_Arming(bool arming);
uint8_t Pyro_Fire(pyros_t pyro);

#endif /* INC_GAUL_DRIVERS_PYROS_H_ */
