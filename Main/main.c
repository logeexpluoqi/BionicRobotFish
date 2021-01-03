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
	
 	TIM3_Int_Init(65535,8400-1);//10Khz计数频率,最大计时6.5秒超出
	OLED_ShowString(1,0,"***+++SSSS+++**",16);
	while(1)
	{	
		OLED_ShowNum(3, 2, k, 10, 16);
		k = k + 1;
		delay_ms(50);
	}
}
 
