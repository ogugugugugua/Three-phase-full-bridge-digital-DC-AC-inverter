#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
#define PWM_Dead_Time 17//单位为系统时钟周期PWM_DEAD_TIME<127
#define PWM_Period 4800//72MHZ
#define PWM_Period_TIM8 2400
#define PWM_Dead_Time_TIM8 17
void TIM1_PWM_Init(u32 arr,u32 psc);
void TIM8_PWM_Init(u32 arr,u32 psc);
#endif






//LOOK AT HERE!!!!!!!!!!!!!
//deadtime 70------400ns
//deadtime 17------100ns
