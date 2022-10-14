/*
 * initializeHardware.cpp
 *
 *  Created on: 9 авг. 2022 г.
 *      Author: Rumyantzev_D
 */

#include <initializeHardware.h>

CRC_HandleTypeDef hcrc;
extern TIM_HandleTypeDef htim10;
extern TIM_HandleTypeDef timeoutTim;

static void MX_GPIO_Init(void);
static void MX_CRC_Init(void);
static void MX_TIM10_Init(void);
static void timeoutTimInit(void);

void initHardware()

{
	HAL_Init();
	SystemClock_Config();

	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	__HAL_RCC_TIM1_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_TIM3_CLK_ENABLE();
	__HAL_RCC_TIM4_CLK_ENABLE();
	__HAL_RCC_TIM5_CLK_ENABLE();
	__HAL_RCC_TIM10_CLK_ENABLE();
	__HAL_RCC_TIM9_CLK_ENABLE();
	__HAL_RCC_TIM11_CLK_ENABLE();

	__HAL_RCC_USART1_CLK_ENABLE();

	MX_GPIO_Init();
	MX_CRC_Init();
	MX_TIM10_Init();
	timeoutTimInit();
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_CRC_Init(void)
{
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }

  __HAL_RCC_CRC_CLK_ENABLE();
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  HAL_GPIO_WritePin(GPIOC, RC_LMT__Pin|RC_LMT_C15_Pin|TRIG_OUT_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  HAL_GPIO_WritePin(GPIOB, LED1_Pin|LED2_Pin|DIR1_Pin|DIR2_Pin
                          |LED3_Pin|LED4_Pin|LED5_Pin|LED6_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = TRS_FINE_Pin|ON_OFF_Pin|AXIS0_Pin|AXIS1_Pin
                          |RUN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = RC_LMT__Pin|RC_LMT_C15_Pin|TRIG_OUT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin =  TRIG_IN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = AXIS2_Pin|ENC_IND_1_Pin|ENC_IND_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = BOARD_ID_Pin|Emergency_status_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LED1_Pin|LED2_Pin|LED3_Pin|LED4_Pin|LED5_Pin|LED6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

static void MX_TIM10_Init(void)
{
	htim10.Instance = TIM10;
	htim10.Init.Prescaler = 99;
	htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim10.Init.Period = 1000;
	htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
	{ Error_Handler(); }

	HAL_NVIC_SetPriority (TIM1_UP_TIM10_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ (TIM1_UP_TIM10_IRQn);
	HAL_TIM_Base_Start_IT (&htim10);
 }

static void timeoutTimInit(void)
{
	timeoutTim.Instance = TIM4;
	timeoutTim.Init.Prescaler = 64000-1;
	timeoutTim.Init.CounterMode = TIM_COUNTERMODE_UP;
	timeoutTim.Init.Period = 3000;
	timeoutTim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	timeoutTim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(&timeoutTim) != HAL_OK)
	{ Error_Handler(); }

	HAL_NVIC_SetPriority (TIM4_IRQn, 2, 1);
	HAL_NVIC_EnableIRQ (TIM4_IRQn);
	HAL_TIM_Base_Start_IT (&timeoutTim);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {  }
}
