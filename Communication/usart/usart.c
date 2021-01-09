/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:54:11 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-09 10:35:57
 */
/* 
 * @brief: This file is created by 正点原子, modified by luoqi
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

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //使能USART1时钟

	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); //GPIOA10复用为USART1

	//USART1端口配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化PA9，PA10

	//USART1 初始化设置
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
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		res = USART_ReceiveData(USART1); //(USART1->DR), 读取接收到的数据

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
						usart_rx_state = 0; //接收数据错误,重新开始接收
				}
			}
		}
	}
}

