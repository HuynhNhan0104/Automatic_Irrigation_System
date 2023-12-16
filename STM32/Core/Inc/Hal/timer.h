#ifndef INC_HAL_TIMER_H_
#define INC_HAL_TIMER_H_

#include "stdio.h"
#include "stdbool.h"

#include "main.h"

typedef void (*TIMER_fn)(void);

#define DEVICE_1 	TIM_CHANNEL_1 //PA6
#define DEVICE_2 	TIM_CHANNEL_2 //PA7
#define DEVICE_3 	TIM_CHANNEL_3 //PB0
#define DEVICE_4 	TIM_CHANNEL_4 //PB1

#define MAX_PWM_VALUE  999

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim1;

bool TIMER_init();
bool TIMER_attach_intr_1ms(void (*fn)(void));
void PWM_set_pulse(uint64_t value, uint64_t device_pin);

#endif /* INC_HAL_TIMER_H_ */
