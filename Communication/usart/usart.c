/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:54:11 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-27 13:13:34
 */
#include "usart.h"
#include "config.h"
#include "stdio.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_dma.h"
#include "msg_distribute.h"
#include "dma.h"
#include "misc.h"
#include "ak_motor.h"

extern AkMotorCtrlTypedef ak_motor_ctrl_data;

UsartMsgTypedef usart1_msg;

static unsigned char usart_dma_rx_buf[USART_RX_LEN + 2]; // 2 byte is SOF and EOF

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
	while ((USART1->SR & 0X40) == 0); // until send over
	USART1->DR = (u8)ch;
	return ch;
}
#endif
/* ****************************end***************************** */

void usart1_init(unsigned int bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	/* GPIO init */
	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* USART port init */
	USART_InitStructure.USART_BaudRate 			  = bound;
	USART_InitStructure.USART_WordLength 		  = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits 		      = USART_StopBits_1;
	USART_InitStructure.USART_Parity 			  = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode 				  = USART_Mode_Rx | USART_Mode_Tx;
 	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);

	/* USART interrupt init */
	NVIC_InitStructure.NVIC_IRQChannel 					 = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd 				 = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	usart_dma_rx_init(usart_dma_rx_buf, 30);
	usart_dma_tx_init();

	USART_ClearFlag(USART1, USART_FLAG_IDLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);

	usart1_msg.rx_cnt = 0;
}

void usart1_dma_tx_data(unsigned char *msg, unsigned char len)
{
	usart_dma_tx_config(msg, len);
	usart_dma_tx_data(DMA2_Stream7, len);
}

/* UART data receive interrupt function
 * data must start of '{' and end of '}'
 * and mast confirm the data length
 * example: '{' | data_0 | data_1 | .... | '}'
 */
void USART1_IRQHandler(void)
{
	unsigned char rx_len;

	if(USART_GetFlagStatus(USART1, USART_FLAG_IDLE) != RESET)
	{
		DMA_Cmd(DMA2_Stream5, DISABLE);
		USART1 -> SR; // clear flag
		USART1 -> DR; // clear flag
		rx_len = USART_DMA_RCV_BUF_SIZE - DMA_GetCurrDataCounter(DMA2_Stream5);
		if((rx_len != 0) && (usart_dma_rx_buf[0] == '{') && (usart_dma_rx_buf[USART_RX_LEN + 1] == '}'))
		{
			mem_cpy(usart_dma_rx_buf + 1, usart1_msg.rx_data, USART_RX_LEN);
			usart1_msg.tx_en = 1;
			#if ! AK_MOTOR_GROUP_CTRL
				msg_distribute(usart1_msg.rx_data);
			#endif
		}
    	DMA_SetCurrDataCounter(DMA2_Stream5, USART_DMA_RCV_BUF_SIZE);
		DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF5 | DMA_FLAG_FEIF5 | DMA_FLAG_DMEIF5 | DMA_FLAG_TEIF5 | DMA_FLAG_HTIF5);
		USART_ClearITPendingBit(USART1, USART_IT_TC); 
    	DMA_Cmd(DMA2_Stream5, ENABLE);
	}
}

