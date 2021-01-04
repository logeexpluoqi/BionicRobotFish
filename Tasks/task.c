/*
 * @Author: luoqi 
 * @Date: 2021-01-04 10:41:02 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-04 18:49:26
 */

#include "task.h"
#include "time_slice.h"

#include "oled.h"
#include "led.h"

TimeSliceInitTypedef Tasks[TASK_NUM] = 
{
    {0, 5, 5, task_5ms},
    {0, 10, 10 ,task_10ms},
    {0, 20, 20, task_20ms},
    {0, 50, 50, task_50ms},
    {0, 100, 100, task_100ms},
    {0, 200, 200, task_200ms},
    {0, 500, 500, task_500ms},
    {0, 1000, 1000, task_1s},
    {0, 5000, 5000, task_5s}
};

void task_5ms()
{

}

void task_10ms()
{

}

void task_20ms()
{

}

void task_50ms()
{

}

void task_100ms()
{
    
}

void task_200ms()
{

}

void task_500ms()
{

}

void task_1s()
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

void task_5s()
{
    
}
