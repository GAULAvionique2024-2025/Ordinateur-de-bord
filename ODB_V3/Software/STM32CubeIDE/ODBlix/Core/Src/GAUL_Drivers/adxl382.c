/*
 * adxl382.c
 *
 *  Created on: 13 avr. 2026
 *      Author: gagno
 */


#include "GAUL_Drivers/adxl382.h"

static int8_t ADXL382_ReadReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *data) {
    if(HAL_I2C_Mem_Read(hi2c, ADXL382_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY) != HAL_OK) {
        return -1;
    }

    return 0; // success
}

static int8_t ADXL382_WriteReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t data) {
    if(HAL_I2C_Mem_Write(hi2c, ADXL382_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, HAL_MAX_DELAY) != HAL_OK) {
        return -1;
    }

    return 0; // success
}

static int8_t ADXL382_ReadRegs(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *data, uint16_t length) {
    if(HAL_I2C_Mem_Read(hi2c, ADXL382_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, length, HAL_MAX_DELAY) != HAL_OK) {
        return -1;
    }

    return 0; // success
}

static int8_t ADXL382_Reset(adxl382_t *dev) {
    if(ADXL382_WriteReg(dev->hi2c, ADXL382_REG_REG_RESET, 0x02) != HAL_OK) {
        return -1;
    }
    HAL_Delay(10);

    dev->mode = ADXL382_MODE_STANDBY;
    dev->acc_x = 0;
    dev->acc_y = 0;
    dev->acc_z = 0;

    return 0; // success
}


static int8_t ADXL382_SetMode(adxl382_t *dev, adxl382_mode_t mode) {
    if(ADXL382_WriteReg(dev->hi2c, ADXL382_REG_OP_MODE, ADXL382_MODE_STANDBY) != HAL_OK) {
        return -1;
    }
    HAL_Delay(2);

    if(ADXL382_WriteReg(dev->hi2c, ADXL382_REG_OP_MODE, mode) != HAL_OK) {
        return -1;
    }
    HAL_Delay(2); 

    dev->mode = mode;
    
    return 0; // success
}


int8_t ADXL382_Init(adxl382_t *dev) {
    uint8_t id = 0;
    if(ADXL382_ReadReg(dev->hi2c, ADXL382_REG_DEVID_AD, &id) != 0) {
        return -1;
    }
    if(id != ADXL382_DEVID_AD_VAL) {
        return -1;
    }

    ADXL382_Reset(dev);
    
    // Enable XYZ
    if (ADXL382_WriteReg(dev->hi2c, ADXL382_REG_DIG_EN, 0xF0) != 0) {
        return -1;
    }
    
    if(ADXL382_SetMode(dev, dev->mode) != 0) {
        return -1;
    }
    
    return 0; // success
}

bool ADXL382_IsDataReady(adxl382_t *dev) {
    uint8_t ready = 0;
    if(ADXL382_ReadReg(dev->hi2c, ADXL382_REG_STATUS3, &ready) == 0) {
    	return (ready & 0x01) ? true : false;
    }

    return 0; // failure
}

int8_t ADXL382_ReadData(adxl382_t *dev) {
    uint8_t buffer[6];
    if(ADXL382_ReadRegs(dev->hi2c, ADXL382_REG_XDATA_H, buffer, 6) == 0) {
        int16_t x = (int16_t)((buffer[0] << 8) | buffer[1]);
        int16_t y = (int16_t)((buffer[2] << 8) | buffer[3]);
        int16_t z = (int16_t)((buffer[4] << 8) | buffer[5]);

        dev->acc_x = (float)x * (2.0f / 32768.0f);
        dev->acc_y = (float)y * (2.0f / 32768.0f);
        dev->acc_z = (float)z * (2.0f / 32768.0f);
    } else {
        return -1;
    }

    return 0; // success
}
