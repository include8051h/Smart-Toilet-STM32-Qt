#ifndef __LOCK_H
#define __LOCK_H

#include "stm32f10x.h"                  // Device header

#define Lock_RCC RCC_APB2Periph_GPIOA
#define Lock_Port GPIOA
#define Lock_Pin GPIO_Pin_7
#define ON 1
#define OFF 0//函数名不可与宏定义重叠

void Lock_Init(void);

void Lock_OFF(void);
//void LED_ON(void);

void Lock_ON(void);
//void LED_OFF(void);

#endif
