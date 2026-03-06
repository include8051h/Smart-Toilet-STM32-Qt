#ifndef __WATERBOM_H
#define __WATERBOM_H

#include "stm32f10x.h"                  // Device header

#define Waterbom_RCC RCC_APB2Periph_GPIOA
#define Waterbom_Port GPIOA
#define Waterbom_Pin GPIO_Pin_7
#define ON 1
#define OFF 0//函数名不可与宏定义重叠

void Waterbom_Init(void);

void Waterbom_OFF(void);
//void LED_ON(void);

void Waterbom_ON(void);
//void LED_OFF(void);

#endif
