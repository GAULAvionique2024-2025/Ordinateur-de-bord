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
	PYRO_OK = 0,
	PYRO_INIT_ERROR = -1,
	PYRO_ARMING_ERROR = -2,
	PYRO_FIRE_ERROR = -3
} pyros_state_t;

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


pyros_state_t Pyro_Init(pyro_t *dev, system_measurements_t *measures);

pyros_state_t Pyro_Arming(system_measurements_t *measures, bool arming, bool is_test);
bool Pyro_IsArmed(system_measurements_t *measures);
pyros_state_t Pyro_Fire(pyro_t *dev, system_measurements_t *measures);

pyro_t* Pyro_GetByRole(pyro_role_t role);

#endif /* INC_DRIVERS_PYROS_H_ */
