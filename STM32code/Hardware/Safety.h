#ifndef __SAFETY_H
#define __SAFETY_H

#include "stm32f10x.h"

enum DoorState
{
	DoorState_Opened,
	DoorState_Closed
};

void safety_timer_init(uint8_t target_minutes);
void safety_timer_start(void);
void safety_timer_stop(void);
void safety_timer_reset(void);
void safety_timer_set_target(uint8_t target_minutes);
uint32_t safety_timer_get_elapsed(void);
//uint32_t safety_timer_get_remaining(void);
uint16_t safety_timer_get_accident_flag(void);
void safety_timer_clear_accident_flag(void);

#endif
