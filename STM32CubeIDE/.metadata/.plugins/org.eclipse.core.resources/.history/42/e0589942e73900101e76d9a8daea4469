#ifndef INC_GAUL_DRIVERS_LOW_LEVEL_SPI_DRIVER_H_
#define INC_GAUL_DRIVERS_LOW_LEVEL_SPI_DRIVER_H_

#include "stm32f4xx.h"


#define SPI_BAUDRATE_48    (0b000 << 3) // 48 MHz
#define SPI_BAUDRATE_24    (0b001 << 3) // 24 MHz
#define SPI_BAUDRATE_12    (0b010 << 3) // 12 MHz
#define SPI_BAUDRATE_6   (0b011 << 3) // 6 MHz
#define SPI_BAUDRATE_3   (0b100 << 3) // 3 MHz
#define SPI_BAUDRATE_1_5   (0b101 << 3) // 1.5 MHz
#define SPI_BAUDRATE_0_750  (0b110 << 3) // 750 kHz
#define SPI_BAUDRATE_0_375  (0b111 << 3) // 375 kHz

#define TIMEOUT 1000  // Max iterations (Not in ms !!!)


void SPI_InitPeriph(SPI_TypeDef *SPIx, unsigned short baudrate);

int8_t SPI_MOSI(SPI_TypeDef *SPIx, uint8_t *data, size_t size);
int8_t SPI_MISO(SPI_TypeDef *SPIx, uint8_t *data, size_t size);
int8_t SPI_TransmitReceive(SPI_TypeDef *SPIx, uint8_t *pTxData, uint8_t *pRxData, size_t size, uint32_t timeout);

#endif /* INC_GAUL_DRIVERS_LOW_LEVEL_SPI_DRIVER_H_ */
