#ifndef INC_GAUL_DRIVERS_LOW_LEVEL_GPIO_DRIVER_H_
#define INC_GAUL_DRIVERS_LOW_LEVEL_GPIO_DRIVER_H_

#include "stm32f4xx.h"
#include "stm32f4xx_ll_gpio.h"
#include "stm32f4xx_ll_exti.h"

// Directions
#define IN 	0       // Input
#define OUT 1      // Output mode
#define ALT 2	   // Alternate function mode
#define AN 	3	   // Analog mode

// Output type
#define O_PP 0  // Push-pull output
#define O_OD 1  // Open-drain output

// Options for input/output
#define IO_NPP 0  // No pull-up/down
#define IO_PU  1  // Pull-up
#define IO_PD  2  // Pull-down

// Options for alternate fonctions
#define AF0  0b0000
#define AF1  0b0001
#define AF2  0b0010
#define AF3  0b0011
#define AF4  0b0100
#define AF5  0b0101
#define AF6  0b0110
#define AF7  0b0111
#define AF8  0b1000
#define AF9  0b1001
#define AF10 0b1010
#define AF11 0b1011
#define AF12 0b1100
#define AF13 0b1101
#define AF14 0b1110
#define AF15 0b1111

// GPIO Speed
#define GPIO_SPEED_0 0x0
#define GPIO_SPEED_1 0x1
#define GPIO_SPEED_2 0x2
#define GPIO_SPEED_3 0x3

// State definition
#define LOW  0
#define HIGH 1

void GPIO_InitPeriph(GPIO_TypeDef *GPIOx, unsigned short pin, unsigned short dir, unsigned short type, unsigned short opt, unsigned short speed);
int8_t GPIO_ReadPin(GPIO_TypeDef *GPIOx, unsigned short pin);
void GPIO_WritePin(GPIO_TypeDef *GPIOx, unsigned short pin, unsigned short state);
void GPIO_Toggle(GPIO_TypeDef *GPIOx, unsigned short pin);

#endif /* INC_GAUL_DRIVERS_LOW_LEVEL_GPIO_DRIVER_H_ */
