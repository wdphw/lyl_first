/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dht11.h"
#include "freertos_task.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

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
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_USART3_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */
  // float temp = 0;
  // uint8_t humi = 0;
  // uint8_t ligh = 0;
  // uint8_t air_flag = 0;
  // uint8_t w_flag = 0;
  // uint8_t led_flag = 0;
  // uint8_t beep_flag = 0;
  // char buffer[150];

  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  Servo_SetAngle(0);

  FreeRtos_Start();
  // HAL_UART_Receive(&huart1, rbuf, 1000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

    // HAL_UART_Receive(&huart3, rbuf, 2, 1000);

    // if (rbuf[0] == 'A')
    // {
    //   sscanf((char *)rbuf, "A%d", &air_flag);
    //   if (air_flag)
    //   {
    //     Air_on();
    //   }
    //   else
    //   {
    //     Air_off();
    //   }
    // }
    // else if (rbuf[0] == 'W')
    // {
    //   sscanf((char *)rbuf, "W%d", &w_flag);
    //   if (w_flag)
    //   {
    //     Servo_SetAngle(90);
    //   }
    //   else
    //   {
    //     Servo_SetAngle(0);
    //   }
    // }
    // else if (rbuf[0] == 'L')
    // {
    //   sscanf((char *)rbuf, "L%d", &led_flag);
    //   if (led_flag)
    //   {
    //     HAL_GPIO_WritePin(GPIOA, CESUO_Pin, GPIO_PIN_RESET);
    //     HAL_GPIO_WritePin(GPIOB, BEDROOM_Pin, GPIO_PIN_RESET);
    //     HAL_GPIO_WritePin(GPIOB, CHUFANG_Pin, GPIO_PIN_RESET);
    //     HAL_GPIO_WritePin(GPIOB, KETING_Pin, GPIO_PIN_RESET);
    //     led_flag = 0;
    //   }
    //   else
    //   {
    //     HAL_GPIO_WritePin(GPIOA, CESUO_Pin, GPIO_PIN_SET);
    //     HAL_GPIO_WritePin(GPIOB, BEDROOM_Pin, GPIO_PIN_SET);
    //     HAL_GPIO_WritePin(GPIOB, CHUFANG_Pin, GPIO_PIN_SET);
    //     HAL_GPIO_WritePin(GPIOB, KETING_Pin, GPIO_PIN_SET);
    //   }
    // }
    // else if (rbuf[0] == 'B')
    // {
    //   sscanf((char *)rbuf, "B%d", &beep_flag);
    //   if (beep_flag)
    //   {
    //     HAL_GPIO_WritePin(GPIOB, WARN_Pin, GPIO_PIN_RESET);
    //   }
    //   else
    //   {
    //     HAL_GPIO_WritePin(GPIOB, WARN_Pin, GPIO_PIN_SET);
    //   }
    // }
    // if (data[2] < 500)
    // {
    //   HAL_GPIO_WritePin(GPIOA, CESUO_Pin, GPIO_PIN_RESET);
    //   HAL_GPIO_WritePin(GPIOB, BEDROOM_Pin, GPIO_PIN_RESET);
    //   HAL_GPIO_WritePin(GPIOB, CHUFANG_Pin, GPIO_PIN_RESET);
    //   HAL_GPIO_WritePin(GPIOB, KETING_Pin, GPIO_PIN_RESET);
    // }
    // else if (data[1] < 500)
    // {
    //   HAL_GPIO_WritePin(GPIOB, WARN_Pin, GPIO_PIN_RESET);
    // }

    // ligh = 200 - data[0] * 200 / 4095;
    // Dht11_GetData(&temp, &humi);
    // sprintf(zbuf, "T%.1fH%dL%d\n", temp, humi, ligh);
    // sprintf(buffer, "{\"temp\":{\"value\":%.1f},\"humi\":{\"value\":%d},\"light\":{\"value\":%d},\"fire\":{\"value\":%d},\"air\":{\"value\":%s},\"led\":{\"value\":%s},\"window\":{\"value\":%s}}\n", temp, humi, ligh, data[1], air_flag ? "true" : "false", led_flag ? "true" : "false", w_flag ? "true" : "false");
    // HAL_UART_Transmit(&huart1, (uint8_t *)zbuf, strlen(zbuf), 1000);
    // HAL_UART_Transmit(&huart3, (uint8_t *)buffer, strlen(buffer), 1000);

    // HAL_Delay(1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void m_delay_us(uint32_t udelay)
{
  uint32_t startval, tickn, delays, wait;

  startval = SysTick->VAL;
  tickn = HAL_GetTick();
  // sysc = 72000;  //SystemCoreClock / (1000U / uwTickFreq);
  delays = udelay * 72; // sysc / 1000 * udelay;
  if (delays > startval)
  {
    while (HAL_GetTick() == tickn)
    {
    }
    wait = 72000 + startval - delays;
    while (wait < SysTick->VAL)
    {
    }
  }
  else
  {
    wait = startval - delays;
    while (wait < SysTick->VAL && HAL_GetTick() == tickn)
    {
    }
  }
}

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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

#ifdef  USE_FULL_ASSERT
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
