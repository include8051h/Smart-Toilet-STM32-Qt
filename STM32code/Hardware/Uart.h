#ifndef __UART_H
#define __UART_H

#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <string.h>
#include <LED.h>
#include "Servo.h"
#include "Waterbom.h"
#include "Wind.h"

extern uint8_t RX_buf[];	//串口接收到的数据存储到数组中
extern uint16_t RX_cnt;	//计数
//extern uint8_t RX_Flag;	//接收标志位
extern uint8_t Ctrl_Light;	//管理员控制灯命令
extern char Light_State[];	//灯状态

extern char door_current_state[];
extern uint8_t Ctrl_Door;

extern char waterbom_current_state[];
extern uint8_t Ctrl_Waterbom;

extern char wind_current_state[];
extern uint8_t Ctrl_Wind;

extern char lightsensor_current_state[];

extern char watersensor_current_state[];

extern char airsensor_current_state[];

extern char accident_state[];
extern uint8_t accident_gotit;

extern char toilet_state[];

void Uart_Init(void);
void USART_SendString(USART_TypeDef* USARTx, char *str);
void send_sensor_data(void);


#endif
