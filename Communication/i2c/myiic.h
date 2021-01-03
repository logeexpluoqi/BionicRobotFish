#ifndef __MYIIC_H
#define __MYIIC_H
#include "sys.h"

#define u8 unsigned char
//IO��������
#define SDA_IN()                          \
	{                                     \
		GPIOF->MODER &= ~(3 << (10 * 2)); \
		GPIOF->MODER |= 0 << 10 * 2;      \
		GPIOF->PUPDR &= ~(3 << (10 * 2)); \
		GPIOF->PUPDR |= 0 << (10 * 2);    \
	} //PF10����ģʽ

#define SDA_OUT()                         \
	{                                     \
		GPIOF->MODER &= ~(3 << (10 * 2)); \
		GPIOF->MODER |= 1 << 10 * 2;      \
		GPIOF->PUPDR &= ~(3 << (10 * 2)); \
		GPIOF->PUPDR |= 1 << (10 * 2);    \
	} //PF10���ģʽ
//IO��������
#define IIC_SCL PFout(8)  //SCL
#define IIC_SDA PFout(10) //SDA
#define READ_SDA PFin(10) //����SDA

//IIC���в�������
void IIC_Init(void);				 //��ʼ��IIC��IO��
void IIC_Start(void);				 //����IIC��ʼ�ź�
void IIC_Stop(void);				 //����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);			 //IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack); //IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void);				 //IIC�ȴ�ACK�ź�
void IIC_Ack(void);					 //IIC����ACK�ź�
void IIC_NAck(void);				 //IIC������ACK�ź�

void IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 IIC_Read_One_Byte(u8 daddr, u8 addr);
#endif
