/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:54:11 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-09 10:35:57
 */
/* 
 * @brief: This file is created by ����ԭ��, modified by luoqi
 */
#include "usart.h"

unsigned char usart_rx_data_buf[USART_REC_LEN]; // receive data buff,  max byte: USART_REC_LEN 

/* bit 15: receive a frame data finish flag 
 * bit 14: receive 0x0d, CR symbal
 * bit 13-0: receive data length x, x Byte, range: 0-2^14
 */
unsigned short usart_rx_state = 0; //uart receive state

/* *********************************************************** */
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
/* ************************************************************ */

void usart_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //ʹ��USART1ʱ��

	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //GPIOA10����ΪUSART1

	//USART1�˿�����
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//��ʼ��PA9��PA10

	//USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

	USART_Cmd(USART1, ENABLE);

	USART_ClearFlag(USART1, USART_FLAG_TC);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/* UART data receive interrupt function
 * data must start of 0x0d and end of 0x0a
 * example: 0x0d | data_0 | data_1 | .... | 0x0a
 */
void USART1_IRQHandler(void)
{
	u8 res;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		res = USART_ReceiveData(USART1); //(USART1->DR), ��ȡ���յ�������

		if ((usart_rx_state & 0x8000) == 0) // receive not finished
		{
			if (usart_rx_state & 0x4000) // receive 0x0d
			{
				if (res != 0x0a)
					usart_rx_state = 0; // receive fault, start agin
				else
					usart_rx_state |= 0x8000; // receive finished
			}
			else // not receive 0x0d, CR symbol
			{
				if (res == 0x0d)
					usart_rx_state |= 0x4000;
				else
				{
					usart_rx_data_buf[usart_rx_state & 0X3FFF] = res;
					usart_rx_state++;
					if (usart_rx_state > (USART_REC_LEN - 1))
						usart_rx_state = 0; //�������ݴ���,���¿�ʼ����
				}
			}
		}
	}
}

