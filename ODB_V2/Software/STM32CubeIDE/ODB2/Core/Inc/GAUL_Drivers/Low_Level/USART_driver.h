#ifndef INC_GAUL_DRIVERS_LOW_LEVEL_USART_DRIVER_H_
#define INC_GAUL_DRIVERS_LOW_LEVEL_USART_DRIVER_H_

#include "stm32f4xx.h"


#define TIMEOUT 1000  // Max iterations (Not in ms !!!)

 /*
 * PCLK1 = 48 MHz  → USART2/3/4/5
 * PCLK2 = 96 MHz  → USART1/6
 * ---------------------------------------
 * | Baudrate |  APB1 (48MHz) | APB2 (96MHz) |
 * |----------|---------------|--------------|
 * | 9600     |   0x1388      |   0x2710     |
 * | 19200    |   0x09C4      |   0x1388     |
 * | 38400    |   0x04E2      |   0x09C4     |
 * | 57600    |   0x0341      |   0x0683     |
 * | 115200   |   0x01A1      |   0x0341     |
 * ---------------------------------------
 */
// ---------- APB1 = 48 MHz (USART2, USART3) ----------
#define BRR_APB1_9600     0x1388
#define BRR_APB1_19200    0x09C4
#define BRR_APB1_38400    0x04E2
#define BRR_APB1_57600    0x0341
#define BRR_APB1_115200   0x01A1

// ---------- APB2 = 96 MHz (USART1, USART6) ----------
#define BRR_APB2_9600     0x2710
#define BRR_APB2_19200    0x1388
#define BRR_APB2_38400    0x09C4
#define BRR_APB2_57600    0x0683
#define BRR_APB2_115200   0x0341

void USART_InitPeriph(USART_TypeDef *USARTx, unsigned long baudrate);

int8_t USART_TX(USART_TypeDef *USARTx, uint8_t *data, size_t size);
int8_t USART_RX(USART_TypeDef *USARTx, uint8_t *data, size_t size);


#endif /* INC_GAUL_DRIVERS_LOW_LEVEL_USART_DRIVER_H_*/
