/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:33:45 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-04 09:36:37
 */
/* 
 * @brief: This file is created by 正点原子, modified by luoqi
 */

#pragma once
#ifndef _SYS_H
#define _SYS_H	 
#include "stm32f4xx.h" 

/* Bit-band operation, implement similar funtionality like 51 
 * reference <<The Definitive Guide to ARM Cortex-M3 and Cortex-M4 Processors>>
 * IO operation*/
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 

/* IO port addr remapping */
#define GPIOA_ODR_Addr    (GPIOA_BASE+20) //0x40020014
#define GPIOB_ODR_Addr    (GPIOB_BASE+20) //0x40020414 
#define GPIOC_ODR_Addr    (GPIOC_BASE+20) //0x40020814 
#define GPIOD_ODR_Addr    (GPIOD_BASE+20) //0x40020C14 
#define GPIOE_ODR_Addr    (GPIOE_BASE+20) //0x40021014 
#define GPIOF_ODR_Addr    (GPIOF_BASE+20) //0x40021414    
#define GPIOG_ODR_Addr    (GPIOG_BASE+20) //0x40021814   
#define GPIOH_ODR_Addr    (GPIOH_BASE+20) //0x40021C14    
#define GPIOI_ODR_Addr    (GPIOI_BASE+20) //0x40022014     

#define GPIOA_IDR_Addr    (GPIOA_BASE+16) //0x40020010 
#define GPIOB_IDR_Addr    (GPIOB_BASE+16) //0x40020410 
#define GPIOC_IDR_Addr    (GPIOC_BASE+16) //0x40020810 
#define GPIOD_IDR_Addr    (GPIOD_BASE+16) //0x40020C10 
#define GPIOE_IDR_Addr    (GPIOE_BASE+16) //0x40021010 
#define GPIOF_IDR_Addr    (GPIOF_BASE+16) //0x40021410 
#define GPIOG_IDR_Addr    (GPIOG_BASE+16) //0x40021810 
#define GPIOH_IDR_Addr    (GPIOH_BASE+16) //0x40021C10 
#define GPIOI_IDR_Addr    (GPIOI_BASE+16) //0x40022010 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  // input 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  // output

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  // input  
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  // output 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  // input  
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  // output 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  // input  
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  // output 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  // input  
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  // output

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  // input  
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  // output

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  // input  
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  // output

#define PHout(n)   BIT_ADDR(GPIOH_ODR_Addr,n)  // input 
#define PHin(n)    BIT_ADDR(GPIOH_IDR_Addr,n)  // output

#define PIout(n)   BIT_ADDR(GPIOI_ODR_Addr,n)  // input  
#define PIin(n)    BIT_ADDR(GPIOI_IDR_Addr,n)  // output

/* ASM function */
void WFI_SET(void);		// run WFI instruct 
void INTX_DISABLE(void);// close all interrupt 
void INTX_ENABLE(void);	// open all interruput 
void MSR_MSP(u32 addr);	// set stack addr 
#endif











