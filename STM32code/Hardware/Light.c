#include "Light.h"

//uint16_t light_threshold = Light_Threshold_Default;

void Light_Init(void)
{
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//开启GPIOA的RCC外设时钟
	
	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//选用模拟输出	
	GPIO_InitStructure.GPIO_Pin = Light_Pin;//选中0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO速度一般选择50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	//ADC1_Init();
}

//uint16_t Light_GetThrehold(uint16_t threhold)
//{
//	return light_threshold = threhold;
//}

uint8_t Light_IsDark(void)
{
	uint16_t light_value = ADC1_ReadChannel(0);
	return (light_value < 2000) ? 0 : 1;
}
