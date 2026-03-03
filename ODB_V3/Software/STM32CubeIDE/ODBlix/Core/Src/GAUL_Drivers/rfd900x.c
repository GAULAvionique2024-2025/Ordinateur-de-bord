/*
 * RFD900.c
 *
 *  Created on: Feb 19, 2024
 *      Author: gagno
 *
 *  Edited on: Mar 02, 2026
 *  	Author: AudaceLol12
 */

#include <GAUL_Drivers/rfd900x.h>


uint8_t RFD900_Init(rfd900x_t *dev, rfd900x_id_t id) {
	dev->id = id;
    dev->header = 0x00;
    dev->data = NULL;
    dev->crc = 0x00;
    dev->size = 0x00;

    return 1; // ok
}

// TODO: make 9bits integration (bit shift << 8 = rfd900x_id_t) => in function
uint8_t RFD900_Send(rfd900x_t *dev) {

    uint16_t delim = '$';
    uint16_t crc_delim = '*';
    uint16_t new_line = '\n';

    HAL_UART_Transmit(dev->USARTx, &delim, 1, HAL_MAX_DELAY); // Start
    HAL_UART_Transmit(dev->USARTx, &dev->header, 1, HAL_MAX_DELAY);
    HAL_UART_Transmit(dev->USARTx, dev->data, dev->size, HAL_MAX_DELAY);
    HAL_UART_Transmit(dev->USARTx, &crc_delim, 1, HAL_MAX_DELAY); // CRC
    HAL_UART_Transmit(dev->USARTx, dev->crc, 2, HAL_MAX_DELAY);
    HAL_UART_Transmit(dev->USARTx, &new_line, 1, HAL_MAX_DELAY); // End

    return 1; // ok
}

