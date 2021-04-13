/*
 * Led.c
 *
 *  Created on: 2020-10-9
 *      Author: Rick
 */
#include<KF8A200GQS.h>
#include "includes.h"
#include "PinConfig.h"
#include "Work.h"
#include "Led.h"
#include "SoftSpi.h"

extern unsigned char LED_Stop_PWM_Flag;
extern unsigned int Stop_High_Addr,Stop_Low_Addr=0;
unsigned int Stop_PWM_H1,Stop_PWM_L1,Stop_PWM_H2,Stop_PWM_L2=0;

void Change_PWM_5(void)
{
	Stop_PWM_H1 = 0;
	Stop_PWM_L1 = 0xFA;
	Stop_PWM_H2 = 0x12;
	Stop_PWM_L2 = 0x8E;
}
void Change_PWM_80(void)
{
	Stop_PWM_H1 = 0x0F;
	Stop_PWM_L1 = 0xA0;
	Stop_PWM_H2 = 0x03;
	Stop_PWM_L2 = 0xE8;
}
void LED_Stop_AllOpen(void)
{
	T1IF = 0;
	T1IE = 0;	//使能T1定时器的中断功能
	Change_PWM_80();
	Stop_High_Addr=0xFFFF;
	Stop_Low_Addr=0xFFFF;
	Stop_Timer_Start();
}
void LED_Stop_PWMOpen(void)
{
	T1IF = 0;
	T1IE = 0;	//使能T1定时器的中断功能
	Change_PWM_5();
	Stop_High_Addr=0xFFFF;
	Stop_Low_Addr=0xFFFF;
	Stop_Timer_Start();
}
void LED_Stop_AllClose(void)
{
	Stop_High_Addr=0;
	Stop_Low_Addr=0xFFFF;
}
void Led_RT_AllOpen(void)
{
	uchar i;
	for(i=OUT1;i<=OUT15;i++)
	{
		SPI_Write_2Byte(U7,i,0xFF);
	}
	SPI_Write_2Byte(U7,0x37,0x00);//update
	for(i=OUT1;i<=OUT18;i++)
	{
		SPI_Write_2Byte(U8,i,0xFF);
	}
	SPI_Write_2Byte(U8,0x37,0x00);//update
}
void Led_RT_WaterOpen(void)
{
	uchar i;
	for(i=OUT2;i<=OUT14;i+=3)
	{
		SPI_Write_2Byte(U7,i-1,0xFF);
		SPI_Write_2Byte(U7,i,0xFF);
		SPI_Write_2Byte(U7,i+1,0xFF);
		SPI_Write_2Byte(U7,0x37,0x00);//update
		delay_ms(RT_Interval);
	}
	for(i=OUT2;i<=OUT17;i+=3)
	{
		SPI_Write_2Byte(U8,i-1,0xFF);
		SPI_Write_2Byte(U8,i,0xFF);
		SPI_Write_2Byte(U8,i+1,0xFF);
		SPI_Write_2Byte(U8,0x37,0x00);//update
		delay_ms(RT_Interval);
	}
	delay_ms(60);
}
void Led_RT_AllClose(void)
{
	uchar i;
	for(i=OUT1;i<=OUT15;i++)
	{
		SPI_Write_2Byte(U7,i,0);
	}
	SPI_Write_2Byte(U7,0x37,0x00);//update
	for(i=OUT1;i<=OUT18;i++)
	{
		SPI_Write_2Byte(U8,i,0);
	}
	SPI_Write_2Byte(U8,0x37,0x00);//update
}
void Led_Tail_AllOpen(void)
{
	uchar i;
	for(i=OUT1;i<=OUT14;i++)
	{
		SPI_Write_2Byte(U1,i,0xFF);
	}
	for(i=OUT15;i<=OUT18;i++)
	{
		SPI_Write_2Byte(U1,i,0x57);//34%
	}
	SPI_Write_2Byte(U1,0x37,0x00);//update
	for(i=OUT1;i<=OUT16;i++)
	{
		SPI_Write_2Byte(U4,i,0x57);
	}
	SPI_Write_2Byte(U4,0x37,0x00);//update
}
void Led_Tail_AllClose(void)
{
	uchar i;
	for(i=OUT1;i<=OUT18;i++)
	{
		SPI_Write_2Byte(U1,i,0);
	}
	SPI_Write_2Byte(U1,0x37,0x00);//update
	for(i=OUT1;i<=OUT16;i++)
	{
		SPI_Write_2Byte(U4,i,0);
	}
	SPI_Write_2Byte(U4,0x37,0x00);//update
}
void Tail12_Breath_Open(void)
{
	uint8 i;
	for(i=0;i<52;i++)
	{
		SPI_Write_2Byte(1,OUT1,i*5);//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		delay_ms(LED_Interval);
	}
}
void Tail12_Breath_Close(void)
{
	uint8 i;
	for(i=0;i<52;i++)
	{
		SPI_Write_2Byte(1,OUT1,0xFF-i*5);//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		delay_ms(LED_Interval);
	}
}
void Tail12_Breath_CloseTo10(void)
{
	uint8 i;
	for(i=0;i<49;i++)
	{
		SPI_Write_2Byte(1,0x1F,0xFF-i*5);//update
		SPI_Write_2Byte(1,0x37,0x00);//update
	}
}
void Tail_LowWater_Open(void)
{
	uint8 i;
	Stop_High_Addr = 0;
	Stop_Low_Addr = 0;
	for(i=OUT2;i<=OUT14;i++)
	{
		SPI_Write_2Byte(U1,i,0x55);//update
		SPI_Write_2Byte(U1,0x37,0x00);//update
		delay_ms(LED_Interval);
	}
}
void Tail_HighWater_Open(void)
{
	SPI_Write_2Byte(1,OUT1,0x55);//update
	SPI_Write_2Byte(1,OUT2,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(50);
	SPI_Write_2Byte(1,OUT2,0x55);//update
	SPI_Write_2Byte(1,OUT3,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(50);
	SPI_Write_2Byte(1,OUT3,0x55);//update
	SPI_Write_2Byte(1,OUT4,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(50);
	SPI_Write_2Byte(1,OUT4,0x55);//update
	SPI_Write_2Byte(1,OUT5,0xFF);//update
	SPI_Write_2Byte(1,OUT15,0x55);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(1,OUT16,0x55);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(1,OUT17,0x55);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(16);
	SPI_Write_2Byte(1,OUT5,0x55);//update
	SPI_Write_2Byte(1,OUT6,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(1);
	SPI_Write_2Byte(1,OUT18,0x55);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(U4,OUT1,0x55);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(U4,OUT2,0x55);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(15);
	SPI_Write_2Byte(1,OUT6,0x55);//update
	SPI_Write_2Byte(1,OUT7,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(2);
	SPI_Write_2Byte(U4,OUT3,0x55);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(U4,OUT4,0x55);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(U4,OUT5,0x55);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(14);
	SPI_Write_2Byte(1,OUT7,0x55);//update
	SPI_Write_2Byte(1,OUT8,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(3);
	SPI_Write_2Byte(U4,OUT6,0x55);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(U4,OUT7,0x55);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(U4,OUT8,0x55);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(13);
	SPI_Write_2Byte(1,OUT8,0x55);//update
	SPI_Write_2Byte(1,OUT9,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(4);
	SPI_Write_2Byte(U4,OUT9,0x55);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	Stop_Low_Addr = 0xFFF;
	Stop_High_Addr = 1<<0;
	delay_ms(17);
	Stop_High_Addr += 1<<1;
	delay_ms(12);
	SPI_Write_2Byte(1,OUT9,0x55);//update
	SPI_Write_2Byte(1,OUT10,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(5);
	Stop_High_Addr += 1<<2;
	delay_ms(17);
	Stop_High_Addr += 1<<3;
	delay_ms(17);
	SPI_Write_2Byte(U4,OUT10,0x55);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(11);
	SPI_Write_2Byte(1,OUT10,0x55);//update
	SPI_Write_2Byte(1,OUT11,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(6);
	SPI_Write_2Byte(U4,OUT11,0x55);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(U4,OUT12,0x55);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	Stop_High_Addr += 1<<4;
//	SPI_Write_2Byte(U4,OUT13,0x55);//update
//	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(10);
	SPI_Write_2Byte(1,OUT11,0x55);//update
	SPI_Write_2Byte(1,OUT12,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(7);
	Stop_High_Addr += 1<<5;
	delay_ms(17);
	Stop_High_Addr += 1<<6;
	delay_ms(17);
	Stop_High_Addr += 1<<7;
	delay_ms(9);
	SPI_Write_2Byte(1,OUT12,0x55);//update
	SPI_Write_2Byte(1,OUT13,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(8);
	SPI_Write_2Byte(U4,OUT14,0x55);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(U4,OUT15,0x55);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	Stop_High_Addr += 1<<8;
	delay_ms(8);
	SPI_Write_2Byte(1,OUT12,0x55);//update
	SPI_Write_2Byte(1,OUT13,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(9);
	Stop_High_Addr += 1<<9;
	delay_ms(17);
	Stop_High_Addr += 1<<10;
	delay_ms(17);
	Stop_High_Addr += 1<<11;
	delay_ms(7);
	SPI_Write_2Byte(1,OUT13,0x55);//update
	SPI_Write_2Byte(1,OUT14,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(10);
	SPI_Write_2Byte(U4,OUT16,0x55);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(40);
	SPI_Write_2Byte(1,OUT14,0x55);//update
	SPI_Write_2Byte(1,0x37,0x00);//update

}
void Tail_LowWater_Blinky(void)
{
	uint8 i;
	for(i=0x20;i<=0x25;i++)
	{
		SPI_Write_2Byte(1,i-1,0xFF);//update
		SPI_Write_2Byte(1,i,0x10);//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		delay_ms(40);
	}
	for(i=0x26;i<=0x30;i++)
	{
		Stop_Low_Addr += 1<<(i-0x26);
		SPI_Write_2Byte(1,i-1,0xFF);//update
		SPI_Write_2Byte(1,i,0x10);//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		delay_ms(40);
	}
	Stop_Low_Addr = 0xFFF;
	delay_ms(40);
}
void Stop_HighBackWater_Open(void)
{
	uint8 i;
	uint16_t Low_Temp = 0x1000;
	SPI_Write_2Byte(U4,OUT16,0xFF);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(50);
	SPI_Write_2Byte(U4,OUT16,0x10);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	for(i=0;i<4;i++)
	{
		Low_Temp >>= 1;
		Stop_Low_Addr = 0xFFF-Low_Temp;
		delay_ms(50);
	}
	Stop_Low_Addr = 0xFFF;
	for(i=0;i<3;i++)
	{
		SPI_Write_2Byte(U4,OUT16-i,0x10);//update
		SPI_Write_2Byte(U4,OUT15-i,0xFF);//update
		SPI_Write_2Byte(U4,0x37,0x00);//update
		delay_ms(50);
	}
	SPI_Write_2Byte(U4,OUT13,0x10);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	for(i=0;i<4;i++)
	{
		Low_Temp >>= 1;
		Stop_Low_Addr = 0xFFF-Low_Temp;
		delay_ms(50);
	}
	Stop_Low_Addr = 0xFFF;
	for(i=0;i<3;i++)
	{
		SPI_Write_2Byte(U4,OUT13-i,0x10);//update
		SPI_Write_2Byte(U4,OUT12-i,0xFF);//update
		SPI_Write_2Byte(U4,0x37,0x00);//update
		delay_ms(50);
	}
	SPI_Write_2Byte(U4,OUT10,0x10);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	for(i=0;i<4;i++)
	{
		Low_Temp >>= 1;
		Stop_Low_Addr = 0xFFF-Low_Temp;
		delay_ms(50);
	}
	Stop_Low_Addr = 0xFFF;
	for(i=0;i<9;i++)
	{
		SPI_Write_2Byte(U4,OUT10-i,0x10);//update
		SPI_Write_2Byte(U4,OUT9-i,0xFF);//update
		SPI_Write_2Byte(U4,0x37,0x00);//update
		delay_ms(50);
	}
	SPI_Write_2Byte(U4,OUT1,0x10);//update
	SPI_Write_2Byte(U1,OUT18,0xFF);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	SPI_Write_2Byte(U1,0x37,0x00);//update
	delay_ms(50);
	for(i=0;i<3;i++)
	{
		SPI_Write_2Byte(U1,OUT18-i,0x10);//update
		SPI_Write_2Byte(U1,OUT17-i,0xFF);//update
		SPI_Write_2Byte(U1,0x37,0x00);//update
		delay_ms(50);
	}
	SPI_Write_2Byte(U1,OUT15,0x10);//update
	SPI_Write_2Byte(U1,0x37,0x00);//update
}
void Logo_Breath_Open(void)
{
	uint8 i;
	for(i=18;i<52;i++)
	{
		SPI_Write_2Byte(1,OUT1,i*5);//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		delay_ms(LED_Interval);
	}
//	if(LOGO_EN==0)
//	{
		LOGO_OUT=1;
	//}
}
void Tail1_FullWater_Open(void)
{
	SPI_Write_2Byte(1,OUT2,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(50);
	SPI_Write_2Byte(1,OUT3,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(50);
	SPI_Write_2Byte(1,OUT4,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(50);
	SPI_Write_2Byte(1,OUT5,0xFF);//update
	SPI_Write_2Byte(1,OUT15,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(1,OUT16,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(1,OUT17,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(16);
	SPI_Write_2Byte(1,OUT6,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(1);
	SPI_Write_2Byte(1,OUT18,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(U4,OUT1,0xFF);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(U4,OUT2,0xFF);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(15);
	SPI_Write_2Byte(1,OUT7,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(2);
	SPI_Write_2Byte(U4,OUT3,0xFF);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(U4,OUT4,0xFF);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(U4,OUT5,0xFF);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(14);
	SPI_Write_2Byte(1,OUT8,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(3);
	SPI_Write_2Byte(U4,OUT6,0xFF);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(U4,OUT7,0xFF);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(U4,OUT8,0xFF);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(13);
	SPI_Write_2Byte(1,OUT9,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(4);
	SPI_Write_2Byte(U4,OUT9,0xFF);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	Stop_High_Addr = 0xFFF;
	Stop_Low_Addr = 0xFFF-1<<0;
	delay_ms(17);
	Stop_Low_Addr -= 1<<1;
	delay_ms(12);
	SPI_Write_2Byte(1,OUT10,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(5);
	Stop_Low_Addr -= 1<<2;
	delay_ms(17);
	Stop_Low_Addr -= 1<<3;
	delay_ms(17);
	SPI_Write_2Byte(U4,OUT10,0xFF);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(11);
	SPI_Write_2Byte(1,OUT11,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(6);
	SPI_Write_2Byte(U4,OUT11,0xFF);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(U4,OUT12,0xFF);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	Stop_Low_Addr -= 1<<4;
	delay_ms(10);
	SPI_Write_2Byte(1,OUT12,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(7);
	Stop_Low_Addr -= 1<<5;
	delay_ms(17);
	Stop_Low_Addr -= 1<<6;
	delay_ms(17);
	Stop_Low_Addr -= 1<<7;
	delay_ms(9);
	SPI_Write_2Byte(1,OUT13,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(8);
	SPI_Write_2Byte(U4,OUT14,0xFF);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	SPI_Write_2Byte(U4,OUT15,0xFF);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(17);
	Stop_Low_Addr -= 1<<8;
	delay_ms(8);
	SPI_Write_2Byte(1,OUT13,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(9);
	Stop_Low_Addr -= 1<<9;
	delay_ms(17);
	Stop_Low_Addr -= 1<<10;
	delay_ms(17);
	Stop_Low_Addr -= 1<<11;
	delay_ms(7);
	SPI_Write_2Byte(1,OUT14,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(10);
	SPI_Write_2Byte(U4,OUT16,0xFF);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	delay_ms(40);
	SPI_Write_2Byte(1,OUT14,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
}
void Tail1_NoStop_WaterClose(void)
{
	uint8 i;
	for(i=OUT15;i<=OUT18;i++)
	{
		SPI_Write_2Byte(U1,i,0x55);//update
		SPI_Write_2Byte(U1,0x37,0x00);//update
		delay_ms(50);
	}
	for(i=OUT1;i<=OUT16;i++)
	{
		SPI_Write_2Byte(U4,i,0x55);//update
		SPI_Write_2Byte(U4,0x37,0x00);//update
		delay_ms(50);
	}
	for(i=OUT15;i<=OUT18;i++)
	{
		SPI_Write_2Byte(U1,i,0);//update
		SPI_Write_2Byte(U1,0x37,0x00);//update
		delay_ms(50);
	}
	for(i=OUT1;i<=OUT16;i++)
	{
		SPI_Write_2Byte(U4,i,0);//update
		SPI_Write_2Byte(U4,0x37,0x00);//update
		delay_ms(50);
	}
}
void Tail1_2_Stop_BackWater_Close(void)
{
	uint8 i;
	Stop_High_Addr = 0;
	Stop_Low_Addr = 0xFFF;
	for(i=0x30;i>=0x26;i--)
	{
		Stop_High_Addr += 1<<(i-0x26);
		SPI_Write_2Byte(1,i,0x10);//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		delay_ms(40);
	}
	Stop_High_Addr += 1<<(11);
	delay_ms(40);
	for(i=0x25;i>=0x20;i--)
	{
		SPI_Write_2Byte(1,i,0x10);//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		delay_ms(40);
	}
}
void Tail1_2_Stop_FullWater_Close(void)
{
	uint8 i,j;
	Stop_High_Addr = 0xFFF;
	Stop_Low_Addr = 0xFFF;
	for(i=0x30;i>=0x26;i--)
	{
		Stop_Low_Addr -= (1<<(i-0x1F));
		SPI_Write_2Byte(1,i,0x10);//update
		SPI_Write_2Byte(1,i-1,0xFF);//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		delay_ms(40);
	}
	Stop_Low_Addr = 0;
	delay_ms(40);
	Stop_High_Addr = 0;
	Stop_Low_Addr = 0;
	for(i=0x2B;i>=0x20;i--)
	{
		if(i<=0x25)
		{
			Stop_Low_Addr = 1<<(13-(i-0x1F));
			Stop_High_Addr = ~(Stop_Low_Addr);
			SPI_Write_2Byte(1,i,0x10);//update
			SPI_Write_2Byte(1,i-1,0xFF);//update
			SPI_Write_2Byte(1,0x37,0x00);//update
			delay_ms(40);
		}
		else
		{
			Stop_Low_Addr = 1<<(13-(i-0x1F));
			Stop_High_Addr = ~(Stop_Low_Addr);
		}
	}
	Tail12_Breath_CloseTo10();//熄灭
	for(j=1;j<6;j++)//除1全亮
	{
		for(i=0x25;i>=0x20;i--)//除1全亮
		{
			SPI_Write_2Byte(1,i,j*51);//update
			SPI_Write_2Byte(1,0x37,0x00);//update
		}
		delay_ms(40);
	}
	for(i=0;i<3;i++)//T2点亮 一半
	{
		SPI_Write_2Byte(1,0x26+i,0xFF );//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		delay_ms(40);
	}
	for(i=0;i<2;i++)//整根左移,T1到底
	{
		SPI_Write_2Byte(1,0x23-i,0x10);//update
		SPI_Write_2Byte(1,0x22-i,0xFF);//update
		SPI_Write_2Byte(1,0x23+i,0x10);//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		SPI_Write_2Byte(1,0x24+i,0xFF );//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		delay_ms(40);
	}
	for(i=0;i<3;i++)//左移到底后缩进到T2的一半
	{
		SPI_Write_2Byte(2,0x1F+i,0x10 );//update
		SPI_Write_2Byte(2,0x37,0x00);//update
		delay_ms(40);
	}
	for(i=0;i<6;i++)//继续缩进，右边一半开始往回缩进
	{
		SPI_Write_2Byte(2,0x24+i,0 );//update
		SPI_Write_2Byte(2,0x24-i,0 );//update
		SPI_Write_2Byte(2,0x37,0x00);//update
		delay_ms(40);
	}
	delay_ms(160);
	Stop_High_Addr = 0;
	Stop_Low_Addr = 0;
	for(i=0;i<7;i++)//T2开始收拢，T1熄灭,制动流水
	{
		SPI_Write_2Byte(1,0x28-i,0xFF );//update
		SPI_Write_2Byte(1,0x27-i,0xFF );//update
		SPI_Write_2Byte(1,0x26-i,0x10 );//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		delay_ms(40);
		SPI_Write_2Byte(1,0x28-i,0 );//update
		SPI_Write_2Byte(1,0x27-i,0 );//update
		SPI_Write_2Byte(1,0x26-i,0 );//update

		SPI_Write_2Byte(1,0x29-i,0 );//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		Stop_Low_Addr = 1<<(12-i);
		Stop_High_Addr = ~Stop_Low_Addr;
	}
	for(i=0;i<7;i++)//T1开始收拢,制动流水
	{
		SPI_Write_2Byte(1,0x29-i,0xFF );//update
		SPI_Write_2Byte(1,0x28-i,0x10 );//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		delay_ms(40);
		SPI_Write_2Byte(1,0x29-i,0 );//update
		SPI_Write_2Byte(1,0x28-i,0 );//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		if(i<3)
		{
			Stop_Low_Addr = 1<<(2-i);
			Stop_High_Addr = ~Stop_Low_Addr;
		}
		else
		{
			Stop_Low_Addr = 0;
			Stop_High_Addr = ~Stop_Low_Addr;
		}
	}

}






//位置单颗低亮流水
void Tail_Single_Low_water(void){
	uint8 i;

	synchroFlay syn = {0,0,0,0,0};
	SPI_Write_2Byte(1,0x1F,0x10);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(40);
	for(i=0x20;i<=0x2D;i++)
	{
		SPI_Write_2Byte(1,i-1,0xFF);//update
		SPI_Write_2Byte(1,i,0x10);//update
		syn.frequency++;
		if(syn.frequency >= 3){  //3个位置灯
			SPI_Write_2Byte(1,OUT15,0x10);//update
			SPI_Write_2Byte(1,OUT16,0x10);//update
			SPI_Write_2Byte(1,OUT17,0x10);//update
			SPI_Write_2Byte(1,OUT18,0x10);//update
		}
		if((syn.frequency >= 4)&&(syn.Flay == 0)){
			syn.Data1 = i-4;
			SPI_Write_2Byte(U4,syn.Data1,0x10);//update
			SPI_Write_2Byte(U4,syn.Data1+1,0x10);//update
			SPI_Write_2Byte(U4,syn.Data1+2,0x10);//update
			if(syn.frequency >= 10){
				syn.Flay = 1;
			}
		}
		if(syn.frequency >= 12){
			SPI_Write_2Byte(U4,OUT12,0x10);//update
			SPI_Write_2Byte(U4,OUT11,0x10);//update
			SPI_Write_2Byte(U4,OUT10,0x10);//update
		}
		if(syn.frequency >= 13){
			SPI_Write_2Byte(U4,OUT13,0x10);//update
			SPI_Write_2Byte(U4,OUT14,0x10);//update
			SPI_Write_2Byte(U4,OUT15,0x10);//update
		}
		if(syn.frequency >= 14){
			SPI_Write_2Byte(U4,OUT16,0x10);//update
		}
		SPI_Write_2Byte(1,0x37,0x00);//update
		SPI_Write_2Byte(U4,0x37,0x00);//update
		delay_ms(40);
	}

}


//位置整体低亮反流水
void Tail_whole_Low_water_reverse(void){
	uint8 i;
	uint32 d =1;
	synchroFlay syn = {0,0,0,0};
	delay_ms(960);
	for(i=0;i<=12;i++){
		Stop_Low_Addr = d;
		d = ( d << 1 ) + 1;
		delay_ms(40);
	}
	delay_ms(280);
	for(i=0x2C;i>=0x1F;i--){
		SPI_Write_2Byte(1,i,0x10);//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		delay_ms(40);
	}
}

//位置单颗不亮反流水
void Tail_Single_Out_water_reverse(void){
	uint8 i;
	synchroFlay syn = {0,0,0,0};
	for(i=0x2D;i<=72;i++){
		syn.frequency++;
		if(syn.frequency<=4){
		SPI_Write_2Byte(1,i,0x00);//update
		}
		if(syn.frequency>=5){
			SPI_Write_2Byte(U4,i-0x12,0x00);//update
			if(syn.frequency == 14){
				Stop_High_Addr = 0xFFFC;
				i -=1;
			}
			if(syn.frequency == 15){
				Stop_High_Addr = 0xFFF0;
				i -=1;
			}
			if(syn.frequency == 19){
				Stop_High_Addr = 0xFFC0;
				i -=1;
			}
			if(syn.frequency == 20){
				Stop_High_Addr = 0xFF00;
				i -=1;
			}
			if(syn.frequency == 24){
				Stop_High_Addr = 0xFC00;
				i -=1;
			}
			if(syn.frequency == 25){
				Stop_High_Addr = 0xF000;
				i -=1;
			}
		}
		SPI_Write_2Byte(1,0x37,0x00);//update
		SPI_Write_2Byte(U4,0x37,0x00);//update
		delay_ms(20);
	}
	delay_ms(320);
	SPI_Write_2Byte(1,OUT14,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	SPI_Write_2Byte(U4,OUT16,0x10);//update
	SPI_Write_2Byte(U4,0x37,0x00);//update
	Stop_High_Addr = 0xFC00;
	delay_ms(40);
	syn.frequency = 0;
	for(i=OUT13;i>=OUT4;i--)
	{
		syn.frequency ++;
		SPI_Write_2Byte(1,i+1,0x10);//update
		SPI_Write_2Byte(1,i,0xFF);//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		if(syn.frequency == 1){
			Stop_High_Addr = 0xF300;
			SPI_Write_2Byte(U4,OUT15,0x10);//update
			SPI_Write_2Byte(U4,OUT16,0x00);//update
		}
		if(syn.frequency == 2){
			Stop_High_Addr = 0xF080;
			SPI_Write_2Byte(U4,OUT15,0x00);//update
			SPI_Write_2Byte(U4,OUT14,0x10);//update
			SPI_Write_2Byte(U4,OUT13,0x10);//update
		}
		if(syn.frequency == 3){
			SPI_Write_2Byte(U4,OUT14,0x00);//update
			SPI_Write_2Byte(U4,OUT13,0x00);//update
			Stop_High_Addr = 0xF070;
		}
		if(syn.frequency == 4){
			Stop_High_Addr = 0xF000;
			SPI_Write_2Byte(U4,OUT12,0x10);//update
			SPI_Write_2Byte(U4,OUT10,0x10);//update
			SPI_Write_2Byte(U4,OUT11,0x10);//update

		}
		if(syn.frequency == 5){
			Stop_High_Addr = 0xF00E;
			SPI_Write_2Byte(U4,OUT11,0x00);//update
			SPI_Write_2Byte(U4,OUT10,0x00);//update
			SPI_Write_2Byte(U4,OUT12,0x00);//update
		}
		if(syn.frequency == 6){
			SPI_Write_2Byte(U4,OUT9,0x10);//update
			SPI_Write_2Byte(U4,OUT8,0x10);//update
			Stop_High_Addr = 0xF001;
		}
		if(syn.frequency == 7){
			Stop_High_Addr = 0xF000;
			SPI_Write_2Byte(U4,OUT9,0x00);//update
			SPI_Write_2Byte(U4,OUT8,0x00);//update
			SPI_Write_2Byte(U4,OUT7,0x10);//update
			SPI_Write_2Byte(U4,OUT6,0x10);//update
			SPI_Write_2Byte(U4,OUT5,0x10);//update
		}
		if(syn.frequency == 8){
			SPI_Write_2Byte(U4,OUT5,0x00);//update
			SPI_Write_2Byte(U4,OUT6,0x00);//update
			SPI_Write_2Byte(U4,OUT7,0x00);//update
			SPI_Write_2Byte(U4,OUT4,0x10);//update
			SPI_Write_2Byte(U4,OUT3,0x10);//update
			SPI_Write_2Byte(U4,0x32,0x10);//update
		}
		if(syn.frequency == 9){

			SPI_Write_2Byte(U4,OUT4,0x00);//update
			SPI_Write_2Byte(U4,OUT3,0x00);//update
			SPI_Write_2Byte(U4,OUT2,0x00);//update
			SPI_Write_2Byte(U4,OUT1,0x10);//update
			SPI_Write_2Byte(1,OUT18,0x10);//update
			SPI_Write_2Byte(1,OUT17,0x10);//update

		}
		if(syn.frequency == 10){
			SPI_Write_2Byte(1,OUT16,0x10);//update
			SPI_Write_2Byte(1,OUT15,0x10);//update
			SPI_Write_2Byte(U4,OUT1,0x00);//update
			SPI_Write_2Byte(1,OUT18,0x00);//update
			SPI_Write_2Byte(1,OUT17,0x00);//update
		}
		SPI_Write_2Byte(1,0x37,0x00);//update
		SPI_Write_2Byte(U4,0x37,0x00);//update
		delay_ms(40);
		SPI_Write_2Byte(1,OUT15,0x00);//update
		SPI_Write_2Byte(1,OUT17,0x00);//update
		SPI_Write_2Byte(1,OUT16,0x00);//update
		SPI_Write_2Byte(1,0x37,0x00);//update
	}

}

//乱闪
void Random_flash(void){
	uint8 i;
	synchroFlay syn = {0,0,0,0};
	SPI_Write_2Byte(1,OUT4,0x10);//update
	SPI_Write_2Byte(1,OUT3,0xFF);//update
	SPI_Write_2Byte(1,OUT1,0xFF);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(40);
	SPI_Write_2Byte(1,OUT2,0xFF);//update
	SPI_Write_2Byte(1,OUT1,0x00);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(20);
	for(i=OUT2;i<=OUT14;i++){
		SPI_Write_2Byte(1,i,0xFF);//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		delay_ms(20);
	}
	for(i=OUT2;i<=OUT11;i++){
		SPI_Write_2Byte(1,i,0x10);//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		delay_ms(20);
	}
//	for(i=OUT3;i<=OUT11;i++){
//		SPI_Write_2Byte(1,i,0x10);//update
//		SPI_Write_2Byte(1,0x37,0x00);//update
//		delay_ms(40);
//	}
	SPI_Write_2Byte(1,OUT1,0xFF);//update
	SPI_Write_2Byte(1,OUT2,0x00);//update
	for(i=OUT11;i>=OUT2;i--){
		syn.frequency++;
		SPI_Write_2Byte(1,i+1,0x00);//update
		SPI_Write_2Byte(1,i,0x10);//update
		if(syn.frequency== 1){
			SPI_Write_2Byte(1,OUT12,0x00);//update
		}
		if(syn.frequency== 2){
			SPI_Write_2Byte(1,OUT13,0x00);//update
		}
		if(syn.frequency== 3){
			SPI_Write_2Byte(1,OUT14,0x00);//update
		}
		SPI_Write_2Byte(1,0x37,0x00);//update
		delay_ms(40);
	}
	for(i=OUT14;i>=OUT2;i--){
		SPI_Write_2Byte(1,i+1,0x00);//update
		SPI_Write_2Byte(1,i,0xFF);//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		delay_ms(40);
	}
	SPI_Write_2Byte(1,OUT2,0x00);//update
	SPI_Write_2Byte(1,0x37,0x00);//update
	delay_ms(40);
	for(i=0;i<52;i++)
	{
		SPI_Write_2Byte(1,OUT1,0xFF-i*5);//update
		SPI_Write_2Byte(1,0x37,0x00);//update
		delay_ms(20);
	}
	delay_ms(1500);
	PZ_OUT = 0;
}







