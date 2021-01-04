/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:40:15 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-04 09:45:23
 */
/* 
 * @brief: This file is created by 正点原子, modified by luoqi
 */
#include "delay.h"
#include "sys.h"

/* If use ucos, need too include follows */
#if SYSTEM_SUPPORT_UCOS
#include "includes.h" //ucos 使用
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
#ifdef OS_CRITICAL_METHOD //如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
	u32 reload;
#endif
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SYSCLK / 8; //不论是否使用ucos,fac_us都需要使用

#ifdef OS_CRITICAL_METHOD					   //如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
	reload = SYSCLK / 8;					   //每秒钟的计数次数 单位为K
	reload *= 1000000 / OS_TICKS_PER_SEC;	   //根据OS_TICKS_PER_SEC设定溢出时间
											   //reload为24位寄存器,最大值:16777216,在168M下,约合0.7989s左右
	fac_ms = 1000 / OS_TICKS_PER_SEC;		   //代表ucos可以延时的最少单位
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; //开启SYSTICK中断
	SysTick->LOAD = reload;					   //每1/OS_TICKS_PER_SEC秒中断一次
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  //开启SYSTICK
#else
	fac_ms = (u16)fac_us * 1000; //非ucos下,代表每个ms需要的systick时钟数
#endif
}

#ifdef OS_CRITICAL_METHOD //如果OS_CRITICAL_METHOD定义了,说明使用ucosII了.
//延时nus
//nus:要延时的us数.
void delay_us(u32 nus)
{
	u32 ticks;
	u32 told, tnow, tcnt = 0;
	u32 reload = SysTick->LOAD; //LOAD的值
	ticks = nus * fac_us;		//需要的节拍数
	tcnt = 0;
	OSSchedLock();		 //阻止ucos调度，防止打断us延时
	told = SysTick->VAL; //刚进入时的计数器值
	while (1)
	{
		tnow = SysTick->VAL;
		if (tnow != told)
		{
			if (tnow < told)
				tcnt += told - tnow; //这里注意一下SYSTICK是一个递减的计数器就可以了.
			else
				tcnt += reload - tnow + told;
			told = tnow;
			if (tcnt >= ticks)
				break; //时间超过/等于要延迟的时间,则退出.
		}
	};
	OSSchedUnlock(); //开启ucos调度
}
//延时nms
//nms:要延时的ms数
void delay_ms(u16 nms)
{
	if (OSRunning == OS_TRUE && OSLockNesting == 0) //如果os已经在跑了
	{
		if (nms >= fac_ms) //延时的时间大于ucos的最少时间周期
		{
			OSTimeDly(nms / fac_ms); //ucos延时
		}
		nms %= fac_ms; //ucos已经无法提供这么小的延时了,采用普通方式延时
	}
	delay_us((u32)(nms * 1000)); //普通方式延时
}
#else //不用ucos时

/* Delay n us 
 * n < 78915
 */
void delay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD = nus * fac_us;			  //时间加载
	SysTick->VAL = 0x00;					  //清空计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开始倒数
	do
	{
		temp = SysTick->CTRL;
	} while ((temp & 0x01) && !(temp & (1 << 16))); //等待时间到达
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;		//关闭计数器
	SysTick->VAL = 0X00;							//清空计数器
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
	SysTick->LOAD = (u32)nms * fac_ms;		  //时间加载(SysTick->LOAD为24bit)
	SysTick->VAL = 0x00;					  //清空计数器
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开始倒数
	do
	{
		temp = SysTick->CTRL;
	} while ((temp & 0x01) && !(temp & (1 << 16))); //等待时间到达
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;		//关闭计数器
	SysTick->VAL = 0X00;							//清空计数器
}

/* Delay n ms
 * nms: 0 - 65535
 */
void delay_ms(u16 nms)
{
	u8 repeat = nms / 540; //这里用540,是考虑到某些客户可能超频使用,
						   //比如超频到248M的时候,delay_xms最大只能延时541ms左右了
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
