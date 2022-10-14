/*
 * uart.cpp
 *
 *  Created on: 4 окт. 2022 г.
 *      Author: kio
 */

#include "uart.h"
#include "motionController.h"

extern Axis axis[4];

UART_HandleTypeDef huart1;
RingBuffer <1024> uartBuffer;
TIM_HandleTypeDef timeoutTim;
extern uint8_t boardID;

unsigned char currentCommandByte = 0;
volatile bool getValue = false;
volatile bool RunCommand = false;
volatile bool getAxis = false;
char commandBuf[2];
char valueBuf[9];
uint8_t commandLenght = 0;
uint8_t valueLenght = 0;
int32_t commandValue = 0;
char currentAxis;
char TEMP[10];

bool timeout = false;
bool checkingTimeError = false;
bool error = false;
bool readStatus = false;

Packet uartPacket;

constexpr char BOARD_ID[] = "BID";
//constexpr char SWTICH_ON_AXIS[] = "MO";
constexpr char MODE_MOTION[] = "MM";
constexpr char SPEED[] = "SP";
constexpr char ACCELERATION[] = "AC";
constexpr char DECCELERATION[] = "DC";
constexpr char CURRENT_POSITION[] = "PS";
//constexpr char SET_ABSOLUTE_POSITION[] = "AP";
//constexpr char STOP_INTERVAL_WITH_FEEDBACK[] = "SW";
constexpr char MOTOR_IMPULSE_PER_SI_UNIT[] = "SC";
constexpr char ENCODER_COUNTS_PER_SI_UNIT[] = "EC";
//constexpr char FEEDBACK[] = "NC";
//constexpr char MAX_ERROR_POSITION[] = "ER";
//constexpr char INVERSE_ENCODER[] = "IE";
//constexpr char INVERSE_MOTOR_ROTATION[] = "IM";
//constexpr char LIMIT_TRIGGER_LOGIC[] = "LP";
//constexpr char LIMIT_ENABLE[] = "LE";
constexpr char LIMIT_STATUS[] = "LS";
//constexpr char ENABLE_COMPARE_FUNCTION[] = "PG[0]";
//constexpr char COMPARE_MODE[] = "PG[1]";
//constexpr char SPACE_OR_COMPARE[] = "PG[2]";
//constexpr char COMPARE_FUNCTION_FIRST_POINT[] = "PG[3]";
//constexpr char COMPARE_FUNCTION_LAST_POINT[] = "PG[4]";
//constexpr char COMPARE_FUNCTION_POINT_ARRAY[] = "AR[100_000]";
//constexpr char PWM_FREQUENCY[] = "SF";
//constexpr char MOTOR_CONTROL_FAULT[] = "MF";
//constexpr char MOTION_STATUS[] = "MS";
//constexpr char POSITION_ERROR[] = "PE";
//constexpr char CURRENT_VELOCITY[] = "IV";
//constexpr char CURRENT_ACCELERATION[] = "IA";
//constexpr char CURRENT_DECELERATION[] = "ID";
constexpr char BEGIN_MOVING[] = "BG";
constexpr char STOP_MOVING[] = "ST";
//constexpr char EMERGENCY_STOP[] = "AB";
//constexpr char FIND_HOME[] = "FH";

//constexpr char HIGH_SOFTWARE_LIMIT[] = "HL";
//constexpr char LOW_SOFTWARE_LIMIT[] = "LL";
//constexpr char MOTOR_STATUS[] = "MO";
//constexpr char EMERGENCY_STOP_CAUSE[] = "EM";

void initUart()
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.OverSampling = UART_OVERSAMPLING_16;

		if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}

	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	HAL_NVIC_SetPriority(USART1_IRQn, 2, 0);
	HAL_NVIC_EnableIRQ(USART1_IRQn);

	__HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);

	  RingBuffer <1024> uartBuffer;
	  uartBuffer.Clear();
}

void USART1_IRQHandler(void)
{
	if((__HAL_UART_GET_FLAG(&(huart1), UART_FLAG_RXNE)))
		{
			uint8_t Recive = (USART1 -> DR) & 0xFF;
			uartBuffer.Write(Recive);
			__HAL_UART_CLEAR_FLAG(&(huart1), UART_FLAG_RXNE);
		}
		__HAL_UART_CLEAR_FLAG(&(huart1), UART_FLAG_RXNE);
		HAL_UART_IRQHandler(&huart1);
}

void handlerPacket ()
{

}

void checkUARTbuffer()
{
	if ((!uartBuffer.IsEmpty()) && (!uartPacket.getEndPacket))
	{
		uartBuffer.Read(currentCommandByte);
	  	if (currentCommandByte != '\n')
	  	{
	  		if (uartPacket.getValue)
	  		{
	  			if (currentCommandByte < '0' || currentCommandByte > '9')
	  			{
	  				error = true;
	  			  	Send_String(&huart1,(char *)("Error Value"));
	  			}

	  			valueBuf[valueLenght] = currentCommandByte;
	  			valueLenght++;
	  		}

	  		if ((currentCommandByte != '=') && (!uartPacket.getValue))
	  		{
	  			if (!uartPacket.getAxis)
	  			{
	  				uartPacket.cAxis = currentCommandByte;
	  				uartPacket.getAxis = true;
	  			}

	  			else
	  			{
	  				if (commandLenght > 2)
	  				{
	  					error = true;
	  					Send_String(&huart1,(char *)("Miss ="));
	  				}

	  				commandBuf[commandLenght] = currentCommandByte;
	  				commandLenght++;
	  			}
	  		}

	  		else
	  		{
	  			uartPacket.getValue = true;
	  			commandValue = false;
	  		}

	  		checkingTimeError = true;
	  	}

	  	else
	  	{
	  		uartPacket.getEndPacket = true;
	  		checkingTimeError = false;
	  	}
	}
//--------------------------------------------------------------
	if (uartPacket.getEndPacket)
	{
		uartPacket.command[0] = commandBuf[0];
		uartPacket.command[1] = commandBuf[1];
//--------------------------------------------------------------
		switch (uartPacket.cAxis)
		{
			case 'X':	uartPacket.iAxis = 0; 	break;
			case 'Y':	uartPacket.iAxis = 1;	break;
			case 'Z':	uartPacket.iAxis = 2;	break;
			case 'W':	uartPacket.iAxis = 3;	break;
			case 'E':	uartPacket.iAxis = 4;	break;
			case 'F':	uartPacket.iAxis = 5;	break;
			case 'G':	uartPacket.iAxis = 6;	break;
			case 'H':	uartPacket.iAxis = 7;	break;
			case 'U':	uartPacket.iAxis = 8;	break;
			case 'V':	uartPacket.iAxis = 9;	break;

			case 'B':	uartPacket.command[2] = uartPacket.command[1];
						uartPacket.command[1] = uartPacket.command[0];
						uartPacket.command[0] = 'B';

			default:	error = true;
		}
//--------------------------------------------------------------
		if ((uartPacket.getValue) && ((strncmp(uartPacket.command,MOTOR_IMPULSE_PER_SI_UNIT, 0x2) == 0)
								  ||(strncmp(uartPacket.command,ENCODER_COUNTS_PER_SI_UNIT, 0x2) == 0)))
		{
			int pointLocation = 0;
			uartPacket.dValue = double(0);

			for (int i = valueLenght; i > 0; i--)
			{
				if (valueBuf[i-1] != '.') { pointLocation++; }
			}

			int j=10;
			for (int i = (valueLenght-pointLocation); i > valueLenght; i++)
			{
				uartPacket.dValue = uartPacket.dValue + ((valueBuf[i] - '0')/j);
				j*=10;
			}

			j=1;
			for (int i = (valueLenght-pointLocation-2); i > 0; i--)
			{
				uartPacket.dValue = uartPacket.dValue + ((valueBuf[i] - '0')*j);
				j*=10;
			}
		}
		else if (uartPacket.getValue)
	  	{
			int j = 1;
			uartPacket.iValue = 0;
			for (int i = valueLenght; i > 0; i--)
			{
				uartPacket.iValue = uartPacket.iValue + ((valueBuf[i-1] - '0')*j);
				j*= 10;
			}
	  	}
//--------------------------------------------
					if (strncmp(uartPacket.command,BOARD_ID, 0x3) == 0)
					{
						Send_String(&huart1,(char *)("BID="));
						Send_String(&huart1,(char *)(__utoa(boardID, TEMP, 10)));
						Send_String(&huart1,(char *)("\n"));
					}
//--------------------------------------------
					else if (strncmp(uartPacket.command,MODE_MOTION, 0x2) == 0)
	  	    		{
	  	    			if (uartPacket.getValue)
	  	    			{
	  	    				(uartPacket.iValue >= 1) ? (uartPacket.iValue = 1) : (uartPacket.iValue = 0);
	  	    				axis[uartPacket.iAxis].setModeMoving(uartPacket.iValue);
	  	    			}

	  	    			Send_Char(&huart1,&uartPacket.cAxis);
	  	    			Send_String(&huart1,(char *)("MM="));
	  	    			Send_String(&huart1,(char *)(__utoa(axis[uartPacket.iAxis].modeMoving(), TEMP, 10)));
	  	    			Send_String(&huart1,(char *)("\n"));
	  	    		}
//--------------------------------------------
	  	    		else if (strncmp(uartPacket.command,SPEED, 0x2) == 0)
	  	    		{
	  	    			if (uartPacket.getValue)
	  	    			{
							if (uartPacket.iValue >= 0)
							{	axis[uartPacket.iAxis].setDirection(eDirection::Positive);
								if (uartPacket.iValue >= 30000000) {uartPacket.iValue = 30000000;} 	}
							else
							{	axis[uartPacket.iAxis].setDirection(eDirection::Negative);
								if (uartPacket.iValue <= -30000000) {uartPacket.iValue = -30000000;}	}

								int32_t absIntValue = abs(uartPacket.iValue);
								axis[uartPacket.iAxis].setSpeed(absIntValue);
	  	    			}

						Send_Char(&huart1,&uartPacket.cAxis);
		  	    		Send_String(&huart1,(char *)("SP="));
		  	    		Send_String(&huart1,(char *)(__utoa(axis[uartPacket.iAxis].getSpeed(), TEMP, 10)));
		  	    		Send_String(&huart1,(char *)("\n"));
	  	    		}
//--------------------------------------------
	  	    		else if (strncmp(uartPacket.command,ACCELERATION, 0x2) == 0)
	  	    		{
	  	    			if (uartPacket.getValue)
	  	    			{
							if (uartPacket.iValue >= 120000000) {uartPacket.iValue = 120000000;}
							else if (uartPacket.iValue <= 0) {uartPacket.iValue = 0;}
							axis[uartPacket.iAxis].setAcc(uartPacket.iValue);
	  	    			}

	  	    			Send_Char(&huart1,&uartPacket.cAxis);
		  	    		Send_String(&huart1,(char *)("AC="));
		  	    		Send_String(&huart1,(char *)(__utoa(axis[uartPacket.iAxis].getAcc(), TEMP, 10)));
		  	    		Send_String(&huart1,(char *)("\n"));
	  	    		}
//--------------------------------------------
	  	    		else if (strncmp(uartPacket.command,DECCELERATION, 0x2) == 0)
	  	    		{
	  	    			if (uartPacket.getValue)
	  	    			{
	  	    				if (uartPacket.iValue >= 120000000) {uartPacket.iValue = 120000000;}
	  	    				else if (uartPacket.iValue <= 0) {uartPacket.iValue = 0;}
	  	    				axis[uartPacket.iAxis].setDcc(uartPacket.iValue);
	  	    			}

	  	    			Send_Char(&huart1,&uartPacket.cAxis);
		  	    		Send_String(&huart1,(char *)("DC="));
		  	    		Send_String(&huart1,(char *)(__utoa(axis[uartPacket.iAxis].getDcc(), TEMP, 10)));
		  	    		Send_String(&huart1,(char *)("\n"));
	  	    		}
//--------------------------------------------
	  	    		else if (strncmp(uartPacket.command,BEGIN_MOVING, 0x2) == 0)
	  	    		{
	  	    			if (uartPacket.getValue == false)
	  	    			{ axis[uartPacket.iAxis].begin(); }
	  	    			else
	  	    			{ Send_String(&huart1,(char *)("ERR\n")); }
	  	    		}
//--------------------------------------------
	  	    		else if (strncmp(uartPacket.command,STOP_MOVING, 0x2) == 0)
	  	    		{
	  	    			if (uartPacket.getValue == false)
	  	    			{ axis[uartPacket.iAxis].stop(); }
	  	    			else
	  	    			{ Send_String(&huart1,(char *)("ERR\n")); }
	  	    		}
//--------------------------------------------
					else if (strncmp(uartPacket.command,CURRENT_POSITION, 0x2) == 0)
					{
						if (uartPacket.getValue == true)
						{ axis[uartPacket.iAxis].setPosition(uartPacket.iValue); }

						Send_Char(&huart1,&uartPacket.cAxis);
					    Send_String(&huart1,(char *)("PS="));
					    Send_String(&huart1,(char *)(__utoa(axis[uartPacket.iAxis].getPosition(), TEMP, 10)));
					    Send_String(&huart1,(char *)("\n"));
					}
//--------------------------------------------
					else if (strncmp(uartPacket.command,LIMIT_STATUS, 0x2) == 0)
					{
						if (uartPacket.getValue == false)
						{ Send_Char(&huart1,&uartPacket.cAxis);
		  	    		  Send_String(&huart1,(char *)("LS="));
		  	    		  Send_String(&huart1,(char *)(__utoa(axis[uartPacket.iAxis].getLimStatus(), TEMP, 10)));
		  	    		  Send_String(&huart1,(char *)("\n")); }
						else
						{ Send_String(&huart1,(char *)("ERR\n")); }
					}
//--------------------------------------------

	  	    		else
	  	    		{ Send_String(&huart1,(char *)("ERR\n")); }

//---------------------------------------- RESTART -----------------------------------------

	  	    		for (int i = commandLenght + 1; i != 0; i--)
	  	    		{ commandBuf[i] = 0; }

	  	    		RunCommand = false;
	  	    		commandLenght = 0;
	  	    		valueLenght = 0;
	  	    		getValue = false;
	  	    		getAxis = false;

	  	    		uartPacket.getEndPacket = false;
	  	    		uartPacket.getAxis = false;
	  	    		uartPacket.getCommand = false;
	  	    		uartPacket.getValue = false;
	  	    		uartPacket.getArrayIndex = false;
	  	    		__HAL_TIM_SET_COUNTER(&timeoutTim, 0x0000);
	  	    	}
}

void Send_String (UART_HandleTypeDef *huart, char _out[])
	{
		HAL_UART_Transmit (huart, (uint8_t*) _out, strlen(_out), 2000);
	}

void Send_Char (UART_HandleTypeDef *huart, char* _out)
	{
		HAL_UART_Transmit (huart, (uint8_t*) _out, 1, 2000);
	}

void TIM4_IRQHandler(void) // прерывание раз в 2 секунды
{
//	if (checkingTimeError == true)
//	{	error = true;
//		Send_String(&huart1,(char *)("Error Time")); }

	if (axis[uartPacket.iAxis].beginMoving() == true)
	{	axis[uartPacket.iAxis].stop();
		__HAL_TIM_SET_COUNTER(&timeoutTim, 0x0000);}

	HAL_TIM_IRQHandler(&timeoutTim);
}
