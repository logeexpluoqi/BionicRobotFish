/*
 * @Author: luoqi 
 * @Date: 2021-01-04 08:45:16 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-04 18:51:05
 */
#include "init.h"
#include "delay.h"  
#include "oled.h"
#include "time_slice.h"

int main(void)
{ 		
	sys_init();
	
	OLED_ShowString(1,0,"***+++SSSS+++**",16);
	while(1)
	{	
		time_slice_process();
	}
}
 
