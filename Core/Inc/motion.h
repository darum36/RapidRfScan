/*
 * motion.h
 *
 *  Created on: Aug 8, 2022
 *      Author: Kocherov I.O.
 */

#ifndef INC_MOTION_H_
#define INC_MOTION_H_
//
//#include <cmath>

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f4xx_hal.h"
#ifdef __cplusplus
}
#endif

class Motion
{
public:

	Motion(TIM_HandleTypeDef* PWMTim);
	virtual ~Motion()=default;

	void jogging();
	void ptp();

	void setSpeed(float newSpeed);
	float getSpeed();

private:
	void acceleration();
	void decelerarion();
	void moving();
	void direction();

	float mSpeed;
	float mCurrentSpeed;
	float mAcc;
	float mDcc;

	TIM_HandleTypeDef* mPWMTim;
};



#endif /* INC_MOTION_H_ */
