#include "Water.h"

/*****************辰哥单片机设计******************
											STM32
 * 文件			:	水位传感器c文件                   
 * 版本			: V1.0
 * 日期			: 2024.1.27
 * MCU			:	STM32F103C8T6
 * 接口			:	见代码							
 * BILIBILI	:	辰哥单片机设计
 * CSDN			:	辰哥单片机设计
 * 作者			:	辰哥

**********************BEGIN***********************/

void Water_Init(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd (WATER_AO_GPIO_CLK, ENABLE );	// 打开 ADC IO端口时钟
		GPIO_InitStructure.GPIO_Pin = WATER_AO_GPIO_PIN;					// 配置 ADC IO 引脚模式
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		// 设置为模拟输入
		
		GPIO_Init(WATER_AO_GPIO_PORT, &GPIO_InitStructure);				// 初始化 ADC IO

}

/**
  * @brief  获取ADC转换后的数据
  * @param  ADC_Channel 	选择需要采集的ADC通道
  * @param  ADC_SampleTime  选择需要采样时间
  * @retval 返回转换后的模拟信号数值
  */
//u16 ADC_GetValue(void)
//{
//	//配置ADC通道
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 3, ADC_SampleTime_55Cycles5);
//	
//	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //软件触发ADC转换
//	while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); //读取ADC转换完成标志位
//	return ADC_GetConversionValue(ADC1);
//}

uint16_t Water_ADC_Read(void)
{
	//设置指定ADC的规则组通道，采样时间
	return ADC1_ReadChannel(3);
}

//uint16_t WATER_GetData(void)
//{
//	
//	uint32_t  tempData = 0;
//	for (uint8_t i = 0; i < WATER_READ_TIMES; i++)
//	{
//		tempData += WATER_ADC_Read();
//		Delay_ms(5);
//	}

//	tempData /= WATER_READ_TIMES;
//	return tempData;
//	
//}



