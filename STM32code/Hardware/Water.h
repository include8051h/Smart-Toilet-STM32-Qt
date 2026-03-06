#ifndef __WATER_H
#define	__WATER_H
#include "stm32f10x.h"
#include "Delay.h"
#include "math.h"
#include "ADC1.h"

#define WATER_READ_TIMES	10  //WATER传感器ADC循环读取次数

//模式选择	
//模拟AO:	1
//数字DO:	0
#define	MODE 	1

// WATER GPIO宏定义
#if MODE
#define		WATER_AO_GPIO_CLK								RCC_APB2Periph_GPIOA
#define 	WATER_AO_GPIO_PORT							GPIOA
#define 	WATER_AO_GPIO_PIN								GPIO_Pin_3
#define   ADC_CHANNEL_WATER               			ADC_Channel_3	// ADC 通道宏定义

#else
#define		WATER_DO_GPIO_CLK								RCC_APB2Periph_GPIOA
#define 	WATER_DO_GPIO_PORT							GPIOA
#define 	WATER_DO_GPIO_PIN								GPIO_Pin_1			

#endif
/*********************END**********************/


void Water_Init(void);
uint16_t Water_ADC_Read(void);

#endif /* __WATER_H */

