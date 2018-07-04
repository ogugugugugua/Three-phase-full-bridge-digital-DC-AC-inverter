#ifndef CONTROL_H_
#define CONTROL_H_

#define Error_Buf_Size 2
#define Output_Buf_Size 2

// 下面这两行到时候处理 
typedef unsigned short u16;
typedef unsigned char u8;


// 定义参数表大小与缓冲区大小相同可以方便处理
// 但实际缓冲区可以减小一个数量，以尽可能减小搬移数据次数
typedef struct _Control_struct
{
	float Control_Tabel_Error[Error_Buf_Size+1];
	float Control_Tabel_Output[Output_Buf_Size];
	u16 Error_Buf[Error_Buf_Size];
	u16 Output_Buf[Output_Buf_Size];
	u16 Target;
	u16 Error;
	float feedforward_factor;
} Control_struct;

extern Control_struct control;

void Time_Out(u8 TIME);

// 功能：控制初始化
// 参数：
//			control			:	待初始化的控制结构体 
//			Target_Voltage	:	目标电压（V为单位）
//			Tabel_Error		:	控制器对于误差的系数
//			Tabel_Output 	:	控制器对于输出的系数
//			Rin_ratio		:	输入分压比倒数 (RiU+RiD)/RiD
//			Rout_ratio		:	输出分压比倒数 (RoU+RoD)/RoD
void Control_Init(Control_struct* control,float Target_Voltage,float* Tabel_Error,float* Tabel_Output);


// 功能：控制输出计算
// 参数：
//			control			:	控制结构体 
u16 Control_Output(Control_struct* control,u16 feedforward);
// 可以考虑嵌入主函数甚至中断中 

#endif
