/*
 * idefix.c
 *
 *  Created on: 27 mai 2026
 *      Author: gagno
 */


#include "Drivers/idefix.h"
#include <string.h>

#define IDEFIX_SLAVE_ADDRESS (0x69 << 1)
#define IDEFIX_PING_RESPONSE "PONG"
#define IDEFIX_CMD_RESPONSE  "ACK"

static idefix_status_t Idefix_Ping(idefix_t *dev, uint32_t timeout) {
    // Send "PING" message
    uint8_t ping[5] = "PING";
    if(HAL_I2C_Master_Transmit(dev->hi2c, IDEFIX_SLAVE_ADDRESS, ping, strlen((char *)ping), 100) != HAL_OK) {
        return IDEFIX_PINGPONG_ERROR;
    }

    // Wait for response "PONG"
    uint8_t buffer[4];
    if(HAL_I2C_Master_Receive(dev->hi2c, IDEFIX_SLAVE_ADDRESS, buffer, sizeof(buffer), timeout) != HAL_OK) {
        return IDEFIX_PINGPONG_ERROR;
    }
    if(strncmp((char *)buffer, IDEFIX_PING_RESPONSE, 4) != 0) {
        return IDEFIX_PINGPONG_ERROR; 
    }

    return IDEFIX_OK;
}

idefix_status_t Idefix_Init(idefix_t *dev) {
    if(!dev) return IDEFIX_ERROR;
    if(!dev->hi2c) return IDEFIX_I2C_ERROR;
    
    if(Idefix_Ping(dev, 1000) != IDEFIX_OK) {
    	return IDEFIX_I2C_ERROR;
    }

    return IDEFIX_OK;
}

idefix_status_t Idefix_SendCommand(idefix_t *dev, idefix_command_t command) {
    if(!dev || !command) return IDEFIX_ERROR;

    if(HAL_I2C_Master_Transmit(dev->hi2c, IDEFIX_SLAVE_ADDRESS, (uint8_t *)command, strlen(command), 100) != HAL_OK) {
        return IDEFIX_I2C_ERROR;
    }
    
    // Receive response "ACK"
    uint8_t buffer[3];
    if(HAL_I2C_Master_Receive(dev->hi2c, IDEFIX_SLAVE_ADDRESS, buffer, sizeof(buffer), 100) != HAL_OK) {
        return IDEFIX_I2C_ERROR;
    } else {
        if(strncmp((char *)buffer, IDEFIX_CMD_RESPONSE, 3) == 0) {
            return IDEFIX_OK;
        }
    }

    return IDEFIX_CMD_ERROR;
}

void Idefix_SendData(idefix_t *dev, uint8_t *data, uint16_t size) {
    if(!dev || !data || size == 0) return;

    HAL_I2C_Master_Transmit(dev->hi2c, IDEFIX_SLAVE_ADDRESS, data, size, 100);
}
