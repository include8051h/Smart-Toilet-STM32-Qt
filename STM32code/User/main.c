#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "HW.h"
#include "Safety.h"
#include "Body_HW.h"
#include "Servo.h"
#include "MQ135.h"
#include "Water.h"
#include "Light.h"
#include "ADC1.h"
#include "LED.h"
#include "Wind.h"
#include "Lock.h"
#include "Waterbom.h"
#include "Uart.h"


u16 value = 0;
u16 pre_value = 0;

//float ppm = 0;
uint16_t water_value = 0;
char water_state[20] = "none";

uint16_t air_value = 0;
char air_state[20] = "none";

uint16_t hw_state = 0;

uint16_t light_value = 0;
char light_state[20] = "none";

/**
  * @brief  显示安全计时器信息
  * @param  None
  * @retval None
  */
void display_safety_timer_info(void)
{
    //char buffer[20];
    
    // 获取计时信息
    uint32_t elapsed_seconds = safety_timer_get_elapsed();

    uint16_t accident_flag = safety_timer_get_accident_flag();
    
    // 计算分钟和秒
    uint32_t elapsed_minutes = elapsed_seconds / 60;
    uint32_t elapsed_secs = elapsed_seconds % 60;
    

    
    // 显示已计时时间
    OLED_ShowString(2, 1, "Elapsed:        ");
    OLED_ShowNum(2, 9, elapsed_minutes, 2);
    OLED_ShowString(2, 11, "m");
    OLED_ShowNum(2, 12, elapsed_secs, 2);
    OLED_ShowString(2, 14, "s");
    

    
    // 显示事故标志
    OLED_ShowString(3, 1, "Accident Flag:  ");
    OLED_ShowNum(3, 16, accident_flag, 1);
}


int main(void)
{ 
	ADC1_Init();
	// 配置优先级分组为2位抢占，2位响应
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	HW_Init();
	
	OLED_Init();
	safety_timer_init(1);
	Servo_Init();
	Light_Init();
	MQ135_Init();
	Water_Init();
	LED_Init();
	Wind_Init();
	//Lock_Init();
	Waterbom_Init();
	Uart_Init();
	
	
	Servo_SetAngle(90);
	//strcpy(door_current_state, "inclose#");
	BODY_HW_Init();
	Delay_s(2);
  while (1)
  {
		OLED_Clear();

		if(accident_gotit == 'Y')
		{
		DoorControl_OutToIn_Close();
			memset(accident_state, 0, 20);
		strcpy(accident_state, "safety#");
		
		accident_gotit = 'N';
		memset(toilet_state, 0, 20);
		strcpy(toilet_state, "free#");
		
		send_sensor_data();
		}
		else
		{
			send_sensor_data();
		}
//		value = BODY_HW_GetData();
//		Delay_ms(25);
		//if(value != pre_value)//HC传感器检测值与上一个检测值不同
		//{
			
			while(hw_state != 1)
			{
				hw_state = HW_GetData_OUT();
			}
			if(hw_state == 1)//if(value == 1)//HC检测值从无人到有人
			{
				OLED_Clear();
				OLED_ShowString(1, 1, "OneCome");
				
				if(Ctrl_Door == 'N')
				{
				DoorControl_OutToIn_Open();//由外向里开门
				memset(door_current_state, 0, 20);
				strcpy(door_current_state, "inopen#");
				send_sensor_data();
				}
				
				light_value = Light_IsDark();
		
		if(Ctrl_Light == 'N')
		{
			switch(light_value)
			{
				case 0:
					memset(lightsensor_current_state, 0, 20);
					strcpy(lightsensor_current_state, "light#");
//					memset(light_state, 0, sizeof(light_state));
//					strcpy(light_state, "Light");
					LED_OFF();
					memset(Light_State, 0, 20);
					strcpy(Light_State, "off#");
					send_sensor_data();
					break;
				case 1:
					memset(lightsensor_current_state, 0, 20);
					strcpy(lightsensor_current_state, "dark#");
//					memset(light_state, 0, sizeof(light_state));
//					strcpy(light_state, "Dark");
					LED_ON();
					memset(Light_State, 0, 20);
					strcpy(Light_State, "on#");
					send_sensor_data();
			}
		}
					
				safety_timer_set_target(1);
				safety_timer_start();
				
				while(safety_timer_get_accident_flag() != 1 && hw_state != 1)//保持开门状态直到超时或遮挡门外传感器
				{
					hw_state = HW_GetData_OUT();
				}
				
				if(hw_state == 1)
				{
					hw_state = 0;
					
					safety_timer_stop();
					safety_timer_reset();
					
					OLED_Clear();
					OLED_ShowString(1, 1, "HW OUT");
					
					safety_timer_set_target(1);
					safety_timer_start();
					
					while(safety_timer_get_accident_flag() != 1 && hw_state != 1)//等待门内传感器被遮挡或超时
					{
						hw_state = HW_GetData_IN();
					}
					
					if(hw_state == 1)//遮挡门内传感器
					{
						//hw_state = 0;
						
						safety_timer_stop();
						safety_timer_reset();
						
						OLED_Clear();
						OLED_ShowString(1, 1, "HW IN");
						
						safety_timer_set_target(1);
						safety_timer_start();
						
						while(safety_timer_get_accident_flag() != 1 && hw_state == 1)//等待使用者后脚不再遮挡门外传感器
						{
							hw_state = HW_GetData_OUT();
						}
						
						if(hw_state == 0)//使用者不再遮挡门外传感器
						{
							safety_timer_stop();
							safety_timer_reset();
						
							OLED_Clear();
							OLED_ShowString(1, 1, "fHW OUT");
							
							safety_timer_set_target(1);
							safety_timer_start();
							
							hw_state = 1;
							
							while(safety_timer_get_accident_flag() != 1 && hw_state == 1)//等待使用者后脚不再遮挡门外传感器
							{
								hw_state = HW_GetData_IN();
							}
							
							if(hw_state == 0)//使用者不遮挡门内传感器完全进入
							{
								safety_timer_stop();
								safety_timer_reset();
							
								OLED_Clear();
								OLED_ShowString(1, 1, "fHW IN");
								
								if(Ctrl_Door == 'N')
							{
								DoorControl_InToOut_Close();
								memset(door_current_state, 0, 20);
								strcpy(door_current_state, "outclose#");
								send_sensor_data();
							}
								//锁门
							memset(toilet_state, 0, 20);
							strcpy(toilet_state, "using#");
							send_sensor_data();
							Delay_ms(25);
								
								//如厕时间计时
								safety_timer_set_target(1);
								safety_timer_start();
							
							OLED_Clear();
			
								//如厕
								while(1)
								{
									display_safety_timer_info();
									if(safety_timer_get_accident_flag() == 1)//如厕时间超时
									{
										safety_timer_stop();
										safety_timer_reset();
										
										OLED_Clear();
										OLED_ShowString(2, 1, "DANGER");
										
										memset(accident_state, 0, 20);
										strcpy(accident_state, "danger#");
										send_sensor_data();
										
										while(accident_gotit == 'N');
										
										if(accident_gotit == 'Y')
										{
											DoorControl_InToOut_Open();
											memset(door_current_state, 0, 20);
											strcpy(door_current_state, "open#");
											send_sensor_data();
											break;
										}
									}
									else
									{
										if(HW_GetData_IN() == 1)
										{
											safety_timer_stop();
										safety_timer_reset();
											break;
										}
									}
								}
								if(accident_gotit == 'Y')
								{
									continue;
								}
								
								//safety_timer_set_target(1);
								//safety_timer_start();
								while(/*safety_timer_get_accident_flag() == 1 && */hw_state != 1)//等待使用者出来时先遮挡门内传感器
								{
									hw_state = HW_GetData_IN();
								}
								
								//if(hw_state == 1)//门内传感器在使用者要离厕时先把遮挡
								//{
//									safety_timer_stop();
//									safety_timer_reset();
								
									OLED_Clear();
									OLED_ShowString(1, 1, "eHW IN");
								
								//停止如厕时间计时
								safety_timer_stop();
								safety_timer_reset();
								
								//开锁
									
								if(Ctrl_Door == 'N')
							{
									DoorControl_InToOut_Open();
									memset(door_current_state, 0, 20);
									strcpy(door_current_state, "outopen#");
									send_sensor_data();
							}
								
									hw_state = 0;
								
									while(hw_state != 1)//等待使用者离开时前脚遮挡门外传感器
									{
										hw_state = HW_GetData_OUT();
									}
									
										OLED_Clear();
										OLED_ShowString(1, 1, "eHW OUT");
									
										while(hw_state == 1)//等待使用者离开时后脚不再遮挡门内传感器
										{
											hw_state = HW_GetData_IN();
										}
										
										OLED_Clear();
										OLED_ShowString(1, 1, "feHW IN");
										
										hw_state = 1;
										
										while(hw_state == 1)//等待使用者离开时两只脚都不再遮挡门外传感器
										{
											hw_state = HW_GetData_OUT();
										}
										
										OLED_Clear();
										OLED_ShowString(1, 1, "feHW OUT");
										
										if(Ctrl_Door == 'N')
									{
										DoorControl_OutToIn_Close();
										memset(door_current_state, 0, 20);
										strcpy(door_current_state, "inclose#");
										send_sensor_data();
									}
									
										Delay_s(2);
										
							memset(toilet_state, 0, 20);
							strcpy(toilet_state, "free#");
							send_sensor_data();
							Delay_ms(25);
									
										//冲水
									if(Ctrl_Waterbom == 'N')
									{
										memset(waterbom_current_state, 0, 20);
										strcpy(waterbom_current_state, "on#");
										send_sensor_data();
										Waterbom_ON();
										
										Delay_s(2);
									}
									if(Ctrl_Waterbom == 'N')
									{
										Waterbom_OFF();
										memset(waterbom_current_state, 0, 20);
										strcpy(waterbom_current_state, "off#");
										send_sensor_data();
										//Delay_s(2);
									}

										
										//检测水位
										water_value = Water_ADC_Read();
										if(water_value >= 2000)
										{
											memset(watersensor_current_state, 0, 20);
											strcpy(watersensor_current_state, "undry#");
//											memset(water_state, 0, sizeof(water_state));
//											strcpy(water_state, "high");
											send_sensor_data();
										}
										else
										{
											memset(watersensor_current_state, 0, 20);
											strcpy(watersensor_current_state, "dry#");
											send_sensor_data();
										}
										OLED_Clear();
										OLED_ShowNum(3, 1, water_value, 4);
										OLED_ShowString(3, 7, water_state);
										Delay_s(2);
										
										//检测气体
										air_value = MQ135_ADC_Read(); 
										if(Ctrl_Wind == 'N')
										{
										if(air_value >= 200)
										{
											//memset(air_state, 0, sizeof(air_state));
											memset(wind_current_state, 0, 20);
											strcpy(wind_current_state, "on#");
											memset(airsensor_current_state, 0, 20);
											strcpy(airsensor_current_state, "unnormal#");
											send_sensor_data();
											Wind_ON();
											//strcpy(air_state, "unnormal");
											while(air_value > 200)
											{
												air_value = MQ135_ADC_Read();
											}
											memset(wind_current_state, 0, 20);
											strcpy(wind_current_state, "off#");
											memset(airsensor_current_state, 0, 20);
											strcpy(airsensor_current_state, "normal#");
											send_sensor_data();
											Wind_OFF();
										}
										else
										{
											//memset(air_state, 0, sizeof(air_state));
											memset(wind_current_state, 0, 20);
											strcpy(wind_current_state, "off#");
											//strcpy(air_state, "normal#");
											memset(airsensor_current_state, 0, 20);
											strcpy(airsensor_current_state, "normal#");
											send_sensor_data();
											Wind_OFF();
										}
									}
										OLED_Clear();
										OLED_ShowNum(4, 1, air_value, 4);
										OLED_ShowString(4, 7, air_state);
										Delay_s(2);
										
										//冲水
										
//										water_high = WATER_GetData();//测水位
//										if(water_high < 1000)
//										{
//											memset(water_state, 0, sizeof(water_state));
//											strcpy(water_state, "normal");
//										}
//										else
//										{
//											memset(water_state, 0, sizeof(water_state));
//											strcpy(water_state, "water");
//										}
//										OLED_ShowNum(3, 1, water_high, 4);
//										//OLED_ShowString(3, 1, water_state);
//										Delay_s(2);
										
										//是否通风
										OLED_ShowNum(4, 1, air_value, 4);
										Delay_s(2);
										
										
								//}
							}
							else//使用者一直遮挡门内传感器至超时
							{
								safety_timer_stop();
								safety_timer_clear_accident_flag();
								safety_timer_reset();
								
								OLED_Clear();
								OLED_ShowString(1, 1, "fHwInTime");
								
								DoorControl_InToOut_Close();
							}
						}
						else//使用者一直遮挡门外传感器至超时
						{
							safety_timer_stop();
							safety_timer_clear_accident_flag();
							safety_timer_reset();
							
							OLED_Clear();
							OLED_ShowString(1, 1, "fHwOutTime");
							
							DoorControl_InToOut_Close();
						}
					}
					else//等待遮挡门内传感器至超时
					{
						safety_timer_stop();
						safety_timer_clear_accident_flag();
						safety_timer_reset();
						
						OLED_Clear();
						OLED_ShowString(1, 1, "HwInTime");
						
						DoorControl_InToOut_Close();
					}
				}
				else//HC传感器开门状态超时
				{
					safety_timer_stop();
					safety_timer_clear_accident_flag();
					safety_timer_reset();
					
					OLED_Clear();
					OLED_ShowString(1, 1, "OneComeTime");
					
					DoorControl_InToOut_Close();
				}
			}
			else//HC检测值从有人到无人
			{
				OLED_Clear();
				OLED_ShowString(1, 1, "OneHasCame");
				
				DoorState door_state_vnp = GetCurrentDoorState();//获取当前门状态
				
				if(door_state_vnp != DoorState_Closed)//如果门没关就把门关上
				{
					Servo_SetAngle(90);
				}
			}
			
//		}
//		else//HC传感器检测值与上一个检测值相同
//		{
//				OLED_Clear();
//				OLED_ShowString(1, 1, "BodyNotTurn");
//				
//				//Servo_SetAngle(90);
//		}
			
			pre_value = value;
			Delay_ms(100);
		}
}




