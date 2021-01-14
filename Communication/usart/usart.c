/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:54:11 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-14 08:39:54
 */
/* 
 * @brief: This file is created by 正点原子, modified by luoqi
 */
#include "usart.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_usart.h"
#include "dma.h"
#include "msg_distribute.h"

#include "oled_task.h"

UsartMsgTypedef usart1_msg;

/* ****************************start**************************** */
#if 1
#pragma import(__use_no_semihosting)

/* std lib requirs functions */
struct __FILE
{
	int handle;
};

FILE __stdout;

/* Define this function to avoid use half master mode */
_sys_exit(int x)
{
	x = x;
}

/* Redefine fputc function */
int fputc(int ch, FILE *f)
{
	while ((USART1->SR & 0X40) == 0)
		; // until send over
	USART1->DR = (u8)ch;
	return ch;
}
#endif
/* ****************************end***************************** */

void usart1_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 	USART_Init(USART1, &USART_InitStructure);

	DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	DMA_ITConfig(DMA2_Stream7, DMA_IT_TC, ENABLE);

	USART_ClearFlag(USART1, USART_FLAG_RXNE);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);
	usart1_msg.rx_state = 0x0000;
}

void usart1_init_dma()
{
	dma_config(DMA2_Stream7, DMA_Channel_4, (u32)&USART1->DR, (u32)usart1_msg.tx_data, USART_TX_LEN);
}

void usart1_tx_data(unsigned char *tx_data)
{
	unsigned char i;

	for (i = 0; i < USART_TX_LEN; i++)
	{
		USART_SendData(USART1, tx_data[i]);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
}

void usart1_dma_tx_data(unsigned char msg[USART_TX_LEN], unsigned char len)
{
	dma_config(DMA2_Stream7, DMA_Channel_4, (u32)&USART1->DR, (u32)msg, len);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	dma_tx_data(DMA2_Stream7, len);
}

/* UART data receive interrupt function
 * data must start of 0x0d and end of 0x0a
 * example: 0x0d | data_0 | data_1 | .... | 0x0a
 */
void USART1_IRQHandler(void)
{
	unsigned char usart_rx_byte_data;
	unsigned char i;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		usart_rx_byte_data = USART_ReceiveData(USART1); // (USART1->DR), read usart receive register
		for(i = 0; i<112; i++)
		{
			usart1_msg.tx_data[i] = usart_rx_byte_data;
		}
		usart1_dma_tx_data(usart1_msg.tx_data, 112);
		if ((usart1_msg.rx_state & 0x8000) == 0) // receive not finished
		{
			if (usart1_msg.rx_state & 0x4000) // receive 0x0d
			{
				if (usart_rx_byte_data != 0x0a)
					usart1_msg.rx_state = 0; // receive fault, restart
				else
				{
					usart1_msg.rx_state |= 0x8000; // receive finished
					
					/* *************** Receice data decode ************** */
					msg_distribute(usart1_msg.rx_data);
					
					/* ************************************************** */
					for(i = 0; i < 28; i ++)
					{
						printf("%x ", usart1_msg.rx_data[i]);
					}
				}
			}
			else // not receive 0x0d, CR symbol
			{
				if (usart_rx_byte_data == 0x0d)
					usart1_msg.rx_state |= 0x4000;
				else
				{
					usart1_msg.rx_data[usart1_msg.rx_state & 0X3FFF] = usart_rx_byte_data;
					usart1_msg.rx_state++;
					if (usart1_msg.rx_state > (USART_RX_LEN - 1))
						usart1_msg.rx_state = 0x0000; // receive fault, restart
				}
			}
		}
	}
	USART_ClearFlag(USART1, USART_FLAG_RXNE);
}

void DMA2_Stream7_IRQHandler(void)
{
	sys_disp_str(0,0,"DMA",LARGE);
	printf("DMA INT");
	if(DMA_GetFlagStatus(DMA2_Stream7, DMA_FLAG_TCIF7) == SET)
	{
		DMA_ClearFlag(DMA2_Stream7, DMA_FLAG_TCIF7);
	}
}
