#include<KF8A200GQS.h>
#include "includes.h"
#include "PinConfig.h"
#include "SoftSpi.h"
#include "Work.h"
#include "timer.h"
#include "OSC.h"
#include "GPIO.h"
#include "softpwm.h"
uint8_t pwm_run=0;
void main()
{
    Init_OSC(2); //��ϵͳʱ�ӳ�ʼ��Ϊ32M��ʹ���ⲿ����
    Init_GPIO();
    PUPH=0;
    IS31FL3265B_Init();
    Init_1ms();
    Timer1_Start();
    PUIE=1; //ʹ�������ж�
    AIE=1; //���жϿ���
//    PwmDetect();
    Led_Hello_Check();
    //pwm_test();
    while(1)
    {
    	//timer_loop();
//    	if(pwm_run)
//    	{
//    		pwm_run=0;
//    		pwm_loop();
//    	}
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
	}
	if(T1IF & T1IE)
	{
		T1IF =0;
		//pwm_loop();
		//pwm_run=1;
		Timer_PWM_Callback();
		//timer_ticks(); //1ms ticks
	}
	if(T2IF & T2IE)
	{
		T2IF =0;

	}
}

//�жϺ���1:0x14��ڵ�ַ
void int_fun1() __interrupt (1)
{

}

