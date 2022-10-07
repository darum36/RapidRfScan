/*
 * motionController.cpp
 *
 *  Created on: 10 авг. 2022 г.
 *      Author: Kocherov I.O.
 */

#include "motionController.h"

std::array<Axis, 2> axis =
{
	Axis(),
    Axis()
};

Remote remoteController1;

TIM_HandleTypeDef htim10;
ADC_HandleTypeDef hadc1;

bool boardID = 0;
short CurrentAxis = remoteController1.getAxisStatus();
bool emgStatus = HAL_GPIO_ReadPin(Emergency_status_GPIO_Port, Emergency_status_Pin);

void startMotion()
{
	initMotion();
	initUart();

	while (1)
	{
		checkUARTbuffer();
	for (unsigned int i=0; i<axis.size();i++)
		{ axis[i].checkLimits(); }
		checkEmgStatus();
		remoteController1.checkRemoteStatus();
	}
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	if ((CurrentAxis == 1 || CurrentAxis == 0) && (boardID == 0))
	{
			if ((axis[CurrentAxis].checkAbleMoving() == false)||(emgStatus == true))
			{
				axis[CurrentAxis].emgStop();
			}
			else
			{
				int newSpeed = remoteController1.getSpeedStatus();

				if ((remoteController1.getRunStatus() == true) && (newSpeed != 0))
				{
					if (remoteController1.getFineStatus() == true)
					{
						axis[CurrentAxis].tempSetParam(100000, 1000000, 1000000);
						if (newSpeed > 0)
						{
							axis[CurrentAxis].jogging(eDirection::Positive);
						}
						else if (newSpeed < 0)
						{
							axis[CurrentAxis].jogging(eDirection::Negative);
						}
					}
					else
					{
						axis[CurrentAxis].tempSetParam(float(abs(newSpeed)*700), 1000000, 1000000);
						if (newSpeed > 0)
						{
							axis[CurrentAxis].jogging(eDirection::Positive);
						}
						else if (newSpeed < 0)
						{
							axis[CurrentAxis].jogging(eDirection::Negative);
						}
					}
				}
				else
				{
					axis[CurrentAxis].emgStop();
				}
			}
		}
	HAL_TIM_IRQHandler(&htim10);
}

void checkEmgStatus()
{
	bool newEmgStatus = HAL_GPIO_ReadPin(Emergency_status_GPIO_Port, Emergency_status_Pin);
	emgStatus=newEmgStatus;
}

void initMotion()
{
	axis[0].init(TIM2,
			     STEP1_GPIO_Port, STEP1_Pin, GPIO_AF1_TIM2,   		// PWM
			     TIM1,												// TIM ENC
			     ENC_A1_GPIO_Port, ENC_A1_Pin, GPIO_AF1_TIM1, 	    // TIM ENCA
			     ENC_B1_GPIO_Port, ENC_B1_Pin, GPIO_AF1_TIM1,		// TIM ENCB																              // GPIO DIR
			     LIM_1_GPIO_Port, LIM_1_Pin,						// GPIOs LIMIT
			     LIM_1C1_GPIO_Port, LIM_1C1_Pin,                    // GPIOs LIMIT
			     LIM_HOME_1_GPIO_Port, LIM_HOME_1_Pin,		        // GPIOs LIMIT
			     DIR1_GPIO_Port, DIR1_Pin);							// GPIOs DIR

	axis[0].setInverseLim();

	axis[1].init(TIM5,
				 STEP2_GPIO_Port, STEP2_Pin, GPIO_AF2_TIM5,			// PWM
				 TIM3,												// TIM ENC
				 ENC_A2_GPIO_Port, ENC_A2_Pin, GPIO_AF1_TIM1, 	    // TIM ENCA
				 ENC_B2_GPIO_Port, ENC_B2_Pin, GPIO_AF1_TIM1,		// TIM ENCB																              // GPIO DIR
				 LIM_2_GPIO_Port, LIM_2_Pin,						// GPIOs LIMIT
				 LIM_2C3_GPIO_Port, LIM_2C3_Pin,                    // GPIOs LIMIT
				 LIM_HOME_2_GPIO_Port, LIM_HOME_2_Pin,		        // GPIOs LIMIT
				 DIR2_GPIO_Port, DIR2_Pin);							// GPIOs DIR

//	axis[1].setInverseLim();

	remoteController1.init(hadc1, SPEED_GPIO_Port, SPEED_Pin,
						   ON_OFF_GPIO_Port, ON_OFF_Pin,
						   RUN_GPIO_Port, RUN_Pin,
						   TRS_FINE_GPIO_Port, TRS_FINE_Pin,
						   AXIS0_GPIO_Port, AXIS0_Pin,
						   AXIS1_GPIO_Port, AXIS1_Pin,
						   AXIS2_GPIO_Port, AXIS2_Pin);

	boardID = HAL_GPIO_ReadPin(BOARD_ID_GPIO_Port, BOARD_ID_Pin);
}
