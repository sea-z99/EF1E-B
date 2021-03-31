/*
 * Work.c
 *
 *  Created on: 2020-10-7
 *      Author: Rick
 */
#include<KF8A200GQS.h>
#include "includes.h"
#include "PinConfig.h"
#include "Work.h"
#include "Led.h"
#include "SoftSpi.h"
#include "timer.h"
#include "softpwm.h"

int PastMode=0;
int NowMode=0;
int ActMode=0;
int RT_PastMode=0;
int RT_NowMode=0;
int RT_ActMode=0;

volatile unsigned char LED_Stop_PWM_Flag,PWM_Timer_Flag =0;
volatile unsigned int Stop_High_Addr,Stop_Low_Addr=0;
volatile unsigned char Tail_Status,Stop_Status,RT_Status,Fail_Status,PZ_Status,RT_EN_Status=0;
volatile unsigned int PWM_Timer_Counter =0;
extern unsigned int Stop_PWM_H1,Stop_PWM_L1,Stop_PWM_H2,Stop_PWM_L2=0;

struct Timer pwm;
uint64_t Time_Counter = 0;
void Time_Increase(void)
{
	Time_Counter++;
}
void Time_Clear(void)
{
	Time_Counter=0;
}
void Init_1ms(void)
{
	T1CTL2 = 0x0C;	//	时钟源是SCLK/4，8分频
	T1H=0;
	T1L=0;
	PWMMODE = 1;	//使能重载
	PP2=0x03;		//1000
	PP1=0xE8;
	T1CS =0;
	//timer_init(&pwm, Timer_PWM_Callback, 1, 1); //50s loop
//	timer_init(&pwm, pwm_loop, 1, 1); //50s loop
	//timer_start(&pwm);
}
void Timer1_Start(void)
{
	T1ON = 1;		//T1定时器启动
	T1IF = 0;
	T1IE = 1;	//使能T1定时器的中断功能
}
void RT_Check_Input(void)
{
	RT_Status = RT;
	RT_EN_Status = RT_EN;
	if(RT_Status==0)		//转向低关灯
	{
		RT_PastMode = RT_NowMode;RT_NowMode = RT_Mode1_Status;
		if(RT_NowMode==RT_PastMode)
		{
			RT_ActMode=0;
		}
		else
		{
			RT_ActMode=RT_NowMode;
		}
	}
	if(RT_Status==1&&RT_EN_Status==1)		//转向高，转向使能高，常亮
	{
		RT_PastMode = RT_NowMode;RT_NowMode = RT_Mode2_Status;
		if(RT_NowMode==RT_PastMode)
		{
			RT_ActMode=0;
		}
		else
		{
			RT_ActMode=RT_NowMode;
		}
	}
	if(RT_Status==1&&RT_EN_Status==0)		//转向高，转向使能低，流水
	{
		RT_PastMode = RT_NowMode;RT_NowMode = RT_Mode3_Status;
		if(RT_NowMode==RT_PastMode)
		{
			RT_ActMode=0;
		}
		else
		{
			RT_ActMode=RT_NowMode;
		}
	}
	RT_Mode_Act();
}
void RT_Mode_Act(void)
{
	switch(RT_ActMode)
	{
		case RT_Mode1_Status:
			Led_RT_AllClose();		//转向低，转向使能低，关灯
			break;
		case RT_Mode2_Status:
			Led_RT_AllOpen();		//转向高，转向使能高，常亮
			break;
		case RT_Mode3_Status:
			Led_RT_WaterOpen();		//转向高，转向使能低，流水
			break;
		default:break;
	}
}
void PZ_Check_Input(void)
{
	PZ_OUT = PZ;
}
void Tail_Stop_Check_Input(void)
{
	Tail_Status = TAIL;
	Stop_Status = STOP;

	if(Tail_Status==0&&Stop_Status==0)//位置低，制动低
	{
		PastMode = NowMode;NowMode = Mode1_Status;
		if(NowMode==PastMode)
		{
			ActMode=0;
		}
		else
		{
			ActMode=NowMode;
		}
	}
	if(Tail_Status==1&&Stop_Status==0)//位置高，制动低
	{
		PastMode = NowMode;NowMode = Mode2_Status;
		if(NowMode==PastMode)
		{
			ActMode=0;
		}
		else
		{
			ActMode=NowMode;
		}
	}
	if(Tail_Status==0&&Stop_Status==1)//位置低，制动高
	{
		PastMode = NowMode;NowMode = Mode3_Status;
		if(NowMode==PastMode)
		{
			ActMode=0;
		}
		else
		{
			ActMode=NowMode;
		}
	}
	if(Tail_Status==1&&Stop_Status==1)//位置高，制动高
	{
		PastMode = NowMode;NowMode = Mode4_Status;
		if(NowMode==PastMode)
		{
			ActMode=0;
		}
		else
		{
			ActMode=NowMode;
		}
	}
	Mode_Act();
}
void Mode_Act(void)
{
	switch(ActMode)
	{
	case Mode1_Status:
		Led_Tail_AllClose();	//位置灯全关闭
		LED_Stop_AllClose();	//制动灯全关闭
		break;
	case Mode2_Status:
		Led_Tail_AllOpen();		//位置灯全开启
		LED_Stop_PWMOpen();		//制动灯5%PWM开启
		break;
	case Mode3_Status:
		Led_Tail_AllClose();	//位置灯全关闭
		LED_Stop_AllOpen();		//制动灯全开启
		break;
	case Mode4_Status:
		Led_Tail_AllOpen();		//位置灯全开启
		LED_Stop_AllOpen();		//制动灯全开启
		break;
	default:break;
	}
}
void Timer_PWM_Callback(void)
{
	if(PWM_Timer_Flag)
	{
		PWM_Timer_Flag =0;
		PP2=Stop_PWM_H1;		//250
		PP1=Stop_PWM_L1;
		if(Stop_High_Addr & ADDR1)
			STOP_1=1;
		if(Stop_High_Addr & ADDR2)
			STOP_2=1;
		if(Stop_High_Addr & ADDR3)
			STOP_3=1;
		if(Stop_High_Addr & ADDR4)
			STOP_4=1;
		if(Stop_High_Addr & ADDR5)
			STOP_5=1;
		if(Stop_High_Addr & ADDR6)
			STOP_6=1;
		if(Stop_High_Addr & ADDR7)
			STOP_7=1;
		if(Stop_High_Addr & ADDR8)
			STOP_8=1;
		if(Stop_High_Addr & ADDR9)
			STOP_9=1;
		if(Stop_High_Addr & ADDR10)
			STOP_10=1;
		if(Stop_High_Addr & ADDR11)
			STOP_11=1;
		if(Stop_High_Addr & ADDR12)
			STOP_12=1;
	}
	else
	{
		PWM_Timer_Flag =1;
		PP2=Stop_PWM_H2;		//4750
		PP1=Stop_PWM_L2;
		if(Stop_Low_Addr & ADDR1)
			STOP_1=0;
		if(Stop_Low_Addr & ADDR2)
			STOP_2=0;
		if(Stop_Low_Addr & ADDR3)
			STOP_3=0;
		if(Stop_Low_Addr & ADDR4)
			STOP_4=0;
		if(Stop_Low_Addr & ADDR5)
			STOP_5=0;
		if(Stop_Low_Addr & ADDR6)
			STOP_6=0;
		if(Stop_Low_Addr & ADDR7)
			STOP_7=0;
		if(Stop_Low_Addr & ADDR8)
			STOP_8=0;
		if(Stop_Low_Addr & ADDR9)
			STOP_9=0;
		if(Stop_Low_Addr & ADDR10)
			STOP_10=0;
		if(Stop_Low_Addr & ADDR11)
			STOP_11=0;
		if(Stop_Low_Addr & ADDR12)
			STOP_12=0;
	}
}
void Led_Hello(void)
{
	PZ_OUT = 1;
	Led_RT_WaterOpen();//280ms
	delay_ms(200);
	Led_RT_AllClose();
	delay_ms(300);
	Led_RT_WaterOpen();//220ms
	delay_ms(200);
	Led_RT_AllClose();
	delay_ms(300);
	Tail12_Breath_Open();//2080
	Tail_LowWater_Open();//520
	delay_ms(400);
	Tail_HighWater_Open();//700
	delay_ms(1450);
	delay_ms(300);
	Stop_HighBackWater_Open();//1600
	Logo_Breath_Open();//1360
	Tail1_FullWater_Open();//700
	delay_ms(1500);
	Tail1_NoStop_WaterClose();//2000
//	Tail2_Stop_FullWater_Open();
	for(;;);
}
void Led_Bye(void)
{
	Led_Tail_AllOpen();
	LED_Stop_AllOpen();
	delay_ms(1000);
	Led_RT_WaterOpen();
	delay_ms(200);
	Led_RT_AllClose();
	delay_ms(200);
	Tail_LowWater_Blinky();
	delay_ms(1700);
	Tail1_2_Stop_BackWater_Close();
	delay_ms(1160);
	Tail1_2_Stop_FullWater_Close();
	delay_ms(1000);
	Tail12_Breath_Close();
	for(;;);
}
void Led_Hello_Check(void)
{
	Tail_Status = TAIL;
	Stop_Status = STOP;
	RT_Status = RT;
	RT_EN_Status = RT_EN;
	PZ_Status = PZ;
	if(Tail_Status==1&&Stop_Status==1&&PZ_Status==1&&RT_Status==1&&RT_EN_Status==0)
	{
		Led_Hello();
	}
	if(Tail_Status==1&&Stop_Status==1&&PZ_Status==1&&RT_Status==1&&RT_EN_Status==1)
	{
		Led_Bye();
	}
}


