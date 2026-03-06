#include "MQ135.h"

void MQ135_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
		RCC_APB2PeriphClockCmd (MQ135_AO_GPIO_CLK, ENABLE );	// 打开 ADC IO端口时钟
		GPIO_InitStructure.GPIO_Pin = MQ135_AO_GPIO_PIN;					// 配置 ADC IO 引脚模式
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		// 设置为模拟输入
		
		GPIO_Init(MQ135_AO_GPIO_PORT, &GPIO_InitStructure);				// 初始化 ADC IO
//		GPIO_InitTypeDef GPIO_InitStructure;
//		
//		RCC_APB2PeriphClockCmd (MQ135_AO_GPIO_CLK, ENABLE );	// 打开 ADC IO端口时钟
//		GPIO_InitStructure.GPIO_Pin = MQ135_AO_GPIO_PIN;					// 配置 ADC IO 引脚模式
//		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		// 设置为模拟输入
//		
//		GPIO_Init(MQ135_AO_GPIO_PORT, &GPIO_InitStructure);				// 初始化 ADC IO

//		//
//		//开启ADC时钟
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
//	
//	//ADC频率进行6分频
//	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
//	
//	//配置ADC结构体
//	ADC_InitTypeDef ADC_InitStructure;
//	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//独立模式
//	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//数据右对齐
//	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//软件触发
//	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//单次转换
//	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//非扫描模式
//	ADC_InitStructure.ADC_NbrOfChannel = 1;	//总通道数
//	ADC_Init(ADC1, &ADC_InitStructure);	//初始化ADC1
//	
//	//开启ADCx
//	ADC_Cmd(ADC1, ENABLE);
//	
//	//进行ADC校准
//	ADC_ResetCalibration(ADC1);
//	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
//	ADC_StartCalibration(ADC1);
//	while(ADC_GetCalibrationStatus(ADCx) == SET);	
//		//ADCx_Init();
}


uint16_t MQ135_ADC_Read(void)
{
	//设置指定ADC的规则组通道，采样时间
//配置ADC通道
	return ADC1_ReadChannel(2);
}


uint16_t MQ135_GetData(void)
{
	uint32_t  tempData = 0;
	for (uint8_t i = 0; i < MQ135_READ_TIMES; i++)
	{
		tempData += MQ135_ADC_Read();
		Delay_ms(5);
	}

	tempData /= MQ135_READ_TIMES;
	return tempData;
}


float MQ135_GetData_PPM(void)
{
	float  tempData = 0;
	

	for (uint8_t i = 0; i < MQ135_READ_TIMES; i++)
	{
		tempData += MQ135_ADC_Read();
		Delay_ms(5);
	}
	tempData /= MQ135_READ_TIMES;
	
	float Vol = (tempData*5/4096);
	float RS = (5-Vol)/(Vol*0.5);
//	Vol是输出电压也就是负载RL的电压，RL=0.5KΩ，RS是传感器的气敏电阻，Vcc是电源电压
//	Vol = [Vcc / (RS + RL)] × RL
//	Vol = (Vcc × RL) / (RS + RL)
//	Vol × (RS + RL) = Vcc × RL
//	Vol × RS + Vol × RL = Vcc × RL
//	Vol × RS = Vcc × RL - Vol × RL
//	Vol × RS = RL × (Vcc - Vol)
//	RS = [RL × (Vcc - Vol)] / Vol
//	通过计算RS，可以得到传感器在当前环境下的实际电阻值，然后通过与洁净空气中的基准电阻R0比较，最终计算出气体浓度PPM值。
	
	float R0=6.64;
	
	float ppm = pow(11.5428*R0/RS, 0.6549f);
	
	return ppm;
}
