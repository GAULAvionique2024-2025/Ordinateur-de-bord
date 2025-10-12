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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MEM_QSPI_IO2_Pin GPIO_PIN_2
#define MEM_QSPI_IO2_GPIO_Port GPIOE
#define IDEFIX_I2C2_SDA_Pin GPIO_PIN_0
#define IDEFIX_I2C2_SDA_GPIO_Port GPIOF
#define IDEFIX_I2C2_SCL_Pin GPIO_PIN_1
#define IDEFIX_I2C2_SCL_GPIO_Port GPIOF
#define MEM_QSPI_IO3_Pin GPIO_PIN_6
#define MEM_QSPI_IO3_GPIO_Port GPIOF
#define MEM_QSPI_IO0_Pin GPIO_PIN_8
#define MEM_QSPI_IO0_GPIO_Port GPIOF
#define MEM_QSPI_IO1_Pin GPIO_PIN_9
#define MEM_QSPI_IO1_GPIO_Port GPIOF
#define BT_KEY_Pin GPIO_PIN_1
#define BT_KEY_GPIO_Port GPIOA
#define BT_USART2_TX_Pin GPIO_PIN_2
#define BT_USART2_TX_GPIO_Port GPIOA
#define BT_USART2_RX_Pin GPIO_PIN_3
#define BT_USART2_RX_GPIO_Port GPIOA
#define BARO_SPI1_CS_Pin GPIO_PIN_4
#define BARO_SPI1_CS_GPIO_Port GPIOA
#define BARO_SPI1_SCLK_Pin GPIO_PIN_5
#define BARO_SPI1_SCLK_GPIO_Port GPIOA
#define BARO_SPI1_MISO_Pin GPIO_PIN_6
#define BARO_SPI1_MISO_GPIO_Port GPIOA
#define BARO_SPI1_MOSI_Pin GPIO_PIN_7
#define BARO_SPI1_MOSI_GPIO_Port GPIOA
#define MEM_QSPI_CLK_Pin GPIO_PIN_1
#define MEM_QSPI_CLK_GPIO_Port GPIOB
#define SD_SPI5_CS_Pin GPIO_PIN_11
#define SD_SPI5_CS_GPIO_Port GPIOE
#define SD_SPI5_CLK_Pin GPIO_PIN_12
#define SD_SPI5_CLK_GPIO_Port GPIOE
#define SD_SPI5_MISO_Pin GPIO_PIN_13
#define SD_SPI5_MISO_GPIO_Port GPIOE
#define SD_SPI5_MOSI_Pin GPIO_PIN_14
#define SD_SPI5_MOSI_GPIO_Port GPIOE
#define RFD_USART3_TX_Pin GPIO_PIN_8
#define RFD_USART3_TX_GPIO_Port GPIOD
#define RFD_USART3_RX_Pin GPIO_PIN_9
#define RFD_USART3_RX_GPIO_Port GPIOD
#define GPS_ID_Pin GPIO_PIN_4
#define GPS_ID_GPIO_Port GPIOG
#define MEM_QSPI_CS_Pin GPIO_PIN_6
#define MEM_QSPI_CS_GPIO_Port GPIOG
#define GPS_USART6_TX_Pin GPIO_PIN_6
#define GPS_USART6_TX_GPIO_Port GPIOC
#define GPS_USART6_RX_Pin GPIO_PIN_7
#define GPS_USART6_RX_GPIO_Port GPIOC
#define IMU_INT_Pin GPIO_PIN_9
#define IMU_INT_GPIO_Port GPIOC
#define IMU_nRESET_Pin GPIO_PIN_8
#define IMU_nRESET_GPIO_Port GPIOA
#define IMU_USART1_TX_Pin GPIO_PIN_9
#define IMU_USART1_TX_GPIO_Port GPIOA
#define IMU_USART1_RX_Pin GPIO_PIN_10
#define IMU_USART1_RX_GPIO_Port GPIOA
#define SYS_SWDIO_Pin GPIO_PIN_13
#define SYS_SWDIO_GPIO_Port GPIOA
#define SYS_SWCLK_Pin GPIO_PIN_14
#define SYS_SWCLK_GPIO_Port GPIOA
#define SYS_SWO_Pin GPIO_PIN_3
#define SYS_SWO_GPIO_Port GPIOB
#define ACC_I2C1_SCL_Pin GPIO_PIN_6
#define ACC_I2C1_SCL_GPIO_Port GPIOB
#define ACC_I2C1_SDA_Pin GPIO_PIN_7
#define ACC_I2C1_SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
