/*
 * @Author: luoqi 
 * @Date: 2021-01-04 13:44:41 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-04 19:29:18
 */

#include "init.h"
#include "delay.h"
#include "time_slice.h"
#include "oled.h"
#include "led.h"

void sys_init()
{
    delay_init(168);
    OLED_Init();
    OLED_Clear();
    led_init();
    
    time_slice_init();

    OLED_ShowString(30,0,"WIAS_DSFT",16);
	OLED_ShowString(64, 7, "by luoqi", 12);
}
