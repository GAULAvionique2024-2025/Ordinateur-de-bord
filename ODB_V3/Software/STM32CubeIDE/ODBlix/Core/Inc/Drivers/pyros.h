/*
 * Pyros.h
 *
 *  Created on: May 15, 2024
 *      Author: AudaceLol12
 *
 *  Edited on: Mar 02, 2026
 *  	Author: AudaceLol12
 */

#ifndef INC_DRIVERS_PYROS_H_
#define INC_DRIVERS_PYROS_H_

#include "Drivers/system_measurements.h"
#include "Systems/config.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include <stdbool.h>


typedef enum {
	PYRO_1 		= 0,
	PYRO_2 		= 1,
	PYRO_3 		= 2,
	PYRO_4 		= 3,
	PYRO_MAX 	= 4
} pyro_channel_t;

typedef struct {
	GPIO_TypeDef 	*fire_port;
	uint16_t		fire_pin;

	pyro_channel_t  channel;

	bool			is_connected;
	bool			is_fire;
} pyro_t;

int8_t Pyro_Init(pyro_t *dev, system_measurements_t *measures);

bool Pyro_Arming(system_measurements_t *measures, bool arming);
bool Pyro_Fire(pyro_t *dev, system_measurements_t *measures);

pyro_t* Pyro_GetByRole(pyro_role_t role);

#endif /* INC_DRIVERS_PYROS_H_ */
