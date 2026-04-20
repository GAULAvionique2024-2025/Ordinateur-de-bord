/*
 * bno055.h
 *
 * Created on: 20 avr. 2026
 * Author: AudaceLol12
 */

#ifndef INC_GAUL_DRIVERS_BNO055_H_
#define INC_GAUL_DRIVERS_BNO055_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <stdbool.h>

#define BNO055_I2C_ADDR             (0x29 << 1)
#define BNO055_ID_VAL               0xA0
// Page 0
#define BNO055_REG_PAGE_ID          0x07
#define BNO055_REG_CHIP_ID          0x00
#define BNO055_REG_ACC_DATA_X_LSB   0x08 // Accel: 6 octets
#define BNO055_REG_MAG_DATA_X_LSB   0x0E // Mag: 6 octets
#define BNO055_REG_GYR_DATA_X_LSB   0x14 // Gyro: 6 octets
#define BNO055_REG_QUA_DATA_W_LSB   0x20 // Quat: 8 octets
#define BNO055_REG_LIA_DATA_X_LSB   0x28 // Linear Accel: 6 octets
#define BNO055_REG_TEMP             0x34 // Température
#define BNO055_REG_CALIB_STAT       0x35
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

typedef enum {
    BNO055_OK            	= 0,
    BNO055_ERROR         	= -1,
    BNO055_I2C_ERROR     	= -2,
    BNO055_ID_ERROR      	= -3,
    BNO055_CONFIG_ERROR  	= -4
} bno055_error_t;

typedef enum {
    BNO055_UNIT_ACCEL_MS2   = 0x00,
    BNO055_UNIT_ACCEL_MG    = 0x01,
} bno055_unit_accel_t;

typedef enum {
    BNO055_UNIT_GYRO_DPS    = 0x00,
    BNO055_UNIT_GYRO_RPS    = 0x02,
} bno055_unit_gyro_t;

typedef enum {
    BNO055_UNIT_EULER_DEG   = 0x00,
    BNO055_UNIT_EULER_RAD   = 0x04,
} bno055_unit_euler_t;

typedef enum {
    BNO055_ACC_RANGE_2G     = 0x00,
    BNO055_ACC_RANGE_4G     = 0x01,
    BNO055_ACC_RANGE_8G     = 0x02,
    BNO055_ACC_RANGE_16G    = 0x03,
} bno055_acc_range_t;

typedef enum {
    BNO055_MODE_CONFIG      = 0x00,
    BNO055_MODE_AMG         = 0x07, // Not-Fusion (Raw Accel+Mag+Gyro)
    BNO055_MODE_IMU         = 0x08, // Fusion (Accel+Gyro) (Max 4G)
    BNO055_MODE_NDOF        = 0x0C  // Fusion (Accel+Gyro+Mag) (Max 4G)
} bno055_mode_t;

typedef enum {
    BNO055_AXIS_P0 = 0, // Default: X forward, Y left, Z up
    BNO055_AXIS_P1,     // On its side, rotated 90°
    BNO055_AXIS_P2,     // On its side, rotated 180°
    BNO055_AXIS_P3,     // On its side, rotated 270°
    BNO055_AXIS_P4,     // Vertical, on the long side
    BNO055_AXIS_P5,     // Vertical, on the short side
    BNO055_AXIS_P6,     // Vertical, on the other long side
    BNO055_AXIS_P7      // Vertical, on the other short side
} bno055_axis_profile_t;

typedef struct {
    uint8_t data[22];
} bno055_calib_profile_t;

typedef struct {
    I2C_HandleTypeDef       *hi2c;
    GPIO_TypeDef            *drdy_port;
    uint16_t                drdy_pin;
    GPIO_TypeDef            *rst_port;
    uint16_t                rst_pin;

    bno055_mode_t           mode;
    bno055_axis_profile_t   axis_profile;
    bno055_acc_range_t      acc_range;

    bno055_unit_accel_t     acc_unit;
    bno055_unit_gyro_t      gyro_unit;
    bno055_unit_euler_t     euler_unit;

    volatile bool           data_ready_flag;

    float                   scale_acc, scale_gyro;                          // Scale factors for raw data conversion (magnetometer is always 16 LSB/uT)

    uint8_t                 calib_sys, calib_gyro, calib_acc, calib_mag;    // 0-3 (0=not calibrated, 3=perfectly calibrated)

    float                   acc_x, acc_y, acc_z;                            // m/s^2
    float                   gyro_x, gyro_y, gyro_z;                         // Dps
    float                   mag_x, mag_y, mag_z;                            // uT
    float                   temperature;                                    // C

    float                   quat_w, quat_x, quat_y, quat_z;
    float                   lin_x, lin_y, lin_z;                            // Linear Accel (without gravity)
    float                   roll, pitch, yaw;                               // Euler angles (degrees)
} bno055_t;

bno055_error_t BNO055_Init(bno055_t *dev);
void BNO055_HardReset(bno055_t *dev);

bool BNO055_IsDataReady(bno055_t *dev);
bno055_error_t BNO055_ReadAllData(bno055_t *dev);
bno055_error_t BNO055_ReadTemperature(bno055_t *dev);
bno055_error_t BNO055_UpdateCalibration(bno055_t *dev);
void BNO055_ComputeEulerAngles(bno055_t *dev);

bno055_error_t BNO055_GetCalibrationProfile(bno055_t *dev, bno055_calib_profile_t *profile);
bno055_error_t BNO055_SetCalibrationProfile(bno055_t *dev, bno055_calib_profile_t *profile);

#endif // INC_GAUL_DRIVERS_BNO055_H_
