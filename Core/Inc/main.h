/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

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
#define EC11_D_Pin GPIO_PIN_0
#define EC11_D_GPIO_Port GPIOA
#define EC11_A_Pin GPIO_PIN_1
#define EC11_A_GPIO_Port GPIOA
#define EC11_B_Pin GPIO_PIN_2
#define EC11_B_GPIO_Port GPIOA
#define K_BACK_Pin GPIO_PIN_3
#define K_BACK_GPIO_Port GPIOA
#define K_9_Pin GPIO_PIN_4
#define K_9_GPIO_Port GPIOA
#define K_8_Pin GPIO_PIN_5
#define K_8_GPIO_Port GPIOA
#define K_7_Pin GPIO_PIN_6
#define K_7_GPIO_Port GPIOA
#define K_4_Pin GPIO_PIN_7
#define K_4_GPIO_Port GPIOA
#define K_5_Pin GPIO_PIN_0
#define K_5_GPIO_Port GPIOB
#define K_6_Pin GPIO_PIN_1
#define K_6_GPIO_Port GPIOB
#define K_MINUS_Pin GPIO_PIN_2
#define K_MINUS_GPIO_Port GPIOB
#define K_1_Pin GPIO_PIN_10
#define K_1_GPIO_Port GPIOB
#define K_2_Pin GPIO_PIN_11
#define K_2_GPIO_Port GPIOB
#define K_3_Pin GPIO_PIN_12
#define K_3_GPIO_Port GPIOB
#define K_PLUS_Pin GPIO_PIN_13
#define K_PLUS_GPIO_Port GPIOB
#define K_0_Pin GPIO_PIN_14
#define K_0_GPIO_Port GPIOB
#define K_DOT_Pin GPIO_PIN_15
#define K_DOT_GPIO_Port GPIOB
#define K_ENTER_Pin GPIO_PIN_8
#define K_ENTER_GPIO_Port GPIOA
#define K_NUM_Pin GPIO_PIN_7
#define K_NUM_GPIO_Port GPIOB
#define K_DIVISION_Pin GPIO_PIN_8
#define K_DIVISION_GPIO_Port GPIOB
#define K_MULTIPLE_Pin GPIO_PIN_9
#define K_MULTIPLE_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
