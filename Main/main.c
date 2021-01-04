/*
 * @Author: luoqi 
 * @Date: 2021-01-04 08:45:16 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-04 09:55:02
 */
#include "sys.h"
#include "delay.h"  
#include "oled.h"
#include "timer.h"
#include "stm32f4xx.h"


int main(void)
{ 	
	unsigned int k = 0;
	OLED_Init();
	OLED_Clear();
	
	delay_init(168); 
	
 	TIM3_Int_Init(65535,8400-1);//10kHz counter freq, maximum 6.6 sec.
	OLED_ShowString(1,0,"***+++SSSS+++**",16);
	while(1)
	{	
		OLED_ShowNum(3, 2, k, 10, 16);
		k = k + 1;
		delay_ms(1000);
	}
}
 
