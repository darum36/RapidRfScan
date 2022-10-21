/*
 * uart.h
 *
 *  Created on: 4 окт. 2022 г.
 *      Author: kio
 */

#ifndef INC_UART_H_
#define INC_UART_H_

#include "main.h"
#include "ringBuffer.h"
#include "initializeHardware.h"


#include <stdio.h>
#include <string.h>
#include "array"

#ifdef __cplusplus
extern "C" {
#endif

struct Packet
{
	char cAxis;
	uint8_t iAxis;
	char command[3] = {};
	int arrayIndex = 0;
	bool arrayCommand = false;
	double dValue = 0;
	int32_t iValue = 0;

	bool getAxis = false;
	bool getCommand = false;
	bool getArrayIndex = false;
	bool getValue = false;
	bool getEndPacket = false;
};

void initUart();
void USART1_IRQHandler(void);
void checkUARTbuffer();
void resetUart();
void Send_String (UART_HandleTypeDef *huart, char _out[]);
void Send_Char (UART_HandleTypeDef *huart, char* _out);
void TIM4_IRQHandler(void);

#ifdef __cplusplus
}
#endif

#endif /* INC_UART_H_ */
