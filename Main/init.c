/*
 * @Author: luoqi 
 * @Date: 2021-01-04 13:44:41 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-12 16:37:48
 */

#include "init.h"
#include "misc.h"
#include "delay.h"
#include "time_slice.h"
#include "timer.h"
#include "oled_task.h"
#include "led.h"
#include "can.h"
#include "keyboard.h"
#include "ak_motor.h"
#include "usart.h"

void sys_init()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    delay_init(168);
    sys_disp_init();
    led_init();
	keyboard_init();
    usart1_init_dma();
    usart1_init(115200);
    can_init();
    ak_motor_ctrl_init();
    tim5_int_init(500-1, 8400-1);

    time_slice_init();
}
