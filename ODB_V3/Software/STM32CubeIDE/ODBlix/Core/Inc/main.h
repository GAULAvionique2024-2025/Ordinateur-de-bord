/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define IDEFIX_I2C2_SDA_Pin GPIO_PIN_0
#define IDEFIX_I2C2_SDA_GPIO_Port GPIOF
#define IDEFIX_I2C2_SCL_Pin GPIO_PIN_1
#define IDEFIX_I2C2_SCL_GPIO_Port GPIOF
#define CriticalLED_G_Pin GPIO_PIN_2
#define CriticalLED_G_GPIO_Port GPIOF
#define CriticalLED_R_Pin GPIO_PIN_3
#define CriticalLED_R_GPIO_Port GPIOF
#define MEM_QSPI_IO3_Pin GPIO_PIN_6
#define MEM_QSPI_IO3_GPIO_Port GPIOF
#define MEM_QSPI_IO2_Pin GPIO_PIN_7
#define MEM_QSPI_IO2_GPIO_Port GPIOF
#define MEM_QSPI_IO0_Pin GPIO_PIN_8
#define MEM_QSPI_IO0_GPIO_Port GPIOF
#define MEM_QSPI_IO1_Pin GPIO_PIN_9
#define MEM_QSPI_IO1_GPIO_Port GPIOF
#define TEMP_An_Pin GPIO_PIN_0
#define TEMP_An_GPIO_Port GPIOC
#define Vin_An_Pin GPIO_PIN_1
#define Vin_An_GPIO_Port GPIOC
#define V5_An_Pin GPIO_PIN_2
#define V5_An_GPIO_Port GPIOC
#define V3_An_Pin GPIO_PIN_3
#define V3_An_GPIO_Port GPIOC
#define PA_An_Pin GPIO_PIN_0
#define PA_An_GPIO_Port GPIOA
#define Sense_4_Pin GPIO_PIN_1
#define Sense_4_GPIO_Port GPIOA
#define BT_USART2_TX_Pin GPIO_PIN_2
#define BT_USART2_TX_GPIO_Port GPIOA
#define BT_USART2_RX_Pin GPIO_PIN_3
#define BT_USART2_RX_GPIO_Port GPIOA
#define BARO_SPI1_CS_Pin GPIO_PIN_4
#define BARO_SPI1_CS_GPIO_Port GPIOA
#define BARO_SPI1_CLK_Pin GPIO_PIN_5
#define BARO_SPI1_CLK_GPIO_Port GPIOA
#define BARO_SPI1_MISO_Pin GPIO_PIN_6
#define BARO_SPI1_MISO_GPIO_Port GPIOA
#define BARO_SPI1_MOSI_Pin GPIO_PIN_7
#define BARO_SPI1_MOSI_GPIO_Port GPIOA
#define Sense_3_Pin GPIO_PIN_4
#define Sense_3_GPIO_Port GPIOC
#define Sense_2_Pin GPIO_PIN_5
#define Sense_2_GPIO_Port GPIOC
#define Sense_1_Pin GPIO_PIN_0
#define Sense_1_GPIO_Port GPIOB
#define MEM_QSPI_CLK_Pin GPIO_PIN_1
#define MEM_QSPI_CLK_GPIO_Port GPIOB
#define Pyros_CTN_Pin GPIO_PIN_11
#define Pyros_CTN_GPIO_Port GPIOF
#define Pyros_Arm_Pin GPIO_PIN_12
#define Pyros_Arm_GPIO_Port GPIOF
#define Fire_4_Pin GPIO_PIN_13
#define Fire_4_GPIO_Port GPIOF
#define Fire_3_Pin GPIO_PIN_14
#define Fire_3_GPIO_Port GPIOF
#define Fire_2_Pin GPIO_PIN_15
#define Fire_2_GPIO_Port GPIOF
#define Fire_1_Pin GPIO_PIN_0
#define Fire_1_GPIO_Port GPIOG
#define Power_Good_Pin GPIO_PIN_9
#define Power_Good_GPIO_Port GPIOE
#define SD_SPI5_CS_Pin GPIO_PIN_11
#define SD_SPI5_CS_GPIO_Port GPIOE
#define SD_SPI5_CLK_Pin GPIO_PIN_12
#define SD_SPI5_CLK_GPIO_Port GPIOE
#define SD_SPI5_MISO_Pin GPIO_PIN_13
#define SD_SPI5_MISO_GPIO_Port GPIOE
#define SD_SPI5_MOSI_Pin GPIO_PIN_14
#define SD_SPI5_MOSI_GPIO_Port GPIOE
#define EXTEND_IO1_Pin GPIO_PIN_10
#define EXTEND_IO1_GPIO_Port GPIOB
#define EXTEND_IO2_Pin GPIO_PIN_11
#define EXTEND_IO2_GPIO_Port GPIOB
#define EXTEND_IO3_Pin GPIO_PIN_12
#define EXTEND_IO3_GPIO_Port GPIOB
#define EXTEND_IO4_Pin GPIO_PIN_13
#define EXTEND_IO4_GPIO_Port GPIOB
#define BUZZER_PWM_Pin GPIO_PIN_12
#define BUZZER_PWM_GPIO_Port GPIOD
#define MEM_QSPI_CS_Pin GPIO_PIN_6
#define MEM_QSPI_CS_GPIO_Port GPIOG
#define GPS_USART6_TX_Pin GPIO_PIN_6
#define GPS_USART6_TX_GPIO_Port GPIOC
#define GPS_USART6_RX_Pin GPIO_PIN_7
#define GPS_USART6_RX_GPIO_Port GPIOC
#define USB_5V_Pin GPIO_PIN_8
#define USB_5V_GPIO_Port GPIOC
#define ACC_I2C3_SDA_Pin GPIO_PIN_9
#define ACC_I2C3_SDA_GPIO_Port GPIOC
#define ACC_I2C3_SCL_Pin GPIO_PIN_8
#define ACC_I2C3_SCL_GPIO_Port GPIOA
#define RFD_USART1_TX_Pin GPIO_PIN_9
#define RFD_USART1_TX_GPIO_Port GPIOA
#define RFD_USART1_RX_Pin GPIO_PIN_10
#define RFD_USART1_RX_GPIO_Port GPIOA
#define SYS_SWDIO_Pin GPIO_PIN_13
#define SYS_SWDIO_GPIO_Port GPIOA
#define SYS_SWCLK_Pin GPIO_PIN_14
#define SYS_SWCLK_GPIO_Port GPIOA
#define SYS_SWO_Pin GPIO_PIN_3
#define SYS_SWO_GPIO_Port GPIOB
#define IMU_INT_Pin GPIO_PIN_4
#define IMU_INT_GPIO_Port GPIOB
#define IMU_INT_EXTI_IRQn EXTI4_IRQn
#define IMU_nReset_Pin GPIO_PIN_5
#define IMU_nReset_GPIO_Port GPIOB
#define IMU_I2C1_SCL_Pin GPIO_PIN_6
#define IMU_I2C1_SCL_GPIO_Port GPIOB
#define IMU_I2C1_SDA_Pin GPIO_PIN_7
#define IMU_I2C1_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
