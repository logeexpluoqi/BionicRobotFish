/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:54:11 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-19 20:52:56
 */
#include "usart.h"
#include "config.h"
#include "stdio.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "msg_distribute.h"
#include "dma.h"
#include "misc.h"
#include "ak_motor.h"

extern AkMotorCtrl ak_motor_ctrl_data;

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

void usart1_init(unsigned int bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 	USART_Init(USART1, &USART_InitStructure);

	USART_ClearFlag(USART1, USART_FLAG_RXNE);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);
	usart1_msg.rx_cnt = 0;
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

void usart1_dma_tx_data(unsigned char *msg, unsigned char len)
{
	dma_config(DMA2_Stream7, DMA_Channel_4, (u32)&USART1->DR, (u32)msg, len);
	USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	dma_tx_data(DMA2_Stream7, len);
}


/* UART data receive interrupt function
 * data must start of '{' and end of '}'
 * and mast confirm the data length
 * example: '{' | data_0 | data_1 | .... | '}'
 */
void USART1_IRQHandler(void)
{
	unsigned char i;
	unsigned char usart_rx_byte;
	static unsigned char rx_byte_cnt = 0;
	static unsigned char sof = 0; // start of frame
	static unsigned char rx_frame[USART_RX_LEN];

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		usart_rx_byte = USART_ReceiveData(USART1); // (USART1->DR), read usart receive register
		
		if(rx_byte_cnt == USART_RX_LEN)
		{
			if(usart_rx_byte == '}')
			{
				for(i=0; i< USART_RX_LEN; i++)
				{
					usart1_msg.rx_data[i] = rx_frame[i];
				}
				msg_distribute(usart1_msg.rx_data);
				usart1_msg.tx_en = 1;
			#if CTRL_MODE_ONCE
				ak_motor_ctrl(ak_motor_ctrl_data);
			#endif
			}
			sof = 0;
			rx_byte_cnt = 0;
		}
		else if((sof == 1) && (rx_byte_cnt != USART_RX_LEN))
		{
			rx_frame[rx_byte_cnt] = usart_rx_byte;
			rx_byte_cnt ++;
		}
		if((usart_rx_byte == '{') && (sof == 0))
			sof = 1;
	}
}

