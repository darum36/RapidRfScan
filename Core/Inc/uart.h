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

void initUart();
void USART1_IRQHandler(void);
void checkUARTbuffer();
void Send_String (UART_HandleTypeDef *huart, char _out[]);

#ifdef __cplusplus
}
#endif

#endif /* INC_UART_H_ */
