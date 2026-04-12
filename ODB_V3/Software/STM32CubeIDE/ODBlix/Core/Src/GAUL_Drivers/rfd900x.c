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


int8_t RFD900x_Init(rfd900x_t *dev, UART_HandleTypeDef *huart) {
    if (!dev || !huart) return -1;
    dev->huart = huart;
    return 0; // success
}

int8_t RFD900x_Transmit(rfd900x_t *dev, uint8_t *payload, uint16_t length) {
    if (!dev || !payload || length == 0) return -1;

    HAL_StatusTypeDef status = HAL_UART_Transmit(dev->huart, payload, length, HAL_MAX_DELAY);

    return (status == HAL_OK) ? 0 : -1;
}
