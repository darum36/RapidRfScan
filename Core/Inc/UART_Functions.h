/*
 * UART_Functions.h
 *
 *  Created on: 18 июл. 2022 г.
 *      Author: admin
 */

#ifndef INC_UART_FUNCTIONS_H_
#define INC_UART_FUNCTIONS_H_
#include "Ring_Buf.h"
#include "motion.h"
uint8_t RX_data[5];

//глобальные переменные для передачи значений
int PS_int_data = 0;
int AP_int_data;
int AC_int_data;
int DC_int_data;
int SP_int_data;
int HL_int_data;
int LL_int_data;
int MM_int_data;
int MO_int_data;
int BG_int_data;
int ST_int_data;
int EM_int_data;
int MS_int_data;

bool motor_motion;




RingBuf <uint8_t> Buffer(1024);
static int stopbit = 0;
int CRCcounter = 0;

int lenght_num_mess(uint8_t *message)
{
	int lenght = 0;
	for (int i = 4; message[i] != '/'; i++)
	{
		lenght++;
	}
	return lenght;
}

void fill_numb_mass(uint8_t *rcvd_msg, uint8_t *numb, int numb_lenght)
{

	for (int i = 0; i < numb_lenght; i++)
	{
		numb[i] = rcvd_msg[i + 4];
	}
}

int uint8_to_int(uint8_t *numb, int lenght)
{
	int result = 0, order = 1;

	for (int i = 0; i < lenght; i++)
	{
		result = result + (numb[lenght-1 - i]-48) * order;
		order *= 10;
	}
	return result;
}

int lenght_int_num(int int_numb)
{
	int temp = 1;
	int lenght = 0;
	int order = 1;
	for (int i = 0; temp != 0 ; i++)
	{
		temp = int_numb / order;
		order *= 10;
		if (temp != 0)
		{
			lenght++;
		}
	}
	return lenght;
}

void int_to_uint8(int lenght,int int_numb, uint8_t *numb)
{
	int temp;
	int order = 1;
	for (lenght - 1; lenght > 0; lenght--)
	{
		temp = int_numb / order;
		order *= 10;
		temp = temp % 10;
		numb[lenght -1] = temp + 48;
	}
}

void command(UART_HandleTypeDef *huart, uint8_t TX_data[], int len)
{
	switch (TX_data[1])
	{
	case 'P':
		if (TX_data[2] == 'S')
		{
			//HAL_UART_Transmit_IT(huart, TX_data, 15);

			if (TX_data[3] == '=')
			{
				int lenght_rx_numb = lenght_num_mess(TX_data);
				uint8_t *numb = new uint8_t[lenght_rx_numb];
				fill_numb_mass(TX_data, numb, lenght_rx_numb);
				PS_int_data = uint8_to_int(numb, lenght_rx_numb);
				int lenght_int_numb = lenght_int_num(PS_int_data);
				uint8_t *PS_uint8_data = new uint8_t[lenght_int_numb];
				int_to_uint8(lenght_int_numb, PS_int_data, PS_uint8_data);
				HAL_UART_Transmit_IT(huart, PS_uint8_data, lenght_int_numb);
				delete []numb;
				delete []PS_uint8_data;
			}
			else
			{
				int lenght_int_numb = lenght_int_num(PS_int_data);
				uint8_t *PS_uint8_data = new uint8_t[lenght_int_numb];
				int_to_uint8(lenght_int_numb, PS_int_data, PS_uint8_data);
				HAL_UART_Transmit_IT(huart, PS_uint8_data, lenght_int_numb);
				delete []PS_uint8_data;
			}
		}
		else
		{
			//все плохо
		}
		break;
	case 'A':
		if (TX_data[2] == 'P')
		{
			if (TX_data[3] == '=')
			{
				int lenght_rx_numb = lenght_num_mess(TX_data);
				uint8_t *numb = new uint8_t[lenght_rx_numb];
				fill_numb_mass(TX_data, numb, lenght_rx_numb);
				AP_int_data = uint8_to_int(numb, lenght_rx_numb);
				int lenght_int_numb = lenght_int_num(AP_int_data);
				uint8_t *AP_uint8_data = new uint8_t[lenght_int_numb];
				int_to_uint8(lenght_int_numb, AP_int_data, AP_uint8_data);
				HAL_UART_Transmit_IT(huart, AP_uint8_data, lenght_int_numb);
				delete []numb;
				delete []AP_uint8_data;
			}
			else
			{
				int lenght_int_numb = lenght_int_num(PS_int_data);
				uint8_t *AP_uint8_data = new uint8_t[lenght_int_numb];
				int_to_uint8(lenght_int_numb, AP_int_data, AP_uint8_data);
				HAL_UART_Transmit_IT(huart, AP_uint8_data, lenght_int_numb);
				delete []AP_uint8_data;
			}
		}
		else if (TX_data[2] == 'C')
		{
			if (TX_data[3] == '=')
			{
				int lenght_rx_numb = lenght_num_mess(TX_data);
				uint8_t *numb = new uint8_t[lenght_rx_numb];
				fill_numb_mass(TX_data, numb, lenght_rx_numb);
				AC_int_data = uint8_to_int(numb, lenght_rx_numb);
				int lenght_int_numb = lenght_int_num(AC_int_data);
				uint8_t *AC_uint8_data = new uint8_t[lenght_int_numb];
				int_to_uint8(lenght_int_numb, AC_int_data, AC_uint8_data);
				HAL_UART_Transmit_IT(huart, AC_uint8_data, lenght_int_numb);
				delete []numb;
				delete []AC_uint8_data;
			}
			else
			{
			//нельзя сделать запрос установки абсолютной позиции, можно только установить
			}
		}
		else
		{
			//все плохо
		}
		break;
	case 'D':
		if (TX_data[2] == 'C')
		{
			if (TX_data[3] == '=')
			{
				int lenght_rx_numb = lenght_num_mess(TX_data);
				uint8_t *numb = new uint8_t[lenght_rx_numb];
				fill_numb_mass(TX_data, numb, lenght_rx_numb);
				DC_int_data = uint8_to_int(numb, lenght_rx_numb);
				int lenght_int_numb = lenght_int_num(DC_int_data);
				uint8_t *DC_uint8_data = new uint8_t[lenght_int_numb];
				int_to_uint8(lenght_int_numb, DC_int_data, DC_uint8_data);
				HAL_UART_Transmit_IT(huart, DC_uint8_data, lenght_int_numb);
				delete []numb;
				delete []DC_uint8_data;
			}
			else
			{

			}
		}
		else
		{
			//все плохо
		}
		break;
	case 'S':
		if (TX_data[2] == 'T')
		{
			if (TX_data[3] == '=')
			{
				motor_motion = 0;
				//остановка движения неизвестно какие значения буду передаваться
				// возможно стоит просто передавать значение прекращения как-то
			}
			else
			{
				//остановка движения
			}
		}
		else if (TX_data[2] == 'P')
		{
			if (TX_data[3] == '=')
			{
				int lenght_rx_numb = lenght_num_mess(TX_data);
				uint8_t *numb = new uint8_t[lenght_rx_numb];
				fill_numb_mass(TX_data, numb, lenght_rx_numb);
				SP_int_data = uint8_to_int(numb, lenght_rx_numb);
				int lenght_int_numb = lenght_int_num(SP_int_data);
				uint8_t *SP_uint8_data = new uint8_t[lenght_int_numb];
				int_to_uint8(lenght_int_numb, SP_int_data, SP_uint8_data);
				HAL_UART_Transmit_IT(huart, SP_uint8_data, lenght_int_numb);
				delete []numb;
				delete []SP_uint8_data;
			}
			else
			{
				int lenght_int_numb = lenght_int_num(SP_int_data);
				uint8_t *SP_uint8_data = new uint8_t[lenght_int_numb];
				int_to_uint8(lenght_int_numb, SP_int_data, SP_uint8_data);
				HAL_UART_Transmit_IT(huart, SP_uint8_data, lenght_int_numb);
				delete []SP_uint8_data;
			}
		}
		else
		{
			//все плохо
		}
		break;
	case 'H':
		if (TX_data[2] == 'L')
		{
			if (TX_data[3] == '=')
			{

			}
			else
			{

			}
		}
		else
		{
			//все плохо
		}
		break;
	case 'L':
		if (TX_data[2] == 'L')
		{
			if (TX_data[3] == '=')
			{

			}
			else
			{

			}
		}
		else
		{
			//все плохо
		}
		break;
	case 'M':
		if (TX_data[2] == 'M')
		{
			if (TX_data[3] == '=')
			{
				int lenght_rx_numb = lenght_num_mess(TX_data);
				uint8_t *numb = new uint8_t[lenght_rx_numb];
				fill_numb_mass(TX_data, numb, lenght_rx_numb);
				MM_int_data = uint8_to_int(numb, lenght_rx_numb);
				int lenght_int_numb = lenght_int_num(MM_int_data);
				uint8_t *MM_uint8_data = new uint8_t[lenght_int_numb];
				int_to_uint8(lenght_int_numb, MM_int_data, MM_uint8_data);
				HAL_UART_Transmit_IT(huart, MM_uint8_data, lenght_int_numb);
				delete []numb;
				delete []MM_uint8_data;
			}
			else
			{
				int lenght_int_numb = lenght_int_num(MM_int_data);
				uint8_t *MM_uint8_data = new uint8_t[lenght_int_numb];
				int_to_uint8(lenght_int_numb, MM_int_data, MM_uint8_data);
				HAL_UART_Transmit_IT(huart, MM_uint8_data, lenght_int_numb);
				delete []MM_uint8_data;
			}
		}
		else if (TX_data[2] == 'O')
		{
			if (TX_data[3] == '=')
			{

			}
			else
			{

			}
		}
		else if (TX_data[2] == 'S')
		{
			if (TX_data[3] == '=')
			{
				//все плохо
				//нельзя сделать установку запроса в каком состоянии сейчас находится движок
			}
			else
			{

			}
		}
		else
		{
			//все плохо
		}
		break;
	case 'B':
		if (TX_data[2] == 'G')
		{
			if (TX_data[3] == '=')
			{
				//начало движения
				motor_motion = 1;
			}
			else
			{
				//нельзя ставить
			}
		}
		else
		{
			//все плохо
		}
		break;
	case 'E':
		if (TX_data[2] == 'M')
		{
			if (TX_data[3] == '=')
			{
				//причина последней остановки можно только запрашивать
			}
			else
			{

			}
		}
		else
		{
			//все плохо
		}
		break;
	default:
		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);
		break;
	}

}
void getAxes(UART_HandleTypeDef *huart, uint8_t TX_data[], int len)
{
	switch (TX_data[0])
	{
	case 'X':
		command(huart,TX_data, len);
		break;
	case 'Y':
		command(huart,TX_data, len);
		break;
	case 'W':
		command(huart,TX_data, len);
		break;
	case 'E':
		command(huart,TX_data, len);
		break;
	case 'F':
		command(huart,TX_data, len);
		break;
	case 'G':
		command(huart,TX_data, len);
		break;
	case 'H':
		command(huart,TX_data, len);
		break;
	case 'U':
		command(huart,TX_data, len);
		break;
	case 'V':
		command(huart,TX_data, len);
		break;
	default:
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);
		break;
	}
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	Buffer.write(RX_data[0]);
	if (RX_data[0] == '/')
	{
		stopbit = 1;
		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_1);
	}else
	if (RX_data[0] == 'r' && stopbit == 1)
	{
		stopbit = 2;
		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_2);
	}else
	if(RX_data[0] != 'r' && stopbit == 1)
	{
		stopbit = 0;
	}else
	if(stopbit == 2)
	{
		CRCcounter++;
		//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_8);
		//if (CRCcounter == 3)
			//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_9);
		if (CRCcounter == 4)
		{
			stopbit = 3;
			CRCcounter = 0;
			//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_4);

		}
	}
}

void UART(UART_HandleTypeDef *huart)
{
	HAL_UART_Receive_IT(huart, RX_data, 1);
	if (stopbit == 3)
	{
		  if (Buffer.isEmpty() != 1)
				  {
					  int len;
					  len = Buffer.readLenght();
					  uint8_t *TX_data = new uint8_t[len];
					  for (int i = 0; i < len; i++)
					  {
						TX_data[i] = Buffer.read();
					  }
					  getAxes(huart,TX_data,len);
					  //HAL_UART_Transmit_IT(huart, TX_data, len);

					  delete [] TX_data;
					  stopbit = 0;
				  }
	}
};

#endif /* INC_UART_FUNCTIONS_H_ */
