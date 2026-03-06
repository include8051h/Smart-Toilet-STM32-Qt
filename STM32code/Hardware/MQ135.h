#ifndef __MQ135_H
#define	__MQ135_H
#include "stm32f10x.h"
//#include "adcx.h"
#include "Delay.h"
#include "ADC1.h"
#include "math.h"


#define MQ135_READ_TIMES	10  //MQ-135传感器ADC循环读取次数

#define    ADCx                          ADC1
#define    ADC_CLK                       RCC_APB2Periph_ADC1


//模式选择	
//模拟AO:	1
//数字DO:	0
#define	MODE 	1

// MQ-135 GPIO宏定义
#if MODE
#define		MQ135_AO_GPIO_CLK								RCC_APB2Periph_GPIOA
#define 	MQ135_AO_GPIO_PORT							GPIOA
#define 	MQ135_AO_GPIO_PIN								GPIO_Pin_2
#define   ADC_CHANNEL               			ADC_Channel_2	// ADC 通道宏定义

#else
#define		MQ135_DO_GPIO_CLK								RCC_APB2Periph_GPIOA
#define 	MQ135_DO_GPIO_PORT							GPIOA
#define 	MQ135_DO_GPIO_PIN								GPIO_Pin_1			

#endif
/*********************END**********************/

void ADCx_Init(void);
u16 ADC_GetValue(uint8_t ADC_Channel,uint8_t ADC_SampleTime);
void MQ135_Init(void);
uint16_t MQ135_ADC_Read(void);
uint16_t MQ135_GetData(void);
float MQ135_GetData_PPM(void);

#endif /* __ADC_H */

