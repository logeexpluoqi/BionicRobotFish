/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:10:10 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-04 09:36:24
 */
/* 
 * @bref: This i2c funcition is software-simulated i2c signal
 * 	      This file is created by 正点原子, modified by luoqi
 */
#ifndef _MYIIC_H
#define _MYIIC_H

#include "sys.h"

#define u8 unsigned char

/* IO direction set: in/out */
#define SDA_IN()                          \
	{                                     \
		GPIOB->MODER &= ~(3 << (14 * 2)); \
		GPIOB->MODER |= 0 << 14 * 2;      \
		GPIOB->PUPDR &= ~(3 << (14 * 2)); \
		GPIOB->PUPDR |= 0 << (14 * 2);    \
	} //PF10 input mode

#define SDA_OUT()                         \
	{                                     \
		GPIOB->MODER &= ~(3 << (14 * 2)); \
		GPIOB->MODER |= 1 << 14 * 2;      \
		GPIOB->PUPDR &= ~(3 << (14 * 2)); \
		GPIOB->PUPDR |= 1 << (14 * 2);    \
	} //PF10 output mode

/* IO control function */
#define IIC_SCL PBout(13)  // SCL
#define IIC_SDA PBout(14) // SDA output mode
#define READ_SDA PBin(14) // SDA input mode

/* I2C control function */
void IIC_Init(void);				 // init i2c port
void IIC_Start(void);				 // send i2c start signal
void IIC_Stop(void);				 // send i2c stop signal
void IIC_Send_Byte(u8 txd);			 // i2c send a char
u8 IIC_Read_Byte(unsigned char ack); // i2c read a char 
u8 IIC_Wait_Ack(void);				 // i2c wait ack signal
void IIC_Ack(void);					 // i2c send ack signal 
void IIC_NAck(void);				 // i2c send no ack signal 

void IIC_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 IIC_Read_One_Byte(u8 daddr, u8 addr);
#endif
