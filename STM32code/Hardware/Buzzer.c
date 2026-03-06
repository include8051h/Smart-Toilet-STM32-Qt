#include "stm32f10x.h"                  // Device header



void Buzzer_Init(void)
{
	//打开RCC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	//配置端口模式
	//1.定义一个GPIO初始化结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//给结构体成员赋值
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//选用推挽输出
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//选中1号和2号引脚
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO速度一般选择50MHz
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}



void Buzzer_ON(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_12);
}


void Buzzer_OFF(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_12);
}



void Buzzer_Turn(void)
{
	if((GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_12)) == 0)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);
	}
}
