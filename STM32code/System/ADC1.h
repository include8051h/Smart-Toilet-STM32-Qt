#ifndef __ADC1_H
#define __ADC1_H

#include "stm32f10x.h"                  // Device header

void ADC1_Init(void);
uint16_t ADC1_ReadChannel(uint16_t channel);


#endif
