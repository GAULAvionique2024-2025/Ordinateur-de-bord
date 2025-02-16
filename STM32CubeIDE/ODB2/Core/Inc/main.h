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
#define BARO_CS_Pin GPIO_PIN_4
#define BARO_CS_GPIO_Port GPIOE
#define MEM_MISO_Pin GPIO_PIN_5
#define MEM_MISO_GPIO_Port GPIOE
#define Enable7v4_Pin GPIO_PIN_14
#define Enable7v4_GPIO_Port GPIOC
#define nPYROS_Test_Pin GPIO_PIN_2
#define nPYROS_Test_GPIO_Port GPIOF
#define PYROS_Bluetooth_Pin GPIO_PIN_3
#define PYROS_Bluetooth_GPIO_Port GPIOF
#define PYROS_MainODB_Pin GPIO_PIN_9
#define PYROS_MainODB_GPIO_Port GPIOF
#define PYROS_DrogueODB_Pin GPIO_PIN_10
#define PYROS_DrogueODB_GPIO_Port GPIOF
#define RFM_SDN_Pin GPIO_PIN_0
#define RFM_SDN_GPIO_Port GPIOC
#define RFM_NIRQ_Pin GPIO_PIN_1
#define RFM_NIRQ_GPIO_Port GPIOC
#define AN_7v4_Pin GPIO_PIN_3
#define AN_7v4_GPIO_Port GPIOC
#define RFM_CS_Pin GPIO_PIN_2
#define RFM_CS_GPIO_Port GPIOA
#define ICM_INT_Pin GPIO_PIN_3
#define ICM_INT_GPIO_Port GPIOA
#define ICM_CS_Pin GPIO_PIN_4
#define ICM_CS_GPIO_Port GPIOA
#define ICM_RFM_SCK_Pin GPIO_PIN_5
#define ICM_RFM_SCK_GPIO_Port GPIOA
#define ICM_RFM_MISO_Pin GPIO_PIN_6
#define ICM_RFM_MISO_GPIO_Port GPIOA
#define ICM_RFM_MOSI_Pin GPIO_PIN_7
#define ICM_RFM_MOSI_GPIO_Port GPIOA
#define PYROS_ADC_Pin GPIO_PIN_4
#define PYROS_ADC_GPIO_Port GPIOC
#define AN_3v7_Pin GPIO_PIN_5
#define AN_3v7_GPIO_Port GPIOC
#define MEM_SCLK_Pin GPIO_PIN_0
#define MEM_SCLK_GPIO_Port GPIOB
#define MEM_CS_Pin GPIO_PIN_1
#define MEM_CS_GPIO_Port GPIOB
#define BUZZER_PWM_Pin GPIO_PIN_9
#define BUZZER_PWM_GPIO_Port GPIOE
#define BARO_SCK_Pin GPIO_PIN_12
#define BARO_SCK_GPIO_Port GPIOE
#define BARO_MISO_Pin GPIO_PIN_13
#define BARO_MISO_GPIO_Port GPIOE
#define BARO_MOSI_Pin GPIO_PIN_14
#define BARO_MOSI_GPIO_Port GPIOE
#define RFD_TX_Pin GPIO_PIN_6
#define RFD_TX_GPIO_Port GPIOC
#define RFD_RX_Pin GPIO_PIN_7
#define RFD_RX_GPIO_Port GPIOC
#define CriticalLEDEnable_Pin GPIO_PIN_8
#define CriticalLEDEnable_GPIO_Port GPIOA
#define GPS_TX_Pin GPIO_PIN_9
#define GPS_TX_GPIO_Port GPIOA
#define GPS_RX_Pin GPIO_PIN_10
#define GPS_RX_GPIO_Port GPIOA
#define USBData__Pin GPIO_PIN_11
#define USBData__GPIO_Port GPIOA
#define USBData_A12_Pin GPIO_PIN_12
#define USBData_A12_GPIO_Port GPIOA
#define SYS_SWDIO_Pin GPIO_PIN_13
#define SYS_SWDIO_GPIO_Port GPIOA
#define SYS_SWCLK_Pin GPIO_PIN_14
#define SYS_SWCLK_GPIO_Port GPIOA
#define BT_TX_Pin GPIO_PIN_5
#define BT_TX_GPIO_Port GPIOD
#define BT_RX_Pin GPIO_PIN_6
#define BT_RX_GPIO_Port GPIOD
#define RFD_RTS_Pin GPIO_PIN_12
#define RFD_RTS_GPIO_Port GPIOG
#define SYS_SWO_Pin GPIO_PIN_3
#define SYS_SWO_GPIO_Port GPIOB
#define MEM_MOSI_Pin GPIO_PIN_8
#define MEM_MOSI_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
