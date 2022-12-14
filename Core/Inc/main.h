/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void TIM1_UP_TIM10_IRQHandler(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TRS_FINE_Pin GPIO_PIN_13
#define TRS_FINE_GPIO_Port GPIOC
#define RC_LMT__Pin GPIO_PIN_14
#define RC_LMT__GPIO_Port GPIOC
#define RC_LMT_C15_Pin GPIO_PIN_15
#define RC_LMT_C15_GPIO_Port GPIOC
#define LIM_1_Pin GPIO_PIN_0
#define LIM_1_GPIO_Port GPIOC
#define LIM_1C1_Pin GPIO_PIN_1
#define LIM_1C1_GPIO_Port GPIOC
#define LIM_2_Pin GPIO_PIN_2
#define LIM_2_GPIO_Port GPIOC
#define LIM_2C3_Pin GPIO_PIN_3
#define LIM_2C3_GPIO_Port GPIOC
#define STEP2_Pin GPIO_PIN_0
#define STEP2_GPIO_Port GPIOA
#define SPEED_Pin GPIO_PIN_1
#define SPEED_GPIO_Port GPIOA
#define AXIS2_Pin GPIO_PIN_2
#define AXIS2_GPIO_Port GPIOA
#define ENC_IND_1_Pin GPIO_PIN_3
#define ENC_IND_1_GPIO_Port GPIOA
#define ENC_IND_2_Pin GPIO_PIN_4
#define ENC_IND_2_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define ENC_A2_Pin GPIO_PIN_6
#define ENC_A2_GPIO_Port GPIOA
#define ENC_B2_Pin GPIO_PIN_7
#define ENC_B2_GPIO_Port GPIOA
#define TRIG_IN_Pin GPIO_PIN_5
#define TRIG_IN_GPIO_Port GPIOC
#define BOARD_ID_Pin GPIO_PIN_0
#define BOARD_ID_GPIO_Port GPIOB
#define LED1_Pin GPIO_PIN_1
#define LED1_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_2
#define LED2_GPIO_Port GPIOB
#define LIM_HOME_1_Pin GPIO_PIN_10
#define LIM_HOME_1_GPIO_Port GPIOB
#define LIM_HOME_2_Pin GPIO_PIN_12
#define LIM_HOME_2_GPIO_Port GPIOB
#define DIR1_Pin GPIO_PIN_14
#define DIR1_GPIO_Port GPIOB
#define DIR2_Pin GPIO_PIN_15
#define DIR2_GPIO_Port GPIOB
#define TRIG_OUT_Pin GPIO_PIN_6
#define TRIG_OUT_GPIO_Port GPIOC
#define ON_OFF_Pin GPIO_PIN_9
#define ON_OFF_GPIO_Port GPIOC
#define ENC_B1_Pin GPIO_PIN_8
#define ENC_B1_GPIO_Port GPIOA
#define ENC_A1_Pin GPIO_PIN_9
#define ENC_A1_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define STEP1_Pin GPIO_PIN_15
#define STEP1_GPIO_Port GPIOA
#define AXIS0_Pin GPIO_PIN_10
#define AXIS0_GPIO_Port GPIOC
#define AXIS1_Pin GPIO_PIN_11
#define AXIS1_GPIO_Port GPIOC
#define RUN_Pin GPIO_PIN_12
#define RUN_GPIO_Port GPIOC
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
#define LED3_Pin GPIO_PIN_4
#define LED3_GPIO_Port GPIOB
#define Emergency_status_Pin GPIO_PIN_5
#define Emergency_status_GPIO_Port GPIOB
#define LED4_Pin GPIO_PIN_7
#define LED4_GPIO_Port GPIOB
#define LED5_Pin GPIO_PIN_8
#define LED5_GPIO_Port GPIOB
#define LED6_Pin GPIO_PIN_9
#define LED6_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
