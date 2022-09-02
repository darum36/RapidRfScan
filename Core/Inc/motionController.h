/*
 * motionController.h
 *
 *  Created on: 10 авг. 2022 г.
 *      Author: Kocherov I.O.
 */

#ifndef INC_MOTIONCONTROLLER_H_
#define INC_MOTIONCONTROLLER_H_

//#include "main.h"
#include "initializeHardware.h"
#include <string>
#include <cmath>
#include <axis.h>
#include <remote.h>
#include "Ring_Buf.h"
#include <array>


extern "C" {
void TIM1_UP_TIM10_IRQHandler(void);
}


void initMotion();
void initUart();
void startMotion();
void checkEmgStatus();

#endif /* INC_MOTIONCONTROLLER_H_ */
