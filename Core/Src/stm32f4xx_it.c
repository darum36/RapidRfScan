/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f4xx_it.h"

extern TIM_HandleTypeDef htim10;
extern UART_HandleTypeDef huart1;

void NMI_Handler(void)
{
	while (1) {}
}

void HardFault_Handler(void)
{
	while (1) {}
}

void MemManage_Handler(void)
{
	while (1) {}
}

void BusFault_Handler(void)
{
	while (1) {}
}

void UsageFault_Handler(void)
{
	while (1) {}
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{

}

void SysTick_Handler(void)
{
  HAL_IncTick();
}

void USART1_IRQHandler(void)
{

  HAL_UART_IRQHandler(&huart1);
}

