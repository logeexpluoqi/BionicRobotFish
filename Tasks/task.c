/*
 * @Author: luoqi 
 * @Date: 2021-01-04 10:41:02 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-13 17:04:49
 */

#include "task.h"
#include "time_slice.h"
#include "system_run.h"
#include "keyboard_task.h"
#include "ak_motor_ctrl_task.h"

#include "usart.h"

extern UsartMsgTypedef usart1_msg;

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
    ak_motor_ctrl_task();
}

void task_20ms()
{
    key_scan();
}

void task_50ms()
{
    oled_sys_run();
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
    led_sys_run();
}

void task_5s()
{
    unsigned char i;
	for(i = 0; i < 26; i++)
	{
		usart1_msg.tx_data[i] = 'A' + i;
        usart1_msg.tx_data[i+26] = 'a' + i;
        usart1_msg.tx_data[i+36] = '0' + i;
	}
    usart1_tx_data_dma();
}
