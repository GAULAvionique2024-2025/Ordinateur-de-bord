/*
 * L76LM33.h
 *
 *  Created on: May 12, 2024
 *      Author: AudaceLol12
 *
 *  Edited on: Jul 04, 2024
 *      Autor: mathouqc
 *
 *  Edited on: Mar 02, 2026
 *      Autor: AudaceLol12
 */

#ifndef INC_GAUL_DRIVERS_L76LM33_H_
#define INC_GAUL_DRIVERS_L76LM33_H_

#include "stm32f4xx_hal.h"
#include "GAUL_Drivers/LowLevel/nmea.h"
#include "GAUL_Drivers/LowLevel/ringbuffer.h"

#define L76LM33_BUFFER_SIZES 256  	// NMEA sentence is around 80 char max, has to be a power of two.
#define L76LM33_UART_TIMEOUT 500 	// For UART transmit

typedef enum {
    L76LM33_EMPTY_BUFF = -2,
    L76LM33_ERROR      = -1,
    L76LM33_OK         =  0
} l76lm33_state_t;

typedef struct {
	l76lm33_state_t 	state; 										// 1: OK, 0: Error with GNSS module
    UART_HandleTypeDef 	*huart; 									// Pointer to the GNSS module UART handler
    uint8_t 			received_byte; 								// Received char/byte from UART
    ring_buffer_t 		UART_Buffer; 								// Ring buffer to store UART data from GNSS module
    char 				UART_Buffer_arr[L76LM33_BUFFER_SIZES]; 		// UART buffer array for ring buffer
    uint8_t 			new_line_flag; 								// 1: line available in UART buffer, 0: line not available in UART buffer
    char 				NMEA_Buffer[L76LM33_BUFFER_SIZES]; 			// Buffer to store NMEA sentence
    nmea_t 				gps_data; 									// Struct to store parsed NMEA data
} l76lm33_t;

l76lm33_state_t L76LM33_Init(l76lm33_t *dev, UART_HandleTypeDef *huart);

l76lm33_state_t L76LM33_Read(l76lm33_t *dev);
l76lm33_state_t L76LM33_Read_Sentence(l76lm33_t *dev);

void L76LM33_RxCallback(l76lm33_t *dev, UART_HandleTypeDef *huart);

#endif /* INC_GAUL_DRIVERS_L76LM33_H_ */
