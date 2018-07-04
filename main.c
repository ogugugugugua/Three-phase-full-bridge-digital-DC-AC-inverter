#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "adc.h"
#include "dac.h"
#include "key.h"
#include "stdio.h"
#include "timer.h"
#include "control.h"
#include "pwm.h"
#include "oled.h"
#define Kp 40.0
#define Ki 20.0
void double_array_init(double* arr,double init_value,u8 n)
{
	u8 i;
	for(i = 0;i < n;i++)
	{
			arr[i] = init_value;
	}
}
void ADC_Value_Filter(u16* Adc_Value,u16* Adc_Value_After_Filter)//ADC数据滤波
{
	unsigned char i;
	for(i=0;i<6;i++)
	{
		Adc_Value_After_Filter[i]+=Adc_Value[i];
	}
	for(i=0;i<6;i++)
	{
		Adc_Value_After_Filter[i]/=2;
	}
}
u16  Adc_Value[6];//临时存放ADC值
//	Adc_Value0：ADC1_CH_3					Adc_Value3:ADC1_CH_5
//	Adc_Value1:	ADC2_CH_6					Adc_Value4:ADC2_CH_10
//	Adc_Value2:	ADC3_CH_11				Adc_Value5:ADC3_CH_12
u16  Adc_Value_After_Filter[6];//存放ADC滤波后的值,程序使用这个
//	Adc_Value_After_Filter[0]：ADC1_CH_3					Adc_Value_After_Filter[3]:ADC1_CH_5
//	Adc_Value_After_Filter[1]:	ADC2_CH_6					Adc_Value_After_Filter[4]:ADC2_CH_10
//	Adc_Value_After_Filter[2]:	ADC3_CH_11				Adc_Value_After_Filter[5]:ADC3_CH_12
int registe=0;
u16 ADC_CH3_=0;
u16 ADC_CH5_=0;
u16 ADC_CH6_=0;
int main(void)
{
	char str_buf[60];
	float Tabel_Error[Error_Buf_Size+1] = {8.506461,-16.872492,8.435995};
	float Tabel_Output[Output_Buf_Size] = {1.454446,-0.454446};
	float Target_Voltage = 25;
	SystemInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	Control_Init(&control,Target_Voltage,Tabel_Error,Tabel_Output);
	delay_init(168);  	//初始化延时函数
	uart_init(115200);	//初始化串口波特率为115200
	IIC_Init();					//与OLED初始化配合使用
	OLED_Init();				//与IIC初始化配合使用
	Adc_Init(); 				//adc初始化	：	3个ADC，每个ADC有2通道
	LED_Init();					//初始化LED 	
	
	TIM1_PWM_Init(500-1,84-1);	//此处延续了原子的程序，实际上这里的参数并不起作用，PWM的周期和分频在pwm.h中设置
	TIM1->CCR1=1000;						//占空比初始化
	TIM1->CCR2=1000;						//占空比初始化
//	TIM8_PWM_Init(500-1,84-1);	//初始化TIM8后TIM1_CH2N输出将会变成TIM8_CH2N，可以根据需要初始化
//	TIM8->CCR2=1000;						//占空比初始化

	while(1){
				sprintf(str_buf,"%4d",Adc_Value[0]);
		OLED_P6x8Str(0,0,str_buf);
				sprintf(str_buf,"%4d",Adc_Value[1]);
		OLED_P6x8Str(0,1,str_buf);
				sprintf(str_buf,"%4d",Adc_Value[2]);
		OLED_P6x8Str(0,2,str_buf);
				sprintf(str_buf,"%4d",Adc_Value[3]);
		OLED_P6x8Str(0,3,str_buf);
				sprintf(str_buf,"%4d",Adc_Value[4]);
		OLED_P6x8Str(0,4,str_buf);
				sprintf(str_buf,"%4d",Adc_Value[5]);
		OLED_P6x8Str(0,5,str_buf);
		delay_ms(500);
	}
	
}

