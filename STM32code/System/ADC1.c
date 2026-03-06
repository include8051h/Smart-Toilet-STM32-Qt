#include "ADC1.h"

void ADC1_Init(void)
{   
    // 使能时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    // ADC统一配置 - 使用单次转换模式，便于多通道切换
		ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;  // 是否使能连续转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_NbrOfChannel = 1;//扫描模式下，规则组通道需要扫描的数量
    ADC_Init(ADC1, &ADC_InitStructure);
    
    // 校准（只执行一次）
    ADC_Cmd(ADC1, ENABLE);
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
	
		//ADC_SoftwareStartConvCmd(ADC1, ENABLE);//选择软件触发//若选择连续触发，这个函数只需触发一次即可
}

uint16_t ADC1_ReadChannel(uint16_t channel)
{
    // 配置指定通道
    ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_55Cycles5);
	//这里说的是参数3
//	在规则通道组中，可以有多个通道，按照排名顺序依次转换（如果开启了扫描模式）。
//	但是，如果ADC_InitStructure.ADC_NbrOfChannel设置为1，
//	那么即使配置了多个通道，也只转换排名为1的通道。
//	所以，在非扫描模式下，通常只使用一个排名（Rank1），并且每次转换前重新配置通道。
    
    // 启动转换并等待完成
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);//等待软件触发AD转换完成
	//当ADC被设为连续触发时，数据寄存器会不断刷新，因此就无需判断标志位了
    
    return ADC_GetConversionValue(ADC1);
}

