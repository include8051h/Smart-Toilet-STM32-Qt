#include "Safety.h"

// 全局变量定义
volatile uint32_t safety_timer_counter = 0;//安全事故时钟计数值
volatile uint8_t safety_timer_enabled = 0;//安全事故时钟启动标志
volatile uint8_t safety_target_minutes = 0;//安全事故目标计时时间
volatile uint16_t safety_accident_flag = 0;//安全事故发生标志

/*用volatile修饰全局变量，因为这些变量可能在中断服务程序中被修改，
或者在主循环和中断服务程序中被访问。使用volatile可以确保每次访问变量时都从内存中读取，
而不是使用寄存器中的缓存值，从而保证数据的实时性。*/

/**
  * @brief  初始化安全计时器（使用TIM3）
  * @param  target_minutes: 目标计时分钟数 (1-60)
  * @retval None
  */
void safety_timer_init(uint8_t target_minutes)
{
    // 限制输入范围
    if (target_minutes < 1) target_minutes = 1;//最小计时1分钟
    if (target_minutes > 60) target_minutes = 60;//最大计时60分钟
    
    safety_target_minutes = target_minutes;
    safety_timer_counter = 0;
    safety_timer_enabled = 0;
    safety_accident_flag = 0;
    
    // 开启TIM3时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    
    // 定时器配置
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 9999;           // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;        // 预分频值 
    // 72MHz / (7199+1) = 10kHz, 10kHz / 10000 = 1Hz (1秒)
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;//
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    // 使能定时器中断
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    
    // 配置NVIC
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    // 启动定时器（但不立即开始计数）
    TIM_Cmd(TIM3, ENABLE);
}

/**
  * @brief  启动安全计时器
  * @param  None
  * @retval None
  */
void safety_timer_start(void)
{
    safety_timer_counter = 0;
    safety_timer_enabled = 1;
    safety_accident_flag = 0;
}

/**
  * @brief  停止安全计时器
  * @param  None
  * @retval None
  */
void safety_timer_stop(void)
{
    safety_timer_enabled = 0;
}

/**
  * @brief  重置安全计时器
  * @param  None
  * @retval None
  */
void safety_timer_reset(void)
{
    safety_timer_counter = 0;
    safety_accident_flag = 0;
}

/**
  * @brief  更改计时目标时间
  * @param  target_minutes: 新的目标分钟数 (1-60)
  * @retval None
  */
void safety_timer_set_target(uint8_t target_minutes)
{
    if (target_minutes < 1) target_minutes = 1;
    if (target_minutes > 60) target_minutes = 60;
    
    safety_target_minutes = target_minutes;
    safety_timer_reset();
}

/**
  * @brief  获取当前计时状态
  * @param  None
  * @retval 已计时的秒数
  */
uint32_t safety_timer_get_elapsed(void)
{
    return safety_timer_counter;
}

/**
  * @brief  获取剩余时间
  * @param  None
  * @retval 剩余秒数
  */
/*uint32_t safety_timer_get_remaining(void)
{
    uint32_t target_seconds = safety_target_minutes * 60;
    if (safety_timer_counter >= target_seconds) {
        return 0;
    }
    return target_seconds - safety_timer_counter;
}*/

/**
  * @brief  获取安全事故标志
  * @param  None
  * @retval 1:发生安全事故, 0:正常
  */
uint16_t safety_timer_get_accident_flag(void)
{
    return safety_accident_flag;
}

/**
  * @brief  清除安全事故标志（管理员操作）
  * @param  None
  * @retval None
  */
void safety_timer_clear_accident_flag(void)
{
    safety_accident_flag = 0;
}

/**
  * @brief  TIM3中断处理函数
  * @param  None
  * @retval None
  */
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        
        // 如果计时器使能，则计数
        if (safety_timer_enabled) {
            safety_timer_counter++;
            
            // 检查是否超时
            if (safety_timer_counter >= (safety_target_minutes * 60)) {
                safety_accident_flag = 1;
								safety_timer_stop();
								//safety_timer_reset();
            }
        }
    }
}
