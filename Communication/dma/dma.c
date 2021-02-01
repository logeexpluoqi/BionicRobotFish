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
#include "usart.h"

void usart_dma_tx_init()
{
    NVIC_InitTypeDef NVIC_InitStructure;

    DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
	NVIC_InitStructure.NVIC_IRQChannel                   = DMA2_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);
}

void usart_dma_rx_init(u8* mem_addr, u32 mem_size)
{
    DMA_InitTypeDef DMA_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); 

    DMA_DeInit(DMA2_Stream5);
	while (DMA_GetCmdStatus(DMA2_Stream5) != DISABLE);

	DMA_InitStructure.DMA_Channel               = DMA_Channel_4;
	DMA_InitStructure.DMA_PeripheralBaseAddr    = (u32)&USART1->DR; 
	DMA_InitStructure.DMA_Memory0BaseAddr       = (u32)mem_addr;
	DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize            = USART_DMA_RCV_BUF_SIZE;
	DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority              = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode              = DMA_FIFOMode_Disable;  
	DMA_InitStructure.DMA_FIFOThreshold         = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst           = DMA_MemoryBurst_Single; 
	DMA_InitStructure.DMA_PeripheralBurst       = DMA_PeripheralBurst_Single; 
	DMA_Init(DMA2_Stream5, &DMA_InitStructure);

	DMA_Cmd(DMA2_Stream5, ENABLE); 

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
void usart_dma_tx_config(u8* mem_addr, u32 mem_size)
{

    DMA_InitTypeDef DMA_InitStructure;

    DMA_DeInit(DMA2_Stream7);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); 

    while (DMA_GetCmdStatus(DMA2_Stream7) != DISABLE); // wait for dma configuration

    /* DMA Stream */
    DMA_InitStructure.DMA_Channel               = DMA_Channel_4;
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (u32)&USART1->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr       = (u32)mem_addr;
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_MemoryToPeripheral;
    DMA_InitStructure.DMA_BufferSize            = mem_size;
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte; 
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority              = DMA_Priority_Medium;
    DMA_InitStructure.DMA_FIFOMode              = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold         = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst           = DMA_MemoryBurst_Single;       // memory burst single transfer
    DMA_InitStructure.DMA_PeripheralBurst       = DMA_PeripheralBurst_Single;   // peripheral burst single transfer
    DMA_Init(DMA2_Stream7, &DMA_InitStructure); // Stream initialize DMA_Stream
}

/* DMA transfer once
 * @param DMA_Streamx:DMA data flow, DMA1_Stream0~7 / DMA2_Stream0~7
 * @param ndtr: data transfer 
 */
void usart_dma_tx_data(DMA_Stream_TypeDef *DMA_Streamx, u16 mem_size)
{

    DMA_Cmd(DMA_Streamx, DISABLE); // cloese dma transferd
    while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE);
    DMA_SetCurrDataCounter(DMA_Streamx, mem_size); // datatramsfer counter
    DMA_Cmd(DMA_Streamx, ENABLE); //start dma transfer
}

void DMA2_Stream7_IRQHandler(void)
{
	if(DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) != RESET)
	{
		DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
		DMA_ClearITPendingBit(DMA2_Stream7, DMA_IT_FEIF7);
	}
}
