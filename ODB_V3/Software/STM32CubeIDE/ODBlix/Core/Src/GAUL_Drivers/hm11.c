/*
 * dev.c
 *
 *  Created on: 12 avr. 2026
 *      Author: gagno
 */


#include "GAUL_Drivers/hm11.h"
#include <stdio.h>


static bool HM11_SendATCommand(hm11_t *dev, const char *cmd, const char *expected_response) {
    HAL_UART_AbortReceive_IT(dev->huart);
    
    dev->at_rx_buffer[0] = '\0'; 

    HAL_UART_Transmit(dev->huart, (uint8_t*)cmd, strlen(cmd), HM11_TIMEOUT);
    if(expected_response == NULL) {
        HAL_UART_Receive_IT(dev->huart, &dev->rx_byte, 1);
        return true;
    }

    uint16_t index = 0;
    uint32_t start_tick = HAL_GetTick();
    bool success = false;
    while((HAL_GetTick() - start_tick) < HM11_TIMEOUT) {
        uint8_t byte = 0;
        if(HAL_UART_Receive(dev->huart, &byte, 1, 10) == HAL_OK) {
            if(index < HM11_RX_BUFFER_SIZE - 1) {
                dev->at_rx_buffer[index++] = byte;
                dev->at_rx_buffer[index] = '\0';

                if(strstr(dev->at_rx_buffer, expected_response) != NULL) {
                    success = true;
                    break;
                }
            }
        }
    }

    HAL_UART_Receive_IT(dev->huart, &dev->rx_byte, 1);
    
    return success;
}

static bool HM11_SetName(hm11_t *dev, const char *name) {
    if(strlen(name) > 12) return false;

    char cmd[20];
    sprintf(cmd, "AT+NAME%s", name);

    return HM11_SendATCommand(dev, cmd, "OK+Set");
}


static bool HM11_SetBaudRate(hm11_t *dev, uint8_t baud_idx) {
    if(baud_idx > 8) return false;

    char cmd[10];
    sprintf(cmd, "AT+BAUD%d", baud_idx);

    return HM11_SendATCommand(dev, cmd, "OK+Set");
}

static bool HM11_TestConnection(hm11_t *dev) {
    return HM11_SendATCommand(dev, "AT", "OK");
}

bool HM11_Reset(hm11_t *dev) {
    return HM11_SendATCommand(dev, "AT+RESET", "OK+RESET");
}

hm11_state_t HM11_Init(hm11_t *dev) {
    if(!dev || !dev->huart || dev->baudrate > 8 || !dev->name) return HM11_ERROR_INVALID_PARAM;

    hm11_state_t err = HM11_OK;

    // Initialization ring buffer
    RingBuffer_Init(&dev->rx_ring, dev->rx_ring_data, HM11_RX_BUFFER_SIZE);

    // Configuration
    if(!HM11_SetName(dev, dev->name)) {
        err = HM11_SETNAME_FAILED; // Failed to set name
    }
    if(!HM11_SetBaudRate(dev, dev->baudrate)) {
        err = HM11_SETBAUD_FAILED; // Failed to set baud rate
    }
    if(!HM11_Reset(dev)) {
        err = HM11_ERROR; // Failed to reset
    }
    HAL_Delay(500);

    // Reset states
    memset(dev->at_rx_buffer, 0, HM11_RX_BUFFER_SIZE);

    // Start receiving data asynchronously
    HAL_UART_Receive_IT(dev->huart, &dev->rx_byte, 1);

    if(HM11_TestConnection(dev) != 0) {
    	err = HM11_ERROR;
    }

    return err; // success
}

bool HM11_SendData(hm11_t *dev, uint8_t *data, uint16_t length) {
    if(HAL_UART_Transmit(dev->huart, data, length, HM11_TIMEOUT) == HAL_OK) {
        return true;
    }

    return false;
}

bool HM11_SendString(hm11_t *dev, const char *str) {
    return HM11_SendData(dev, (uint8_t*)str, strlen(str));
}

bool HM11_GetMessage(hm11_t *dev, char *out_buffer, uint16_t max_len) {
    if(!dev || !out_buffer || max_len == 0) return false;

    uint8_t byte;
    bool line_found = false;
    size_t items = RingBuffer_NumItems(&dev->rx_ring);
    for (size_t i = 0; i < items; i++) {
        RingBuffer_Peek(&dev->rx_ring, &byte, i);
        if (byte == '\n' || byte == '\r') {
            line_found = true;
            break;
        }
    }

    if (!line_found) {
        return false; 
    }

    uint16_t index = 0;
    while (index < max_len - 1 && RingBuffer_Dequeue(&dev->rx_ring, &byte)) {
        if (byte == '\n' || byte == '\r') {
            break;
        }
        if (byte >= 0x20) {
            out_buffer[index++] = (char)byte;
        }
    }
    out_buffer[index] = '\0';

    while (RingBuffer_Peek(&dev->rx_ring, &byte, 0)) {
        if (byte == '\n' || byte == '\r') {
            RingBuffer_Dequeue(&dev->rx_ring, &byte);
        } else {
            break;
        }
    }

    // Ignore empty line
    if (index == 0) {
		return false;
	}

    return true; // success
}

bool HM11_Sleep(hm11_t *dev) {
    return HM11_SendATCommand(dev, "AT+SLEEP", "OK+SLEEP");
}

bool HM11_WakeUp(hm11_t *dev) {
	const char wake_up_string[] = "wake up!, wake up!, wake up!, wake up!, wake up!, wake up!, wake up!, wake up!, wake up!";
    
	return HM11_SendATCommand(dev, wake_up_string, "OK+WAKE");
}

// Callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    extern hm11_t hm11; 

    if(huart->Instance == hm11.huart->Instance) {
        RingBuffer_Queue(&hm11.rx_ring, hm11.rx_byte);
        
        HAL_UART_Receive_IT(hm11.huart, &hm11.rx_byte, 1);
    }
}
