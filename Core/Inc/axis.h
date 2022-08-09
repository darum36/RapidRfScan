/*
 * axis.h
 *
 *  Created on: Aug 8, 2022
 *      Author: Kocherov
 */

#ifndef INC_AXIS_H_
#define INC_AXIS_H_

#include <motion.h>

class Axis
{
public:

	Axis(TIM_TypeDef* PWMTim, GPIO_TypeDef port, uint16_t pin, uint8_t afMaping,              // PWM
		TIM_TypeDef* EncTim,  GPIO_TypeDef portEncA, uint16_t pinEncA, uint8_t afMapingEncA,     // TIM ENCA
							  GPIO_TypeDef portEncB, uint16_t pinEncB, uint8_t afMapingEncB,	  // TIM ENCB																              // GPIO DIR
		GPIO_TypeDef portLimPlus, uint16_t	pinLimPlus, GPIO_TypeDef portLimMinus, uint16_t pinLimMinus,     // GPIOs LIMIT
		GPIO_TypeDef portLimHome, uint16_t	pinLimHome
																			                  //


		);
	virtual ~Axis()=default;

private:
	TIM_TypeDef* mPwmTimTypeDef;
	TIM_HandleTypeDef mPWMTim;

	Motion mMotion;
	GPIO_TypeDef mPort;
	uint16_t mPin;
	uint8_t mAfMaping;

	void init();
};



#endif /* INC_AXIS_H_ */
