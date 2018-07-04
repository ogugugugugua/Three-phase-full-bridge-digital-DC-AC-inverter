#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
#define Vout ADC_CH3_
#define Vin ADC_CH5_
#define Iout ADC_CH6_
extern u16 ADC_CH3_;//全局变量存放ADC通道3的值，用于在中断中获取
extern u16 ADC_CH5_;//全局变量存放ADC通道5的值，用于在中断中获取
extern u16 ADC_CH6_;//全局变量存放ADC通道6的值，用于在中断中获取
extern u16 AD_TIME;
void TIM3_Int_Init(u16 arr,u16 psc);
#endif
