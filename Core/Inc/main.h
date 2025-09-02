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
#include "stm32l0xx_hal.h"

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
#define ENUM_SLAVE_ADDR 0x7F
#define PRIMARY_SLAVE_ADDR 0x44
#define EX_Pin GPIO_PIN_4
#define EX_GPIO_Port GPIOA
#define EX_EXTI_IRQn EXTI4_15_IRQn
#define N_CRESP_Pin GPIO_PIN_5
#define N_CRESP_GPIO_Port GPIOA
#define N_CTS_Pin GPIO_PIN_6
#define N_CTS_GPIO_Port GPIOA
#define N_RST_Pin GPIO_PIN_7
#define N_RST_GPIO_Port GPIOA
#define BE_Pin GPIO_PIN_0
#define BE_GPIO_Port GPIOB
#define INT_OUT_Pin GPIO_PIN_8
#define INT_OUT_GPIO_Port GPIOA
#define I2C1_SCK_Pin GPIO_PIN_9
#define I2C1_SCK_GPIO_Port GPIOA
#define STATUS_LED_Pin GPIO_PIN_11
#define STATUS_LED_GPIO_Port GPIOA
#define nTEST_Pin GPIO_PIN_12
#define nTEST_GPIO_Port GPIOA
#define STM_SWDIO_Pin GPIO_PIN_13
#define STM_SWDIO_GPIO_Port GPIOA
#define N_CMD_Pin GPIO_PIN_4
#define N_CMD_GPIO_Port GPIOB
#define TP1_Pin GPIO_PIN_5
#define TP1_GPIO_Port GPIOB
#define UART1_TX_Pin GPIO_PIN_6
#define UART1_TX_GPIO_Port GPIOB
#define UART1_RX_Pin GPIO_PIN_7
#define UART1_RX_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
