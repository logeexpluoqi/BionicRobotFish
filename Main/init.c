/*
 * @Author: luoqi 
 * @Date: 2021-01-04 13:44:41 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-08 13:54:16
 */

#include "init.h"
#include "delay.h"
#include "time_slice.h"
#include "oled.h"
#include "led.h"
#include "can.h"
#include "keyboard.h"
#include "ak_motor.h"

void sys_init()
{
    delay_init(168);
    OLED_Init();
    OLED_Clear();
    led_init();
	keyboard_init();
    ak_motor_ctrl_init();
    can_init();

    time_slice_init();

    OLED_ShowString(30,0,"WIAS_DSFT",FONT_LARGE);
	OLED_ShowString(72, 7, "by luoqi", FONT_SMALL);
}
