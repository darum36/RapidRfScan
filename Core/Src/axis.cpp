/*
 * axis.cpp
 *
 *  Created on: Aug 8, 2022
 *      Author: Kocherov I.O.
 */

#include <axis.h>

extern void Error_Handler();
extern Remote remoteController1;

Axis::Axis():mMotion(&mPWMTim, &mEncTim)
{
	tempStatus = 0;
	perPosMoving = false;
	permNegMoving = false ;
	homecome = false;
	ableMoving = false;

	turnOnAxis = 0;
	inverseLim = 0;
	inverseEnc = 0;
	inverseMot = 0;
	enableLim = 0;

	modeMov = 0;
	beginMov = false;
}


short Axis::getLimStatus()
{
	short indLimPlus = 0;
	short indMinusPlus = 1;
	short indHomePlus = 2;

	short limPlusStatus = (HAL_GPIO_ReadPin(gLimPlusPort, gLimPlusPin) == GPIO_PIN_SET) ? 0 : 1;
	short limMinusStatus = (HAL_GPIO_ReadPin(gLimMinusPort, gLimMinusPin)== GPIO_PIN_SET) ? 0 : 1;
	short limHomeStatus = (HAL_GPIO_ReadPin(gLimHomePort, gLimHomePin)== GPIO_PIN_SET) ? 0 : 1;

	if (((inverseLim >> 0) & 1) == 1) { limPlusStatus =! limPlusStatus; }
	if (((inverseLim >> 1) & 1) == 1) { limMinusStatus =! limMinusStatus; }
	if (((inverseLim >> 2) & 1) == 1) { limHomeStatus =! limHomeStatus; }

	tempStatus = 0;
	if (limPlusStatus) tempStatus|=(1<<indLimPlus);
	if (limMinusStatus) tempStatus|=(1<<indMinusPlus);
	if (limHomeStatus) tempStatus|=(1<<indHomePlus);

	return tempStatus;
}

void Axis::checkLimits()
{
	short limPlusStatus = (HAL_GPIO_ReadPin(gLimPlusPort, gLimPlusPin) == GPIO_PIN_SET) ? 0 : 1;
	short limMinusStatus=(HAL_GPIO_ReadPin(gLimMinusPort, gLimMinusPin)== GPIO_PIN_SET) ? 0 : 1;
	short limHomeStatus=(HAL_GPIO_ReadPin(gLimHomePort, gLimHomePin)== GPIO_PIN_SET) ? 0 : 1;

	if (((inverseLim >> 0) & 1) == 1) { limPlusStatus =! limPlusStatus; }
	if (((inverseLim >> 1) & 1) == 1) { limMinusStatus =! limMinusStatus; }
	if (((inverseLim >> 2) & 1) == 1) { limHomeStatus =! limHomeStatus; }

	perPosMoving = (!limPlusStatus || (!((enableLim >> 0) & 1))) ? true : false;
	permNegMoving = (!limMinusStatus || (!((enableLim >> 1) & 1))) ? true : false;
	homecome = (!limHomeStatus || (!((enableLim >> 2) & 1))) ? false : true;
}

bool Axis::checkAbleMoving()
{
	eDirection dir;
	bool direct = remoteController1.getSpeedStatus() < 0 ? 0 : 1;
	(direct == 1) ? (dir = eDirection::Positive) : (dir = eDirection::Negative);

	ableMoving =(((perPosMoving == true) && (dir == eDirection::Positive))||((permNegMoving == true && (dir == eDirection::Negative)))) ? true : false;

	return (ableMoving);
}


void Axis::setDirection(eDirection dir)
{
	if (inverseMot == 0)
	{
		if (dir == eDirection::Positive)
		{ HAL_GPIO_WritePin(gDirPort, gDirPin, GPIO_PIN_SET); }
		else
		{ HAL_GPIO_WritePin(gDirPort, gDirPin, GPIO_PIN_RESET); }
	}
	else
	{
		if (dir == eDirection::Positive)
		{ HAL_GPIO_WritePin(gDirPort, gDirPin, GPIO_PIN_RESET); }
		else
		{ HAL_GPIO_WritePin(gDirPort, gDirPin, GPIO_PIN_SET); }
	}
}

int Axis::getDirection()
{
	int direction;
	direction = (HAL_GPIO_ReadPin(gDirPort, gDirPin) == GPIO_PIN_SET) ? 1 : -1;
	return direction;
}


void Axis::setTurnOnAxis( int32_t newTurnOnAxis)
{ turnOnAxis = newTurnOnAxis; }

int32_t Axis::getTurnOnAxis()
{ return turnOnAxis; }

//--------------------------------------INVERSE--------------------------------------------------

void Axis::setInverseLim(int32_t newInverseLim)
{ inverseLim = newInverseLim; }

int32_t Axis::getInverseLim()
{ return inverseLim; }


void Axis::setEnableLim(int32_t newEnableLim)
{ enableLim = newEnableLim; }

int32_t Axis::getEnableLim()
{ return enableLim; }


void Axis::setInverseEncoder(int32_t newInveseEnc)
{
	inverseEnc = newInveseEnc;
	sConfigEnc.IC1Polarity = inverseEnc ? TIM_ICPOLARITY_FALLING : TIM_ICPOLARITY_RISING;
	HAL_TIM_Encoder_Init(&mEncTim, &sConfigEnc);
}

int32_t Axis::getInverseEncoder()
{ return inverseEnc; }


void Axis::setInverseMotor(int32_t newInverseMot)
{ inverseMot = newInverseMot; }

int32_t Axis::getInverseMotor()
{ return inverseMot; }

//-----------------------------------------------------------------------------------------------

void Axis::jogging(bool beginJog)
{ mMotion.jogging(beginJog); }

void Axis::ptp()
{
	mMotion.ptp();
}

void Axis::tempSetParam(float newSpeed, float newAcc, float newDcc)
{
	mMotion.setSpeed(newSpeed);
	mMotion.setAcc(newAcc);
	mMotion.setDcc(newDcc);
}

void Axis::setDefaultParam()
{
	mMotion.setSpeed(1000000);
	mMotion.setAcc(1000000);
	mMotion.setDcc(1000000);
	mMotion.setPosition(0);

}

void Axis::updateEnc()
{ mMotion.updateEnc(); }

//--------------------------------------MOVING--------------------------------------------------

bool Axis::beginMoving()
{ return (beginMov == true) ? true : false; }

void Axis::begin()
{ beginMov = true; }

void Axis::stop()
{ beginMov = false; }

void Axis::emgStop()
{ mMotion.resetMotion(); }

//--------------------------------------READ----------------------------------------------

int32_t Axis::getSpeed()
{
	int32_t axisSpeed = int32_t(mMotion.getSpeed());
	return axisSpeed;
}

int32_t Axis::getAcc()
{
	int32_t axisAcc = int32_t(mMotion.getAcc());
	return axisAcc;
}

int32_t Axis::getDcc()
{
	int32_t axisDcc = int32_t(mMotion.getDcc());
	return axisDcc;
}

int32_t Axis::modeMoving()
{ return int32_t(modeMov); }

int32_t Axis::getPosition()
{
	int32_t position = mMotion.getPosition();
	return position;
}

//--------------------------------------SET----------------------------------------------------

void Axis::setSpeed(int32_t newSpeed)
{
	float axisSpeed = float(newSpeed);
	mMotion.setSpeed(axisSpeed);
}

void Axis::setAcc(int32_t newAcc)
{
	float axisAcc = float(newAcc);
	mMotion.setAcc(axisAcc);
}

void Axis::setDcc(int32_t newDcc)
{
	float axisDcc = float(newDcc);
	mMotion.setDcc(axisDcc);
}

void Axis::setModeMoving(int32_t MM)
{ modeMov = MM; }

void Axis::setPosition(int32_t newPosition)
{ mMotion.setPosition(newPosition); }

//---------------------------------------INIT---------------------------------------------------

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
	Motion mMotion(&mPWMTim, &mEncTim);

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
	sConfigEnc = {0};
	sMasterConfig = {0};

	mEncTim.Instance = mEncTimTypeDef;
	mEncTim.Init.Prescaler = 0;
	mEncTim.Init.CounterMode = TIM_COUNTERMODE_UP;
	mEncTim.Init.Period = 65535;
	mEncTim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	mEncTim.Init.RepetitionCounter = 0;
	mEncTim.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
	sConfigEnc.EncoderMode = TIM_ENCODERMODE_TI12;
	sConfigEnc.IC1Polarity = TIM_ICPOLARITY_RISING;
	sConfigEnc.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	sConfigEnc.IC1Prescaler = TIM_ICPSC_DIV2;
	sConfigEnc.IC1Filter = 15;
	sConfigEnc.IC2Polarity = TIM_ICPOLARITY_RISING;
	sConfigEnc.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	sConfigEnc.IC2Prescaler = TIM_ICPSC_DIV2;
	sConfigEnc.IC2Filter = 15;
	if (HAL_TIM_Encoder_Init(&mEncTim, &sConfigEnc) != HAL_OK)
	{
		Error_Handler();
	}

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&mEncTim, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	__HAL_TIM_SET_COUNTER(&mEncTim,0);
	HAL_TIM_Encoder_Start(&mEncTim, TIM_CHANNEL_ALL);

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

