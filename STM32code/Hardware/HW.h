#ifndef __HW_H
#define	__HW_H
#include "stm32f10x.h"
//#include "adcx.h"
#include "Delay.h"
#include "math.h"

// HW GPIO∫Í∂®“Â

#define		HW_GPIO_CLK								RCC_APB2Periph_GPIOB
#define 	HW_GPIO_PORT							GPIOB
#define 	HW_GPIO_PIN_OUT							GPIO_Pin_13	
#define  	HW_GPIO_PIN_IN							GPIO_Pin_14


void HW_Init(void);
uint16_t HW_GetData_OUT(void);
uint16_t HW_GetData_IN(void);

#endif /* __ADC_H */

