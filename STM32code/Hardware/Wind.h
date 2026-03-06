#ifndef __WIND_H
#define __WIND_H

#include "stm32f10x.h"                  // Device header

#define Wind_RCC RCC_APB2Periph_GPIOB
#define Wind_Port GPIOB
#define Wind_Pin GPIO_Pin_1
#define ON 1
#define OFF 0//函数名不可与宏定义重叠

void Wind_Init(void);

void Wind_OFF(void);
//void LED_ON(void);

void Wind_ON(void);
//void LED_OFF(void);

#endif
