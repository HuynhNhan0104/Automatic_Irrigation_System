/*
 * delay_timer.h
 *
 *  Created on: Sep 28, 2023
 *      Author: NHAN
 */

#ifndef INC_DELAY_TIMER_H_
#define INC_DELAY_TIMER_H_
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_tim.h"
void DELAY_TIM_Init(TIM_HandleTypeDef *htim);
void DELAY_TIM_Us(TIM_HandleTypeDef *htim, uint16_t time);
void DELAY_TIM_Ms(TIM_HandleTypeDef *htim, uint16_t Time);


#endif /* INC_DELAY_TIMER_H_ */
