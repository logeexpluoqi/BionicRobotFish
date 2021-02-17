/*
 * @Author: luoqi 
 * @Date: 2021-01-19 19:56:32 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-02-17 22:42:33
 */

#include "config.h"
#include "oled_task.h"

#define NULL (void*) 0

/* @breif: clear memery, after clear, the value is mem_val 
 * @param: *mem, memery position, &s_structure or array;
 * @param: mem_val, set the memery value, unsigned char type.
 */
void mem_set(void* mem, unsigned char mem_val)
{
    unsigned char *p = mem;
    unsigned int m_size = sizeof(p);
    while(m_size > 0)
    {
        *p = mem_val;
        p ++;
        m_size --;
    }
}

void mem_cpy(void* mem_src, void* mem_dst, unsigned int m_size)
{
    char* p_src = (char*)mem_src;
    char* p_dst = (char*)mem_dst;
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
    
}

void sys_config_display()
{
    sys_disp_config(SYS_DISP_ENABLE);
    sys_disp_str(0, 0, "SysInfo: ", SMALL);
    sys_disp_str(0, 2, "M_Num: ", SMALL);
    sys_disp_num(64, 2, AK_MOTOR_NUM_MAX, 1, SMALL);
    
    sys_disp_str(0, 3, "GrpCtrl: ", SMALL);
}
