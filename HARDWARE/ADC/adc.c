#include "adc.h"
#include "delay.h"		
#include "lcd.h"
#include "stdio.h"
#include "oled.h"
#define Filter_Buf_N 10
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


//两路ADC，每路3个通道														   
//void  Adc_Init(void)
//{    
// 
//  GPIO_InitTypeDef  		GPIO_InitStructure;
//	ADC_CommonInitTypeDef ADC_CommonInitStructure;
//	ADC_InitTypeDef       ADC_InitStructure;
//	DMA_InitTypeDef 			DMA_InitStructure;
//	
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟 
//  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//使能ADC1时钟
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
//	
//		
////	DMA_DeInit(DMA2_Stream0);//官方文件并没有，但是应该没有多大关系
//	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
//	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC_CCR_ADDRESS;//(u32)&ADC1->DR;//DMA外设地址
//  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&Adc_Value;//DMA 存储器0地址
//  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//外设到存储器模式
//  DMA_InitStructure.DMA_BufferSize = 6;//数据传输量
//  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
//  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
//  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据长度:8位
//  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//存储器数据长度:8位
//  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// 使用循环模式 / DMA_Mode_Normal
//  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//高等优先级
//  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
//  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
//  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//  DMA_Init(DMA2_Stream0, &DMA_InitStructure);//初始化DMA Stream
//	
//  DMA_Cmd(DMA2_Stream0, ENABLE); 	
//	
////初始化IO口
//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4 | GPIO_Pin_3 | GPIO_Pin_2 | GPIO_Pin_1 | GPIO_Pin_0 | GPIO_Pin_6 | GPIO_Pin_7 ;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_DOWN;//下拉 GPIO_PuPd_NOPULL
//  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 
//	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_2  | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_0;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_DOWN;//下拉
//  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化 	
// 
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	//ADC1复位
//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
// 
// 
//  ADC_CommonInitStructure.ADC_Mode = ADC_DualMode_RegSimult;//规则组同时转换模式
//  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
//  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;//ADC_DMAAccessMode_Disabled;// //DMA使能
//  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2; //APB 84MHz,ADC 21MHz
//  ADC_CommonInit(&ADC_CommonInitStructure);
//	
//  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
//  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式	
//  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
//  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
//  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
//  ADC_InitStructure.ADC_NbrOfConversion = 3;//3个转换在规则序列中 
//  ADC_Init(ADC1, &ADC_InitStructure);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_3Cycles ); 
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_3Cycles ); 
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 3, ADC_SampleTime_3Cycles ); 

//	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
//  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式	
//  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
//  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
//  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
//  ADC_InitStructure.ADC_NbrOfConversion = 3;//3个转换在规则序列中 
//  ADC_Init(ADC2, &ADC_InitStructure);
//	ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 1, ADC_SampleTime_3Cycles ); 
//	ADC_RegularChannelConfig(ADC2, ADC_Channel_11, 2, ADC_SampleTime_3Cycles ); 
//	ADC_RegularChannelConfig(ADC2, ADC_Channel_12, 3, ADC_SampleTime_3Cycles ); 

//	//ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
//	//ADC_Cmd(ADC1, ENABLE);//开启AD转换器	 
//	//ADC_DMACmd(ADC1, ENABLE);
//	
//	ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
//	
//  ADC_Cmd(ADC1, ENABLE);
//  ADC_Cmd(ADC2, ENABLE);
//	
//	ADC_SoftwareStartConv(ADC1);

//}

//三路ADC，每路2个通道
void  Adc_Init(void)
{    
 
  GPIO_InitTypeDef  		GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef 			DMA_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);//使能ADC1时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
		
//	DMA_DeInit(DMA2_Stream0);//官方文件并没有，但是应该没有多大关系
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC_CCR_ADDRESS;//(u32)&ADC1->DR;//DMA外设地址
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&Adc_Value;//DMA 存储器0地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//外设到存储器模式
  DMA_InitStructure.DMA_BufferSize = 6;//数据传输量
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据长度:8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//存储器数据长度:8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// 使用循环模式 / DMA_Mode_Normal
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//高等优先级
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);//初始化DMA Stream
	
  DMA_Cmd(DMA2_Stream0, ENABLE); 	
	
//初始化IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5  | GPIO_Pin_3 | GPIO_Pin_6  ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_DOWN;//下拉 GPIO_PuPd_NOPULL
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_2  | GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//GPIO_PuPd_DOWN;//下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化 	
 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	//ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束	 
 
 
  ADC_CommonInitStructure.ADC_Mode = ADC_TripleMode_RegSimult;//规则组同时转换模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;//ADC_DMAAccessMode_Disabled;// //DMA使能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4; //APB 84MHz,ADC 21MHz
  ADC_CommonInit(&ADC_CommonInitStructure);
	
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 2;//2个转换在规则序列中 
  ADC_Init(ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_15Cycles ); 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 2, ADC_SampleTime_15Cycles ); 

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 2;//2个转换在规则序列中 
  ADC_Init(ADC2, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC2, ADC_Channel_6, 1, ADC_SampleTime_15Cycles ); 
	ADC_RegularChannelConfig(ADC2, ADC_Channel_10, 2, ADC_SampleTime_15Cycles ); 

	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;//扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 2;//2个转换在规则序列中 
  ADC_Init(ADC3, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC3, ADC_Channel_11, 1, ADC_SampleTime_15Cycles ); 
	ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 2, ADC_SampleTime_15Cycles ); 

	//ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE);
	//ADC_Cmd(ADC1, ENABLE);//开启AD转换器	 
	//ADC_DMACmd(ADC1, ENABLE);
	
	ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
	
  ADC_Cmd(ADC1, ENABLE);
  ADC_Cmd(ADC2, ENABLE);
  ADC_Cmd(ADC3, ENABLE);	
	
	ADC_SoftwareStartConv(ADC1);
	ADC_SoftwareStartConv(ADC3);
}	

//获得ADC值
//ch:通道值 0~16
//返回值:转换结果
u16 Get_Adc(u8 ch)   
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度			    
  
	ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}
//获取通道ch的转换值，取times次,然后平均 
//ch:通道编号
//times:获取次数
//返回值:通道ch的times次转换结果平均值

// @brief : 滑动平均滤波
// @param :
//	buf			: 数据缓冲区
//	new_data:	添加到数据缓冲区的新数据
//	n				: 数据缓冲区长度
// @ret		: 滤波后得出的计算结果
double filter(double* buf,double new_data,u8 n)
{
	u8 i;
	double sum = 0;
	double max = buf[0];
	double min = buf[0];
	for(i = 0;i < n-1;i++)
	{
			buf[i] = buf[i+1];
			sum += buf[i];
		if(buf[i]>max) max = buf[i];
		if(buf[i]<min) min = buf[i];
	}
	buf[i] = new_data;
	sum += buf[i];
	if(buf[i]>max) max = buf[i];
	if(buf[i]<min) min = buf[i];
	return (sum-max-min)/(n-2);
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_us(20);
	}
	return temp_val/times;
} 
	 
void Change_Channel_And_Display(u8 channel,double* adc_cal_value)
{
		static double filter_buf[4][Filter_Buf_N];
		u16 adcx=0;
		char str_buf[30];
		//float temp=0;
		double temp=0;	
		switch(channel)
		{
			case 6://Select channel ONE and display its voltage
			{	adcx=Get_Adc_Average(ADC_Channel_6,10);		//得到ADC转换值	  
				temp=(float)adcx*(3.3/4096);			        //得到ADC电压值
				adc_cal_value[0] = temp=filter(filter_buf[0],20*(temp+0.197)/2.45,Filter_Buf_N);
				sprintf(str_buf,"Vi VOL:%2.2fV",temp);
				LCD_ShowString(30,190,200,16,16,(u8*)str_buf);//CH0
			}
			case 3://Select channel TWO and display its voltage
			{	adcx=Get_Adc_Average(ADC_Channel_3,10);		//得到ADC转换值	  
				temp=(float)adcx*(3.3/4096);			        //得到ADC电压值
				adc_cal_value[1] = temp=filter(filter_buf[1],30*(temp+0.197)/2.428,Filter_Buf_N);
				sprintf(str_buf,"Vo VOL:%2.2fV",temp);
				LCD_ShowString(30,210,200,16,16,(u8*)str_buf);//CH0
			}
			case 11://Select channel FIVE and display its current
			{	adcx=Get_Adc_Average(ADC_Channel_11,10);		//得到ADC转换值	  
				temp=(float)adcx*(3.3/4096);			        //得到ADC电压值
				adc_cal_value[2] = temp=filter(filter_buf[2],(2.42-temp)/0.184,Filter_Buf_N);
				sprintf(str_buf,"Io VOL:%2.2fA",temp);
				LCD_ShowString(30,230,200,16,16,(u8*)str_buf);//CH0
			}
			case 5://Select channel SIX and display its voltage
			{	adcx=Get_Adc_Average(ADC_Channel_5,10);		//得到ADC转换值	  
				temp=(float)adcx*(3.3/4096);			        //得到ADC电压值
				adc_cal_value[3] = temp=filter(filter_buf[3],(2.41-temp)/0.184,Filter_Buf_N);
				sprintf(str_buf,"Ii VOL:%2.2fA",temp);
				LCD_ShowString(30,250,200,16,16,(u8*)str_buf);//CH0
			
			}
	}
}

void ADC_DMA_Init()
{
		
}





