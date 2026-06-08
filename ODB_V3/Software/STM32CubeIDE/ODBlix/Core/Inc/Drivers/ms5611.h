/*
 * ms561101ba03.h
 *
 * Created on: 14 mars 2026
 * Author: Luka
 */

#ifndef INC_DRIVERS_MS5611_H_
#define INC_DRIVERS_MS5611_H_

#include "stm32f4xx_hal.h"
#include <stdbool.h>


typedef enum {
    MS5611_OK,
    MS5611_ERR_SPI,
    MS5611_ERR_CRC,
    MS5611_ERR_DEV,
	MS5611_NOT_READY,	// first conversion
} ms5611_error_t;

typedef enum {
    MS5611_STATE_PRESSURE,
    MS5611_STATE_TEMPERATURE
} ms5611_state_t;

typedef enum {
    OSR256  = 0x00, // fastest conversion time -> lowest resolution
    OSR512  = 0x02,
    OSR1024 = 0x04,
    OSR2048 = 0x06,
    OSR4096 = 0x08 // slowest conversion time -> highest resolution
} ms5611_osr_t;

typedef struct {
    SPI_HandleTypeDef   *spi;
    GPIO_TypeDef        *cs_port;
    uint16_t            cs_pin;

    uint8_t             cmd_pressure;
    uint8_t             cmd_temperature;
    uint8_t             delay_read_pressure;
    uint8_t             delay_read_temperature;

    uint16_t            calib_coeffs[8];

    ms5611_state_t      state;
    uint32_t            last_conversion_time;
    uint8_t             skip_counter_temperature;
    bool 				first_conversion_done;

    uint32_t			ground_pressure;
    uint32_t            raw_pressure;
    uint32_t            raw_temperature;
} ms5611_t;


ms5611_error_t MS5611_Init(ms5611_t *dev, ms5611_osr_t osr_pressure, ms5611_osr_t osr_temperature);

ms5611_error_t MS5611_Update(ms5611_t *dev);
ms5611_error_t MS5611_Compute(ms5611_t *dev, float *temperature, float *pressure);

#endif /* INC_DRIVERS_MS5611_H_ */
