/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:54:24 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-12 14:56:15
 */
#ifndef _USART_H
#define _USART_H

#define USART_TX_LEN 112
#define USART_RX_LEN 28

typedef struct 
{
    unsigned char tx_en; // 1: enable, 0: disable
    unsigned char rx_en; // 1: enable, 0: disable
    unsigned char tx_data[USART_TX_LEN];
    unsigned char rx_data[USART_RX_LEN];
    unsigned char rx_cnt;
} UsartMsgTypedef;


void usart1_init(unsigned int bound);
void usart1_init_dma(void);
void usart1_tx_data(unsigned char *tx_data);
void usart1_dma_tx_data(unsigned char msg[USART_TX_LEN], unsigned char len);
#endif
