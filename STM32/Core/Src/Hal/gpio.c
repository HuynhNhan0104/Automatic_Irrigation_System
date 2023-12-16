

#ifndef SRC_HAL_GPIO_C_
#define SRC_HAL_GPIO_C_

#include "main.h"
#include "Hal/gpio.h"

bool GPIO_init(){
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
}

#endif /* SRC_HAL_GPIO_C_ */
