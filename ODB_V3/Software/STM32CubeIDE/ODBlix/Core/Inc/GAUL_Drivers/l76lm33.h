/*
 * L76LM33.h
 *
 * Created on: May 12, 2024
 * Author: AudaceLol12
 *
 * Edited on: Jul 04, 2024
 * Autor: mathouqc
 *
 * Edited on: Mar 02, 2026
 * Autor: AudaceLol12
 */

#ifndef INC_GAUL_DRIVERS_L76LM33_H_
#define INC_GAUL_DRIVERS_L76LM33_H_

#include "stm32f4xx_hal.h"
#include "GAUL_Drivers/LowLevel/nmea.h"
#include "GAUL_Drivers/LowLevel/ringbuffer.h"

#define L76LM33_BUFFER_SIZES 256    // NMEA sentence is around 80 char max, has to be a power of two.
#define L76LM33_UART_TIMEOUT 500    // For UART transmit

typedef enum {
    L76LM33_EMPTY_BUFF = -2,
    L76LM33_ERROR      = -1,
    L76LM33_OK         =  0
} l76lm33_state_t;

typedef enum {
    L76_FLIGHT_PROFILE_30K  = 0,    // Aviation Mode (Max 10 000m / 32 800ft)
    L76_FLIGHT_PROFILE_100K = 1     // Balloon Mode (Max 80 000m / 262 000ft)
} l76_flight_profile_t;

typedef struct {
    UART_HandleTypeDef   *huart;                                // Pointer to the GNSS module UART handler
    l76_flight_profile_t profile;                               // Flight profile for the GNSS module
    
    uint8_t              dma_buffer[L76LM33_BUFFER_SIZES];      // Buffer for UART DMA reception
    uint8_t              ring_buffer_arr[L76LM33_BUFFER_SIZES]; // Array for ring buffer storage
    ring_buffer_t        UART_Buffer;                           // Ring buffer to store UART data from GNSS module
    uint16_t             old_pos;
    volatile uint8_t     line_count;                            // Counter for lines received
    nmea_t               gps_data;                              // Struct to store parsed NMEA data
} l76lm33_t;

l76lm33_state_t L76LM33_Init(l76lm33_t *dev);

l76lm33_state_t L76LM33_Read(l76lm33_t *dev);

// Callback
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size);

#endif /* INC_GAUL_DRIVERS_L76LM33_H_ */
