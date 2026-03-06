#ifndef __LIGHT_H
#define __LIGHT_H

#include "stm32f10x.h"                  // Device header
#include "ADC1.h"

// 默认光线阈值（0-4095，值越小表示越暗）
//#define Light_Threshold_Default 2000

#define Light_Port	GPIOA
#define Light_Pin	GPIO_Pin_0

void Light_Init(void);
//uint16_t Light_GetValue(void);
//uint16_t Light_GetThrehold(uint16_t threhold);
uint8_t Light_IsDark(void);

#endif
