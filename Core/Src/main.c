/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdbool.h"
#include "stdint.h"
#include "referee_protocol.h"
#include "referee_frame_process.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NUM_KEYS 3
#define DELAY_MS 25

// #define LED38_Pin GPIO_PIN_15
// #define LED38_GPIO_Port GPIOB
#define KEY_50_Pin GPIO_PIN_14
#define KEY1_GPIO_Port GPIOB
#define KEY_100_Pin GPIO_PIN_15
#define KEY2_GPIO_Port GPIOB
#define KEY_200_Pin GPIO_PIN_8
#define KEY3_GPIO_Port GPIOA
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
bool key_pressed[NUM_KEYS] = {false};
bool key_flag[NUM_KEYS] = {true}; 

custom_client_data_t custom_client_data = {0};
uint8_t client_data[8] = {0};

typedef struct {
    uint16_t x_pos;
    uint16_t y_pos;
} Position;

const Position key_positions[NUM_KEYS] = {
    {1160, 560},  //50�?
    {1210, 560},  //100�?
    {1210, 560}   
};

const Position common_positions[3] = {
    {960, 670},   
    {860, 560}  //确认
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void send_mouse_sequence(uint16_t x, uint16_t y);
void send_common_sequence(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  uint8_t Led_state = GPIO_PIN_RESET;
  uint8_t Lastkey1_state = GPIO_PIN_SET;
  uint8_t Lastkey2_state = GPIO_PIN_SET;
  uint8_t Lastkey3_state = GPIO_PIN_SET;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    for (int i = 0; i < NUM_KEYS; i++)
    {
      GPIO_PinState pin_state;
      
      switch (i) 
      {
        case 0: pin_state = HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY_50_Pin); break;
        case 1: pin_state = HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY_100_Pin); break;
        case 2: pin_state = HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY_200_Pin); break;
      }
      
      key_pressed[i] = (pin_state == GPIO_PIN_RESET);
      if (pin_state == GPIO_PIN_SET) 
      {
        key_flag[i] = false;
      }
      
      if (key_pressed[i] && !key_flag[i]) 
      {
        custom_client_data = (custom_client_data_t)
        {
          .key_value = 79,
          .x_position = 0,
          .y_position = 0,
          .mouse_left = 0,
          .mouse_right = 0
        };
        memcpy(client_data, &custom_client_data, 8);
        HAL_UART_Transmit(&huart1, referee_pack_data(0x0306, client_data, 8), getRefSentDataLen(), 30);
        HAL_Delay(DELAY_MS);

        send_mouse_sequence(key_positions[i].x_pos, key_positions[i].y_pos);
        if(i == 2)
        send_mouse_sequence(key_positions[i].x_pos, key_positions[i].y_pos);

        send_common_sequence();
            
        custom_client_data = (custom_client_data_t){
          .key_value = 79,
          .x_position = 0,
          .y_position = 0,
          .mouse_left = 0,
          .mouse_right = 0
        };
        memcpy(client_data, &custom_client_data, 8);
        HAL_UART_Transmit(&huart1, referee_pack_data(0x0306, client_data, 8), getRefSentDataLen(), 30);
        HAL_Delay(DELAY_MS);
    
        send_mouse_sequence(0, 0);

        key_flag[i] = true;
        key_pressed[i] = false;
        }
    }
    HAL_Delay(10);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void send_mouse_sequence(uint16_t x, uint16_t y)
{
    custom_client_data = (custom_client_data_t){
        .key_value = 0,			//网上任意键值
        .x_position = x,
        .y_position = y,
        .mouse_left = 0,
        .mouse_right = 0
    };
    memcpy(client_data, &custom_client_data, 8);
    HAL_UART_Transmit(&huart1, referee_pack_data(0x0306, client_data, 8), getRefSentDataLen(), 30);
    HAL_Delay(DELAY_MS);
    
    custom_client_data.mouse_left = 1;
    memcpy(client_data, &custom_client_data, 8);
    HAL_UART_Transmit(&huart1, referee_pack_data(0x0306, client_data, 8), getRefSentDataLen(), 30);
    HAL_Delay(DELAY_MS);
    
    custom_client_data.mouse_left = 0;
    memcpy(client_data, &custom_client_data, 8);
    HAL_UART_Transmit(&huart1, referee_pack_data(0x0306, client_data, 8), getRefSentDataLen(), 30);
    HAL_Delay(DELAY_MS);
}
void send_common_sequence(void)
{
    send_mouse_sequence(common_positions[0].x_pos, common_positions[0].y_pos);
    
    send_mouse_sequence(common_positions[1].x_pos, common_positions[1].y_pos);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
