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

TIM_HandleTypeDef htim10;
UART_HandleTypeDef huart1;

void initMotion();
void initUart();

void initMotion()
{
	Axis firstAxis(TIM2, STEP1_GPIO_Port, STEP1_Pin, GPIO_AF1_TIM2,   // PWM
				  TIM1, ENC_A1_GPIO_Port, ENC_A1_Pin, GPIO_AF1_TIM1, // TIM ENCA
				  ENC_B1_GPIO_Port, ENC_B1_Pin, GPIO_AF1_TIM1,		 // TIM ENCB																              // GPIO DIR
				  LIM_1_GPIO_Port, LIM_1_Pin,						 // GPIOs LIMIT
				  LIM_1C1_GPIO_Port, LIM_1C1_Pin,                    // GPIOs LIMIT
				  LIM_HOME_1_GPIO_Port, LIM_HOME_1_Pin,		         // GPIOs LIMIT
				  DIR1_GPIO_Port, DIR1_Pin);
}

Axis firstAxis(TIM2, STEP1_GPIO_Port, STEP1_Pin, GPIO_AF1_TIM2,   // PWM
			  TIM1, ENC_A1_GPIO_Port, ENC_A1_Pin, GPIO_AF1_TIM1, // TIM ENCA
			  ENC_B1_GPIO_Port, ENC_B1_Pin, GPIO_AF1_TIM1,		 // TIM ENCB																              // GPIO DIR
			  LIM_1_GPIO_Port, LIM_1_Pin,						 // GPIOs LIMIT
			  LIM_1C1_GPIO_Port, LIM_1C1_Pin,                    // GPIOs LIMIT
			  LIM_HOME_1_GPIO_Port, LIM_HOME_1_Pin,		         // GPIOs LIMIT
			  DIR1_GPIO_Port, DIR1_Pin);

void TIM1_UP_TIM10_IRQHandler(void) //Прерывание раз в 1 мс
{
	firstAxis.jogging(eDirection::Positive);

	HAL_TIM_IRQHandler(&htim10);
}

int main(void)
{
	initHardware();
	initMotion();
	initUart();

	firstAxis.tempSetParam(1400000, 1000000, 1000000);

	while (1)
	{
//		UART(&huart1);
	}
}

void initUart()
{

}
