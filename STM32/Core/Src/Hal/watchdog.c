

#include "Hal/watchdog.h"


IWDG_HandleTypeDef hiwdg = {
	.Instance = IWDG,
	.Init = {
		.Prescaler = IWDG_PRESCALER_64,
		.Reload = 4096
	}
};

void WATCHDOG_init(){
	hiwdg.Instance = IWDG;
	hiwdg.Init.Prescaler = IWDG_PRESCALER_64;
	hiwdg.Init.Reload = 4095;
	if (HAL_IWDG_Init(&hiwdg) != HAL_OK)
	{
		Error_Handler();
	}
}

void WATCHDOG_refresh(){
	HAL_IWDG_Refresh(&hiwdg);
}


void WATCHDOG_test(){
	utils_log_info("WDG\r\n");
	while(1){
		utils_log_info("WDG\r\n");
		HAL_Delay(1000);
	}
}
