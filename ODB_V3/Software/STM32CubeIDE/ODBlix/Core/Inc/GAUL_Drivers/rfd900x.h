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

#include "GAUL_Drivers/LowLevel/ringbuffer.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#define RFD_TX_RING_SIZE 2048

typedef enum {
    RFD_OK,
    RFD_BUSY,
    RFD_ERROR,
} rfd900x_state_t;

typedef struct {
    UART_HandleTypeDef  *huart;
    ring_buffer_t       tx_ring;
    uint8_t             tx_ring_data[RFD_TX_RING_SIZE];
    uint8_t             dma_busy_buffer[255];
    volatile bool       is_transmitting;
} rfd900x_t;


rfd900x_state_t RFD900x_Init(rfd900x_t *dev);

rfd900x_state_t RFD900x_Transmit(rfd900x_t *dev, uint8_t *payload, uint16_t length);
void RFD900x_ProcessTX(rfd900x_t *dev);

#endif /* INC_GAUL_DRIVERS_RFD900X_H_ */
