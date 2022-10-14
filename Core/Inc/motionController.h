/*
 * motionController.h
 *
 *  Created on: 10 авг. 2022 г.
 *      Author: Kocherov I.O.
 */

#ifndef INC_MOTIONCONTROLLER_H_
#define INC_MOTIONCONTROLLER_H_

#include "initializeHardware.h"
#include <string>
#include <cmath>
#include <axis.h>
#include <remote.h>
#include <array>
#include "uart.h"

extern "C"
{ void TIM1_UP_TIM10_IRQHandler(void); }

void initMotion();
void startMotion();
void checkEmgStatus();
void RemoteMoving();
void SoftMoving();


#endif /* INC_MOTIONCONTROLLER_H_ */
