/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:54:24 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-09 10:34:36
 */
/* 
 * @brief: This file is created by 正点原子, modified by luoqi
 */
#ifndef _USART_H
#define _USART_H
#include "stdio.h"
#include "stm32f4xx_conf.h"
#include "sys.h"

#define USART_REC_LEN 200 

void usart_init(u32 bound);
#endif
