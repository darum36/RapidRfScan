/*
 * motion.h
 *
 *  Created on: Aug 8, 2022
 *      Author: Kocherov I.O.
 */

#include <remote.h>

#ifndef INC_MOTION_H_
#define INC_MOTION_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f4xx_hal.h"
#ifdef __cplusplus
}
#endif

enum class ModeMoving
{
	LowSpeed,
	NormalSpeed
};

class Motion: public Remote
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
	void setSteps(uint64_t newSteps);
	void resetMotion();

private:

	void acceleration();
	void deceleration();
	void moving();
	bool motorOn();

	float mSpeed;
	float mCurrentSpeed;
	float mAcc;
	float mDcc;
	uint64_t mSteps;

	uint64_t mMovingTime;
	uint64_t mMovingSteps;
	ModeMoving PtPModeMoving;

	TIM_HandleTypeDef* mPWMTim;


};

#endif /* INC_MOTION_H_ */
