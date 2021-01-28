/*
 * @Author: luoqi 
 * @Date: 2021-01-04 13:44:41 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-12 16:37:48
 */

#include "stdio.h"
#include "init.h"
#include "config.h"
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
#include "dma.h"

void sys_init()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
    delay_init(168);
    sys_disp_init();
    sys_config_display();
    led_init();
	keyboard_init();
    usart1_init(921600);
    can_init();
    ak_motor_ctrl_init();
    time_slice_init();

    sys_disp_str(72, 7, "by luoqi", SMALL);
}
