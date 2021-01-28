/*
 * @Author: luoqi 
 * @Date: 2021-01-12 11:24:53 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-12 14:26:15
 */

#include "dma.h"
#include "misc.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_rcc.h"

void usart_dma_init()
{
    NVIC_InitTypeDef NVIC_InitStructure;

    DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);
}


/* DMAx configuration channels
 * This transfer function need user to modify in different conditon
 * memory -> peripherals / data width: 8bit / memory inc mode
 * DMA_Streamx: DMA data flow, DMA1_Stream0 - 7 / DMA2_Stream0 - 7
 * chx: DMA channels, @ref DMA_channel DMA_Channel_0 - DMA_Channel_7
 * par: peripheral address
 * mar: memory address
 * ndtr: data transfer num
 */
void usart_dma_tx_config(DMA_Stream_TypeDef *DMA_Streamx, u32 chx, u32 par, u32 mar, u16 ndtr)
{

    DMA_InitTypeDef DMA_InitStructure;

    if ((u32)DMA_Streamx > (u32)DMA2) 
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); 
    }
    else
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); 
    }
    DMA_DeInit(DMA_Streamx);

    while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE); // wait for dma configuration

    /* DMA Stream */
    DMA_InitStructure.DMA_Channel               = chx;
    DMA_InitStructure.DMA_PeripheralBaseAddr    = par;
    DMA_InitStructure.DMA_Memory0BaseAddr       = mar;
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize            = ndtr;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte; 
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority              = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode              = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold         = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst           = DMA_MemoryBurst_Single;       // memory burst single transfer
    DMA_InitStructure.DMA_PeripheralBurst       = DMA_PeripheralBurst_Single;   // peripheral burst single transfer
    DMA_Init(DMA_Streamx, &DMA_InitStructure); // Stream initialize DMA_Stream
}

// void usart_dma_rx_config(DMA_Stream_TypeDef *DMA_Streamx, u32 chx, u32 par, u32 mar, u16 ndtr)

/* DMA transfer once
 * @param DMA_Streamx:DMA data flow, DMA1_Stream0~7 / DMA2_Stream0~7
 * @param ndtr: data transfer 
 */
void usart_dma_tx_data(DMA_Stream_TypeDef *DMA_Streamx, u16 ndtr)
{

    DMA_Cmd(DMA_Streamx, DISABLE); // cloese dma transferd
    while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE);
    DMA_SetCurrDataCounter(DMA_Streamx, ndtr); // datatramsfer counter
    DMA_Cmd(DMA_Streamx, ENABLE); //start dma transfer
}

void DMA2_Stream7_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) != RESET)
	{
		DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_FEIF7);
		DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
	}
}
