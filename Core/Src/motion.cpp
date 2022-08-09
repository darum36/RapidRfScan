///*
// * motion.cpp
// *
// *  Created on: Aug 8, 2022
// *      Author: Kocherov
// */

#include <motion.h>
#include "math.h"

Motion::Motion(TIM_HandleTypeDef* PWMTim):
mPWMTim(PWMTim)
{}

void Motion::acceleration() 						// Ускорение
{

float deltaSpeed=mAcc/float(1000);
uint32_t currentPeriod;

if (!mMotorOn) {mCurrentSpeed=0;}
if (mSpeed+deltaSpeed<=mSpeed)																/* Пока скорость не достигла максимальной будем ускорятся */
{
	if (mMotorOn)																			/* Если мы двигались до этого, то увеличим скорость */
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
			currentPeriod=round(mCurrentSpeed);
			if (!currentPeriod) currentPeriod = 100000000;

			__HAL_TIM_SET_COMPARE(mPWMTim,TIM_CHANNEL_1,(currentPeriod)/2+1);     			/* Установка скважности Шима */
			__HAL_TIM_SET_AUTORELOAD(mPWMTim, currentPeriod);								/* Установка периода Шима */
			HAL_TIM_PWM_Start(mPWMTim, TIM_CHANNEL_1);								        /* Запуск мотора */
		}
	}
}
}

void Motion::deceleration()										/* Торможение */
{
	float deltaSpeed=mDcc/double(1000);
	uint32_t currentPeriod;

	if (mMotorOn)
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
			HAL_TIM_PWM_Stop(mPWMTim, TIM_CHANNEL_1);								/* Выключение мотора */
		}
	}
}

void Motion::moving (uint64_t movingSteps)										/* Равномерное движение */
{
	if (mMotorOn)
	{
		if (!mNeedMoving)
		{
			mMovingTime=movingSteps*1000;
			mMovingTime=mMovingTime/uint64_t(round(mCurrentSpeed));
			mNeedMoving=true;
		}
		(mMovingTime>0) ? mMovingTime-- :	mNeedMoving=false;
	}
}

void Motion::jogging()								/* Движение при нажатой кнопке */
{
float deltaSpeedAcc=mAcc/double(1000);
float deltaSpeedDcc=mDcc/double(1000);
uint32_t currentPeriod;

if (!mMotorOn) {mCurrentSpeed=0;}

if (mCurrentSpeed>0 || HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)==GPIO_PIN_RESET)																/* Пока скорость больше нуля или нажата кнопка */ 		/*!!!!!!!!!!!!! Необходимо вписать номер порта и пина для кнопки!!!!!!!!!!!!!!!!! */
{
	if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13)==GPIO_PIN_RESET)																		/* Если кнопка нажата */								/*!!!!!!!!!!!!! Необходимо вписать номер порта и пина для кнопки!!!!!!!!!!!!!!!!! */
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
		else																				/* Старт */
		{
			if (deltaSpeedAcc>=mSpeed)																/* Если шаг больше чем максимальная скорость, то */
			{
				mCurrentSpeed=mSpeed;																		/* скорость равна максимальной */
				currentPeriod=round(float(100000000)/mCurrentSpeed);

				__HAL_TIM_SET_COMPARE(mPWMTim,TIM_CHANNEL_1,currentPeriod/2+1);			/* Установка скважности Шима */
				__HAL_TIM_SET_AUTORELOAD(mPWMTim, currentPeriod);						/* Установка периода Ш�?Ма */
				HAL_TIM_PWM_Start(mPWMTim, TIM_CHANNEL_1);								/* Запуск мотора */
			}
			else																			/* Если шаг меньше чем максимальная скорость, то */
			{
				mCurrentSpeed=deltaSpeedAcc;																	/* Увеличиваем скорость на один шаг */
				currentPeriod=round(float(100000000)/mCurrentSpeed);

				__HAL_TIM_SET_COMPARE(mPWMTim,TIM_CHANNEL_1,currentPeriod/2+1);			/* Установка скважности Шима */
				__HAL_TIM_SET_AUTORELOAD(mPWMTim, currentPeriod);						/* Установка периода Ш�?Ма */
				HAL_TIM_PWM_Start(mPWMTim, TIM_CHANNEL_1);								/* Запуск мотора */
			}
		}
	}
	else																					/* Если кнопка не нажата и мы все ещё движемся */
	{
		if (mCurrentSpeed>deltaSpeedDcc)
		{
			mCurrentSpeed=mCurrentSpeed-deltaSpeedDcc;																		/* Замедляемся */
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

void Motion::setSpeed(float newSpeed)
{
	if (fabs(newSpeed) > 1000000)
	{
		newSpeed = newSpeed < 0 ? -1000000 : 1000000;
	}
	mSpeed = newSpeed;
}

float Motion::getSpeed()
{
	return mSpeed;
}
