#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"                  // Device header

#define LED_Port GPIOB
#define LED_Pin GPIO_Pin_0
#define ON 1
#define OFF 0//函数名不可与宏定义重叠

void LED_Init(void);

void LED_ON(void);

void LED_OFF(void);

#endif
