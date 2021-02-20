/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:54:24 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-12 14:56:15
 */
#ifndef _USART_H
#define _USART_H

#define USART_TX_LEN_MAX 300
#define USART_RX_LEN_MAX 300

typedef enum 
{
    USART_1
} UsartPort;
typedef struct 
{
    unsigned char tx_en; // 1: enable, 0: disable
    unsigned char rx_en; // 1: enable, 0: disable
    unsigned char tx_data[USART_TX_LEN_MAX];
    unsigned char rx_data[USART_RX_LEN_MAX];
    unsigned char rx_cnt;
} UsartMsgTypedef;


void usart1_init(unsigned int bound);
void usart1_init_dma(void);
void usart1_tx_data(unsigned char *tx_data);
void usart1_dma_tx_data(unsigned char *msg, unsigned char len);

unsigned char get_usart_tx_flag(UsartPort port);
unsigned char get_usart_rx_flag(UsartPort port);
void usart_clear_tx_flag(UsartPort port);
void usart_clear_rx_flag(UsartPort port);
void usart_set_tx_flag(UsartPort port);
void usart_set_rx_flag(UsartPort port);

#endif
