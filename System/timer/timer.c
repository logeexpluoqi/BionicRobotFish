/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:46:07 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-04 09:53:40
 */
#include "timer.h"
#include "time_slice.h"
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "oled_task.h"
#include "misc.h"

/* Timer initialize
 * arr: auto reload value
 * psc: clock prescale value
 * timer overflow time: Tout = ((arr + 1) * (psc + 1)) / Ft us
 * Ft: timer working frequency, unit MHz
 */

/* Used to time slice system clock 
 *
 */
void tim3_int_init(uint16_t arr, uint16_t psc)
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // enable TIM3 clock 

	TIM_TimeBaseInitStructure.TIM_Period  		= arr;						
	TIM_TimeBaseInitStructure.TIM_Prescaler  	= psc;				
	TIM_TimeBaseInitStructure.TIM_CounterMode  	= TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); // enable TIM3 update interrupt 
	TIM_Cmd(TIM3, ENABLE);					   // enable TIM3

	NVIC_InitStructure.NVIC_IRQChannel  				 = TIM3_IRQn;				 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority 		 = 1;		 
	NVIC_InitStructure.NVIC_IRQChannelCmd  				 = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


/* Used to count run-time */
void tim5_int_init()
{ 
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); // enable TIM3 clock 

	TIM_TimeBaseInitStructure.TIM_Prescaler 	= 839;
	TIM_TimeBaseInitStructure.TIM_CounterMode 	= TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStructure);
	TIM_UpdateDisableConfig(TIM5, DISABLE);
	TIM_Cmd(TIM5, DISABLE);
}

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) == SET) 
	{
		time_slice_tick();
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
}
