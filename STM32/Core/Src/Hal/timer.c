
#ifndef SRC_HAL_TIMER_C_
#define SRC_HAL_TIMER_C_

#include "main.h"
#include "Hal/timer.h"


#define TIMER_FN_MAX_SIZE	10

static TIMER_fn fn_table[TIMER_FN_MAX_SIZE];
static size_t fn_table_len = 0;

TIM_HandleTypeDef htim3 = {
	.Instance = TIM3,
	.Init = {
		.Prescaler = 15,
		.CounterMode = TIM_COUNTERMODE_UP,
		.Period = 999,
		.ClockDivision = TIM_CLOCKDIVISION_DIV1,
		.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE
	}
};
TIM_HandleTypeDef htim1 = {
	.Instance = TIM1,
	.Init = {
		.Prescaler = 15,
		.CounterMode = TIM_COUNTERMODE_UP,
		.Period = 65535 - 1,
		.ClockDivision = TIM_CLOCKDIVISION_DIV1,
		.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE
	}
};

bool TIMER_init(){
	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};

	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
	  {
	    Error_Handler();
	  }
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK)
	{
	    Error_Handler();
	}
	if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
	{
	    Error_Handler();
	}
	if (HAL_TIM_Base_Start_IT(&htim3) != HAL_OK)
	{
		Error_Handler();
	}

	HAL_TIM_MspPostInit(&htim3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4);
//	tim1 start here
		TIM_ClockConfigTypeDef sClockSourceConfig1 = {0};
		TIM_MasterConfigTypeDef sMasterConfig1 = {0};

	  /* USER CODE BEGIN TIM1_Init 1 */

	  /* USER CODE END TIM1_Init 1 */
	  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sClockSourceConfig1.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig1) != HAL_OK)
	  {
	    Error_Handler();
	  }
	  sMasterConfig1.MasterOutputTrigger = TIM_TRGO_RESET;
	  sMasterConfig1.MasterOutputTrigger2 = TIM_TRGO2_RESET;
	  sMasterConfig1.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig1) != HAL_OK)
	  {
	    Error_Handler();
	  }

	  if (HAL_TIM_Base_Start(&htim1))
	  {
			Error_Handler();
      }

	return true;
}

void PWM_set_pulse(uint64_t value, uint64_t device_pin){
	__HAL_TIM_SET_COMPARE(&htim3,device_pin,value);
}

bool TIMER_attach_intr_1ms(void (*fn)(void)){
	if(fn_table_len >= TIMER_FN_MAX_SIZE){
		return false;
	}
	fn_table[fn_table_len++] = fn;
	return true;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim){
	if(htim->Instance == htim3.Instance){
		for (int fn_idx = 0; fn_idx < fn_table_len; ++fn_idx) {
			fn_table[fn_idx]();
		}
	}
}

#endif /* SRC_HAL_TIMER_C_ */
