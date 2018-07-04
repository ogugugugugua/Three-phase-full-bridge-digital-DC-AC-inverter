#include "control.h"
#include "pwm.h"
#include "led.h"

Control_struct control;

void Time_Out(u8 TIME)
{
	LED0=0;
	while(1);
}

void Control_Init(Control_struct* control,float Target_Voltage,float* Tabel_Error,float* Tabel_Output)
{
	int i =0;
	for(i = 0;i < Error_Buf_Size;i++)
	{
		control->Control_Tabel_Error[i] = Tabel_Error[i];
		control->Error_Buf[i] = 0;
	}
	control->Control_Tabel_Error[Error_Buf_Size] = Tabel_Error[Error_Buf_Size];
	for(i = 0;i < Output_Buf_Size;i++)
	{
		control->Control_Tabel_Output[i] = Tabel_Output[i];
		control->Output_Buf[i] = 0;
	}
	
	//	code: 通过公式把电压转换为AD寄存器值（带校正） 
	// 校正方向：把准确的电压转换后应得的AD寄存器值校正为实际不准确的AD寄存器值 
	control->Target = (u16)(Target_Voltage/8.5/3.3*4096);
}


u16 Control_Output(Control_struct* control,u16 feedforward)
{
	int i;
	float sum = 0;
	#if (Error_Buf_Size > 2)
		for(i = 0;i < Error_Buf_Size-1;i++)
		{
			sum += control->Error_Buf[i] * control->Control_Tabel_Error[i];
			control->Error_Buf[i] = control->Error_Buf[i+1];
		}
	#else
		sum += control->Error_Buf[0] * control->Control_Tabel_Error[i];
		control->Error_Buf[0] = control->Error_Buf[1];
	#endif
	
	sum += control->Error_Buf[Error_Buf_Size-1] * control->Control_Tabel_Error[Error_Buf_Size-1]
		+ control->Error * control->Control_Tabel_Error[Error_Buf_Size];
	control->Error_Buf[Error_Buf_Size-1] = control->Error;
	sum /= feedforward;
	
	#if (Output_Buf_Size > 2) 
		for(i = 0;i < Output_Buf_Size-1;i++)
		{
			sum += control->Output_Buf[i] * control->Control_Tabel_Output[i];
			control->Output_Buf[i] = control->Output_Buf[i+1];
		}
	#else
		sum += control->Output_Buf[0] * control->Control_Tabel_Output[1];
		control->Output_Buf[0] = control->Output_Buf[1];
	#endif
	
	sum += control->Output_Buf[Output_Buf_Size-1] * control->Control_Tabel_Output[Output_Buf_Size-1];
	// 限幅 
	if(sum > 1) sum = 0.98;
	else if(sum < 0) sum = 0;
	control->Output_Buf[Output_Buf_Size-1] = sum;
	
	return (u16)(sum*PWM_Period);
}

