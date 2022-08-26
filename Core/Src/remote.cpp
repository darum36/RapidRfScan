/*
 * remote.cpp
 *
 *  Created on: 25 авг. 2022 г.
 *      Author: Kocherov I.O.
 */

#include <remote.h>

extern void Error_Handler();

Remote::Remote()

{
	mOnOffStatus = false;
	mFineStatus = false;
	mRunStatus = false;
	mCurrentAdcSpeed = 0;
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
	HAL_ADC_Start(&hadc1);
	uint32_t newAdcSpeed = HAL_ADC_GetValue(&hadc1);
	mCurrentAdcSpeed = newAdcSpeed;
	HAL_ADC_Stop(&hadc1);
}

void Remote::checkNumberAxe()
{

}

void Remote::checkRemoteStatus()
{
	checkOnOffStatus();
	checkRunStatus();
	checkFineStatus();
	checkSpeed();
}

void Remote::init(ADC_TypeDef* ADCSpeed,
				  GPIO_TypeDef* portSpeed, uint16_t pinSpeed,
				  GPIO_TypeDef* portOnOff, uint16_t	pinOnOff,
				  GPIO_TypeDef* portRun, uint16_t	pinRun,
				  GPIO_TypeDef* portFine, uint16_t	pinFine,
				  GPIO_TypeDef* portAxis0, uint16_t	pinAxis0,
				  GPIO_TypeDef* portAxis1, uint16_t	pinAxis1,
				  GPIO_TypeDef* portAxis2, uint16_t	pinAxis2)
{
	mADCSpeedTypeDef = ADCSpeed;

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
}

