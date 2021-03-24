/*
 * SoftSpi.c
 *
 *  Created on: 2020-10-6
 *      Author: Rick
 */
#include<KF8A200GQS.h>
#include "includes.h"
#include "PinConfig.h"
#include "SoftSpi.h"
#include "Work.h"
void spi_init(void)
{
	TR64 =1;
	TR65 =0;
	TR63 =0;
	APFCTL9 = 0;

	SSCI2EN=0;				//�ر�SPIģ�飬��ʼ����֮���ٴ�

	SSCI2CTL0=0X01;			//SPI ����ģʽ��ʱ�� = ����ʱ��/64
	SSCI2CTL1=0X00;
	SSCI2CTL2=0X00;
	SSCI2STA=0X40;  //����ʱΪ�͵�ƽ����SCL�½��ط�������

	SSCI2EN=1;

	SDB1=1;
	SDB2=1;
	Spi_CS_High(0);
}
void Spi_CS_High(char id)
{
	switch(id)
	{
		case 0:U1_CS=1;U2_CS=1;U3_CS=1;U4_CS=1;break;
		case 1:U1_CS=1;break;
		case 2:U2_CS=1;break;
		case 3:U3_CS=1;break;
		case 4:U4_CS=1;break;
	}
}
void Spi_CS_Low(char id)
{
	switch(id)
	{
		case 0:U1_CS=0;U2_CS=0;U3_CS=0;U4_CS=0;break;
		case 1:U1_CS=0;break;
		case 2:U2_CS=0;break;
		case 3:U3_CS=0;break;
		case 4:U4_CS=0;break;
	}
}
void SPI_Write_2Byte(unsigned char id,unsigned char addr,unsigned char dat)
{
	char i=0;
	volatile unsigned buf;
	volatile unsigned char SPI_adr, SPI_data;
	SPI_adr =addr;
	SPI_data =dat;

	Spi_CS_Low(id);
	SSCI2BUFR=SPI_adr; //�������͵��ֽڷ��뷢�ͻ�����
	while(!SSCI2IF); //�ȴ�������Ϻ�SSCIIF��λ��
	SSCI2IF=0;
	buf =SSCI2BUFR;

	SSCI2BUFR=SPI_data; //�������͵��ֽڷ��뷢�ͻ�����
	while(!SSCI2IF); //�ȴ�������Ϻ�SSCIIF��λ��
	SSCI2IF=0;
	buf =SSCI2BUFR;
	Spi_CS_High(id);
}
void IS31FL3265B_Init(void)
{
	spi_init();
	SPI_Write_2Byte(0,0x00,0x01);
}
