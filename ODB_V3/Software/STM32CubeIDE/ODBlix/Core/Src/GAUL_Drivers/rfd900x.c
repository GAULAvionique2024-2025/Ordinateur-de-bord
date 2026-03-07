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


uint8_t RFD900X_Init(rfd900x_t *dev, rfd900x_id_t id) {
	dev->id = id;
    dev->header = 0x00;
    dev->data = NULL;
    dev->crc = 0x00;
    dev->size = 0x00;

    return 1; // ok
}

// TODO: make 9bits integration (bit shift << 8 = rfd900x_id_t) => in function
uint8_t RFD900X_Send(rfd900x_t *dev) {

    uint8_t delim = '$';
    uint8_t crc_delim = '*';
    uint8_t new_line = '\n';

    uint8_t header_lvb = dev->header;
    uint8_t header_mvb = dev->header>>8;

    uint8_t data_lvb = *dev->data;
	uint8_t data_mvb = *dev->data>>8;

	uint8_t crc_lvb = *dev->crc;
	uint8_t crc_mvb = *dev->crc>>8;


    HAL_UART_Transmit(dev->UARTx, &delim, 1, HAL_MAX_DELAY); // Start
    HAL_UART_Transmit(dev->UARTx, &header_lvb, 1, HAL_MAX_DELAY);
    HAL_UART_Transmit(dev->UARTx, &header_mvb, 1, HAL_MAX_DELAY);
    HAL_UART_Transmit(dev->UARTx, &data_lvb, dev->size, HAL_MAX_DELAY);
    HAL_UART_Transmit(dev->UARTx, &data_mvb, dev->size, HAL_MAX_DELAY);
    HAL_UART_Transmit(dev->UARTx, &crc_delim, 1, HAL_MAX_DELAY); // CRC
    HAL_UART_Transmit(dev->UARTx, &crc_lvb, 2, HAL_MAX_DELAY);
    HAL_UART_Transmit(dev->UARTx, &crc_mvb, 2, HAL_MAX_DELAY);
    HAL_UART_Transmit(dev->UARTx, &new_line, 1, HAL_MAX_DELAY); // End

    return 1; // ok
}

