#ifndef INC_GAUL_DRIVERS_LOW_LEVEL_PWM_DRIVER_H_
#define INC_GAUL_DRIVERS_LOW_LEVEL_PWM_DRIVER_H_

#include "stm32f4xx.h"


void PWM_Init(TIM_TypeDef *TIMx, uint16_t prescaler, uint16_t arr);

void PWM_SetDutyCycle(TIM_TypeDef *TIMx, uint16_t canal, uint16_t duty);
void PWM_SetFrequency(TIM_TypeDef *TIMx, uint32_t freq);

#endif /* INC_GAUL_DRIVERS_LOW_LEVEL_PWM_DRIVER_H_ */
