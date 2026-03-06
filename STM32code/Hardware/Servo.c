#include "Servo.h"

DoorState current_door_state;

void Servo_Init(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);			//开启TIM2的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);			//开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);							//将PA1引脚初始化为复用推挽输出	
																	//受外设控制的引脚，均需要配置为复用模式
	
	/*配置时钟源*/
	TIM_InternalClockConfig(TIM2);		//选择TIM2为内部时钟，若不调用此函数，TIM默认也为内部时钟
	
	/*时基单元初始化*/
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;				//定义结构体变量
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元功能
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //计数器模式，选择向上计数
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;				//计数周期，即ARR的值
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;				//预分频器，即PSC的值
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            //重复计数器，高级定时器才会用到
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);             //将结构体变量交给TIM_TimeBaseInit，配置TIM2的时基单元
	
	/*输出比较初始化*/ 
	TIM_OCInitTypeDef TIM_OCInitStructure;							//定义结构体变量
	TIM_OCStructInit(&TIM_OCInitStructure);                         //结构体初始化，若结构体没有完整赋值
	                                                                //则最好执行此函数，给结构体所有成员都赋一个默认值
	                                                                //避免结构体初值不确定的问题
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //输出比较模式，选择PWM模式1
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //输出极性，选择为高，若选择极性为低，则输出高低电平取反
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输出使能
	TIM_OCInitStructure.TIM_Pulse = 0;								//初始的CCR值
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);                        //将结构体变量交给TIM_OC2Init，配置TIM2的输出比较通道2
	
	/*TIM使能*/
	TIM_Cmd(TIM2, ENABLE);			//使能TIM2，定时器开始运行
}

void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2, Compare);		//设置CCR2的值
}

void Servo_SetAngle(float Angle)
{
	PWM_SetCompare2(Angle / 180 * 2000 + 500);	//设置占空比
												//将角度线性变换，对应到舵机要求的占空比范围上
}

//门由外向内开
void DoorControl_OutToIn_Open(void)
{
    current_door_state = Door_OutToIn_Opening;
    
    // 从90°快速旋转到180°
    for(uint16_t angle = 90; angle <= 180; angle += 2)
    {
        Servo_SetAngle(angle);
        Delay_ms(20); // 快速转动，延时较短
    }
    
    current_door_state = Door_OutToIn_Opened;
}

//门由内向外关（缓慢）
void DoorControl_InToOut_Close(void)
{
    current_door_state = Door_InToOut_Closing;
    
    // 从180°缓慢旋转到90°
   for(uint16_t angle = 180; angle > 90; angle -= 1)
    {
        Servo_SetAngle(angle);
        Delay_ms(25); // 缓慢转动，延时较长
    }
    
    current_door_state = Door_Closed;
}

//门由内向外开（缓慢）
void DoorControl_InToOut_Open(void)
{
		current_door_state = Door_InToOut_Opening;
    
    // 从90°缓慢旋转到0°
    for(int16_t angle = 90; angle >= 0; angle -= 1)//
    {
        Servo_SetAngle(angle);
        Delay_ms(25); // 缓慢转动，延时较长
    }
    
    current_door_state = Door_InToOut_Opened;
}

// 由外向内关门（缓慢）
void DoorControl_OutToIn_Close(void)
{
    current_door_state = Door_OutToIn_Closing;
    
    // 从0°缓慢旋转到90°
   for(uint16_t angle = 0; angle < 90; angle += 1)
    {
        Servo_SetAngle(angle);
        Delay_ms(25); // 缓慢转动，延时较长
    }
    
    current_door_state = Door_Closed;
}

// 获取当前门状态
DoorState GetCurrentDoorState(void)
{
    return current_door_state;
}
