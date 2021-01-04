/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:45:36 
 * @Last Modified by:   luoqi 
 * @Last Modified time: 2021-01-04 09:45:36 
 */
/* 
 * @brief: This file is created by 正点原子, modified by luoqi
 */
#include "sys.h"  

//系统时钟初始化	
//包括时钟设置/中断管理/GPIO设置等

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(u32 addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}
















