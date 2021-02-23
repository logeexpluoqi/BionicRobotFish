/*
 * @Author: luoqi 
 * @Date: 2021-01-19 19:56:32 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-02-23 14:57:34
 */

#include "config.h"
#include "timer.h"
#include "oled_task.h"
#include "stm32f4xx_tim.h"
#include "core_cm4.h"

/* @breif: clear memery, after clear, the value is mem_val 
 * @param: *mem, memery position, &s_structure or array;
 * @param: mem_val, set the memery value, uint8_t type.
 */
void mem_set(void* mem, uint8_t mem_val)
{
    uint8_t *p = mem;
    uint32_t m_size = sizeof(p);
    while(m_size > 0)
    {
        *p = mem_val;
        p ++;
        m_size --;
    }
}

void mem_cpy(void* mem_src, void* mem_dst, uint32_t m_size)
{
    int8_t* p_src = (int8_t*)mem_src;
    int8_t* p_dst = (int8_t*)mem_dst;
    while(m_size --)
        *p_dst++ = *p_src++;
}

void sys_disp_config(SysDispState state)
{
    if(state == SYS_DISP_ENABLE)
        sys_disp_open();
    else if(state == SYS_DISP_DISABLE)
        sys_disp_close();
}

void sys_reset()
{
    SCB->AIRCR  = ((0x5FA << SCB_AIRCR_VECTKEY_Pos)     |
                  (SCB->AIRCR & SCB_AIRCR_PRIGROUP_Msk) |
                   SCB_AIRCR_SYSRESETREQ_Msk);
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
