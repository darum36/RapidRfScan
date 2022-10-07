/*
 * remote.h
 *
 *  Created on: 25 авг. 2022 г.
 *      Author: Kocherov I.O.
 */

#ifndef INC_REMOTE_H_
#define INC_REMOTE_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f4xx_hal.h"
#ifdef __cplusplus
}
#endif

class Remote
{
public:

	Remote();
	virtual ~Remote() = default;

	void checkRemoteStatus();
	bool getOnOffStatus();
	bool getFineStatus();
	bool getRunStatus();
	int getSpeedStatus();
	short getAxisStatus();

	void init(ADC_HandleTypeDef adc,
			  GPIO_TypeDef* portSpeed, uint16_t pinSpeed,
			  GPIO_TypeDef* portOnOff, uint16_t	pinOnOff,
			  GPIO_TypeDef* portRun, uint16_t	pinRun,
			  GPIO_TypeDef* portFine, uint16_t	pinFine,
			  GPIO_TypeDef* portAxis0, uint16_t	pinAxis0,
			  GPIO_TypeDef* portAxis1, uint16_t	pinAxis1,
			  GPIO_TypeDef* portAxis2, uint16_t	pinAxis2);

private:

		ADC_HandleTypeDef mAdc;

		GPIO_TypeDef* gPortSpeed;
		uint16_t 	 gPinSpeed;

		GPIO_TypeDef* gPortOnOff;
		uint16_t     gPinOnOff;

		GPIO_TypeDef* gPortRun;
		uint16_t 	 gPinRun;

		GPIO_TypeDef* gPortFine;
		uint16_t 	 gPinFine;

		GPIO_TypeDef* gPortAxis0;
		uint16_t 	 gPinAxis0;

		GPIO_TypeDef* gPortAxis1;
		uint16_t 	 gPinAxis1;

		GPIO_TypeDef* gPortAxis2;
		uint16_t 	 gPinAxis2;

	void checkOnOffStatus();
	void checkFineStatus();
	void checkRunStatus();
	void checkNumberAxe();
	void checkSpeed();

	bool mOnOffStatus;
	bool mFineStatus;
	bool mRunStatus;
	short mCurrentAxis;

	uint32_t mMaxAdcValue;
	uint32_t mAvrAdcValue;
	uint32_t mMinAdcValue;
	uint32_t mSizeDeadZone;
	uint32_t mCurrentAdcSpeed;

};

#endif /* INC_REMOTE_H_ */
