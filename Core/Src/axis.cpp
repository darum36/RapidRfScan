/*
 * axis.cpp
 *
 *  Created on: Aug 8, 2022
 *      Author: Kocherov I.O.
 */

#include <axis.h>

extern void Error_Handler();

Axis::Axis():mMotion(&mPWMTim)
{
	tempStatus = 0;
	perPosMoving= false;
	permNegMoving= false ;
	homecome= false;
	ableMoving= false;
	inverseLim=false;
}

short Axis::getLimStatus()
{
	short indLimPlus = 0;
	short indMinusPlus = 1;
	short indHomePlus = 2;

	short limPlusStatus = (HAL_GPIO_ReadPin(gLimPlusPort, gLimPlusPin) == GPIO_PIN_SET) ? 0 : 1;
	short limMinusStatus = (HAL_GPIO_ReadPin(gLimMinusPort, gLimMinusPin)== GPIO_PIN_SET) ? 0 : 1;
	short limHomeStatus = (HAL_GPIO_ReadPin(gLimHomePort, gLimHomePin)== GPIO_PIN_SET) ? 0 : 1;

	if (limPlusStatus) tempStatus|=(1<<indLimPlus);
	if (limMinusStatus) tempStatus=tempStatus&(1<<indMinusPlus);
	if (limHomeStatus) tempStatus=tempStatus&(1<<indHomePlus);

	return tempStatus;
}

void Axis::checkLimits()
{

	short limPlusStatus = (HAL_GPIO_ReadPin(gLimPlusPort, gLimPlusPin) == GPIO_PIN_SET) ? 0 : 1;
	short limMinusStatus=(HAL_GPIO_ReadPin(gLimMinusPort, gLimMinusPin)== GPIO_PIN_SET) ? 0 : 1;
	short limHomeStatus=(HAL_GPIO_ReadPin(gLimHomePort, gLimHomePin)== GPIO_PIN_SET) ? 0 : 1;

	if (inverseLim == true)
	{
		limPlusStatus=!limPlusStatus;
		limMinusStatus=!limMinusStatus;
		limHomeStatus=!limHomeStatus;
	}

	perPosMoving = (limPlusStatus) ? false : true;
	permNegMoving =(limMinusStatus) ? false : true;
	homecome = (limHomeStatus) ? true : false;
}

bool Axis::checkAbleMoving()
{
	eDirection dir;
	bool direct=HAL_GPIO_ReadPin(gDirPort, gDirPin);
	(direct == 1) ? (dir = eDirection::Positive) : (dir = eDirection::Negative);

	ableMoving =(((perPosMoving == true) && (dir==eDirection::Positive))||((permNegMoving == true && (dir==eDirection::Negative)))) ? true : false;

	return (ableMoving);
}

void Axis::setDirection(eDirection dir)
{
		if (dir == eDirection::Positive)
		{
			HAL_GPIO_WritePin(gDirPort, gDirPin, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(gDirPort, gDirPin, GPIO_PIN_RESET);
		}
}

void Axis::setInverseLim()
{
	inverseLim=true;
}

void Axis::jogging(eDirection dir)
{
	setDirection(dir);
	mMotion.jogging();
}

void Axis::ptp(eDirection dir)
{
	setDirection(dir);
	mMotion.ptp();
}

void Axis::tempSetParam(float newSpeed, float newAcc, float newDcc)
{
	mMotion.setSpeed(newSpeed);
	mMotion.setAcc(newAcc);
	mMotion.setDcc(newDcc);
}

void Axis::tempDefaultParam()
{
	mMotion.setSpeed(1000000);
	mMotion.setAcc(1000000);
	mMotion.setDcc(1000000);
	mMotion.setSteps(5000000);
}

void Axis::emgStop()
{
 mMotion.resetMotion();
}

void Axis::init(TIM_TypeDef* PWMTim, GPIO_TypeDef* portPWM, uint16_t pinPWM, uint8_t afMapingPWM,             		 	// PWM
		TIM_TypeDef* EncTim,  GPIO_TypeDef* portEncA, uint16_t pinEncA, uint8_t afMapingEncA,    		 				// TIM ENCA
							  GPIO_TypeDef* portEncB, uint16_t pinEncB, uint8_t afMapingEncB,					 		// TIM ENCB																              // GPIO DIR
		GPIO_TypeDef* portLimPlus, uint16_t	pinLimPlus, GPIO_TypeDef* portLimMinus, uint16_t pinLimMinus,    			// GPIOs LIMIT
		GPIO_TypeDef* portLimHome, uint16_t	pinLimHome,
		GPIO_TypeDef* portDir, uint16_t	pinDir)
{

	mPwmTimTypeDef=PWMTim;
	gPwmPort=portPWM;
	gPwmPin=pinPWM;
	gAfMapingPWM=afMapingPWM;
	mEncTimTypeDef=EncTim;
	gEncAPort=portEncA;
	gEncAPin=pinEncA;
	gAfMapingEncA=afMapingEncA;
	gEncBPort=portEncB;
	gEncBPin=pinEncB;
	gAfMapingEncB=afMapingEncB;
	gLimPlusPort=portLimPlus;
	gLimPlusPin=pinLimPlus;
	gLimMinusPort=portLimMinus;
	gLimMinusPin=pinLimMinus;
	gLimHomePort=portLimHome;
	gLimHomePin=pinLimHome;
	gDirPort=portDir;
	gDirPin=pinDir;
	Motion mMotion(&mPWMTim);

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
	HAL_GPIO_Init(gPwmPort, &GPIO_InitStruct);

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
    HAL_GPIO_Init(gEncAPort, &GPIO_InitStruct);

	GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = gEncBPin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = gAfMapingEncB;
    HAL_GPIO_Init(gEncBPort, &GPIO_InitStruct);

    GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = gLimPlusPin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(gLimPlusPort, &GPIO_InitStruct);

    GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = gLimMinusPin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(gLimMinusPort, &GPIO_InitStruct);

    GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = gLimHomePin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(gLimHomePort, &GPIO_InitStruct);

    GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = gDirPin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(gDirPort, &GPIO_InitStruct);

}

