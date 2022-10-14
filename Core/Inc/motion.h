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

	Motion(TIM_HandleTypeDef* PWMTim, TIM_HandleTypeDef* EncTim);
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
	int32_t getPosition();
	void setPosition(int32_t newPosition);
	void updateEnc();

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
	int32_t mPosition;
	int32_t mPrevEncValue;

	uint64_t mMovingTime;
	uint64_t mMovingSteps;
	ModeMoving PtPModeMoving;

	TIM_HandleTypeDef* mPWMTim;
	TIM_HandleTypeDef* mEncTim;
};

#endif /* INC_MOTION_H_ */
