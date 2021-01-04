/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:40:15 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-04 09:45:23
 */
/* 
 * @brief: This file is created by ����ԭ��, modified by luoqi
 */
#include "delay.h"
#include "sys.h"

/* If use ucos, need too include follows */
#if SYSTEM_SUPPORT_UCOS
#include "includes.h" //ucos ʹ��
#endif

static u8 fac_us = 0;  //us delay factor 
static u16 fac_ms = 0; //ms delay factor 

#ifdef OS_CRITICAL_METHOD // if defined OS_CRITICAL_METHOD, used ucosII

void SysTick_Handler(void)
{
	OSIntEnter(); // enter interrupt 
	OSTimeTick(); // call ucos clock service 
	OSIntExit();  // trigger soft interruput 
}
#endif

/* Initialize delay function
 * SYSCLK = HCLK/8
 * SYSCLK: system clock
 */
void delay_init(u8 SYSCLK)
{
#ifdef OS_CRITICAL_METHOD //���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
	u32 reload;
#endif
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SYSCLK / 8; //�����Ƿ�ʹ��ucos,fac_us����Ҫʹ��

#ifdef OS_CRITICAL_METHOD					   //���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
	reload = SYSCLK / 8;					   //ÿ���ӵļ������� ��λΪK
	reload *= 1000000 / OS_TICKS_PER_SEC;	   //����OS_TICKS_PER_SEC�趨���ʱ��
											   //reloadΪ24λ�Ĵ���,���ֵ:16777216,��168M��,Լ��0.7989s����
	fac_ms = 1000 / OS_TICKS_PER_SEC;		   //����ucos������ʱ�����ٵ�λ
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; //����SYSTICK�ж�
	SysTick->LOAD = reload;					   //ÿ1/OS_TICKS_PER_SEC���ж�һ��
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  //����SYSTICK
#else
	fac_ms = (u16)fac_us * 1000; //��ucos��,����ÿ��ms��Ҫ��systickʱ����
#endif
}

#ifdef OS_CRITICAL_METHOD //���OS_CRITICAL_METHOD������,˵��ʹ��ucosII��.
//��ʱnus
//nus:Ҫ��ʱ��us��.
void delay_us(u32 nus)
{
	u32 ticks;
	u32 told, tnow, tcnt = 0;
	u32 reload = SysTick->LOAD; //LOAD��ֵ
	ticks = nus * fac_us;		//��Ҫ�Ľ�����
	tcnt = 0;
	OSSchedLock();		 //��ֹucos���ȣ���ֹ���us��ʱ
	told = SysTick->VAL; //�ս���ʱ�ļ�����ֵ
	while (1)
	{
		tnow = SysTick->VAL;
		if (tnow != told)
		{
			if (tnow < told)
				tcnt += told - tnow; //����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else
				tcnt += reload - tnow + told;
			told = tnow;
			if (tcnt >= ticks)
				break; //ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}
	};
	OSSchedUnlock(); //����ucos����
}
//��ʱnms
//nms:Ҫ��ʱ��ms��
void delay_ms(u16 nms)
{
	if (OSRunning == OS_TRUE && OSLockNesting == 0) //���os�Ѿ�������
	{
		if (nms >= fac_ms) //��ʱ��ʱ�����ucos������ʱ������
		{
			OSTimeDly(nms / fac_ms); //ucos��ʱ
		}
		nms %= fac_ms; //ucos�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ
	}
	delay_us((u32)(nms * 1000)); //��ͨ��ʽ��ʱ
}
#else //����ucosʱ

/* Delay n us 
 * n < 78915
 */
void delay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD = nus * fac_us;			  //ʱ�����
	SysTick->VAL = 0x00;					  //��ռ�����
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //��ʼ����
	do
	{
		temp = SysTick->CTRL;
	} while ((temp & 0x01) && !(temp & (1 << 16))); //�ȴ�ʱ�䵽��
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;		//�رռ�����
	SysTick->VAL = 0X00;							//��ռ�����
}

/* Delay n ms
 * SysTick -> load is a 24bit register
 * nms < 0xfffffff*8*1000 / SYSCLK
 * SYSCLK unit is Hz, nms unit is ms
 * SYSCLK = 168MHz, nms <= 798ms
 */
void delay_xms(u16 nms)
{
	u32 temp;
	SysTick->LOAD = (u32)nms * fac_ms;		  //ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL = 0x00;					  //��ռ�����
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //��ʼ����
	do
	{
		temp = SysTick->CTRL;
	} while ((temp & 0x01) && !(temp & (1 << 16))); //�ȴ�ʱ�䵽��
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;		//�رռ�����
	SysTick->VAL = 0X00;							//��ռ�����
}

/* Delay n ms
 * nms: 0 - 65535
 */
void delay_ms(u16 nms)
{
	u8 repeat = nms / 540; //������540,�ǿ��ǵ�ĳЩ�ͻ����ܳ�Ƶʹ��,
						   //���糬Ƶ��248M��ʱ��,delay_xms���ֻ����ʱ541ms������
	u16 remain = nms % 540;
	while (repeat)
	{
		delay_xms(540);
		repeat--;
	}
	if (remain)
		delay_xms(remain);
}
#endif
