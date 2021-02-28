/*
 * @Author: luoqi 
 * @Date: 2021-01-13 16:54:58 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-13 17:05:31
 */

#include "system_run.h"
#include "oled_task.h"
#include "led.h"

void led_sys_run()
{
    static uint8_t state = 0;
    
    state = !state;
    if(state)
    {
        led_set(LED_0, LED_ON);
    }
    else
    {
        led_set(LED_0, LED_OFF);
    }
}

void oled_sys_run()
{
    static uint8_t i = 0;
    i = i + 1;
    sys_disp_num(35, 7, i, 3, SMALL);
}


