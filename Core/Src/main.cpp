/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "initializeHardware.h"
#include <string>
#include <cmath>
#include <axis.h>

#include "Ring_Buf.h"
#include "UART_Functions.h"

extern TIM_HandleTypeDef htim10;
extern UART_HandleTypeDef huart1;

void TIM1_UP_TIM10_IRQHandler(void);
void initMotion();
void initUart();

int main(void)
{
	initHardware();
	initMotion();
	initUart();

	while (1)
	{
		UART(&huart1);
	}
}

void initMotion()
{

}

void initUart()
{

}

void TIM1_UP_TIM10_IRQHandler(void) //Прерывание раз в 1 мс
{


	HAL_TIM_IRQHandler(&htim10);
}
