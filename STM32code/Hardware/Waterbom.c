#include "Waterbom.h"

void Waterbom_Init(void)
{
	//打开RCC时钟
	RCC_APB2PeriphClockCmd(Waterbom_RCC, ENABLE);
	
	//配置端口模式
	//1.定义一个GPIO初始化结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	
	//给结构体成员赋值
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//选用推挽输出
//	在STM32中，GPIO可以配置为多种模式，包括推挽输出、开漏输出、模拟输入、浮空输入等。对于驱动LED，通常选择推挽输出模式，原因如下：

//推挽输出（Push-Pull Output）：

//推挽输出结构能够主动输出高电平和低电平。当输出高电平时，P-MOS管导通，将输出拉到VDD（3.3V）；当输出低电平时，N-MOS管导通，将输出拉到GND。

//推挽输出模式提供较强的驱动能力，可以吸收和提供电流，因此可以直接驱动LED等负载。

//开漏输出（Open-Drain Output）：

//开漏输出只能主动拉低电平，不能主动输出高电平。高电平需要外部上拉电阻才能实现。

//开漏输出通常用于总线（如I2C）或者需要电平转换的场合。
	
	GPIO_InitStructure.GPIO_Pin = Waterbom_Pin;//选中B1号
	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO速度一般选择50MHz
	
	GPIO_Init(Waterbom_Port, &GPIO_InitStructure);
	
	//GPIO_SetBits(Wind_Port, Wind_Pin);//推挽输出初始状态下默认输出低电平
	//因此如果LED一脚接正情况下，LED一上电就会亮起，所以要将单片机输出拉高
}

void Waterbom_OFF(void)
{
	GPIO_ResetBits(Waterbom_Port, Waterbom_Pin);
}

void Waterbom_ON(void)
{
	GPIO_SetBits(Waterbom_Port, Waterbom_Pin);
}

