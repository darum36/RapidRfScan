/*
 * axis.cpp
 *
 *  Created on: Aug 8, 2022
 *      Author: Kocherov
 */

#include <axis.h>

extern void Error_Handler();

Axis::Axis(TIM_TypeDef* mPwmTimTypeDef, GPIO_TypeDef port, uint16_t pin, uint8_t afMaping):
	mPwmTimTypeDef(mPwmTimTypeDef), mMotion(&mPWMTim),
	mPort(port), mPin(pin), mAfMaping(afMaping)
{
	init();
}

void Axis::init()
{
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	mPWMTim.Instance = mPwmTimTypeDef;
	mPWMTim.Init.Prescaler = 0;
	mPWMTim.Init.CounterMode = TIM_COUNTERMODE_UP;
	mPWMTim.Init.Period = 1000-1;
	mPWMTim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	mPWMTim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	if (HAL_TIM_PWM_Init(&mPWMTim) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&mPWMTim, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 10-1;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&mPWMTim, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}

    GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = mPin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = mAfMaping;
	HAL_GPIO_Init(&mPort, &GPIO_InitStruct);
}
