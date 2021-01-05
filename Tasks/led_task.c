/*
 * @Author: luoqi 
 * @Date: 2021-01-05 22:24:27 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-05 22:25:41
 */

#include "led_task.h"
#include "led.h"

void led_sys_run()
{
    static unsigned char state = 0;
    
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


