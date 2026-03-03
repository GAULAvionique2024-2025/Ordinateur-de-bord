/*
 * RFD900.h
 *
 *  Created on: Feb 19, 2024
 *      Author: gagno
 *
 *  Edited on: Mar 02, 2026
 *  	Author: AudaceLol12
 */

#ifndef INC_GAUL_DRIVERS_RFD900X_H_
#define INC_GAUL_DRIVERS_RFD900X_H_

#include "stm32f4xx_hal.h"

#include <stdint.h>

// TODO: make 9bits integration
typedef enum {
	RFD900X_SUBSTAINER	= 0,
	RFD900X_BOOSTER		= 1,
} rfd900x_id_t;

typedef struct {
	rfd900x_id_t	id;
    USART_TypeDef	*USARTx;
    uint16_t 		header; 	// mode + states
    uint16_t 		*data; 		// Depend du mode
    uint16_t 		*crc;
    uint16_t 		size; 		// depend du mode
} rfd900x_t;

uint8_t RFD900_Init(rfd900x_t *dev, rfd900x_id_t id);

uint8_t RFD900_Send(rfd900x_t *dev);

#endif /* INC_GAUL_DRIVERS_RFD900X_H_ */
