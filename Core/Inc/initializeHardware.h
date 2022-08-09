/*
 * initializeHardware.h
 *
 *  Created on: 9 авг. 2022 г.
 *      Author: Rumyantzev_D
 */

#ifndef INC_INITIALIZEHARDWARE_H_
#define INC_INITIALIZEHARDWARE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

void initHardware();

void SystemClock_Config(void);
void Error_Handler(void);

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

#ifdef __cplusplus
}
#endif

#endif /* INC_INITIALIZEHARDWARE_H_ */
