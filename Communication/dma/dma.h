/*
 * @Author: luoqi 
 * @Date: 2021-01-12 11:22:23 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-12 14:28:35
 */

#ifndef _DMA_H
#define _DMA_H
#include "stm32f4xx.h"

void dma_config(DMA_Stream_TypeDef *DMA_Streamx, u32 chx,u32 par, u32 mar, u16 ndtr);  
void dma_tx_data(DMA_Stream_TypeDef *DMA_Streamx, u16 ndtr); // dma send onece

#endif
