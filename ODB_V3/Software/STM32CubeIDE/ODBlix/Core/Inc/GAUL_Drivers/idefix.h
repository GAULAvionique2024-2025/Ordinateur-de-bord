/*
 * idefix.h
 *
 *  Created on: 27 mai 2026
 *      Author: gagno
 */

#ifndef INC_GAUL_DRIVERS_IDEFIX_H_
#define INC_GAUL_DRIVERS_IDEFIX_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

typedef enum {
	IDEFIX_OK = 0,
	IDEFIX_ERROR = -1,
	IDEFIX_I2C_ERROR = -2,
	IDEFIX_PINGPONG_ERROR = -3,
	IDEFIX_CMD_ERROR = -4
} idefix_status_t;

typedef const char* idefix_command_t;

#define IDEFIX_CMD_SET_COORD   "COORD"
#define IDEFIX_CMD_SET_FREQ    "FREQ"
#define IDEFIX_CMD_SET_CHANNEL "CHANNEL"
#define IDEFIX_CMD_STATUS      "STATUS"

typedef struct {
	I2C_HandleTypeDef *hi2c;
} idefix_t;


idefix_status_t Idefix_Init(idefix_t *dev);
idefix_status_t Idefix_SendCommand(idefix_t *dev, idefix_command_t command);

void Idefix_SendData(idefix_t *dev, uint8_t *data, uint16_t size);

#endif /* INC_GAUL_DRIVERS_IDEFIX_H_ */
