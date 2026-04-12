/*
 * hm11.h
 *
 *  Created on: 12 avr. 2026
 *      Author: gagno
 */

#ifndef INC_GAUL_DRIVERS_HM11_H_
#define INC_GAUL_DRIVERS_HM11_H_

#include "GAUL_Drivers/LowLevel/ringbuffer.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#define HM11_TIMEOUT         500
#define HM11_RX_BUFFER_SIZE  64

typedef enum {
    HM11_OK = 0,
    HM11_ERROR = -1,
    HM11_ERROR_INVALID_PARAM = -2,
    HM11_SETBAUD_FAILED = -3,
    HM11_SETNAME_FAILED = -4
} hm11_state_t;

typedef enum {
    HM11_BAUD_1200 = 7,
    HM11_BAUD_2400 = 6,
    HM11_BAUD_4800 = 5,
    HM11_BAUD_9600 = 0,
    HM11_BAUD_19200 = 1,
    HM11_BAUD_38400 = 2,
    HM11_BAUD_57600 = 3,
    HM11_BAUD_115200 = 4,
    HM11_BAUD_230400 = 8
} hm11_baudrate_t;

typedef struct {
    UART_HandleTypeDef 	*huart;
    char				*name;
    hm11_baudrate_t     baudrate;

    char 				at_rx_buffer[HM11_RX_BUFFER_SIZE];
    uint8_t             rx_byte;
    ring_buffer_t       rx_ring;
    uint8_t             rx_ring_data[HM11_RX_BUFFER_SIZE];
} hm11_t;


hm11_state_t HM11_Init(hm11_t *dev);
bool HM11_TestConnection(hm11_t *dev);

bool HM11_SendData(hm11_t *dev, uint8_t *data, uint16_t length);
bool HM11_SendString(hm11_t *dev, const char *str);
bool HM11_GetMessage(hm11_t *dev, char *out_buffer, uint16_t max_lenth);

bool HM11_Sleep(hm11_t *dev);
bool HM11_WakeUp(hm11_t *dev);

#endif /* INC_GAUL_DRIVERS_HM11_H_ */
