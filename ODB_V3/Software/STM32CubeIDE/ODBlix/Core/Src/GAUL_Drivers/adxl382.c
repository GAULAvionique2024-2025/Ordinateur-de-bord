/*
 * adxl382.c
 *
 *  Created on: 13 avr. 2026
 *      Author: gagno
 */


#include "GAUL_Drivers/adxl382.h"


static int8_t ADXL382_ReadReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *data) {
    if(HAL_I2C_Mem_Read(hi2c, ADXL382_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, 25) != HAL_OK) {
        return -1;
    }

    return 0; // success
}

static int8_t ADXL382_WriteReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t data) {
    if(HAL_I2C_Mem_Write(hi2c, ADXL382_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 25) != HAL_OK) {
        return -1;
    }

    return 0; // success
}

static int8_t ADXL382_ReadRegs(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *data, uint16_t length) {
    if(HAL_I2C_Mem_Read(hi2c, ADXL382_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, length, 25) != HAL_OK) {
        return -1;
    }

    return 0; // success
}

static int8_t ADXL382_Reset(adxl382_t *dev) {
    if(ADXL382_WriteReg(dev->hi2c, ADXL382_REG_REG_RESET, 0x80) != HAL_OK) {
        return -1;
    }
    HAL_Delay(10);

    dev->acc_x = 0;
    dev->acc_y = 0;
    dev->acc_z = 0;

    return 0; // success
}

static int8_t ADXL382_SetRange(adxl382_t *dev, adxl382_range_t range) {
    uint8_t op_mode_reg = 0;
    if(ADXL382_ReadReg(dev->hi2c, ADXL382_REG_OP_MODE, &op_mode_reg) != 0) {
        return -1;
    }

    // Standby mode safe
    uint8_t standby_val = op_mode_reg & 0xF0;
	if(ADXL382_WriteReg(dev->hi2c, ADXL382_REG_OP_MODE, standby_val) != 0) {
		return -1;
	}
	HAL_Delay(2);

    standby_val &= ~(0x03 << 6); 
    standby_val |= ((uint8_t)range << 6);
    uint8_t final_val = standby_val | (op_mode_reg & 0x0F);
    if(ADXL382_WriteReg(dev->hi2c, ADXL382_REG_OP_MODE, final_val) != 0) {
        return -1;
    }

    dev->range = range;

    return 0; // success
}


int8_t ADXL382_SetMode(adxl382_t *dev, adxl382_mode_t mode) {
    uint8_t op_mode_reg = 0;
    if(ADXL382_ReadReg(dev->hi2c, ADXL382_REG_OP_MODE, &op_mode_reg) != 0) {
        return -1;
    }

    uint8_t config_bits = op_mode_reg & 0xF0;
    if(ADXL382_WriteReg(dev->hi2c, ADXL382_REG_OP_MODE, config_bits) != HAL_OK) {
        return -1;
    }
    HAL_Delay(2);

    if(ADXL382_WriteReg(dev->hi2c, ADXL382_REG_OP_MODE, config_bits | (uint8_t)mode) != HAL_OK) {
        return -1;
    }
    HAL_Delay(2);

    dev->mode = mode;

    return 0; // success
}

adxl382_error_t ADXL382_Init(adxl382_t *dev) {
    uint8_t id = 0;
    if(ADXL382_ReadReg(dev->hi2c, ADXL382_REG_DEVID_AD, &id) != 0) {
        return ADXL382_I2C_ERROR;
    }
    if(id != ADXL382_DEVID_AD_VAL) {
        return ADXL382_ID_ERROR;
    }

    ADXL382_Reset(dev);
    
    // Enable XYZ & Temp
    if(ADXL382_WriteReg(dev->hi2c, ADXL382_REG_DIG_EN, 0xF0) != 0) {
		return ADXL382_I2C_ERROR;
	}
    
    // Range
    if(ADXL382_SetRange(dev, dev->range) != 0) {
		return ADXL382_RANGE_ERROR;
	}

    // Mode
	if(ADXL382_SetMode(dev, dev->mode) != 0) {
		return ADXL382_MODE_ERROR;
	}

    return ADXL382_OK; // success
}

bool ADXL382_IsDataReady(adxl382_t *dev) {
    uint8_t ready = 0;
    if(ADXL382_ReadReg(dev->hi2c, ADXL382_REG_STATUS3, &ready) == 0) {
    	return (ready & 0x01) ? true : false;
    }

    return false; // failure
}

// TODO: seperate compute + read reg (like baro)
adxl382_error_t ADXL382_ReadData(adxl382_t *dev, const float current_quat[4]) {
    uint8_t buffer[8];
    if(ADXL382_ReadRegs(dev->hi2c, ADXL382_REG_XDATA_H, buffer, 8) == 0) {
        int16_t x = (int16_t)(((uint16_t)buffer[0] << 8) | buffer[1]);
        int16_t y = (int16_t)(((uint16_t)buffer[2] << 8) | buffer[3]);
        int16_t z = (int16_t)(((uint16_t)buffer[4] << 8) | buffer[5]);
        int16_t t_brut = (int16_t)(((uint16_t)buffer[6] << 8) | buffer[7]);
        t_brut >>= 4; // 16 bits -> 12 bits adc

        dev->temp = 25.0f + ((float)t_brut - 550.0f) / 10.2f;

        float t = dev->temp;
        float delta_t = dev->temp - 25.0f;

        float scale_factor_25c = 2000.0f;
        if(dev->range == ADXL382_RANGE_30G) {
            scale_factor_25c = 1000.0f;
        } else if(dev->range == ADXL382_RANGE_60G) {
            scale_factor_25c = 500.0f;
        }

        // Datasheet Compensation (+0.02%/C)
        float current_scale_factor = scale_factor_25c * (1.0f + (0.0002f * delta_t));
        float acc_x_raw = (float)x / current_scale_factor;
        float acc_y_raw = (float)y / current_scale_factor;
        float acc_z_raw = (float)z / current_scale_factor;

        // Chip Thermal Compensation
        float offset_x = Thermal_ComputeOffset(dev->x_axis_offset, t);
        float offset_y = Thermal_ComputeOffset(dev->y_axis_offset, t);
        float offset_z = Thermal_ComputeOffset(dev->z_axis_offset, t);

        dev->acc_x = acc_x_raw - offset_x;
        dev->acc_y = acc_y_raw - offset_y;
        dev->acc_z = acc_z_raw - offset_z;

        float compensated_accel[3] = {dev->acc_x, dev->acc_y, dev->acc_z};
        dev->acc_vertical = Math_ComputeWorldVerticalAcc(compensated_accel, current_quat, true);

    } else {
        return ADXL382_I2C_ERROR;
    }

    return ADXL382_OK; // success
}
