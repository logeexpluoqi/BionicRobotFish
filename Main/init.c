/*
 * @Author: luoqi 
 * @Date: 2021-01-04 13:44:41 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-04 18:53:36
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
}
