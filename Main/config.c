/*
 * @Author: luoqi 
 * @Date: 2021-01-19 19:56:32 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-02-17 22:42:33
 */

#include "config.h"
#include "timer.h"
#include "oled_task.h"
#include "stm32f4xx_tim.h"

void sys_disp_config(SysDispState state)
{
    if(state == SYS_DISP_ENABLE)
        sys_disp_open();
    else if(state == SYS_DISP_DISABLE)
        sys_disp_close();
}

void sys_reset()
{
    
}

void sys_clk_init()
{
    /* Resolution ratio is 10us */
    tim5_int_init();
}

void sys_clk_start()
{
    TIM5->CNT = 0;
    TIM_Cmd(TIM5, ENABLE);
}

void sys_clk_stop()
{
    TIM_Cmd(TIM5, DISABLE);
    sys_disp_num(24, 6, (TIM5->CNT)*10, 7, SMALL);
}

void sys_config_display()
{
    sys_disp_config(SYS_DISP_ENABLE);
    sys_disp_str(0, 0, "SysInfo: ", SMALL);
    sys_disp_str(0, 2, "M_Num: ", SMALL);
    sys_disp_num(64, 2, AK_MOTOR_NUM_MAX, 1, SMALL);
    sys_disp_str(0, 6, "T: ", SMALL);
}
