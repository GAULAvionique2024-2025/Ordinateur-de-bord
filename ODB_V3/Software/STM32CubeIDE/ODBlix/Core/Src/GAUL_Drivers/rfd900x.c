/*
 * RFD900.c
 *
 *  Created on: Feb 19, 2024
 *      Author: gagno
 *
 *  Edited on: Mar 02, 2026
 *  	Author: AudaceLol12
 */

#include "GAUL_Drivers/rfd900x.h"


rfd900x_state_t RFD900x_Init(rfd900x_t *dev) {
    if (!dev || !dev->huart) return RFD_ERROR;

    return RFD_OK; // success
}

rfd900x_state_t RFD900x_Transmit(rfd900x_t *dev, uint8_t *payload, uint16_t length) {
    if(!dev || !payload || length == 0) return RFD_ERROR;

    if(dev->huart->gState != HAL_UART_STATE_READY) {
        return RFD_BUSY;
    }

    if(HAL_UART_Transmit_DMA(dev->huart, payload, length)) {
    	return RFD_ERROR;
    }

    return RFD_OK;
}
