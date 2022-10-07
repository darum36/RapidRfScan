#include "main.h"

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_0);
}

void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{

}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
  if(hadc->Instance==ADC1)
  {

  }
}

void HAL_CRC_MspInit(CRC_HandleTypeDef* hcrc)
{
  if(hcrc->Instance==CRC)
  {

  }
}

void HAL_CRC_MspDeInit(CRC_HandleTypeDef* hcrc)
{
  if(hcrc->Instance==CRC)
  {

  }
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{

}

void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef* htim_pwm)
{
  if(htim_pwm->Instance==TIM2)
  {

  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
  if(htim_base->Instance==TIM1)
  {

  }
  else if(htim_base->Instance==TIM5)
  {

  }
  else if(htim_base->Instance==TIM10)
  {

  }
  else if(htim_base->Instance==TIM11)
  {

  }
}

void HAL_TIM_PWM_MspDeInit(TIM_HandleTypeDef* htim_pwm)
{
  if(htim_pwm->Instance==TIM2)
  {

  }
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{

}

void HAL_UART_MspDeInit(UART_HandleTypeDef* huart)
{


}
