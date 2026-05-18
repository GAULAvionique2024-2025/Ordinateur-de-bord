/*
 * ms561101ba03.c
 *
 * Created on: 14 mars 2026
 * Author: Luka
 */

#include "GAUL_Drivers/ms5611.h"
#include "GAUL_Drivers/utils.h"


static inline void MS5611_CS_LOW(ms5611_t *dev) {
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_RESET);
}

static inline void MS5611_CS_HIGH(ms5611_t *dev) {
    HAL_GPIO_WritePin(dev->cs_port, dev->cs_pin, GPIO_PIN_SET);
}

static int8_t MS5611_SPI_Transmit(ms5611_t *dev, uint8_t* data, uint16_t size) {
    MS5611_CS_LOW(dev);
    if(HAL_SPI_Transmit(dev->spi, data, size, 5) != HAL_OK) {
        MS5611_CS_HIGH(dev);
        return -1;
    }
    MS5611_CS_HIGH(dev);

    return 0; // success
}

static int8_t MS5611_SPI_TransmitReceive(ms5611_t *dev, uint8_t *cmd, uint8_t* rx_data, uint16_t rx_size) {
    MS5611_CS_LOW(dev);
    if(HAL_SPI_Transmit(dev->spi, cmd, 1, 5) != HAL_OK) {
        MS5611_CS_HIGH(dev);

        return -1;
    }
    if(HAL_SPI_Receive(dev->spi, rx_data, rx_size, 5) != HAL_OK) {
        MS5611_CS_HIGH(dev);

        return -1;
    }
    MS5611_CS_HIGH(dev);

    return 0; // success
}

static int8_t MS5611_SendCmd(ms5611_t* dev, uint8_t cmd) {
    if(MS5611_SPI_Transmit(dev, &cmd, 1) != 0) {
        return -1;
    }

    return 0; // success
}

static int8_t MS5611_ReadADC(ms5611_t *dev, uint32_t* data) {
    uint8_t cmd = MS5611_CMD_ADC_READ;
    uint8_t rx[3] = {0};

    if(MS5611_SPI_TransmitReceive(dev, &cmd, rx, 3) != 0) {
        return -1;
    }

    *data = ((uint32_t)rx[0] << 16) | ((uint32_t)rx[1] << 8) | rx[2];

    return 0; // success
}

static int8_t MS5611_Reset(ms5611_t *dev) {
    uint8_t cmd = MS5611_CMD_RESET;

    if(MS5611_SPI_Transmit(dev, &cmd, 1) != 0) {
        return -1;
    }
    HAL_Delay(10);

    return 0; // success
}

static int8_t MS5611_ReadPROM(ms5611_t *dev, uint8_t index, uint16_t* coeffs) {
    uint8_t cmd = MS5611_CMD_PROM_READ + (index * 2);
    uint8_t rx[2] = {0};

    if(MS5611_SPI_TransmitReceive(dev, &cmd, rx, 2) != 0) {
        return -1;
    }

    *coeffs = (rx[0] << 8) | rx[1];

    return 0; // success
}

static uint8_t MS5611_CRC4(uint16_t prom[]) {
    uint16_t n_rem = 0;
    uint16_t crc_read = prom[7];

    prom[7] &= 0xFF00;
    for(int cnt = 0; cnt < 16; cnt++) {
        if(cnt % 2 == 1) {
            n_rem ^= (uint16_t)(prom[cnt >> 1] & 0x00FF);
        } else {
            n_rem ^= (uint16_t)(prom[cnt >> 1] >> 8);
        }

        for(int n_bit = 8; n_bit > 0; n_bit--) {
            if(n_rem & 0x8000) {
                n_rem = (n_rem << 1) ^ 0x3000;
            } else {
                n_rem <<= 1;
            }   
        }
    }

    n_rem = (n_rem >> 12) & 0x000F;
    prom[7] = crc_read;

    return n_rem ^ 0x00;
}

// AGL (ground reference) calibration
void MS5611_CalibrateGroundPressure(ms5611_t *dev) {
    float temp_p, temp_t;
    float sum_p = 0;
    int samples = 100;
    int valid_samples = 0;

    dev->first_conversion_done = false;

    while(valid_samples < samples) {
		MS5611_Update(dev);
		if(MS5611_Compute(dev, &temp_t, &temp_p) == MS5611_OK) {
			sum_p += temp_p;
			valid_samples++;
		}

		HAL_Delay(5);
	}

	if(valid_samples > 0) {
		dev->ground_pressure = (uint32_t)(sum_p / valid_samples);
	} else {
		dev->ground_pressure = PRESSURE_SEA_LEVEL_HPA;
	}
}

static uint8_t MS5611_GetDelay(ms5611_osr_t osr) {
    switch(osr) {
        case OSR256:  
            return 1;
        case OSR512:
            return 2;
        case OSR1024:
            return 3;
        case OSR2048: 
            return 5;
        case OSR4096:
            return 10;
        default:
            return 10;
    }
}


ms5611_error_t MS5611_Init(ms5611_t *dev, ms5611_osr_t osr_pressure, ms5611_osr_t osr_temperature) {
    if(!dev) return MS5611_ERR_DEV;
    if(!dev->spi || !dev->cs_port) return MS5611_ERR_SPI;

    MS5611_CS_HIGH(dev);

    dev->cmd_pressure = MS5611_CMD_PRESSURE + osr_pressure;
    dev->cmd_temperature = MS5611_CMD_TEMPERATURE + osr_temperature;
    dev->skip_counter_temperature = 0;

    dev->delay_read_pressure = MS5611_GetDelay(osr_pressure);
    dev->delay_read_temperature = MS5611_GetDelay(osr_temperature);

    // Reset
    if(MS5611_Reset(dev) != 0) return MS5611_ERR_SPI;
    for(int i = 0; i < 8; i++) {
    	// Get Calibration coefficients
        if(MS5611_ReadPROM(dev, i, &dev->calib_coeffs[i]) != 0) {
            return MS5611_ERR_SPI;
        }
    }

    // Check CRC
    uint8_t crc_read = dev->calib_coeffs[7] & 0x000F;
    if(MS5611_CRC4(dev->calib_coeffs) != crc_read) {
        return MS5611_ERR_CRC;
    }

    dev->state = MS5611_STATE_PRESSURE;

    dev->raw_pressure = 0;
    dev->raw_temperature = 0;
    dev->first_conversion_done = false;

    if(MS5611_SendCmd(dev, dev->cmd_pressure) != 0) return MS5611_ERR_SPI;

    dev->last_conversion_time = HAL_GetTick();

    // Pressure calibration reference
    dev->ground_pressure = PRESSURE_SEA_LEVEL_HPA;
    MS5611_CalibrateGroundPressure(dev);

    return MS5611_OK;
}

ms5611_error_t MS5611_Update(ms5611_t *dev) {
    uint32_t delay_ms = (dev->state == MS5611_STATE_PRESSURE) ? dev->delay_read_pressure : dev->delay_read_temperature;
    if(HAL_GetTick() - dev->last_conversion_time < delay_ms) {
        return MS5611_OK;
    }

    if(dev->state == MS5611_STATE_PRESSURE) {
        if(MS5611_ReadADC(dev, &dev->raw_pressure) != 0) return MS5611_ERR_SPI;
        if(dev->skip_counter_temperature == 0) {
            if(MS5611_SendCmd(dev, dev->cmd_temperature) != 0) return MS5611_ERR_SPI;

            dev->state = MS5611_STATE_TEMPERATURE;
            dev->skip_counter_temperature = MS5611_SKIP_TEMPERATURE_X_TIME;
        } else {
            dev->skip_counter_temperature--;

            if(MS5611_SendCmd(dev, dev->cmd_pressure) != 0) return MS5611_ERR_SPI;
        }
    } else { /* TEMPERATURE */
        if(MS5611_ReadADC(dev, &dev->raw_temperature) != 0) return MS5611_ERR_SPI;

        dev->first_conversion_done = true;

        if(MS5611_SendCmd(dev, dev->cmd_pressure) != 0) return MS5611_ERR_SPI;

        dev->state = MS5611_STATE_PRESSURE;
    }

    dev->last_conversion_time = HAL_GetTick();

    return MS5611_OK;
}

ms5611_error_t MS5611_Compute(ms5611_t *dev, float *temperature, float *pressure) {
	if(!dev->first_conversion_done) {
		return MS5611_NOT_READY;
	}

    int32_t dt = (int32_t)dev->raw_temperature - ((int32_t)dev->calib_coeffs[5] << 8);
    int32_t temp = 2000 + (int32_t)(((int64_t)dt * dev->calib_coeffs[6]) >> 23);

    int64_t offset = ((int64_t)dev->calib_coeffs[2] << 16) + (((int64_t)dev->calib_coeffs[4] * dt) >> 7);
    int64_t sens = ((int64_t)dev->calib_coeffs[1] << 15) + (((int64_t)dev->calib_coeffs[3] * dt) >> 8);

    int64_t temp2 = 0;
    int64_t offset2 = 0;
    int64_t sens2 = 0;

    if(temp < 2000) {
        temp2 = ((int64_t)dt * dt) >> 31;
        offset2 = 5 * (((int64_t)temp - 2000) * ((int64_t)temp - 2000)) >> 1;
        sens2 = 5 * (((int64_t)temp - 2000) * ((int64_t)temp - 2000)) >> 2;

        if(temp < -1500) {
            offset2 += 7 * (((int64_t)temp + 1500) * ((int64_t)temp + 1500));
            sens2 += (11 * (((int64_t)temp + 1500) * ((int64_t)temp + 1500))) >> 1;
        }
    }

    temp -= temp2;
    offset -= offset2;
    sens -= sens2;

    int32_t p = (int32_t)(((((int64_t)dev->raw_pressure * sens) >> 21) - offset) >> 15);

    *temperature = (float)temp / 100.0f;
    *pressure = (float)p / 100.0f;

    return MS5611_OK;
}
