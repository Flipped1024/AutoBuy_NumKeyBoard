/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin : EC11_D_Pin */
  GPIO_InitStruct.Pin = EC11_D_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(EC11_D_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : EC11_A_Pin */
  GPIO_InitStruct.Pin = EC11_A_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(EC11_A_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : EC11_B_Pin K_BACK_Pin K_9_Pin K_8_Pin
                           K_7_Pin K_4_Pin K_ENTER_Pin */
  GPIO_InitStruct.Pin = EC11_B_Pin|K_BACK_Pin|K_9_Pin|K_8_Pin
                          |K_7_Pin|K_4_Pin|K_ENTER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : K_5_Pin K_6_Pin K_MINUS_Pin K_1_Pin
                           K_2_Pin K_3_Pin K_PLUS_Pin K_NUM_Pin
                           K_DIVISION_Pin K_MULTIPLE_Pin */
  GPIO_InitStruct.Pin = K_5_Pin|K_6_Pin|K_MINUS_Pin|K_1_Pin
                          |K_2_Pin|K_3_Pin|K_PLUS_Pin|K_NUM_Pin
                          |K_DIVISION_Pin|K_MULTIPLE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : K_0_Pin K_DOT_Pin */
  GPIO_InitStruct.Pin = K_0_Pin|K_DOT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
