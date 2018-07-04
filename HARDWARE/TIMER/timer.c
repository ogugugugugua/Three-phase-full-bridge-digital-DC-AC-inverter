#include "timer.h"
#include "adc.h"
#include "dac.h"
#include "control.h"
#include "led.h"

//u16 AD_TIME = 0;



void TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM3,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	static u8 change = 0;
/*	u16 GET=0;
	u8 j=0;
	u16 Temp_MAX = 0; u16 Temp_MIN = 0; u16 Temp_SUM= 0;*/
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		u16 GET=0;
		u8 j=0;
		u16 Temp_MAX = 0; u16 Temp_MIN = 0; u16 Temp_SUM= 0;
		while(TIM1->CNT<=(TIM1->CCR1>>2)){}//此处加上200是为了等待电平跳转稳定，不确定是否需要以及是否起作用，仅作尝试
			/////////////////////////////////////////////////!!!!!!!!!!!!!!!!!!!
			//////////////////////////////////////////

		for(j=0;j<5;j++)//通道3
		{
			ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_3Cycles );	//ADC1,ADC通道,15个周期 
			ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
			while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
			GET = ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
			if(j==0)Temp_MAX=Temp_MIN=GET;//获取首次转换结果
			if(GET>=Temp_MAX)Temp_MAX=GET;//保存最大值
			if(GET<=Temp_MIN)Temp_MIN=GET;//保存最小值
			Temp_SUM+=GET;//求临时总和
		}ADC_CH3_ = (Temp_SUM - Temp_MAX - Temp_MIN)/3;	//获得平均值
		Temp_MAX=Temp_MIN=Temp_SUM=0;//再次初始化
		
		for(j=0;j<5;j++)//通道5
		{
			ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles );	//ADC1,ADC通道,15个周期
			ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
			while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
			GET = ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
			if(j==0)Temp_MAX=Temp_MIN=GET;//获取首次转换结果
			if(GET>=Temp_MAX)Temp_MAX=GET;//保存最大值
			if(GET<=Temp_MIN)Temp_MIN=GET;//保存最小值
			Temp_SUM+=GET;//求临时总和
		}ADC_CH5_ = (Temp_SUM - Temp_MAX - Temp_MIN)/3;	//获得平均值
		Temp_MAX=Temp_MIN=Temp_SUM=0;//再次初始化
	
		for(j=0;j<5;j++)//通道6
		{
			ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_3Cycles );	//ADC1,ADC通道,15个周期
			ADC_SoftwareStartConv(ADC1);		//使能指定的ADC1的软件转换启动功能	
			while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束
			GET = ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
			if(j==0)Temp_MAX=Temp_MIN=GET;//获取首次转换结果
			if(GET>=Temp_MAX)Temp_MAX=GET;//保存最大值
			if(GET<=Temp_MIN)Temp_MIN=GET;//保存最小值
			Temp_SUM+=GET;//求临时总和
		}ADC_CH6_ = (Temp_SUM - Temp_MAX - Temp_MIN)/3;	//获得平均值
	  Temp_MAX=Temp_MIN=Temp_SUM=0;//再次初始化
		
		
		///////////////////
		//此处加入控制计算
		///////////////////
		//计算好后修改TIM1->CCR1
		///////////////////
		control.Error = control.Target - Vout;
		TIM1->CCR1 = Control_Output(&control,Vin);
		 
//		AD_TIME--;
//		if(AD_TIME) Time_Out(AD_TIME);
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}
