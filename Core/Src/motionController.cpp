/*
 * motionController.cpp
 *
 *  Created on: 10 авг. 2022 г.
 *      Author: Kocherov I.O.
 */

#include "motionController.h"
#include "uart.h"

std::array<Axis, 4> axis =
{
	Axis(),
	Axis(),
	Axis(),
    Axis()
};

Remote remoteController1;

TIM_HandleTypeDef htim10;
ADC_HandleTypeDef hadc1;
extern Packet uartPacket;
extern bool wrongBoardID;
uint8_t boardID;
bool emgStatus = HAL_GPIO_ReadPin(Emergency_status_GPIO_Port, Emergency_status_Pin);

void startMotion()
{
	initMotion();
	initUart();

	while (1)
	{
		checkUARTbuffer();
	if (boardID == 1)
	{
		for (unsigned int i = 0; i < 2; i++)
		{ axis[i].checkLimits();
		  axis[i].updateEnc(); }
	}
	else if (boardID == 2)
	{
		for (unsigned int i = 2; i < 4; i++)
		{ axis[i].checkLimits();
		  axis[i].updateEnc(); }
	}

		checkEmgStatus();
		remoteController1.checkRemoteStatus();
	}
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	if (remoteController1.getOnOffStatus() == true)
	{ RemoteMoving(); }
	else
	{ SoftMoving(); }

	HAL_TIM_IRQHandler(&htim10);
}

void RemoteMoving()
{
	bool letsGO = false;
	int rcCurrentAxis = remoteController1.getAxisStatus();

	if (((rcCurrentAxis == 0) || (rcCurrentAxis == 1)) && (boardID == 1))
		{ letsGO = true;}
	else if (((rcCurrentAxis == 2) || (rcCurrentAxis == 3)) && (boardID == 2))
		{ letsGO = true;}

	if (letsGO == true)
	{
		int rcNewSpeed = remoteController1.getSpeedStatus();

			if ((axis[rcCurrentAxis].checkAbleMoving() == false)||(emgStatus == true))
				{ axis[rcCurrentAxis].emgStop(); }
				else
				{
					if ((remoteController1.getRunStatus() == true) && (rcNewSpeed != 0))
					{
						if (remoteController1.getFineStatus() == true)
						{
							axis[rcCurrentAxis].tempSetParam(100000, 1000000, 1000000);
							if (rcNewSpeed > 0)
							{ 	axis[rcCurrentAxis].setDirection(eDirection::Positive);
								axis[rcCurrentAxis].jogging(1);
							}
							else if (rcNewSpeed < 0)
							{ 	axis[rcCurrentAxis].setDirection(eDirection::Negative);
								axis[rcCurrentAxis].jogging(1);
							}
						}
						else
						{
							axis[rcCurrentAxis].tempSetParam(float(abs(rcNewSpeed)*700), 1000000, 1000000);
							if (rcNewSpeed > 0)
							{ 	axis[rcCurrentAxis].setDirection(eDirection::Positive);
								axis[rcCurrentAxis].jogging(1); }
							else if (rcNewSpeed < 0)
							{ 	axis[rcCurrentAxis].setDirection(eDirection::Negative);
								axis[rcCurrentAxis].jogging(1);
							}
						}
					}
					else
					{ axis[rcCurrentAxis].emgStop(); }
				}
	}
}

void SoftMoving()
{
	if (wrongBoardID == false)
	{
		if (boardID == 1)
		{
			for (unsigned int i = 0; i < 2; i++)
			{
				if ((axis[i].checkAbleMoving() == false) || (emgStatus == true))
				{ axis[i].emgStop(); }
				else
				{
					if (axis[i].modeMoving() == 0)
					{ }
					else if (axis[i].modeMoving() == 1)
					{ axis[i].jogging(axis[i].beginMoving()); }
				}
			}
		}
		else if (boardID == 2)
				{
					for (unsigned int i = 2; i < 4; i++)
					{
						if ((axis[i].checkAbleMoving() == false) || (emgStatus == true))
						{ axis[i].emgStop(); }
						else
						{
							if (axis[i].modeMoving() == 0)
							{ }
							else if (axis[i].modeMoving() == 1)
							{ axis[i].jogging(axis[i].beginMoving()); }
						}
					}
				}
	}
}
//		if ((axis[uartPacket.iAxis].checkAbleMoving() == false) || (emgStatus == true))
//		{ axis[uartPacket.iAxis].emgStop(); }
//		else
//		{
//			if (axis[uartPacket.iAxis].modeMoving() == 0)
//			{
//		//			axis[uartPacket.iAxis].ptp(eDirection::Positive);
//			}
//
//			else if (axis[uartPacket.iAxis].modeMoving() == 1)
//			{ axis[uartPacket.iAxis].jogging(); }
//		}
//	}
//}

void checkEmgStatus()
{
	bool newEmgStatus = (HAL_GPIO_ReadPin(Emergency_status_GPIO_Port, Emergency_status_Pin) == true) ? true : false;
	emgStatus=newEmgStatus;
}

void initMotion()
{
	boardID = (HAL_GPIO_ReadPin(BOARD_ID_GPIO_Port, BOARD_ID_Pin) == GPIO_PIN_SET) ? 1 : 2;
	int8_t axisIndex = (boardID == 1) ? 0 : 2;

	axis[axisIndex].init(TIM2,
			     STEP1_GPIO_Port, STEP1_Pin, GPIO_AF1_TIM2,   		// PWM
			     TIM1,												// TIM ENC
			     ENC_A1_GPIO_Port, ENC_A1_Pin, GPIO_AF1_TIM1, 	    // TIM ENCA
			     ENC_B1_GPIO_Port, ENC_B1_Pin, GPIO_AF1_TIM1,		// TIM ENCB																              // GPIO DIR
			     LIM_1_GPIO_Port, LIM_1_Pin,						// GPIOs LIMIT
			     LIM_1C1_GPIO_Port, LIM_1C1_Pin,                    // GPIOs LIMIT
			     LIM_HOME_1_GPIO_Port, LIM_HOME_1_Pin,		        // GPIOs LIMIT
			     DIR1_GPIO_Port, DIR1_Pin);							// GPIOs DIR

	axis[axisIndex].setTurnOnAxis(1);
	axis[axisIndex].setInverseLim(7);
	axis[axisIndex].setEnableLim(7);
	axis[axisIndex].setInverseMotor(0);
	axis[axisIndex].setInverseEncoder(0);


	axis[axisIndex + 1].init(TIM5,
				 STEP2_GPIO_Port, STEP2_Pin, GPIO_AF2_TIM5,			// PWM
				 TIM3,												// TIM ENC
				 ENC_A2_GPIO_Port, ENC_A2_Pin, GPIO_AF1_TIM1, 	    // TIM ENCA
				 ENC_B2_GPIO_Port, ENC_B2_Pin, GPIO_AF1_TIM1,		// TIM ENCB																              // GPIO DIR
				 LIM_2_GPIO_Port, LIM_2_Pin,						// GPIOs LIMIT
				 LIM_2C3_GPIO_Port, LIM_2C3_Pin,                    // GPIOs LIMIT
				 LIM_HOME_2_GPIO_Port, LIM_HOME_2_Pin,		        // GPIOs LIMIT
				 DIR2_GPIO_Port, DIR2_Pin);							// GPIOs DIR

	remoteController1.init(hadc1, SPEED_GPIO_Port, SPEED_Pin,
						   ON_OFF_GPIO_Port, ON_OFF_Pin,
						   RUN_GPIO_Port, RUN_Pin,
						   TRS_FINE_GPIO_Port, TRS_FINE_Pin,
						   AXIS0_GPIO_Port, AXIS0_Pin,
						   AXIS1_GPIO_Port, AXIS1_Pin,
						   AXIS2_GPIO_Port, AXIS2_Pin);

	axis[axisIndex + 1].setTurnOnAxis(1);
	axis[axisIndex + 1].setInverseLim(0);
	axis[axisIndex + 1].setEnableLim(7);
	axis[axisIndex + 1].setInverseMotor(0);
	axis[axisIndex + 1].setInverseEncoder(0);
}
