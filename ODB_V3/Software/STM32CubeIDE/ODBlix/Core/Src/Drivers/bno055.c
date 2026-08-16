/*
 * bno055.c
 *
 * Created on: 20 avr. 2026
 * Author: AudaceLol12
 */

#include "Drivers/bno055.h"
#include "Systems/config.h"
#include "Utils/utils.h"
#include <math.h>

#define BNO055_ID_VAL               0xA0
// Page 0
#define BNO055_REG_SW_REV_ID_LSB    0x04
#define BNO055_REG_SW_REV_ID_MSB    0x05
#define BNO055_REG_PAGE_ID          0x07
#define BNO055_REG_CHIP_ID          0x00
#define BNO055_REG_ACC_DATA_X_LSB   0x08 // Accel: 6 octets
#define BNO055_REG_MAG_DATA_X_LSB   0x0E // Mag: 6 octets
#define BNO055_REG_GYR_DATA_X_LSB   0x14 // Gyro: 6 octets
#define BNO055_REG_QUA_DATA_W_LSB   0x20 // Quat: 8 octets
#define BNO055_REG_LIA_DATA_X_LSB   0x28 // Linear Accel: 6 octets
#define BNO055_REG_TEMP             0x34 // Température
#define BNO055_REG_CALIB_STAT       0x35
#define BNO055_REG_PWR_MODE         0x3E
#define BNO055_REG_SYS_TRIGGER      0x3F
#define BNO055_REG_OPR_MODE         0x3D
#define BNO055_REG_UNIT_SEL         0x3B
#define BNO055_REG_AXIS_MAP_CONFIG  0x41 // Swap axis
#define BNO055_REG_AXIS_MAP_SIGN    0x42 // Sign axis
#define BNO055_REG_ACC_OFFSET_X_LSB 0x55 // Manual offsets
// Page 1
#define BNO055_REG_ACC_CONFIG       0x08
#define BNO055_REG_MAG_CONFIG       0x09
#define BNO055_REG_GYR_CONFIG_0     0x0A
#define BNO055_REG_GYR_CONFIG_1     0x0B

#define BNO055_REG_ST_RESULT        0x36
#define BNO055_REG_SYS_STATUS       0x39
#define BNO055_REG_SYS_ERR          0x3A

//									   P0	 P1	   P2	 P3	   P4	 P5	   P6	 P7
const uint8_t BNO055_REMAP_CONFIG[] = {0x24, 0x21, 0x24, 0x21, 0x18, 0x09, 0x18, 0x09};
const uint8_t BNO055_REMAP_SIGN[]   = {0x00, 0x02, 0x06, 0x04, 0x00, 0x04, 0x05, 0x00};


static int8_t BNO055_ReadReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *data) {
    if(HAL_I2C_Mem_Read(hi2c, BNO055_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, 1, 25) != HAL_OK) {
        return -1;
    }
    return 0; // success
}

static int8_t BNO055_WriteReg(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t data) {
    if(HAL_I2C_Mem_Write(hi2c, BNO055_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, &data, 1, 25) != HAL_OK) {
        return -1;
    }
    return 0; // success
}

static int8_t BNO055_ReadRegs(I2C_HandleTypeDef *hi2c, uint8_t reg, uint8_t *data, uint16_t length) {
    if(HAL_I2C_Mem_Read(hi2c, BNO055_I2C_ADDR, reg, I2C_MEMADD_SIZE_8BIT, data, length, 25) != HAL_OK) {
        return -1;
    }
    return 0; // success
}

static int8_t BNO055_SetPage(I2C_HandleTypeDef *hi2c, uint8_t page) {
    return BNO055_WriteReg(hi2c, BNO055_REG_PAGE_ID, page);
}

static int8_t BNO055_SetUnits(bno055_t *dev, bno055_unit_accel_t acc_unit, bno055_unit_gyro_t gyr_unit, bno055_unit_euler_t eul_unit) {
    uint8_t unit_sel = acc_unit | gyr_unit | eul_unit;
    if(BNO055_WriteReg(dev->hi2c, BNO055_REG_UNIT_SEL, unit_sel) != 0) {
        return -1;
    }

    if(acc_unit == BNO055_UNIT_ACCEL_MS2) {
        dev->scale_acc = 100.0f; // 1 m/s² = 100 LSB
    } else {
        dev->scale_acc = 1.0f;   // 1 mg = 1 LSB
    }

    if(gyr_unit == BNO055_UNIT_GYRO_DPS) {
        dev->scale_gyro = 16.0f;   // 1 Dps = 16 LSB
    } else {
        dev->scale_gyro = 900.0f;  // 1 Rps = 900 LSB
    }

    return 0; // success
}
    
static int8_t BNO055_SetAccConfig(bno055_t *dev, bno055_acc_range_t range) {
    // Set page 1
    if(BNO055_SetPage(dev->hi2c, 0x01) != 0) {
        return -1;
    }

    uint8_t current_acc_config;
    if(BNO055_ReadReg(dev->hi2c, BNO055_REG_ACC_CONFIG, &current_acc_config) != 0) {
        return -1;
    }
    
    // Set range
    uint8_t new_acc_config = (current_acc_config & 0xFC) | range;
    if(BNO055_WriteReg(dev->hi2c, BNO055_REG_ACC_CONFIG, new_acc_config) != 0) {
        return -1;
    }

    // Set page 0
    if(BNO055_SetPage(dev->hi2c, 0x00) != 0) {
        return -1;
    }

    return 0; // success
}

static int8_t BNO055_SetAxisRemap(bno055_t *dev, acc_axis_profile_t profile) {
    uint8_t config = BNO055_REMAP_CONFIG[profile];
    uint8_t sign = BNO055_REMAP_SIGN[profile];
    if(BNO055_WriteReg(dev->hi2c, BNO055_REG_AXIS_MAP_CONFIG, config) != 0) {
        return BNO055_I2C_ERROR;
    }
    if(BNO055_WriteReg(dev->hi2c, BNO055_REG_AXIS_MAP_SIGN, sign) != 0) {
        return BNO055_I2C_ERROR;
    }

    return BNO055_OK;
}

static int8_t BNO055_Reset(bno055_t *dev) {
    if(BNO055_WriteReg(dev->hi2c, BNO055_REG_SYS_TRIGGER, 0x20) != 0) {
        return -1;
    }
    HAL_Delay(700);

    dev->acc_x = 0.0f;
    dev->acc_y = 0.0f;
    dev->acc_z = 0.0f;
    dev->gyro_x = 0.0f;
    dev->gyro_y = 0.0f;
    dev->gyro_z = 0.0f;
    dev->mag_x = 0.0f;
    dev->mag_y = 0.0f;
    dev->mag_z = 0.0f;
    dev->temperature = 0.0f;
    dev->acc_vertical = 0.0f;
    dev->quat.w = 0.0f;
    dev->quat.x = 0.0f;
    dev->quat.y = 0.0f;
    dev->quat.z = 0.0f;
    dev->linear_acc.x = 0.0f;
    dev->linear_acc.y = 0.0f;
    dev->linear_acc.z = 0.0f;
    dev->euler_angles.roll = 0.0f;
    dev->euler_angles.pitch = 0.0f;
    dev->euler_angles.yaw = 0.0f;

    return 0; // success
}


bno055_error_t BNO055_SetPowerMode(bno055_t *dev, bno055_pwr_mode_t power_mode) {
    if(BNO055_SetPage(dev->hi2c, 0x00) != 0) {
        return BNO055_I2C_ERROR;
    }

    if(BNO055_WriteReg(dev->hi2c, BNO055_REG_PWR_MODE, (uint8_t)power_mode) != 0) {
        return BNO055_I2C_ERROR;
    }
    HAL_Delay(30); 

    return BNO055_OK;
}

bno055_error_t BNO055_Init(bno055_t *dev) {
    if(!dev || !dev->hi2c || (dev->op_mode != BNO055_OP_MODE_IMU && dev->op_mode != BNO055_OP_MODE_NDOF && dev->op_mode != BNO055_OP_MODE_AMG)) {
        return BNO055_ERROR;
    }

    // Disable Reset
    HAL_GPIO_WritePin(dev->rst_port, dev->rst_pin, GPIO_PIN_SET);
    HAL_Delay(800);

    // Check ID
    uint8_t id;
    if(BNO055_ReadReg(dev->hi2c, BNO055_REG_CHIP_ID, &id) != 0) {
        return BNO055_I2C_ERROR;
    }
    if(id != BNO055_ID_VAL) {
        return BNO055_ID_ERROR;
    }

    // Reset
    BNO055_Reset(dev);

    // Config mode
    if(BNO055_WriteReg(dev->hi2c, BNO055_REG_OPR_MODE, BNO055_OP_MODE_CONFIG) != 0) {
        return BNO055_CONFIG_ERROR;
    }
    HAL_Delay(25);

    // Set page 0
    if(BNO055_SetPage(dev->hi2c, 0x00) != 0) {
        return BNO055_CONFIG_ERROR;
    }
    HAL_Delay(25);

    // Set units (m/s², Dps, Degrees)
    if(BNO055_SetUnits(dev, dev->acc_unit, dev->gyro_unit, dev->euler_unit) != 0) {
        return BNO055_CONFIG_ERROR;
    }

    // Set axis remap
    dev->axis_profile = (acc_axis_profile_t)current_config.axis_profile;
    if(BNO055_SetAxisRemap(dev, dev->axis_profile) != 0) {
    	return BNO055_CONFIG_ERROR;
    }

    // If in IMU mode, set accelerometer range to ±16G (max for fusion modes is ±4G)
    if(dev->op_mode == BNO055_OP_MODE_AMG) {
        if(BNO055_SetAccConfig(dev, dev->acc_range) != 0) {
             return BNO055_CONFIG_ERROR;
        }
    }

    // Activate operating mode
    if(BNO055_WriteReg(dev->hi2c, BNO055_REG_OPR_MODE, (uint8_t)dev->op_mode) != 0){
        return BNO055_CONFIG_ERROR;
    }
    HAL_Delay(25);

    // Check Self-Test
    uint8_t sys_status, sys_err, st_res;
	HAL_Delay(50);
	if(BNO055_ReadReg(dev->hi2c, BNO055_REG_SYS_STATUS, &sys_status) != 0) {
		return BNO055_I2C_ERROR;
	}
	if(sys_status == 0x01) {
		// Specified error
		BNO055_ReadReg(dev->hi2c, BNO055_REG_SYS_ERR, &sys_err);
		return BNO055_CONFIG_ERROR;
	}
	if(BNO055_ReadReg(dev->hi2c, BNO055_REG_ST_RESULT, &st_res) == 0) {
		if((st_res & 0x0F) != 0x0F) {
			return BNO055_ERROR;
		}
	}

    return BNO055_OK;
}

void BNO055_HardReset(bno055_t *dev) {
    if(dev->rst_port != NULL) {
        HAL_GPIO_WritePin(dev->rst_port, dev->rst_pin, GPIO_PIN_RESET);
        HAL_Delay(10); 
        
        HAL_GPIO_WritePin(dev->rst_port, dev->rst_pin, GPIO_PIN_SET);
        
        HAL_Delay(700); 
    }

    BNO055_Reset(dev);
}

// REV ID 3.11 -> no DRDY, only specific hysteresis
/*
bool BNO055_IsDataReady(bno055_t *dev) {
    if(dev->data_ready_flag) {
        dev->data_ready_flag = false;
        return true;
    }

    return false;
}
*/

bno055_error_t BNO055_ReadAllData(bno055_t *dev) {
    uint8_t buffer[18];
    // Reading Acc, Mag, Gyro
    if(BNO055_ReadRegs(dev->hi2c, BNO055_REG_ACC_DATA_X_LSB, buffer, 18) != 0) {
        return BNO055_I2C_ERROR;
    }

    dev->acc_x  = (float)((int16_t)((buffer[1] << 8) | buffer[0])) / dev->scale_acc;
    dev->acc_y  = (float)((int16_t)((buffer[3] << 8) | buffer[2])) / dev->scale_acc;
    dev->acc_z  = (float)((int16_t)((buffer[5] << 8) | buffer[4])) / dev->scale_acc;

    dev->mag_x  = (float)((int16_t)((buffer[7] << 8) | buffer[6])) / 16.0f;
    dev->mag_y  = (float)((int16_t)((buffer[9] << 8) | buffer[8])) / 16.0f;
    dev->mag_z  = (float)((int16_t)((buffer[11] << 8) | buffer[10])) / 16.0f;

    dev->gyro_x = (float)((int16_t)((buffer[13] << 8) | buffer[12])) / dev->scale_gyro;
    dev->gyro_y = (float)((int16_t)((buffer[15] << 8) | buffer[14])) / dev->scale_gyro;
    dev->gyro_z = (float)((int16_t)((buffer[17] << 8) | buffer[16])) / dev->scale_gyro;

    // Quaternions & Linear Acc
    if(BNO055_ReadRegs(dev->hi2c, BNO055_REG_QUA_DATA_W_LSB, buffer, 14) != 0) {
        return BNO055_I2C_ERROR;
    }

    const float q_scale = 1.0f / 16384.0f;
    dev->quat.w = (float)((int16_t)((buffer[1] << 8) | buffer[0])) * q_scale;
    dev->quat.x = (float)((int16_t)((buffer[3] << 8) | buffer[2])) * q_scale;
    dev->quat.y = (float)((int16_t)((buffer[5] << 8) | buffer[4])) * q_scale;
    dev->quat.z = (float)((int16_t)((buffer[7] << 8) | buffer[6])) * q_scale;

    dev->linear_acc.x = (float)((int16_t)((buffer[9] << 8) | buffer[8])) / dev->scale_acc;
    dev->linear_acc.y = (float)((int16_t)((buffer[11] << 8) | buffer[10])) / dev->scale_acc;
    dev->linear_acc.z = (float)((int16_t)((buffer[13] << 8) | buffer[12])) / dev->scale_acc;

    return BNO055_OK;
}

bno055_error_t BNO055_ReadTemperature(bno055_t *dev) {
    uint8_t reg;
    if(BNO055_ReadReg(dev->hi2c, BNO055_REG_TEMP, &reg) != 0) {
        return BNO055_I2C_ERROR;
    }
    dev->temperature = (int8_t)reg;

    return BNO055_OK;
}

bno055_error_t BNO055_UpdateCalibration(bno055_t *dev) {
    uint8_t reg;
    if(BNO055_ReadReg(dev->hi2c, BNO055_REG_CALIB_STAT, &reg) != 0) {
        return BNO055_I2C_ERROR;
    }

    dev->calib_sys  = (reg >> 6) & 0x03;
    dev->calib_gyro = (reg >> 4) & 0x03;
    dev->calib_acc  = (reg >> 2) & 0x03;
    dev->calib_mag  = reg & 0x03;

    return BNO055_OK;
}

void BNO055_ComputeEulerAngles(bno055_t *dev) {
    float w = dev->quat.w;
    float x = dev->quat.x;
    float y = dev->quat.y;
    float z = dev->quat.z;

    if(w == 0.0f && x == 0.0f && y == 0.0f && z == 0.0f) return;

    // Roll
    float t0 = +2.0f * (w * x + y * z);
    float t1 = +1.0f - 2.0f * (x * x + y * y);
    dev->euler_angles.roll = atan2f(t0, t1) * (180.0f / M_PI);

    // Pitch
    float t2 = +2.0f * (w * y - z * x);
    t2 = (t2 > 1.0f) ? 1.0f : t2;
    t2 = (t2 < -1.0f) ? -1.0f : t2;
    dev->euler_angles.pitch = asinf(t2) * (180.0f / M_PI);

    // Yaw
    float t3 = +2.0f * (w * z + x * y);
    float t4 = +1.0f - 2.0f * (y * y + z * z);
    dev->euler_angles.yaw = atan2f(t3, t4) * (180.0f / M_PI);
}

void BNO055_ComputeVerticalAcc(bno055_t *dev) {
    float accel[3] = {
        dev->linear_acc.x,
        dev->linear_acc.y,
        dev->linear_acc.z
    };

    float quat[4] = {
        dev->quat.w,
        dev->quat.x,
        dev->quat.y,
        dev->quat.z
    };

    dev->acc_vertical = Math_ComputeWorldVerticalAcc(accel, quat, false);
}

// Wait until all calibration data is collected and are at 3/3
bno055_error_t BNO055_GetCalibrationProfile(bno055_t *dev, bno055_calib_profile_t *profile) {
    bno055_op_mode_t current_mode = dev->op_mode;
    
    if(BNO055_WriteReg(dev->hi2c, BNO055_REG_OPR_MODE, BNO055_OP_MODE_CONFIG) != 0) {
        return BNO055_I2C_ERROR;
    }
    HAL_Delay(25);

    if(BNO055_ReadRegs(dev->hi2c, BNO055_REG_ACC_OFFSET_X_LSB, profile->data, 22) != 0) {
        return BNO055_I2C_ERROR;
    }

    if(BNO055_WriteReg(dev->hi2c, BNO055_REG_OPR_MODE, (uint8_t)current_mode) != 0) {
        return BNO055_I2C_ERROR;
    }
    HAL_Delay(25);

    return BNO055_OK;
}

bno055_error_t BNO055_SetCalibrationProfile(bno055_t *dev, bno055_calib_profile_t *profile) {
    bno055_op_mode_t current_mode = dev->op_mode;
    
    if(BNO055_WriteReg(dev->hi2c, BNO055_REG_OPR_MODE, BNO055_OP_MODE_CONFIG) != 0) {
        return BNO055_I2C_ERROR;
    }
    HAL_Delay(25);

    // Write 22 bytes of calibration data starting from ACC_OFFSET_X_LSB
    if(HAL_I2C_Mem_Write(dev->hi2c, BNO055_I2C_ADDR, BNO055_REG_ACC_OFFSET_X_LSB, I2C_MEMADD_SIZE_8BIT, profile->data, 22, 100) != HAL_OK) {
        return BNO055_I2C_ERROR;
    }

    if(BNO055_WriteReg(dev->hi2c, BNO055_REG_OPR_MODE, (uint8_t)current_mode) != 0) {
        return BNO055_I2C_ERROR;
    }
    HAL_Delay(25);

    return BNO055_OK;
}

bno055_error_t BNO055_GetSwRevision(bno055_t *dev, uint16_t *sw_rev) {
    uint8_t buffer[2];
    if(BNO055_SetPage(dev->hi2c, 0x00) != 0) {
        return BNO055_CONFIG_ERROR;
    }

    if(BNO055_ReadRegs(dev->hi2c, BNO055_REG_SW_REV_ID_LSB, buffer, 2) != 0) {
        return BNO055_I2C_ERROR;
    }

    *sw_rev = (uint16_t)((buffer[1] << 8) | buffer[0]);

    return BNO055_OK;
}
