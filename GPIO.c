/*
 * GPIO.c
 *
 *  Created on: 2019-9-27
 *      Author: charles_cai
 */
#include "main.h"
#include "PinConfig.h"
#ifndef GPIO_C_
#define GPIO_C_
/***********************************
*功能：编程口配置成数字输入模式。外部晶振管脚配置为数字输入。其他所有的IO口都初始化为输出低电平。
*输入参数：无
*返回：无
*************************************/
void Init_GPIO()
{
	TR0 = 0x80;	//P00-RST、P01-SPDAT、P02-SPCLK、P06-OSC、P07-OSC 配置为输入
	P0LR = 0;	//P0口输出模式的管脚，输出低电平
	ANS0 = 0;	//关闭模拟功能
	PUR0 = 0x80;	//取消上拉功能

	TR1 = 0x26;	//全部管脚配置为输出
	P1LR =0;	//输出低电平
	ANS1 =0;	//关闭模拟功能
	PUR1 =0;	//取消上拉功能

	TR2 = 0;	//全部管脚配置为输出
	P2LR =0;	//输出低电平
	ANS2 =0;	//关闭模拟功能
	PUR2 =0;	//取消上拉功能

	TR3 = 0;	//全部管脚配置为输出
	P3LR =0;	//输出低电平
	ANS3 =0;	//关闭模拟功能
	PUR3 =0;	//取消上拉功能

	TR4 = 0;	//全部管脚配置为输出
	P4LR = 0;	//输出低电平
	ANS4 = 0;	//关闭模拟功能
	PUR4 = 0;	//取消上拉功能

	TR5 = 0;	//全部管脚配置为输出
	P5LR =0;	//输出低电平
	ANS5 =0;	//关闭模拟功能
	PUR5 =0;	//取消上拉功能

	TR6 = 0;	//全部管脚配置为输出
	P6LR =0;	//输出低电平
	ANS6 =0;	//关闭模拟功能
	PUR6 =0;	//取消上拉功能

	TR7 = 0x06;	//全部管脚配置为输出
	P7LR =0;	//输出低电平
	ANS7 =0;	//关闭模拟功能
	PUR7 =0;	//取消上拉功能

	TR8 = 0;	//全部管脚配置为输出
	P8LR =0;	//输出低电平
	ANS8 =0;	//关闭模拟功能
	PUR8 =0;	//取消上拉功能

	TR9 = 0;	//全部管脚配置为输出
	P9LR =0;	//输出低电平
	ANS9 =0;	//关闭模拟功能
	PUR9 =0;	//取消上拉功能

	TRA = 0x1;	//全部管脚配置为输出
	PALR =0;	//输出低电平
	ANSA =0;	//关闭模拟功能
	PURA =0;	//取消上拉功能

	TRB = 0x8;	//全部管脚配置为输出
	PBLR =0;	//输出低电平
	ANSB =0;	//关闭模拟功能
	PURB =0;	//取消上拉功能

	STOP_1 = 0;
	STOP_2 = 0;
	STOP_3 = 0;
	STOP_4 = 0;
	STOP_5 = 0;
	STOP_6 = 0;
	STOP_7 = 0;
	STOP_8 = 0;
	STOP_9 = 0;
	STOP_10 = 0;
	STOP_11 = 0;
	STOP_12 = 0;
}

#endif /* GPIO_C_ */
