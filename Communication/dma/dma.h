/*
 * @Author: luoqi 
 * @Date: 2021-01-12 11:22:23 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-12 14:28:35
 */

#ifndef _DMA_H
#define _DMA_H
#include "stm32f4xx.h"

#define USART_DMA_RCV_BUF_SIZE 512

void usart_dma_tx_init(void);
void usart_dma_tx_config(u8* mem_addr, u32 mem_size);  
void usart_dma_tx_data(DMA_Stream_TypeDef *DMA_Streamx, u16 ndtr); // dma send onece

void usart_dma_rx_init(u8* mem_addr, u32 mem_size);

#endif
