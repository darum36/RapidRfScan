/*
 * uart.cpp
 *
 *  Created on: 4 окт. 2022 г.
 *      Author: kio
 */

#include "uart.h"
#include "motionController.h"

extern Axis axis[2];

UART_HandleTypeDef huart1;
RingBuffer <1024> uartBuffer;

unsigned char currentCommandByte = 0;
volatile bool getValue = false;
volatile bool RunCommand = false;
char commandBuf[2];
char valueBuf[9];
uint8_t commandLenght = 0;
uint8_t valueLenght = 0;
uint16_t commandValue = 0;
char TEMP[10];

bool timeout = false;
bool checkingTimeError = false;
bool error = false;
bool readStatus = false;

//constexpr char BOARD_ID[] = "BID";
//constexpr char SWTICH_ON_AXIS[] = "MO";
constexpr char MODE_MOTION[] = "MM";
constexpr char SPEED[] = "SP";
constexpr char ACCELERATION[] = "AC";
constexpr char DECCELERATION[] = "DC";
//constexpr char CURRENT_POSITION[] = "PS";
//constexpr char SET_ABSOLUTE_POSITION[] = "AP";
//constexpr char STOP_INTERVAL_WITH_FEEDBACK[] = "SW";
//constexpr char MOTOR_IMPULSE_PER_SI_UNIT[] = "SC";
//constexpr char ENCODER_COUNTS_PER_SI_UNIT[] = "EC";
//constexpr char FEEDBACK[] = "NC";
//constexpr char MAX_ERROR_POSITION[] = "ER";
//constexpr char INVERSE_ENCODER[] = "IE";
//constexpr char INVERSE_MOTOR_ROTATION[] = "IM";
//constexpr char LIMIT_TRIGGER_LOGIC[] = "LP";
//constexpr char LIMIT_ENABLE[] = "LE";
//constexpr char LIMIT_STATUS[] = "LS";
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

	HAL_NVIC_SetPriority(USART1_IRQn, 1, 0);
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

void checkUARTbuffer()
{
	if ((!uartBuffer.IsEmpty()) && (!RunCommand))
	  			{
	  				uartBuffer.Read(currentCommandByte);

	  				if (currentCommandByte != '\n')
	  					{
	  						if (getValue)
	  						{
	  							if (currentCommandByte < '0' || currentCommandByte > '9')
	  							{
	  								error = true;
	  								Send_String(&huart1,(char *)("Error Value"));
	  							}
	  							valueBuf[valueLenght] = currentCommandByte;
	  							valueLenght++;
	  						}
	  						if ((currentCommandByte != '=') && (!getValue))
	  						{
	  							if (commandLenght > 1)
	  							{
	  								error = true;
	  								Send_String(&huart1,(char *)("Miss ="));
	  							}
	  							commandBuf[commandLenght] = currentCommandByte;
	  							commandLenght++;

	  						}
	  						else
	  						{
	  							getValue = true;
	  							commandValue = false;
	  						}

	  						checkingTimeError = true;

	  					}
	  	    		else
	  	    		{
	  	    			RunCommand = true;
	  	    			checkingTimeError = false;
//	  	    			HAL_TIM_Base_Stop_IT(&timeoutTim);
	  	    		}
	  	    	}

	  	    	if (RunCommand)
	  	    	{
	  	    		if (getValue)
	  	    		{
	  	    			int j = 1;
	  	    			commandValue = 0;
	  	    			for (int i=valueLenght; i>0; i--)
	  	    			{
	  	    				commandValue = commandValue + ((valueBuf[i-1] - '0')*j);
	  	    				j*=10;
	  	    			}
	  	    			readStatus = false;
	  	    		}
	  	    		else
	  	    		{
	  	    			readStatus = true;
	  	    		}
	  	    		if (strncmp(commandBuf,MODE_MOTION, 0x2) == 0)
	  	    		{
	  	    			Send_String(&huart1,(char *)("MM = 0"));
	  	    			Send_String(&huart1,(char *)(__utoa(axis[1].getModeMoving(), TEMP, 10)));
	  	    			Send_String(&huart1,(char *)("\n"));

	  	    		}
	  	    		else if (strncmp(commandBuf,SPEED, 0x3) == 0)
	  	    		{
	  	    			Send_String(&huart1,(char *)("SPEED = "));
	  	    			Send_String(&huart1,(char *)(__utoa(axis[1].getSpeed(), TEMP, 10)));
	  	    			Send_String(&huart1,(char *)("\n"));
	  	    			if (getValue == false)
	  	    			{  }
	  	    			else
	  	    			{
	  	    				if (commandValue >= 2) {commandValue = 1;}


	  	    			}
	  	    		}
	  	    		else if (strncmp(commandBuf,ACCELERATION, 0x3) == 0)
	  	    		{
	  	    			if (getValue == false)
	  	    			{  }
	  	    			else
	  	    			{
	  	    				if (commandValue >= 2) {commandValue = 1;}


	  	    			}
	  	    		}
	  	    		else if (strncmp(commandBuf,DECCELERATION, 0x3) == 0)
	  	    		{
	  	    			if (getValue == false)
	  	    			{  }
	  	    			else
	  	    			{
							if (commandValue >= 2) {commandValue = 1;}


	  	    			}

	  	    		}

	  	    		else if (strncmp(commandBuf,BEGIN_MOVING, 0x3) == 0)
	  	    		{
	  	    			if (getValue == false)
	  	    			{  }
	  	    			else
	  	    			{
	  	    				if (commandValue >= 2) {commandValue = 1;}


	  	    			}
	  	    		}
	  	    		else if (strncmp(commandBuf,STOP_MOVING, 0x3) == 0)
	  	    		{
	  	    			if (getValue == false)
	  	    			{  }
	  	    			else
	  	    			{
							if (commandValue >= 2) {commandValue = 1;}


	  	    			}
	  	    		}

	  	    		else
	  	    		{
	  	    			Send_String(&huart1,(char *)("Error command"));	  	    		}

	  	    		for (int i = commandLenght + 1; i != 0; i--)
	  	    		{
	  	    			commandBuf[i] = 0;
	  	    		}

	  	    		RunCommand = false;
	  	    		commandLenght = 0;
	  	    		valueLenght = 0;
	  	    		getValue = false;
	  	    	}
}

void Send_String (UART_HandleTypeDef *huart, char _out[])
	{
		HAL_UART_Transmit (huart, (uint8_t*) _out, strlen(_out), 2000);
	}
