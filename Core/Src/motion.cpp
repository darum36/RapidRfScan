///*
// * motion.cpp
// *
// *  Created on: Aug 8, 2022
// *      Author: Kocherov I.O.
// */

#include <motion.h>
#include "math.h"

extern Remote remoteController1;

Motion::Motion(TIM_HandleTypeDef* PWMTim):
mPWMTim(PWMTim)
{
	mMovingTime = 0;
}

void Motion::acceleration() 										// Ускорение
{

float deltaSpeed=mAcc/float(1000);
uint32_t currentPeriod;


if (!motorOn()) {mCurrentSpeed=0;}
if (mCurrentSpeed+deltaSpeed<=mSpeed)																/* Пока скорость не достигла максимальной будем ускорятся */
{
	if (motorOn())																			/* Если мы двигались до этого, то увеличим скорость */
	{
		mCurrentSpeed=mSpeed+deltaSpeed;													/* Увеличение промежуточной скорости на один шаг */
		currentPeriod=round(float(100000000)/mCurrentSpeed);

		__HAL_TIM_SET_COMPARE(mPWMTim,TIM_CHANNEL_1,(currentPeriod)/2+1);					/* Установка скважности Шима */
		__HAL_TIM_SET_AUTORELOAD(mPWMTim, currentPeriod);									/* Установка периода Шима */
	}
	else																					/* Если мы не двигались до этого, то увеличим скорость и включим мотор */
	{
		if (deltaSpeed>mSpeed)																/* Если шаг ускорения больше чем максимальная скорость, то */
		{
			mCurrentSpeed=mSpeed;															/* Скорость сразу становится максимальной */
			currentPeriod=round(mCurrentSpeed);

			__HAL_TIM_SET_COMPARE(mPWMTim,TIM_CHANNEL_1,(currentPeriod)/2+1);				/* Установка скважности Шима */
			__HAL_TIM_SET_AUTORELOAD(mPWMTim, currentPeriod);								/* Установка периода Шима */
			HAL_TIM_PWM_Start(mPWMTim, TIM_CHANNEL_1);										/* Запуск мотора */
		}
		else																				/* Если шаг ускорения меньше чем максимальная скорость, то */
		{
			mCurrentSpeed=deltaSpeed;
			currentPeriod=round(float(100000000)/mCurrentSpeed);
			if (!currentPeriod) currentPeriod = 100000000;

			__HAL_TIM_SET_COMPARE(mPWMTim,TIM_CHANNEL_1,(currentPeriod)/2+1);     			/* Установка скважности Шима */
			__HAL_TIM_SET_AUTORELOAD(mPWMTim, currentPeriod);								/* Установка периода Шима */
			HAL_TIM_PWM_Start(mPWMTim, TIM_CHANNEL_1);								        /* Запуск мотора */
		}
	}
}
}

void Motion::deceleration()										    /* Торможение */
{
	float deltaSpeed=mDcc/double(1000);
	uint32_t currentPeriod;

	if (motorOn())
	{
		if (mCurrentSpeed>deltaSpeed)																	/* Если дальше тормозить некуда, то */
		{
			mCurrentSpeed=mCurrentSpeed-deltaSpeed;																	/* Уменьшаем скорость на шаг */
			currentPeriod=round(float(100000000)/mCurrentSpeed);

			__HAL_TIM_SET_AUTORELOAD(mPWMTim, currentPeriod);						/* Установка периода Шима */
			__HAL_TIM_SET_COMPARE(mPWMTim,TIM_CHANNEL_1,currentPeriod/2+1);			/* Установка скважности Шима */
		}
		else
		{
			resetMotion();								/* Выключение мотора */
		}
	}
}

void Motion::moving ()												/* Равномерное движение */
{
	if (motorOn())
	{
		if (mMovingTime>0) { mMovingTime--; }
	}
}

bool Motion::motorOn()
{
	return (mPWMTim->Instance->CR1 & 1) ? true : false;
}

void Motion::jogging()								/* Движение при нажатой кнопке */
{
float deltaSpeedAcc=mAcc/double(1000);
float deltaSpeedDcc=mDcc/double(1000);
uint32_t currentPeriod;

if (!motorOn()) {mCurrentSpeed=0;}

if (mCurrentSpeed>0 || remoteController1.getRunStatus() == true)																/* Пока скорость больше нуля или нажата кнопка */ 		/*!!!!!!!!!!!!! Необходимо вписать номер порта и пина для кнопки!!!!!!!!!!!!!!!!! */
{
	if (remoteController1.getRunStatus() == true)																		/* Если кнопка нажата */								/*!!!!!!!!!!!!! Необходимо вписать номер порта и пина для кнопки!!!!!!!!!!!!!!!!! */
	{
		if (mCurrentSpeed>0)																			/* Проверка стартуем ли мы, нужно ли включать моторы */
		{
			if (mCurrentSpeed<=mSpeed)
			{
				mCurrentSpeed=mCurrentSpeed+deltaSpeedAcc;																	/* То ускоряемся */
				currentPeriod=round(float(100000000)/mCurrentSpeed);

				__HAL_TIM_SET_COMPARE(mPWMTim,TIM_CHANNEL_1,currentPeriod/2+1);			/* Установка скважности Шима */
				__HAL_TIM_SET_AUTORELOAD(mPWMTim, currentPeriod);						/* Установка периода Шима */
			}
			else
			{
				mCurrentSpeed=mSpeed;																		/* То скорость равна максимальной */
				currentPeriod=round(float(100000000)/mCurrentSpeed);

				__HAL_TIM_SET_COMPARE(mPWMTim,TIM_CHANNEL_1,currentPeriod/2+1);			/* Установка скважности Шима */
				__HAL_TIM_SET_AUTORELOAD(mPWMTim, currentPeriod);						/* Установка периода Шима */
			}
		}
		else																			/* Старт */
		{
			if (deltaSpeedAcc>=mSpeed)													/* Если шаг больше чем максимальная скорость, то */
			{
				mCurrentSpeed=mSpeed;																		/* скорость равна максимальной */
				currentPeriod=round(float(100000000)/mCurrentSpeed);

				__HAL_TIM_SET_COMPARE(mPWMTim,TIM_CHANNEL_1,currentPeriod/2+1);			/* Установка скважности Шима */
				__HAL_TIM_SET_AUTORELOAD(mPWMTim, currentPeriod);						/* Установка периода Ш�?Ма */
				HAL_TIM_PWM_Start(mPWMTim, TIM_CHANNEL_1);								/* Запуск мотора */
			}
			else																		/* Если шаг меньше чем максимальная скорость, то */
			{
				mCurrentSpeed=deltaSpeedAcc;																	/* Увеличиваем скорость на один шаг */
				currentPeriod=round(float(100000000)/mCurrentSpeed);

				__HAL_TIM_SET_COMPARE(mPWMTim,TIM_CHANNEL_1,currentPeriod/2+1);			/* Установка скважности Шима */
				__HAL_TIM_SET_AUTORELOAD(mPWMTim, currentPeriod);						/* Установка периода Ш�?Ма */
				HAL_TIM_PWM_Start(mPWMTim, TIM_CHANNEL_1);								/* Запуск мотора */
			}
		}
	}
	else																				/* Если кнопка не нажата и мы все ещё движемся */
	{
		if (mCurrentSpeed>deltaSpeedDcc)
		{
			mCurrentSpeed=mCurrentSpeed-deltaSpeedDcc;																		/* Замедляемся */
			currentPeriod=round(float(100000000)/mCurrentSpeed);
			__HAL_TIM_SET_AUTORELOAD(mPWMTim, currentPeriod);							/* Установка периода Шима */
			__HAL_TIM_SET_COMPARE(mPWMTim,TIM_CHANNEL_1,currentPeriod/2+1);				/* Установка скважности Шима */
		}
		else
		{
			HAL_TIM_PWM_Stop(mPWMTim, TIM_CHANNEL_1);									/* Выключаем мотор */
			mCurrentSpeed=0;																			/* Необходимо чтобы выйти из цикла */
		}
	}
}
}

void Motion::ptp()                                                                       /* Движение на заданное количество шагов */
{
float deltaSpeedAcc=mAcc/float(1000);
float deltaSpeedDcc=mDcc/float(1000);
uint32_t currentPeriod;
uint64_t Steps_deltaV;

if (!motorOn()) {mCurrentSpeed=0;}

if (mMovingTime == 0)
{
	Steps_deltaV=round((((deltaSpeedAcc+deltaSpeedDcc)/float(2))+mSpeed)*mSpeed/mAcc);

	if (mSteps<Steps_deltaV)
	{
		PtPModeMoving = ModeMoving::LowSpeed;

		mCurrentSpeed=1000;
		mMovingTime=round(float(mSteps)*float(1000)/mCurrentSpeed);
		currentPeriod=round(float(100000000)/mCurrentSpeed);

		__HAL_TIM_SET_COMPARE(mPWMTim,TIM_CHANNEL_1,currentPeriod/2+1);
		__HAL_TIM_SET_AUTORELOAD(mPWMTim, currentPeriod);
		HAL_TIM_PWM_Start(mPWMTim, TIM_CHANNEL_1);
	}
	else
	{
		PtPModeMoving = ModeMoving::NormalSpeed;

		mMovingSteps=mSteps-Steps_deltaV;
		mMovingTime=mMovingSteps*1000/uint64_t(round(mSpeed));
	}
}
else
{
	if (PtPModeMoving == ModeMoving::LowSpeed)
	{
		if (mMovingTime>0)
		{
			moving();
		}
		else
		{
			resetMotion();
		}
	}
	else if (PtPModeMoving == ModeMoving::NormalSpeed)
	{
		if (mMovingTime>0)
		{
			if (mCurrentSpeed < mSpeed)
			{
				acceleration();
			}
			else
			{
				moving();
			}
		}
		else
		{
			deceleration();
		}
	}
}
}

void Motion::setSpeed(float newSpeed)
{
	if (fabs(newSpeed) > 10000000)
	{
		newSpeed = newSpeed < 0 ? -10000000 : 10000000;
	}
	mSpeed = newSpeed;
}

float Motion::getSpeed()
{
	return mSpeed;
}

void Motion::setAcc(float newAcc)
{
	if (fabs(newAcc) > 1000000)
	{
		newAcc = newAcc < 0 ? -1000000 : 1000000;
	}
	mAcc = newAcc;
}

float Motion::getAcc()
{
	return mAcc;
}

void Motion::setDcc(float newDcc)
{
	if (fabs(newDcc) > 1000000)
	{
		newDcc = newDcc < 0 ? -1000000 : 1000000;
	}
	mDcc = newDcc;
}

float Motion::getDcc()
{
	return mDcc;
}

void Motion::setSteps(uint64_t newSteps)
{
	if (newSteps>0)
	{
		mSteps=newSteps;
	}
}

void Motion::resetMotion()
{
	HAL_TIM_PWM_Stop(mPWMTim, TIM_CHANNEL_1);
	mCurrentSpeed=0;
}

