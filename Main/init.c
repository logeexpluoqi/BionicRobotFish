/*
 * @Author: luoqi 
 * @Date: 2021-01-04 13:44:41 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-03-12 13:38:39
 */

#include "stdio.h"
#include "init.h"
#include "misc.h"
#include "delay.h"
#include "time_slice.h"
#include "timer.h"
#include "oled_task.h"
#include "led.h"
#include "can.h"
#include "keyboard.h"
#include "usart.h"
#include "msg_box.h"
#include "imu.h"

void sys_init()
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
    delay_init(168);
    sys_clk_init();
#if USING_SYS_DISP
    sys_disp_init();
#endif
    sys_config_display();
    led_init();
	keyboard_init();
    usart1_init(115200);
    can1_init();
    msgbox_init();
    imu_init();
    time_slice_init();
    sys_disp_str(72, 7, "by luoqi", SMALL);
}
