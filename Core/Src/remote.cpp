/*
 * remote.cpp
 *
 *  Created on: 25 авг. 2022 г.
 *      Author: Kocherov I.O.
 */

#include <remote.h>
#include <motion.h>

extern void Error_Handler();

Remote::Remote()
{
	mOnOffStatus = false;
	mFineStatus = false;
	mRunStatus = false;
	mCurrentAxis = 0;
}

void Remote::checkOnOffStatus()
{
	bool newOnOffStatus = (HAL_GPIO_ReadPin(gPortOnOff, gPinOnOff) == GPIO_PIN_SET) ? false : true;
	mOnOffStatus = newOnOffStatus;
}

bool Remote::getOnOffStatus()
{
	return mOnOffStatus;
}

void Remote::checkRunStatus()
{
	bool newRunStatus = (HAL_GPIO_ReadPin(gPortRun, gPinRun) == GPIO_PIN_SET) ? true : false;
	mRunStatus = newRunStatus;
}

bool Remote::getRunStatus()
{
	return mRunStatus;
}

void Remote::checkFineStatus()
{
	bool newFineStatus = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET) ? true : false;
	mFineStatus = newFineStatus;
}

bool Remote::getFineStatus()
{
	return mFineStatus;
}

void Remote::checkSpeed()
{
	HAL_ADC_Start(&mAdc);
	HAL_ADC_PollForConversion(&mAdc, 100);
	uint32_t newAdcSpeed = HAL_ADC_GetValue(&mAdc);
	mCurrentAdcSpeed = newAdcSpeed;
}

int Remote::getSpeedStatus()
{
	int speedStatus;

	if ((mCurrentAdcSpeed <= mAvrAdcValue + mSizeDeadZone) && (mCurrentAdcSpeed >= mAvrAdcValue-mSizeDeadZone)) {speedStatus = 0;}
	else if (mCurrentAdcSpeed > mAvrAdcValue + mSizeDeadZone) {speedStatus = (-int(mCurrentAdcSpeed - mAvrAdcValue));}
	else if (mCurrentAdcSpeed < mAvrAdcValue - mSizeDeadZone) {speedStatus = (int(mAvrAdcValue) - int(mCurrentAdcSpeed));}

	return speedStatus;
}

void Remote::checkNumberAxe()
{
	bool newAxis0 = (HAL_GPIO_ReadPin(gPortAxis0, gPinAxis0) == GPIO_PIN_SET) ? 1 : 0;
	bool newAxis1 = (HAL_GPIO_ReadPin(gPortAxis1, gPinAxis1) == GPIO_PIN_SET) ? 1 : 0;
	bool newAxis2 = (HAL_GPIO_ReadPin(gPortAxis2, gPinAxis2) == GPIO_PIN_SET) ? 1 : 0;

	mCurrentAxis = (newAxis2 << 2) + (newAxis1 << 1) + (newAxis0 << 0);
}

short Remote::getAxisStatus()
{
	return mCurrentAxis;
}

void Remote::checkRemoteStatus()
{
	checkOnOffStatus();
	checkRunStatus();
	checkFineStatus();
	checkSpeed();
	checkNumberAxe();
}

void Remote::init(ADC_HandleTypeDef adc,
				  GPIO_TypeDef* portSpeed, uint16_t pinSpeed,
				  GPIO_TypeDef* portOnOff, uint16_t	pinOnOff,
				  GPIO_TypeDef* portRun, uint16_t	pinRun,
				  GPIO_TypeDef* portFine, uint16_t	pinFine,
				  GPIO_TypeDef* portAxis0, uint16_t	pinAxis0,
				  GPIO_TypeDef* portAxis1, uint16_t	pinAxis1,
				  GPIO_TypeDef* portAxis2, uint16_t	pinAxis2)
{
	mAdc = adc;

	gPortSpeed = portSpeed;
	gPinSpeed = pinSpeed;
	gPortOnOff = portOnOff;
	gPinOnOff = pinOnOff;
	gPortRun = portRun;
	gPinRun = pinRun;
	gPortFine = portFine;
	gPinFine = pinFine;
	gPortAxis0 = portAxis0;
	gPinAxis0 = pinAxis0;
	gPortAxis1 = portAxis1;
	gPinAxis1 = pinAxis1;
	gPortAxis2 = portAxis2;
	gPinAxis2 = pinAxis2;

	mMaxAdcValue = 4000;
	mAvrAdcValue = 2000;
	mMinAdcValue = 20;
	mSizeDeadZone = 200;

	__HAL_RCC_ADC1_CLK_ENABLE();
	  ADC_ChannelConfTypeDef sConfig = {0};

	  mAdc.Instance = ADC1;
	  mAdc.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
	  mAdc.Init.Resolution = ADC_RESOLUTION_12B;
	  mAdc.Init.ScanConvMode = DISABLE;
	  mAdc.Init.ContinuousConvMode = ENABLE;
	  mAdc.Init.DiscontinuousConvMode = DISABLE;
	  mAdc.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	  mAdc.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	  mAdc.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	  mAdc.Init.NbrOfConversion = 1;
	  mAdc.Init.DMAContinuousRequests = DISABLE;
	  mAdc.Init.EOCSelection = ADC_EOC_SINGLE_CONV;


	  if (HAL_ADC_Init(&mAdc) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  sConfig.Channel = ADC_CHANNEL_1;
	  sConfig.Rank = 1;
	  sConfig.SamplingTime = ADC_SAMPLETIME_15CYCLES;
	  sConfig.Offset       = 0;

	  if (HAL_ADC_ConfigChannel(&mAdc, &sConfig) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  GPIO_InitTypeDef GPIO_InitStruct = {0};

	  GPIO_InitStruct.Pin = gPinSpeed;
	  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  HAL_GPIO_Init(gPortSpeed, &GPIO_InitStruct);
}

