/*
 * adxl382.h
 *
 *  Created on: 13 avr. 2026
 *      Author: gagno
 */

#ifndef INC_DRIVERS_ADXL382_H_
#define INC_DRIVERS_ADXL382_H_

#include "Protocols/config_protocol.h"
#include "Utils/utils.h"
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>


#define ADXL382_I2C_ADDR (0x1D << 1)


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
    acc_axis_profile_t              axis_profile;
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

#endif /* INC_DRIVERS_ADXL382_H_ */
