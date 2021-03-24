/*
 * pwm.c
 *
 *  Created on: 2021-3-11
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
void PWM1_ON(void)
{
	STOP_1=1;
}
void PWM1_OFF(void)
{
	STOP_1=0;
}
void PWM2_ON(void)
{
	STOP_2=1;
}
void PWM2_OFF(void)
{
	STOP_2=0;
}
void PWM3_ON(void)
{
	STOP_3=1;
}
void PWM3_OFF(void)
{
	STOP_3=0;
}
void PWM4_ON(void)
{
	STOP_4=1;
}
void PWM4_OFF(void)
{
	STOP_4=0;
}
void PWM5_ON(void)
{
	STOP_5=1;
}
void PWM5_OFF(void)
{
	STOP_5=0;
}
void PWM6_ON(void)
{
	STOP_6=1;
}
void PWM6_OFF(void)
{
	STOP_6=0;
}
void PWM7_ON(void)
{
	STOP_7=1;
}
void PWM7_OFF(void)
{
	STOP_7=0;
}
void PWM8_ON(void)
{
	STOP_8=1;
}
void PWM8_OFF(void)
{
	STOP_8=0;
}
void PWM9_ON(void)
{
	STOP_9=1;
}
void PWM9_OFF(void)
{
	STOP_9=0;
}
void PWM10_ON(void)
{
	STOP_10=1;
}
void PWM10_OFF(void)
{
	STOP_10=0;
}
void PWM11_ON(void)
{
	STOP_11=1;
}
void PWM11_OFF(void)
{
	STOP_11=0;
}
void PWM12_ON(void)
{
	STOP_12=1;
}
void PWM12_OFF(void)
{
	STOP_12=0;
}

static void * memset(void *dst, int val, uint8_t count)
{
	char* ret = (char*)dst;
	while (count--)
	{
		*ret++ = (char)val;
	}
	return dst;
}
struct softpwm* pwm_head;

struct softpwm pwm1;
struct softpwm pwm2;
struct softpwm pwm3;
struct softpwm pwm4;
struct softpwm pwm5;
struct softpwm pwm6;
struct softpwm pwm7;
struct softpwm pwm8;
struct softpwm pwm9;
struct softpwm pwm10;
struct softpwm pwm11;
struct softpwm pwm12;
void pwm_init(struct softpwm* handle, uint8_t status, void(*Set)(),void(*ReSet)(),uint16_t freq_counter,uint16_t duty_counter)
{
	//pwm_head = 0;
	memset(handle,0,8);
	//handle = 0;
	handle->status = status;
	handle->Set = Set;
	handle->ReSet = ReSet;
	handle->counter = 0;
	handle->freq_counter = freq_counter;
	handle->duty_counter = duty_counter;
}
void pwm_set(struct softpwm* handle, uint16_t freq_counter,uint16_t duty_counter)
{
	handle->freq_counter = freq_counter;
	handle->duty_counter = duty_counter;
}
int pwm_start(struct softpwm* handle)
{
	struct softpwm* target = pwm_head;
	while(target) {
		if(target == handle) return -1;	//already exist.
		target = target->next;
	}
	handle->next = pwm_head;
	pwm_head = handle;
	return 0;
}
void pwm_stop(struct softpwm* handle)
{
	handle->status = 0;
}
struct Timer breath;
uint32 breath_flag=0;
uint32 breath_counter=0;
void breath_callback(void)
{
	if(breath_flag==0)
	{
		if(breath_counter<10)
		{
			breath_counter++;
			pwm_set(&pwm1,10,breath_counter);
			//pwm_set(&pwm2,10,breath_counter);
		}
		else
		{
			breath_flag=1;
		}
	}
	else
	{
		if(breath_counter>0)
		{
			breath_counter--;
			pwm_set(&pwm1,10,breath_counter);
			//pwm_set(&pwm2,10,breath_counter);
		}
		else
		{
			breath_flag=0;
		}
	}
}
void pwm_test(void)
{
	memset(pwm_head,0,8);
	pwm_head = 0;
	pwm_init(&pwm1,1,PWM1_ON,PWM1_OFF,10,0);
	pwm_init(&pwm2,1,PWM2_ON,PWM2_OFF,10,1);
	pwm_init(&pwm3,1,PWM3_ON,PWM3_OFF,10,2);
	pwm_init(&pwm4,1,PWM4_ON,PWM4_OFF,10,3);
	pwm_init(&pwm5,1,PWM5_ON,PWM5_OFF,10,4);
	pwm_init(&pwm6,1,PWM6_ON,PWM6_OFF,10,5);
	pwm_init(&pwm7,1,PWM7_ON,PWM7_OFF,10,6);
	pwm_init(&pwm8,1,PWM8_ON,PWM8_OFF,10,7);
	pwm_init(&pwm9,1,PWM9_ON,PWM9_OFF,10,8);
	pwm_init(&pwm10,1,PWM10_ON,PWM10_OFF,10,9);
	pwm_init(&pwm11,1,PWM11_ON,PWM11_OFF,10,10);
	pwm_init(&pwm12,1,PWM12_ON,PWM12_OFF,10,1);
	pwm_start(&pwm1);
	pwm_start(&pwm2);
	pwm_start(&pwm3);
	pwm_start(&pwm4);
	pwm_start(&pwm5);
	pwm_start(&pwm6);
	pwm_start(&pwm7);
	pwm_start(&pwm8);
	pwm_start(&pwm9);
	pwm_start(&pwm10);
	pwm_start(&pwm11);
	pwm_start(&pwm12);
	timer_init(&breath, breath_callback, 50, 50); //50s loop
	timer_start(&breath);
}
extern uint64_t _timer_ticks;
void pwm_loop(void)
{
	struct softpwm* target;
	for(target=pwm_head; target=target; target=target->next)
	{
		if(target->status == 0)
		{
			target->ReSet();
			target->counter=0;
		}
		else
		{
			target->counter++;
			if(target->counter<=target->duty_counter&&target->counter<=target->freq_counter)
			{
				target->Set();
			}
			else if(target->counter>target->duty_counter&&target->counter<=target->freq_counter)
			{
				target->ReSet();
			}
			else if(target->counter>target->freq_counter)
			{
				target->counter=0;
			}
		}
	}
}
