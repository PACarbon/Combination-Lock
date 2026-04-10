#ifndef _LED_H_
#define _LED_H_

#include "sys.h"

#define LED_GPIO_PIN   GPIO_Pin_13
#define LED_GPIO_PORT  GPIOC
#define BEEP_GPIO_PIN   GPIO_Pin_1
#define BEEP_GPIO_PORT  GPIOB
#define INFRARED_GPIO_PIN   GPIO_Pin_8
#define INFRARED_GPIO_PORT  GPIOA

#define LED_GPIO_CLK RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA


void LED_GPIO_Config(void);
void LED_change(void);

#endif
