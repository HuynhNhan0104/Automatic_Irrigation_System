/*
 * delay_timer.c
 *
 *  Created on: Sep 28, 2023
 *      Author: NHAN
 */

#include "Device/delay_timer.h"
#include "Hal/timer.h"


void DELAY_TIM_Init(TIM_HandleTypeDef *htim)
{
	HAL_TIM_Base_Start(htim);
}

void DELAY_TIM_Us(TIM_HandleTypeDef *htim, uint16_t time)
{
	htim->Instance->CNT = 0;
	while(htim->Instance->CNT<time){}
}
void DELAY_TIM_Ms(TIM_HandleTypeDef *htim, uint16_t Time)
{
	__HAL_TIM_SET_COUNTER(htim,0);
	while(Time--)
	{
		while(__HAL_TIM_GET_COUNTER(htim)<1000){}
	}
}
