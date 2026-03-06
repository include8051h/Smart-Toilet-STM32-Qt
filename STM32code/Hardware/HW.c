#include "HW.h"

void HW_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd (HW_GPIO_CLK, ENABLE );	// 打开连接 传感器DO 的单片机引脚端口时钟
		GPIO_InitStructure.GPIO_Pin = HW_GPIO_PIN_OUT | HW_GPIO_PIN_IN;			// 配置连接 传感器DO 的单片机引脚模式
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			// 设置为上拉输入,因为光电红外传感器的有效输入是低电平
		
		GPIO_Init(HW_GPIO_PORT, &GPIO_InitStructure);				// 初始化 
	
}

uint16_t HW_GetData_OUT(void)
{
	uint16_t tempData;
	tempData = !GPIO_ReadInputDataBit(HW_GPIO_PORT, HW_GPIO_PIN_OUT);//这里做一个翻转，将有效电平从低变为高
	return tempData;
}

uint16_t HW_GetData_IN(void)
{
	uint16_t tempData;
	tempData = !GPIO_ReadInputDataBit(HW_GPIO_PORT, HW_GPIO_PIN_IN);//这里做一个翻转，将有效电平从低变为高
	return tempData;
}

