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

static u8 fac_us = 0;  //us delay factor 
static u16 fac_ms = 0; //ms delay factor 


/* Initialize delay function
 * SYSCLK = HCLK/8
 * SYSCLK: system clock
 */
void delay_init(u8 SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SYSCLK / 8; //不论是否使用ucos,fac_us都需要使用

	fac_ms = (u16)fac_us * 1000; //非ucos下,代表每个ms需要的systick时钟数
}


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

