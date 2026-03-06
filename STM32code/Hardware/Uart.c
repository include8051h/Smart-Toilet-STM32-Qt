#include "Uart.h"

void Uart_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);//开启RCC时钟
	

	
	GPIO_InitTypeDef GPIO_InitStructure;
		//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10 

	 //USART 初始化设置
	USART_InitTypeDef USART_InitStructure;
	USART_DeInit(USART1);//串口复位
	USART_InitStructure.USART_BaudRate = 9600;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1

	NVIC_InitTypeDef NVIC_InitStructure;
	//Usart1 NVIC 配置
 	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
	USART_ITConfig(USART1, USART_IT_TXE, DISABLE);//开启串口接受中断
	
	
	USART_Cmd(USART1, ENABLE);         //使能串口1
	
	USART1->SR = ~(0x00F0);

}

#define RX_len 256 //数组大小
uint8_t RX_buf[RX_len];	//串口接收到的数据存储到数组中
uint16_t RX_cnt = 0;	//计数
//uint8_t RX_Flag = 0;	//接收标志位

uint8_t Ctrl_Light = 'N';
char Light_State[20] = "off#";

char door_current_state[20] = "inclose#";
uint8_t Ctrl_Door = 'N';

char waterbom_current_state[20] = "off#";
uint8_t Ctrl_Waterbom = 'N';

char wind_current_state[20] = "off#";
uint8_t Ctrl_Wind = 'N';

char lightsensor_current_state[20] = "unknown#";

char watersensor_current_state[20] = "unknown#";

char airsensor_current_state[20] = "unknown#";

char accident_state[20] = "safety#";
uint8_t accident_gotit = 'N';

char toilet_state[10] = "free#";

void send_sensor_data(void)
{
    char send_buffer[256];
    
    // 发送光敏传感器数据
	sprintf(send_buffer,
	"light_state:%sdoor_state:%swaterbom_state:%swind_state:%slightsensor_state:%swatersensor_state:%sairsensor_state:%saccident_state:%stoilet_state:%s", 
		Light_State, door_current_state, waterbom_current_state, 
	wind_current_state, lightsensor_current_state, watersensor_current_state,
	airsensor_current_state, accident_state, toilet_state);  // Light_State已经是"on#"或"off#"格式
    USART_SendString(USART1, send_buffer);
    
    // 可以添加延时避免发送过于频繁
    // Delay_ms(1000);
}

// 实现发送函数
void USART_SendString(USART_TypeDef* USARTx, char *str)
{
    while(*str != '\0')
    {
        USART_SendData(USARTx, *str);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
        str++;
    }
}

//串口接收中断
void USART1_IRQHandler()
{
	uint8_t receive;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		USART_ClearFlag(USART1, USART_IT_RXNE);//清除标志位
		
		receive = USART_ReceiveData(USART1);
		
		RX_buf[RX_cnt] = receive;
		
		RX_cnt++;
		
		if(RX_cnt > 256)
		{
			return;
		}
		
		if(RX_buf[RX_cnt-1] == '#')
		{
		//利用串口接收中断实现Qt控制LED灯亮灭
		if(strcmp((const char*)RX_buf,"Cmd_Light_On#") == 0)
		{
			Ctrl_Light = 'Y';
			LED_ON();
			memset(Light_State, 0, 20);
			strcpy(Light_State, "on#");
		}
		else if(strcmp((const char*)RX_buf,"Cmd_Light_Off#") == 0)
		{
			Ctrl_Light = 'Y';
			LED_OFF();
			memset(Light_State, 0, 20);
			strcpy(Light_State, "off#");
		}
		else if(strcmp((const char*)RX_buf,"Cmd_Light_Auto#") == 0)
		{
			Ctrl_Light = 'N';
		}
		
		//门
		else if(strcmp((const char*)RX_buf,"Cmd_Door_Open#") == 0)
		{
			Ctrl_Door = 'Y';
			if(strcmp(door_current_state, "outclose#") == 0)
			{
				DoorControl_InToOut_Open();
				memset(door_current_state, 0, sizeof(door_current_state));
				strcpy(door_current_state, "outopen#");
			}
			else if(strcmp(door_current_state, "inclose#") == 0)
			{
				DoorControl_OutToIn_Open();
				memset(door_current_state, 0, sizeof(door_current_state));
				strcpy(door_current_state, "inopen#");
			}
		}
		else if(strcmp((const char*)RX_buf,"Cmd_Door_Close#") == 0)
		{
			Ctrl_Door = 'Y';
			if(strcmp(door_current_state, "outopen#") == 0)
			{
				DoorControl_OutToIn_Close();
				memset(door_current_state, 0, sizeof(door_current_state));
				strcpy(door_current_state, "inclose#");
			}
			else if(strcmp(door_current_state, "inopen#") == 0)
			{
				DoorControl_InToOut_Close();
				memset(door_current_state, 0, sizeof(door_current_state));
				strcpy(door_current_state, "outclose#");
			}
		}
		else if(strcmp((const char*)RX_buf,"Cmd_Door_Auto#") == 0)
		{
			Ctrl_Door = 'N';
		}
		
		//水泵
		else if(strcmp((const char*)RX_buf,"Cmd_Waterbom_On#") == 0)
		{
			Ctrl_Waterbom = 'Y';
			Waterbom_ON();
			memset(waterbom_current_state, 0, 20);
			strcpy(waterbom_current_state, "on#");
		}
		else if(strcmp((const char*)RX_buf,"Cmd_Waterbom_Off#") == 0)
		{
			Ctrl_Waterbom = 'Y';
			Waterbom_OFF();
			memset(waterbom_current_state, 0, 20);
			strcpy(waterbom_current_state, "off#");
		}
		else if(strcmp((const char*)RX_buf,"Cmd_Waterbom_Auto#") == 0)
		{
			Ctrl_Waterbom = 'N';
		}
		
		//通风
		else if(strcmp((const char*)RX_buf,"Cmd_Wind_On#") == 0)
		{
			Ctrl_Wind = 'Y';
			Wind_ON();
			memset(wind_current_state, 0, 20);
			strcpy(wind_current_state, "on#");
		}
		else if(strcmp((const char*)RX_buf,"Cmd_Wind_Off#") == 0)
		{
			Ctrl_Wind = 'Y';
			Wind_OFF();
			memset(wind_current_state, 0, 20);
			strcpy(wind_current_state, "off#");
		}
		else if(strcmp((const char*)RX_buf,"Cmd_Wind_Auto#") == 0)
		{
			Ctrl_Wind = 'N';
		}
		else if(strcmp((const char*)RX_buf,"Cmd_Accident#") == 0)
		{
			accident_gotit = 'Y';
		}
		
//		else if(strcmp((const char*)RX_buf,"Cmd_Light_Off#") == 0)
//		{
//			Ctrl_Light = 'Y';
//			LED_OFF();
//			memset(Light_State, 0, 20);
//			strcpy(Light_State, "off#");
//		}
//		else if(strcmp((const char*)RX_buf,"Cmd_Light_Auto#") == 0)
//		{
//			Ctrl_Light = 'N';
//		}
		
		RX_cnt = 0;
		memset(RX_buf, 0, RX_len);
	}
	}
}

