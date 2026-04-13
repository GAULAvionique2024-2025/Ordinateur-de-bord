/*
 * ms561101ba03.c
 *
 *  Created on: 14 mars 2026
 *      Author: Luka
 */

#include "GAUL_Drivers/ms5611.h"

error_ms5611 MS5611_Init(ms5611_dev* dev, SPI_HandleTypeDef* spi, osr_option osr_pressure, osr_option ors_temperature)
{
	if(dev == NULL)
	    return MS5611_ERR_DEV_NULL;

	if(spi == NULL)
	    return MS5611_ERR_SPI_NULL;

	dev->spi = spi;
	dev->cmd_pressure = MS5611_CMD_PRESSURE + osr_pressure;
	dev->cmd_temperature = MS5611_CMD_TEMPERATURE + ors_temperature;
	dev->skip_counter_temperature = 0;

	dev->delay_read_pressure = MS5611_GetDelay(osr_pressure);
	dev->delay_read_temperature = MS5611_GetDelay(ors_temperature);

	MS5611_Reset(dev->spi);

	uint16_t prom[8];

	for(int i = 0; i < 8; i++)
	{
	    if(MS5611_ReadPROM(dev->spi, i, &prom[i]) != MS5611_OK)
	    	return MS5611_ERR_SPI;
	}

	if(MS5611_CRC4(prom) != (prom[0] & 0xF))
		return MS5611_ERR_CRC;

	for(int i=1;i<=6;i++)
	    dev->C[i] = prom[i];

	dev->state = MS5611_STATE_PRESSURE;
	dev->last_conversion_time = HAL_GetTick();


	MS5611_CS_LOW();
	if(HAL_SPI_Transmit(dev->spi, &dev->cmd_pressure, 1, HAL_MAX_DELAY) != HAL_OK)
		goto error_spi;
	MS5611_CS_HIGH();

	return MS5611_OK;

error_spi:
	MS5611_CS_HIGH();
	return MS5611_ERR_SPI;
}

error_ms5611 MS5611_Update(ms5611_dev* dev)
{
    uint32_t delay_ms =
        (dev->state == MS5611_STATE_PRESSURE) ?
        dev->delay_read_pressure :
        dev->delay_read_temperature;

    if(HAL_GetTick() - dev->last_conversion_time < delay_ms)
        return MS5611_OK;

    if(dev->state == MS5611_STATE_PRESSURE)
    {
        if(MS5611_ReadADC(dev->spi, &dev->raw_pressure) != MS5611_OK)
        	return MS5611_ERR_SPI;

        if(dev->skip_counter_temperature == 0)
        {
            if(MS5611_SendCmd(dev, dev->cmd_temperature) != MS5611_OK)
            	return MS5611_ERR_SPI;

            dev->state = MS5611_STATE_TEMPERATURE;
            dev->skip_counter_temperature = MS5611_SKIP_TEMPERATURE_X_TIME;
        }
        else
        {
            dev->skip_counter_temperature--;
        }
    }
    else // TEMPERATURE
    {
        if(MS5611_ReadADC(dev->spi, &dev->raw_temperature) != MS5611_OK)
        	return MS5611_ERR_SPI;

        if(MS5611_SendCmd(dev, dev->cmd_pressure) != MS5611_OK)
        	return MS5611_ERR_SPI;

        dev->state = MS5611_STATE_PRESSURE;
    }

    dev->last_conversion_time = HAL_GetTick();
    return MS5611_OK;
}


void MS5611_Compute(ms5611_dev* dev, float *temperature, float *pressure)
{

	int32_t dT = (int32_t)dev->raw_temperature - ((int32_t)dev->C[5] << 8);

	int32_t TEMP = 2000 + ((int64_t)dT * dev->C[6]) / 8388608;

    int64_t OFF = ((int64_t)dev->C[2] << 16) + ((int64_t)dev->C[4] * dT) / 128;
    int64_t SENS = ((int64_t)dev->C[1] << 15) + ((int64_t)dev->C[3] * dT) / 256;

    int32_t P = ((((int64_t)dev->raw_pressure * SENS) / 2097152 - OFF) / 32768);

    *temperature = TEMP / 100.0;
    *pressure = P / 100.0;
}


error_ms5611 MS5611_ReadADC(SPI_HandleTypeDef* hspi, uint32_t* data)
{
    uint8_t cmd = MS5611_CMD_ADC_READ;
    uint8_t rx[3];

    MS5611_CS_LOW();

    if(HAL_SPI_Transmit(hspi, &cmd, 1, HAL_MAX_DELAY) != HAL_OK)
    	goto error_spi;

    if(HAL_SPI_Receive(hspi, rx, 3, HAL_MAX_DELAY) != HAL_OK)
    	goto error_spi;

    MS5611_CS_HIGH();

    *data = ((uint32_t)rx[0] << 16) | ((uint32_t)rx[1] << 8) | rx[2];
    return MS5611_OK;

error_spi:
	MS5611_CS_HIGH();
	return MS5611_ERR_SPI;
}

error_ms5611 MS5611_Reset(SPI_HandleTypeDef* hspi)
{
	uint8_t cmd = MS5611_CMD_RESET;

    MS5611_CS_LOW();

    if(HAL_SPI_Transmit(hspi, &cmd, 1, HAL_MAX_DELAY) != HAL_OK)
    	goto error_spi;

    MS5611_CS_HIGH();

    HAL_Delay(3);
    return MS5611_OK;

error_spi:
	MS5611_CS_HIGH();
	return MS5611_ERR_SPI;
}

error_ms5611 MS5611_ReadPROM(SPI_HandleTypeDef* hspi, uint8_t index, uint16_t* c)
{
	uint8_t cmd = MS5611_CMD_PROM_READ + (index * 2);
	uint8_t rx[2];

    MS5611_CS_LOW();

    if(HAL_SPI_Transmit(hspi, &cmd, 1, HAL_MAX_DELAY) != HAL_OK)
    	goto error_spi;

    if(HAL_SPI_Receive(hspi, rx, 2, HAL_MAX_DELAY) != HAL_OK)
    	goto error_spi;

    MS5611_CS_HIGH();

    *c = (rx[0] << 8) | rx[1];
    return MS5611_OK;

error_spi:
	MS5611_CS_HIGH();
	return MS5611_ERR_SPI;
}

uint8_t MS5611_CRC4(uint16_t prom[])
{
	uint16_t n_rem = 0;
    uint16_t crc_read = prom[0];

    prom[0] &= 0xFFF0;
    prom[7] = 0;

    for (int cnt = 0; cnt < 16; cnt++)
    {
        if (cnt % 2 == 1)
            n_rem ^= (uint16_t)(prom[cnt >> 1] & 0x00FF);
        else
            n_rem ^= (uint16_t)(prom[cnt >> 1] >> 8);

        for (int n_bit = 8; n_bit > 0; n_bit--)
        {
            if (n_rem & 0x8000)
                n_rem = (n_rem << 1) ^ 0x3000;
            else
                n_rem <<= 1;
        }
    }

    n_rem = (n_rem >> 12) & 0xF;

    prom[0] = crc_read;

    return n_rem;
}

void MS5611_CS_LOW()
{
    HAL_GPIO_WritePin(MS5611_CS_PORT, MS5611_CS_PIN, GPIO_PIN_RESET);
}

void MS5611_CS_HIGH()
{
    HAL_GPIO_WritePin(MS5611_CS_PORT, MS5611_CS_PIN, GPIO_PIN_SET);
}

static error_ms5611 MS5611_SendCmd(ms5611_dev* dev, uint8_t cmd)
{
    MS5611_CS_LOW();

    if(HAL_SPI_Transmit(dev->spi, &cmd, 1, HAL_MAX_DELAY) != HAL_OK)
    {
        MS5611_CS_HIGH();
        return MS5611_ERR_SPI;
    }

    MS5611_CS_HIGH();
    return MS5611_OK;
}

static uint8_t MS5611_GetDelay(osr_option osr)
{
    switch (osr) {
        case OSR256:  return 1;
        case OSR512:  return 2;
        case OSR1024: return 3;
        case OSR2048: return 5;
        case OSR4096: return 10;
        default:      return 10;
    }
}

