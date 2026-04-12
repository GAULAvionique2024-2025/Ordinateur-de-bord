/*
 * ms561101ba03.h
 *
 *  Created on: 14 mars 2026
 *      Author: Luka
 */

#ifndef INC_GAUL_DRIVERS_MS5611_H_
#define INC_GAUL_DRIVERS_MS5611_H_

#include "utils.h"

#define MS5611_CMD_RESET       			0x1E
#define MS5611_CMD_ADC_READ    			0x00

#define MS5611_CMD_PRESSURE     		0x40
#define MS5611_CMD_TEMPERATURE  		0x50

#define MS5611_CMD_PROM_READ   			0xA0

#define MS5611_CS_PORT 					GPIOA
#define MS5611_CS_PIN  					GPIO_PIN_4

#define MS5611_SKIP_TEMPERATURE_X_TIME	10

typedef enum _error_ms5611 {
    MS5611_OK,
    MS5611_ERR_SPI,
	MS5611_ERR_CRC,
	MS5611_ERR_TYPE,
	MS5611_ERR_DEV_NULL,
	MS5611_ERR_SPI_NULL,
} error_ms5611;

typedef enum {
    MS5611_PRESSURE,
    MS5611_TEMPERATURE
} ms5611_data_type;

typedef enum {
    MS5611_STATE_PRESSURE,
    MS5611_STATE_TEMPERATURE
} ms5611_state;

typedef enum osr_option {
	OSR256 	= 0x00,
	OSR512 	= 0x02,
	OSR1024 = 0x04,
	OSR2048 = 0x06,
	OSR4096 = 0x08
} osr_option;

typedef struct ms5611_dev {
	SPI_HandleTypeDef* spi;

	uint8_t cmd_pressure;
	uint8_t cmd_temperature;

	uint8_t delay_read_pressure;
	uint8_t delay_read_temperature;

	ms5611_state state;
	uint32_t last_conversion_time;

	uint16_t C[7];

	uint32_t raw_pressure;
	uint32_t raw_temperature;

	uint8_t skip_counter_temperature;

} ms5611_dev;

error_ms5611 MS5611_Init(ms5611_dev* dev, SPI_HandleTypeDef* spi, osr_option osr_pressure, osr_option ors_temperature);

error_ms5611 MS5611_ReadData(ms5611_dev* dev);
void MS5611_Compute(ms5611_dev* dev, float *temperature, float *pressure);
error_ms5611 MS5611_ReadADC(SPI_HandleTypeDef* hspi, uint32_t* data);
error_ms5611 MS5611_Reset(SPI_HandleTypeDef* hspi);
error_ms5611 MS5611_ReadPROM(SPI_HandleTypeDef* hspi, uint8_t index, uint16_t* c);
uint8_t MS5611_CRC4(uint16_t prom[]);

void MS5611_CS_LOW();
void MS5611_CS_HIGH();

static error_ms5611 MS5611_SendCmd(ms5611_dev* dev, uint8_t cmd);
static uint8_t MS5611_GetDelay(osr_option osr);

#endif /* INC_GAUL_DRIVERS_MS5611_H_ */
