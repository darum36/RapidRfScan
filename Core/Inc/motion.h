/*
 * motion.h
 *
 *  Created on: Aug 8, 2022
 *      Author: Kocherov I.O.
 */

#ifndef INC_MOTION_H_
#define INC_MOTION_H_

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
	void setAcc(float newAcc);
	float getAcc();
	void setDcc(float newDcc);
	float getDcc();
	void resetMotion();

private:

	void acceleration();
	void deceleration();
	void moving(uint64_t movingSteps);
	bool motorOn();

	float mSpeed;
	float mCurrentSpeed;
	float mAcc;
	float mDcc;

	bool mNeedMoving;
	uint64_t mMovingTime;

	TIM_HandleTypeDef* mPWMTim;

};

#endif /* INC_MOTION_H_ */
