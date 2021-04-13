#include<KF8A200GQS.h>
#include "includes.h"
#include "PinConfig.h"
#include "SoftSpi.h"
#include "Work.h"
#include "OSC.h"
#include "GPIO.h"
void main()
{
    Init_OSC(2); //��ϵͳʱ�ӳ�ʼ��Ϊ32M��ʹ���ⲿ����
    Init_GPIO();
    IS31FL3265B_Init();
    Stop_Timer_Init();
    Stop_Timer_Start();
    PUIE=1; //ʹ�������ж�
    AIE=1; //���жϿ���
    PwmDetect();
    Led_Hello_Check(SearchPwmFlag());
    while(1)
    {
    	PZ_Check_Input();
    	Tail_Stop_Check_Input();
		RT_Check_Input();
    }
}
//�жϺ���0:0X04��ڵ�ַ
void int_fun0() __interrupt (0)
{
	if(T0IF & T0IE)		//500us��ʱ
	{
		T0IF= 0;
		Hello_Bye_Callback();
	}
	if(T1IF & T1IE)
	{
		T1IF =0;
		STOP_PWM_Callback();
	}
	if(INT1IE && INT1IF) //INT1�жϵ���Ӧ
	{
		INT1IF=0;
		PwmFromInteruppt();
	}
}

//�жϺ���1:0x14��ڵ�ַ
void int_fun1() __interrupt (1)
{

}

