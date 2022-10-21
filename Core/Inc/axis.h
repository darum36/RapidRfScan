/*
 * axis.h
 *
 *  Created on: Aug 8, 2022
 *      Author: Kocherov I.O.
 */

#ifndef INC_AXIS_H_
#define INC_AXIS_H_

#include <motion.h>

enum class eDirection
{
	Positive,
	Negative
};

class Axis
{
public:
	Axis();
	virtual ~Axis()=default;

	short getLimStatus();
	void checkLimits();
	bool checkAbleMoving();

	void begin();
	void stop();
	bool beginMoving();
	void emgStop();

	void jogging(bool beginJog);
	void ptp();

	void setDirection(eDirection dir);
	int getDirection();

	void setTurnOnAxis (int32_t newTurnOnAxis);
	int32_t getTurnOnAxis();

	void setInverseLim(int32_t newInverseLim);
	int32_t getInverseLim();

	void setEnableLim(int32_t newEnableLim);
	int32_t getEnableLim();

	void setInverseEncoder(int32_t newInveseEnc);
	int32_t getInverseEncoder();

	void setInverseMotor(int32_t newInverseMot);
	int32_t getInverseMotor();

	int32_t getSpeed();
	int32_t getAcc();
	int32_t getDcc();
	int32_t modeMoving();
	int32_t getPosition();

	void setSpeed(int32_t newSpeed);
	void setAcc(int32_t newAcc);
	void setDcc(int32_t newDcc);
	void setModeMoving(int32_t MM);
	void setPosition(int32_t newPosition);
	void updateEnc();

	void tempSetParam(float newSpeed, float newAcc, float newDcc);
	void setDefaultParam();
	void init(TIM_TypeDef* PWMTim,																	// PWM
			  GPIO_TypeDef* portPWM, uint16_t pinPWM, uint8_t afMapingPWM,             		 		// PWM
			  TIM_TypeDef* EncTim,																	// TIM ENC
			  GPIO_TypeDef* portEncA, uint16_t pinEncA, uint8_t afMapingEncA,    		 			// TIM ENCA
			  GPIO_TypeDef* portEncB, uint16_t pinEncB, uint8_t afMapingEncB,					    // TIM ENCB																              // GPIO DIR
			  GPIO_TypeDef* portLimPlus, uint16_t	pinLimPlus,										// GPIOs LIMIT
			  GPIO_TypeDef* portLimMinus, uint16_t pinLimMinus,     								// GPIOs LIMIT
			  GPIO_TypeDef* portLimHome, uint16_t	pinLimHome,										// GPIOs LIMIT
			  GPIO_TypeDef* portDir, uint16_t	pinDir);											// GPIOs LIMIT

private:

	TIM_TypeDef*      mPwmTimTypeDef;
	TIM_HandleTypeDef mPWMTim;

	GPIO_TypeDef* gPwmPort;
	uint16_t 	  gPwmPin;
	uint8_t 	  gAfMapingPWM;

	TIM_TypeDef*      mEncTimTypeDef;
	TIM_HandleTypeDef mEncTim;
	TIM_Encoder_InitTypeDef sConfigEnc;

	GPIO_TypeDef* gEncAPort;
	uint16_t 	  gEncAPin;
	uint8_t 	  gAfMapingEncA;

	GPIO_TypeDef* gEncBPort;
	uint16_t      gEncBPin;
	uint8_t 	  gAfMapingEncB;

	GPIO_TypeDef* gLimPlusPort;
	uint16_t      gLimPlusPin;

	GPIO_TypeDef* gLimMinusPort;
	uint16_t 	  gLimMinusPin;

	GPIO_TypeDef* gLimHomePort;
	uint16_t 	  gLimHomePin;

	GPIO_TypeDef* gDirPort;
	uint16_t 	  gDirPin;

	Motion 	  	  mMotion;

	short tempStatus;
	bool perPosMoving;
	bool permNegMoving;
	bool homecome;
	bool ableMoving;

	int32_t turnOnAxis;
	int32_t inverseLim;
	int32_t inverseEnc;
	int32_t inverseMot;
	int32_t enableLim;

	int32_t modeMov;
	bool beginMov;

};

#endif /* INC_AXIS_H_ */
