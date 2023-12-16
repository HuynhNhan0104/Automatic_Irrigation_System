#ifndef INC_HAL_GPIO_H_
#define INC_HAL_GPIO_H_

#include "main.h"
#include "stdio.h"
#include "stdbool.h"


typedef struct {
	GPIO_TypeDef * port;
	GPIO_InitTypeDef init_info;
}GPIO_info_t;

bool GPIO_init();


#endif /* INC_HAL_GPIO_H_ */
