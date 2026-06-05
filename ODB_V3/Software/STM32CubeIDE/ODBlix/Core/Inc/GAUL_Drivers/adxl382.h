/*
 * adxl382.h
 *
 *  Created on: 13 avr. 2026
 *      Author: gagno
 */

#ifndef INC_GAUL_DRIVERS_ADXL382_H_
#define INC_GAUL_DRIVERS_ADXL382_H_

#include "stm32f4xx_hal.h"
#include "GAUL_Drivers/utils.h"

#include <stdint.h>
#include <stdbool.h>

#define ADXL382_I2C_ADDR          (0x1D << 1)
#define ADXL382_DEVID_AD_VAL      0xAD
#define ADXL382_DEVID_MST_VAL     0x1D
#define ADXL382_PART_ID_VAL       0x17

#define ADXL382_REG_DEVID_AD      0x00
#define ADXL382_REG_DEVID_MST     0x01
#define ADXL382_REG_PART_ID       0x02
#define ADXL382_REG_STATUS3       0x14
#define ADXL382_REG_XDATA_H       0x15
#define ADXL382_REG_XDATA_L       0x16
#define ADXL382_REG_YDATA_H       0x17
#define ADXL382_REG_YDATA_L       0x18
#define ADXL382_REG_ZDATA_H       0x19
#define ADXL382_REG_ZDATA_L       0x1A
#define ADXL382_REG_OP_MODE       0x26
#define ADXL382_REG_DIG_EN        0x27
#define ADXL382_REG_REG_RESET     0x2A


typedef enum {
    ADXL382_OK 			= 0,
    ADXL382_ERROR 		= -1,
    ADXL382_I2C_ERROR 	= -2,
    ADXL382_ID_ERROR 	= -3,
    ADXL382_MODE_ERROR 	= -4,
    ADXL382_RANGE_ERROR = -5
} adxl382_error_t;

typedef enum {
    ADXL382_MODE_STANDBY = 0x00,
    ADXL382_MODE_HS      = 0x01, // Heart Sounds
    ADXL382_MODE_ULP     = 0x02, // Ultra Low Power
    ADXL382_MODE_VLP     = 0x03, // Very Low Power
    ADXL382_MODE_LP      = 0x04, // Low Power
    ADXL382_MODE_RBW     = 0x08, // Reduced Bandwidth
    ADXL382_MODE_HP      = 0x0C  // High Performance
} adxl382_mode_t;

typedef enum {
    ADXL382_RANGE_15G = 0x00,
    ADXL382_RANGE_30G = 0x01,
    ADXL382_RANGE_60G = 0x02
} adxl382_range_t;

typedef struct {
    I2C_HandleTypeDef               *hi2c;
    adxl382_mode_t                  mode;
    adxl382_range_t                 range;
    coeff_poly3_t					x_axis_offset, y_axis_offset, z_axis_offset;

    uint8_t                         raw_buffer[8];	// Data stored by registers
    float             				acc_x;	        // m/s2, after thermal compensation and local frame transformation
    float             				acc_y;	        // m/s2, after thermal compensation and local frame transformation
    float             				acc_z;	        // m/s2, after thermal compensation and local frame transformation
    float            				acc_vertical;   // m/s2, vertical acceleration after world frame transformation and gravity compensation (effective acceleration)
    float               			temp;	        // C
} adxl382_t;


adxl382_error_t ADXL382_Init(adxl382_t *dev);

bool ADXL382_IsDataReady(adxl382_t *dev);
int8_t ADXL382_SetMode(adxl382_t *dev, adxl382_mode_t mode);
adxl382_error_t ADXL382_ReadData(adxl382_t *dev);
void ADXL382_Compute(adxl382_t *dev, const float current_quat[4]);

#endif /* INC_GAUL_DRIVERS_ADXL382_H_ */
