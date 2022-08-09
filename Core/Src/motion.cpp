///*
// * motion.cpp
// *
// *  Created on: Aug 8, 2022
// *      Author: Kocherov
// */

#include <motion.h>

Motion::Motion(TIM_HandleTypeDef* PWMTim):
mPWMTim(PWMTim)
{

}

Motion::acceleration() 						// Ускорение
{
deltaSpeed=acc/1000;
flag=current_motor->Instance->CR1 & 1;
if (!flag) {V=0;}
	if (V+deltaV<=Vmax)																		/* Пока скорость не достигла максимальной будем ускорятся */
	{
		if (flag)																			/* Если мы двигались до этого, то увеличим скорость */
		{
			V=V+deltaV;																		/* Увеличение промежуточной скорости на один шаг */
			__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,(100000000/V)/2+1);			/* Установка скважности Шима */
			__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);							/* Установка периода Шима */
		}
		else																				/* Если мы не двигались до этого, то увеличим скорость и включим мотор */
		{
			if (deltaV>Vmax)																/* Если шаг ускорения больше чем максимальная скорость, то */
			{
				V=Vmax;																		/* Скорость сразу становится максимальной */
				__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,(100000000/V)/2+1);		/* Установка скважности Шима */
				__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);						/* Установка периода Шима */
				HAL_TIM_PWM_Start(current_motor, TIM_CHANNEL_1);								/* Запуск мотора */
			}
			else																			/* Если шаг ускорения меньше чем максимальная скорость, то */
			{
				V=deltaV;
				__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,(100000000/V)/2+1); 		/* Установка скважности Шима */
				__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);						/* Установка периода Шима */
				HAL_TIM_PWM_Start(current_motor, TIM_CHANNEL_1);								/* Запуск мотора */
			}
		}
	}
}

void Motion::Deceleration(uint32_t Dcc)										/* Торможение */
{
deltaV=Dcc/1000;
flag=current_motor->Instance->CR1 & 1;

if (Stop_flag)
{
	V=100000000/(current_motor->Instance->ARR);
	Stop_flag=!Stop_flag;
}

if (flag)
{
	if (V>deltaV)																	/* Если дальше тормозить некуда, то */
	{
		V=V-deltaV;																	/* Уменьшаем скорость на шаг */
		__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);						/* Установка периода Шима */
		__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,100000000/V/2+1);			/* Установка скважности Шима */
	}
	else
	{
		HAL_TIM_PWM_Stop(current_motor, TIM_CHANNEL_1);								/* Выключение мотора */
		Stop_flag=!Stop_flag;
	}
}
}

void Moving (uint32_t M_Steps)										/* Равномерное движение */
{
flag=current_motor->Instance->CR1 & 1;

if (flag)
{
	if (Moving_flag)
	{
		V=100000000/(mPWMTim->Instance->ARR);
		Moving_Time=M_Steps;
		Moving_Time=Moving_Time*1000;
		Moving_Time=Moving_Time/V;
		Moving_flag=!Moving_flag;
	}

	(Moving_Time>0) ? Moving_Time-- :	Moving_flag=!Moving_flag;
}
}

void jogging(uint32_t Vmax,uint32_t Acc)								/* Движение при нажатой кнопке */
{
deltaV=Acc/1000;


flag=current_motor->Instance->CR1 & 1;

if (!flag)
{
	V=0;
}

if (V>0 || HAL_GPIO_ReadPin(TRS_FINE_GPIO_Port, TRS_FINE_Pin)==GPIO_PIN_RESET)																/* Пока скорость больше нуля или нажата кнопка */ 		/*!!!!!!!!!!!!! Необходимо вписать номер порта и пина для кнопки!!!!!!!!!!!!!!!!! */
{
	if (HAL_GPIO_ReadPin(TRS_FINE_GPIO_Port, TRS_FINE_Pin)==GPIO_PIN_RESET)																		/* Если кнопка нажата */								/*!!!!!!!!!!!!! Необходимо вписать номер порта и пина для кнопки!!!!!!!!!!!!!!!!! */
	{
		if (V>0)																			/* Проверка стартуем ли мы, нужно ли включать моторы */
		{
			if (V<=Vmax)
			{
				V=V+deltaV;																	/* То ускоряемся */
				__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,100000000/V/2+1);			/* Установка скважности Шима */
				__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);						/* Установка периода Шима */
			}
			else
			{
				V=Vmax;																		/* То скорость равна максимальной */
				__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,100000000/V/2+1);			/* Установка скважности Шима */
				__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);						/* Установка периода Шима */
			}
		}
		else																				/* Старт */
		{
			if (deltaV>=Vmax)																/* Если шаг больше чем максимальная скорость, то */
			{
				V=Vmax;																		/* скорость равна максимальной */
				__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,100000000/V/2+1);			/* Установка скважности Шима */
				__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);						/* Установка периода Ш�?Ма */
				HAL_TIM_PWM_Start(current_motor, TIM_CHANNEL_1);								/* Запуск мотора */
			}
			else																			/* Если шаг меньше чем максимальная скорость, то */
			{
				V=deltaV;																	/* Увеличиваем скорость на один шаг */
				__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,100000000/V/2+1);			/* Установка скважности Шима */
				__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);						/* Установка периода Ш�?Ма */
				HAL_TIM_PWM_Start(current_motor, TIM_CHANNEL_1);								/* Запуск мотора */
			}
		}
	}
	else																					/* Если кнопка не нажата и мы все ещё движемся */
	{
		if (V>deltaV)
		{
			V=V-deltaV;																		/* Замедляемся */
			__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);							/* Установка периода Шима */
			__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,100000000/V/2+1);				/* Установка скважности Шима */
		}
		else
		{
			HAL_TIM_PWM_Stop(current_motor, TIM_CHANNEL_1);									/* Выключаем мотор */
			V=0;																			/* Необходимо чтобы выйти из цикла */
		}
	}
}
}

void direction(bool DIR)
{
		if (DIR)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
		}
}

Motion::setSpeed(float newSpeed)
{
	if (fabs(newSpeed) > 1000000)
	{
		newSpeed = newSpeed < 0 ? -1000000 : 1000000;
	}
	mSpeed = newSpeed;
}

