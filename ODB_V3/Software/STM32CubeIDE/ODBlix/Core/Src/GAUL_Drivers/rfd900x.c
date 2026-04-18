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

	RingBuffer_Init(&dev->tx_ring, dev->tx_ring_data, RFD_TX_RING_SIZE);
	dev->is_transmitting = false;

    return RFD_OK; // success
}

rfd900x_state_t RFD900x_Transmit(rfd900x_t *dev, uint8_t *payload, uint16_t length) {
    if(!dev || !payload || length == 0) return RFD_ERROR;

    if(RFD_TX_RING_SIZE - RingBuffer_NumItems(&dev->tx_ring) < length) {
        return RFD_BUSY;
    }

    RingBuffer_Queue_Array(&dev->tx_ring, payload, length);

    RFD900x_ProcessTX(dev);

    return RFD_OK;
}

void RFD900x_ProcessTX(rfd900x_t *dev) {
    if(dev->is_transmitting || RingBuffer_IsEmpty(&dev->tx_ring)) {
        return;
    }

    size_t to_send = RingBuffer_NumItems(&dev->tx_ring);
    if(to_send > sizeof(dev->dma_busy_buffer)) {
        to_send = sizeof(dev->dma_busy_buffer);
    }

    RingBuffer_Dequeue_Array(&dev->tx_ring, dev->dma_busy_buffer, to_send);

    dev->is_transmitting = true;
    if(HAL_UART_Transmit_DMA(dev->huart, dev->dma_busy_buffer, (uint16_t)to_send) != HAL_OK) {
        dev->is_transmitting = false;
    }
}
