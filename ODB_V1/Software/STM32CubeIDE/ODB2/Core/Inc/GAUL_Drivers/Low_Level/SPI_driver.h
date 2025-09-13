#ifndef INC_GAUL_DRIVERS_LOW_LEVEL_SPI_DRIVER_H_
#define INC_GAUL_DRIVERS_LOW_LEVEL_SPI_DRIVER_H_

#include "stm32f4xx.h"


#define SPI_BAUDRATE_0    (0b000 << 3) // 48 MHz / 8MHz
#define SPI_BAUDRATE_1    (0b001 << 3) // 24 MHz / 4MHz
#define SPI_BAUDRATE_2    (0b010 << 3) // 12 MHz / 2MHz
#define SPI_BAUDRATE_3   (0b011 << 3) // 6 MHz / 1MHz
#define SPI_BAUDRATE_4   (0b100 << 3) // 3 MHz / 500kHz
#define SPI_BAUDRATE_5   (0b101 << 3) // 1.5 MHz 250kHz
#define SPI_BAUDRATE_6  (0b110 << 3) // 750 kHz / 125kHz
#define SPI_BAUDRATE_7  (0b111 << 3) // 375 kHz / 62.5kHz

#define TIMEOUT 1000  // Max iterations (Not in ms !!!)


void SPI_InitPeriph(SPI_TypeDef *SPIx, unsigned short baudrate);

int8_t SPI_MOSI(SPI_TypeDef *SPIx, uint8_t *data, size_t size);
int8_t SPI_MISO(SPI_TypeDef *SPIx, uint8_t *data, size_t size);
int8_t SPI_TransmitReceive(SPI_TypeDef *SPIx, uint8_t *pTxData, uint8_t *pRxData, size_t size, uint32_t timeout);

#endif /* INC_GAUL_DRIVERS_LOW_LEVEL_SPI_DRIVER_H_ */
