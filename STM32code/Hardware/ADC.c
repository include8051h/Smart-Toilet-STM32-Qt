#include "stm32f10x.h"                  // Device header


void AD_Init(void)
{
	//开启时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//开启ADC1的RCC外设时钟，ADC都是APB2总线上的外设
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//开启GPIOA的RCC外设时钟
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);//开启ADCCLK时钟，需对72MHz进行6分频
	
	//配置GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//选用模拟输出	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;//选中0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//GPIO速度一般选择50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	
	//配置ADC
	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//ADC的模式，独立模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//16为数据存储器，但是ADC数据只有12位
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	//给ADC上电
	ADC_Cmd(ADC1, ENABLE);
	
	//对ADC进行校准
	ADC_ResetCalibration(ADC1);
  while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET);
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//选择软件触发//若选择连续触发，这个函数只需触发一次即可
}

uint16_t AD_GetValue(uint8_t ADC_Channel)
{
	//选择ADC通道
	ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
	//while(ADC_GetSoftwareStartConvStatus(ADC1) == RESET);//等待软件触发AD转换完成
	//当ADC被设为连续触发时，数据寄存器会不断刷新，因此就无需判断标志位了
	return ADC_GetConversionValue(ADC1);
}
