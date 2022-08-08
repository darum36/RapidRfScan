///*
// * motion.cpp
// *
// *  Created on: Jul 5, 2022
// *      Author: Student
// */
//
//#include <motion.h>
////#include "UART_Functions.h"
//
////Переменные
//extern ADC_HandleTypeDef hadc1;
//extern CRC_HandleTypeDef hcrc;
//extern TIM_HandleTypeDef htim1;
//extern TIM_HandleTypeDef htim2;
//extern TIM_HandleTypeDef htim5;
//extern TIM_HandleTypeDef htim10;
//extern UART_HandleTypeDef huart1;
//
//int Check=1;
//bool flag;
//bool Stop_flag=1;
//bool Moving_flag=1;
//bool PtP_Moving_Mode;
//bool PtP_Done=0;
//bool Able_Moving;
//uint64_t Moving_Time;
//uint64_t Steps_deltaV;
//uint64_t Moving_Steps;
//TIM_HandleTypeDef* current_motor;
//uint32_t deltaV;
//uint32_t V;
//extern bool motor_motion;
//
//class Motor
//{
//private:
//public:
//	Motor(int motor)
//		{
//			switch (motor)
//			{
//			case 1:
//				current_motor = &htim2;
//				break;
//			case 2:
//				current_motor = &htim5;
//				break;
//			default:
//				break;
//			}
//		}
//	void Acceleration(uint32_t Vmax,uint32_t Acc) 						// Ускорение
//	{
//		deltaV=Acc/1000;
//		flag=current_motor->Instance->CR1 & 1;
//		if (!flag)
//		{
//			V=0;
//		}
//
//		if (V+deltaV<=Vmax)																		/* Пока скорость не достигла максимальной будем ускорятся */
//		{
//			if (flag)																			/* Если мы двигались до этого, то увеличим скорость */
//			{
//				V=V+deltaV;																		/* Увеличение промежуточной скорости на один шаг */
//				__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,(100000000/V)/2+1);			/* Установка скважности Шима */
//				__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);							/* Установка периода Шима */
//			}
//			else																				/* Если мы не двигались до этого, то увеличим скорость и включим мотор */
//			{
//				if (deltaV>Vmax)																/* Если шаг ускорения больше чем максимальная скорость, то */
//				{
//					V=Vmax;																		/* Скорость сразу становится максимальной */
//					__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,(100000000/V)/2+1);		/* Установка скважности Шима */
//					__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);						/* Установка периода Шима */
//					HAL_TIM_PWM_Start(current_motor, TIM_CHANNEL_1);								/* Запуск мотора */
//				}
//				else																			/* Если шаг ускорения меньше чем максимальная скорость, то */
//				{
//					V=deltaV;
//					__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,(100000000/V)/2+1); 		/* Установка скважности Шима */
//					__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);						/* Установка периода Шима */
//					HAL_TIM_PWM_Start(current_motor, TIM_CHANNEL_1);								/* Запуск мотора */
//				}
//			}
//		}
//		else
//		{
//			Check++;
//		}
//	}
//	void Deceleration(uint32_t Dcc)										/* Торможение */
//	{
//		deltaV=Dcc/1000;
//		flag=current_motor->Instance->CR1 & 1;
//
//		if (Stop_flag)
//		{
//			V=100000000/(current_motor->Instance->ARR);
//			Stop_flag=!Stop_flag;
//		}
//
//		if (flag)
//		{
//			if (V>deltaV)																	/* Если дальше тормозить некуда, то */
//			{
//				V=V-deltaV;																	/* Уменьшаем скорость на шаг */
//				__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);						/* Установка периода Шима */
//				__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,100000000/V/2+1);			/* Установка скважности Шима */
//			}
//			else
//			{
//				HAL_TIM_PWM_Stop(current_motor, TIM_CHANNEL_1);								/* Выключение мотора */
//				Stop_flag=!Stop_flag;
//				Check=1;
//			}
//		}
//		else
//		{
//			Check=1;
//		}
//	}
//	void Moving (uint32_t M_Steps)										/* Равномерное движение */
//	{
//		flag=current_motor->Instance->CR1 & 1;
//
//		if (flag)
//		{
//			if (Moving_flag)
//			{
//				V=100000000/(current_motor->Instance->ARR);
//				Moving_Time=M_Steps;
//				Moving_Time=Moving_Time*1000;
//				Moving_Time=Moving_Time/V;
//				Moving_flag=!Moving_flag;
//			}
//
//			if (Moving_Time>0)														/* Если Шим включен */
//			{
//				Moving_Time--;
//			}
//			else
//			{
//				Moving_flag=!Moving_flag;
//				Check++;
//			}
//		}
//		else
//		{
//			Check++;
//		}
//	}
//	void Jogging(uint32_t Vmax,uint32_t Acc)								/* Движение при нажатой кнопке */
//	{
//		deltaV=Acc/1000;
//
//
//		flag=current_motor->Instance->CR1 & 1;
//
//		if (!flag)
//		{
//			V=0;
//		}
//
//		if (V>0 || HAL_GPIO_ReadPin(TRS_FINE_GPIO_Port, TRS_FINE_Pin)==GPIO_PIN_RESET)																/* Пока скорость больше нуля или нажата кнопка */ 		/*!!!!!!!!!!!!! Необходимо вписать номер порта и пина для кнопки!!!!!!!!!!!!!!!!! */
//		{
//			if (HAL_GPIO_ReadPin(TRS_FINE_GPIO_Port, TRS_FINE_Pin)==GPIO_PIN_RESET)																		/* Если кнопка нажата */								/*!!!!!!!!!!!!! Необходимо вписать номер порта и пина для кнопки!!!!!!!!!!!!!!!!! */
//			{
//				if (V>0)																			/* Проверка стартуем ли мы, нужно ли включать моторы */
//				{
//					if (V<=Vmax)
//					{
//						V=V+deltaV;																	/* То ускоряемся */
//						__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,100000000/V/2+1);			/* Установка скважности Шима */
//						__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);						/* Установка периода Шима */
//					}
//					else
//					{
//						V=Vmax;																		/* То скорость равна максимальной */
//						__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,100000000/V/2+1);			/* Установка скважности Шима */
//						__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);						/* Установка периода Шима */
//					}
//				}
//				else																				/* Старт */
//				{
//					if (deltaV>=Vmax)																/* Если шаг больше чем максимальная скорость, то */
//					{
//						V=Vmax;																		/* скорость равна максимальной */
//						__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,100000000/V/2+1);			/* Установка скважности Шима */
//						__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);						/* Установка периода Ш�?Ма */
//						HAL_TIM_PWM_Start(current_motor, TIM_CHANNEL_1);								/* Запуск мотора */
//					}
//					else																			/* Если шаг меньше чем максимальная скорость, то */
//					{
//						V=deltaV;																	/* Увеличиваем скорость на один шаг */
//						__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,100000000/V/2+1);			/* Установка скважности Шима */
//						__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);						/* Установка периода Ш�?Ма */
//						HAL_TIM_PWM_Start(current_motor, TIM_CHANNEL_1);								/* Запуск мотора */
//					}
//				}
//			}
//			else																					/* Если кнопка не нажата и мы все ещё движемся */
//			{
//				if (V>deltaV)
//				{
//					V=V-deltaV;																		/* Замедляемся */
//					__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);							/* Установка периода Шима */
//					__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,100000000/V/2+1);				/* Установка скважности Шима */
//				}
//				else
//				{
//					HAL_TIM_PWM_Stop(current_motor, TIM_CHANNEL_1);									/* Выключаем мотор */
//					V=0;																			/* Необходимо чтобы выйти из цикла */
//				}
//			}
//		}
//	}
//	void Point_to_Point(uint32_t Vmax, uint32_t Acc, uint64_t Steps)		/* Движение на заданное количество шагов */
//	{
//		flag=current_motor->Instance->CR1 & 1;
//
//
//		if (!flag)
//		{
//			deltaV=Acc/1000;
//			V=0;
//			Steps_deltaV=deltaV+Vmax;
//			Steps_deltaV=Steps_deltaV*Vmax;
//			Steps_deltaV = Steps_deltaV/Acc;
//
//			if (Steps<Steps_deltaV)
//			{
//				V=1000;
//				Moving_Time=Steps;
//				Moving_Time=Moving_Time*1000;
//				Moving_Time=Moving_Time/V;
//				__HAL_TIM_SET_COMPARE(current_motor,TIM_CHANNEL_1,100000000/V/2+1);
//				__HAL_TIM_SET_AUTORELOAD(current_motor, 100000000/V);
//				HAL_TIM_PWM_Start(current_motor, TIM_CHANNEL_1);
//				PtP_Moving_Mode=1;
//			}
//			else
//			{
//				PtP_Moving_Mode=0;
//				Moving_Steps=Steps-Steps_deltaV;
//				Moving_Steps=Moving_Steps;
//			}
//		}
//
//			if (PtP_Moving_Mode)
//			{
//				if (Moving_Time>0)
//				{
//					Moving_Time--;
//				}
//				else
//				{
//					HAL_TIM_PWM_Stop(current_motor, TIM_CHANNEL_1);
//					PtP_Done=!PtP_Done;
//					motor_motion = 0;
//				}
//			}
//			else
//			{
//				switch (Check)
//				{
//				case 1:
//					Acceleration(Vmax,Acc);
//				break;
//
//				case 2:
//					Moving(Moving_Steps);
//				break;
//
//				case 3:
//					Deceleration(Acc);
//					if (!(current_motor->Instance->CR1 & 1))
//					{
//						PtP_Done=!PtP_Done;
//						motor_motion = 0;
//					}
//				break;
//				}
//			}
//	}
//	void Check_Limits (void)														// Проверка на срабатывание концевиков
//	{
//		bool Lim_P1;//Lim_M1,Lim_P2,Lim_M2,Lim_H1,Lim_H2;
//
//		//Lim_P1=HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_0)==GPIO_PIN_RESET;
//		Lim_P1=HAL_GPIO_ReadPin(TRS_FINE_GPIO_Port, TRS_FINE_Pin)==GPIO_PIN_RESET;
//		//Lim_M1=HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1)==GPIO_PIN_RESET;
//
//		//Lim_P2=HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_3)==GPIO_PIN_RESET;
//		//Lim_M2=HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)==GPIO_PIN_RESET;
//
//		//Lim_H1=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_10)==GPIO_PIN_RESET;
//		//Lim_H2=HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12)==GPIO_PIN_RESET;
//
//		if (Lim_P1)// || Lim_M1 || Lim_P2 ||Lim_M2)
//		{
//			HAL_TIM_PWM_Stop(&htim2, TIM_CHANNEL_1);
//			HAL_TIM_PWM_Stop(&htim5, TIM_CHANNEL_1);
//			Able_Moving=0;
//		}
//		else
//		{
//			Able_Moving=1;
//		}
//	}
//	void Direction(int motor,bool DIR)
//	{
//		switch (motor)
//		{
//		case 1:
//			if (DIR)
//			{
//				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
//			}
//			else
//			{
//				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
//			}
//		break;
//
//		case 2:
//			if (DIR)
//			{
//				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
//			}
//			else
//			{
//				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
//			}
//		break;
//
//		default:
//			return;
//		break;
//		}
//	}
//
//};
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
