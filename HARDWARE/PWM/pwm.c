#include "pwm.h"
#include "led.h"
#include "usart.h"

//TIM14 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM1_PWM_Init(u32 arr,u32 psc)
{		 					 
	u32 arrr=arr;
	u32 ppsc=psc;
	//结构体
	GPIO_InitTypeDef GPIO_InitStructure;              //gpio
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   //tim
	TIM_OCInitTypeDef  TIM_OCInitStructure;    				//oc
	TIM_BDTRInitTypeDef TIM_BDTRInitStruct;   			  //bdtr
	//时钟使能：定时器时钟，IO口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);  	//TIM1时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTA时钟	
	//IO口复用开启
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource8,GPIO_AF_TIM1); //GPIOA8复用为定时器1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_TIM1); //GPIOA8复用为定时器1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_TIM1); //GPIOB13复用为定时器1
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM1); //GPIOB13复用为定时器1	
	//IO口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;           //GPIOA8
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始TIM1_CH1
	GPIO_ResetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_9);										 //置低
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;           //GPIOB13 
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化TIM1_CHIN
	GPIO_ResetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14);									 //置低
	
	TIM_TimeBaseStructure.TIM_Prescaler=0;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=PWM_Period-1;   //自动重装载值，决定周期
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);//初始化定时器1
	
	TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Disable;             //该位用于当MOE=1且通道为互补输出时，
	//TIM_OSSRState_Disable：当定时器不工作时，禁止OC/OCN输出(OC/OCN使能输出信号=0)；
	//TIM_OSSRState_Enable：当定时器不工作时，一旦CCxE=1或CCxNE=1， OC/OCN首先输出其空闲电平，然后OC/OCN使能输出信号=1
	TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;            //定时器不工作时，禁止OC/OCN输出(OC/OCN使能输出信号=0)；
	TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;                //锁定寄存器全关闭。寄存器无写保护
	TIM_BDTRInitStruct.TIM_DeadTime  = 0x7f&PWM_Dead_Time;               //72mhz 13.89ns*44*=0.6us
	TIM_BDTRInitStruct.TIM_Break = TIM_Break_Disable;                    //TIM1刹车输入失能
	TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_Low;       //TIM1刹车极性高
	TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable; //MOE只能被软件置’1’
	TIM_BDTRConfig(TIM1,&TIM_BDTRInitStruct);                            //设置刹车特性，死去时间，锁电平，OSSI,OSSR状态和

	//初始化TIM1 Channe1 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //互补输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //TIM_OCPolarity输出极性高
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High; //TIM_OCPolarity互补输出极性高
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; //TIM1_OCIdleState选择空闲状态下的非工作状态(MOE=0时设置TIM1输出比较空闲状态）
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset; //MOE = 0时重置互补输出的输出比较空闲状态
	TIM_OCInitStructure.TIM_Pulse = TIM1->CCR1;//不确定
	
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1OC1
	TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
	
	TIM_OCInitStructure.TIM_Pulse = TIM1->CCR2;//不确定
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1OC1
	TIM_OC2PreloadConfig(TIM1, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
	
	TIM_Cmd(TIM1, ENABLE);  //使能TIM1
	TIM_CtrlPWMOutputs(TIM1,ENABLE);//设置MOE位为1，上电后不关闭
  TIM_ARRPreloadConfig(TIM1,ENABLE);//ARPE使能 
					  
}  
void TIM8_PWM_Init(u32 arr,u32 psc)
{		 					 
	u32 arrr=arr;
	u32 ppsc=psc;
	//结构体
	GPIO_InitTypeDef 					GPIO_InitStructure;             //gpio
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;   				//tim
	TIM_OCInitTypeDef 				TIM_OCInitStructure;    				//oc
	TIM_BDTRInitTypeDef 			TIM_BDTRInitStruct;   			  	//bdtr
	//时钟使能：定时器时钟，IO口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	//TIM8时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOB, ENABLE); 	//使能PORTB|C时钟	
	//IO口复用开启
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8); //GPIOC7复用为定时器8
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_TIM8); //GPIOB14复用为定时器8	
	//IO口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;           //GPIOC7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Fast_Speed;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure);              //初始TIM1_CH1
	GPIO_ResetBits(GPIOC , GPIO_Pin_7);										 //置低
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;           //GPIOB14
	GPIO_Init(GPIOB,&GPIO_InitStructure);              //初始化TIM1_CHIN
	GPIO_ResetBits(GPIOB , GPIO_Pin_14);									 //置低
	
	TIM_TimeBaseStructure.TIM_Prescaler=0;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=PWM_Period_TIM8-1;   //自动重装载值，决定周期
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);//初始化定时器8
	
	TIM_BDTRInitStruct.TIM_OSSRState = TIM_OSSRState_Disable;             //该位用于当MOE=1且通道为互补输出时，
	//TIM_OSSRState_Disable：当定时器不工作时，禁止OC/OCN输出(OC/OCN使能输出信号=0)；
	//TIM_OSSRState_Enable：当定时器不工作时，一旦CCxE=1或CCxNE=1， OC/OCN首先输出其空闲电平，然后OC/OCN使能输出信号=1
	TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Disable;            //定时器不工作时，禁止OC/OCN输出(OC/OCN使能输出信号=0)；
	TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;                //锁定寄存器全关闭。寄存器无写保护
	TIM_BDTRInitStruct.TIM_DeadTime  = 0x7f&PWM_Dead_Time_TIM8;               //72mhz 13.89ns*44*=0.6us
	TIM_BDTRInitStruct.TIM_Break = TIM_Break_Disable;                    //TIM1刹车输入失能
	TIM_BDTRInitStruct.TIM_BreakPolarity = TIM_BreakPolarity_Low;       //TIM1刹车极性高
	TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable; //MOE只能被软件置’1’
	TIM_BDTRConfig(TIM8,&TIM_BDTRInitStruct);                            //设置刹车特性，死去时间，锁电平，OSSI,OSSR状态和
	
	//初始化TIM8 Channe2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable; //互补输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //TIM_OCPolarity输出极性高
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCPolarity_High; //TIM_OCPolarity互补输出极性高
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset; //TIM1_OCIdleState选择空闲状态下的非工作状态(MOE=0时设置TIM1输出比较空闲状态）
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset; //MOE = 0时重置互补输出的输出比较空闲状态	
	TIM_OCInitStructure.TIM_Pulse = TIM8->CCR2;//不确定
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM1OC1
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM1在CCR1上的预装载寄存器
	
	TIM_Cmd(TIM8, ENABLE);  //使能TIM1
	TIM_CtrlPWMOutputs(TIM8,ENABLE);//设置MOE位为1，上电后不关闭
  TIM_ARRPreloadConfig(TIM8,ENABLE);//ARPE使能 
					  
}

