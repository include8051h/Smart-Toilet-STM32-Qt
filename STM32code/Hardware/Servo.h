#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"                  // Device header
#include "Delay.h"

#define Servo_Pin GPIO_Pin_1
#define Servo_Port GPIOA

// ÃÅ×´Ì¬¶¨Òå
typedef enum {
    Door_Closed = 0,
    Door_InToOut_Opening,
    Door_InToOut_Opened,
    Door_OutToIn_Closing,
    Door_OutToIn_Opening,
    Door_OutToIn_Opened,
    Door_InToOut_Closing
} DoorState;

void Servo_Init(void);
void PWM_SetCompare2(uint16_t compare);
void Servo_SetAngle(float angle);
void DoorControl_OutToIn_Open(void);
void DoorControl_InToOut_Close(void);
void DoorControl_InToOut_Open(void);
void DoorControl_OutToIn_Close(void);
DoorState GetCurrentDoorState(void);

#endif
