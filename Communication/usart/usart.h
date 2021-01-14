/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:54:24 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-12 14:56:15
 */
/* 
 * @brief: This file is created by 正点原子, modified by luoqi
 */
#ifndef _USART_H
#define _USART_H
#include "stdio.h"
#include "stm32f4xx_conf.h"
#include "sys.h"

#define USART_TX_LEN 112
#define USART_RX_LEN 28 

typedef struct 
{
    unsigned char tx_data[USART_TX_LEN];
    unsigned char rx_data[USART_RX_LEN];

    /* bit 15: receive a frame data finish flag 
     * bit 14: receive 0x0d, CR symbal
     * bit 13-0: receive data length x, x Byte, range: 0-2^14
     */
    unsigned short rx_state;
} UsartMsgTypedef;


void usart1_init(u32 bound);
void usart1_init_dma(void);
void usart1_tx_data(unsigned char *tx_data);
void usart1_dma_tx_data(unsigned char msg[USART_TX_LEN], unsigned char len);
#endif
