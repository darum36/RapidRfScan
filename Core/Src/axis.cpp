/*
 * axis.cpp
 *
 *  Created on: Aug 8, 2022
 *      Author: Kocherov
 */

#include <axis.h>

extern void Error_Handler();

Axis::Axis(TIM_TypeDef* PWMTim,
		   GPIO_TypeDef portPWM, uint16_t pinPWM, uint8_t afMapingPWM,
		   TIM_TypeDef* EncTim,
		   GPIO_TypeDef portEncA, uint16_t pinEncA, uint8_t afMapingEncA,
		   GPIO_TypeDef portEncB, uint16_t pinEncB, uint8_t afMapingEncB,
		   GPIO_TypeDef portLimPlus, uint16_t	pinLimPlus,
		   GPIO_TypeDef portLimMinus, uint16_t pinLimMinus,
		   GPIO_TypeDef portLimHome, uint16_t	pinLimHome,
		   GPIO_TypeDef portDir, uint16_t	pinDir):

	mPwmTimTypeDef(PWMTim),
	gPwmPort(portPWM), gPwmPin(pinPWM), gAfMapingPWM(afMapingPWM),
	mEncTimTypeDef(EncTim),
	gEncAPort(portEncA), gEncAPin(pinEncA), gAfMapingEncA(afMapingEncA),
	gEncBPort(portEncB), gEncBPin(pinEncB), gAfMapingEncB(afMapingEncB),
	gLimPlusPort(portLimPlus), gLimPlusPin(pinLimPlus),
	gLimMinusPort(portLimMinus), gLimMinusPin(pinLimMinus),
	gLimHomePort(portLimHome), gLimHomePin(pinLimHome),
	gDirPort(portDir), gDirPin(pinDir),
	mMotion(&mPWMTim)
{
	init();
}

void Axis::setDirection(eDirection dir)
{
		if (dir == eDirection::Positive)
		{
			HAL_GPIO_WritePin(&gDirPort, gDirPin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(&gDirPort, gDirPin, GPIO_PIN_RESET);
		}
}

void Axis::init()
{
/* Шим сигнал  */

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

	GPIO_InitStruct.Pin = gPwmPin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = gAfMapingPWM;
	HAL_GPIO_Init(&gPwmPort, &GPIO_InitStruct);

	/* Энкодер  */
	TIM_Encoder_InitTypeDef sConfig = {0};
	sMasterConfig = {0};

	mEncTim.Instance = mEncTimTypeDef;
	mEncTim.Init.Prescaler = 0;
	mEncTim.Init.CounterMode = TIM_COUNTERMODE_UP;
	mEncTim.Init.Period = 65535;
	mEncTim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	mEncTim.Init.RepetitionCounter = 0;
	mEncTim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	sConfig.EncoderMode = TIM_ENCODERMODE_TI1;
	sConfig.IC1Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC1Filter = 0;
	sConfig.IC2Polarity = TIM_ICPOLARITY_RISING;
	sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
	sConfig.IC2Filter = 0;
	if (HAL_TIM_Encoder_Init(&mEncTim, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&mEncTim, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}

	GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = gEncAPin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = gAfMapingEncA;
    HAL_GPIO_Init(&gEncAPort, &GPIO_InitStruct);

	GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = gEncBPin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = gAfMapingEncB;
    HAL_GPIO_Init(&gEncBPort, &GPIO_InitStruct);

    GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = gLimPlusPin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(&gLimPlusPort, &GPIO_InitStruct);

    GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = gLimMinusPin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(&gLimMinusPort, &GPIO_InitStruct);

    GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = gLimHomePin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(&gLimHomePort, &GPIO_InitStruct);

    GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = gDirPin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(&gDirPort, &GPIO_InitStruct);
}


