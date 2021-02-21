/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:54:24 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-12 14:56:15
 */
#ifndef _USART_H
#define _USART_H
#include "config.h"

#define USART_TX_LEN_MAX 300
#define USART_RX_LEN_MAX 300

typedef enum 
{
    USART_1
} UsartPort;
typedef struct 
{
    uint8_t tx_en; // 1: enable, 0: disable
    uint8_t rx_en; // 1: enable, 0: disable
    uint8_t tx_data[USART_TX_LEN_MAX];
    uint8_t rx_data[USART_RX_LEN_MAX];
    uint8_t rx_cnt;
} UsartMsgTypedef;


void usart1_init(uint32_t bound);
void usart1_init_dma(void);
void usart1_tx_data(uint8_t *tx_data);
void usart1_dma_tx_data(uint8_t *msg, uint8_t len);

uint8_t get_usart_tx_flag(UsartPort port);
uint8_t get_usart_rx_flag(UsartPort port);
void usart_clear_tx_flag(UsartPort port);
void usart_clear_rx_flag(UsartPort port);
void usart_set_tx_flag(UsartPort port);
void usart_set_rx_flag(UsartPort port);

#endif
