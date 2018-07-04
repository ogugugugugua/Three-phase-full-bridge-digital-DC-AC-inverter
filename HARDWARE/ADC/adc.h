#ifndef __ADC_H
#define __ADC_H	
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//ADC 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2014/5/6
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
extern  u16 Adc_Value[];							   
#define ADC_CCR_ADDRESS    ((uint32_t)0x40012308)
void Adc_Init(void); 				//ADC通道初始化
void ADC_DMA_Init(void);
u16  Get_Adc(u8 ch); 				//获得某个通道值 
u16 Get_Adc_Average(u8 ch,u8 times);//得到某个通道给定次数采样的平均值  
void Change_Channel_And_Display(u8 channel,double* adc_cal_value);
double filter(double* buf,double new_data,u8 n);//滑动平均滤波
#endif 















