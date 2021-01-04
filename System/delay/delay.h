/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:40:23 
 * @Last Modified by:   luoqi 
 * @Last Modified time: 2021-01-04 09:40:23 
 */
/* 
 * @brief: This file is created by 正点原子, modified by luoqi
 */
#ifndef _DELAY_H
#define _DELAY_H 			   
#include <sys.h>	    
 
void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif

