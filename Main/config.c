/*
 * @Author: luoqi 
 * @Date: 2021-01-19 19:56:32 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-19 21:06:54
 */

#include "config.h"
#include "oled_task.h"

/* @breif: clear memery, after clear, the value is c 
 * @param: *mem, memery position, &s_structure or array;
 * @param: c, set the memery value, unsigned char type.
 */
void mem_set(void* mem, unsigned char c)
{
    unsigned char *p = mem;
    unsigned int m_size = sizeof(p);
    while(m_size > 0)
    {
        *p = c;
        p ++;
        m_size --;
    }
}

void sys_config_display()
{
    sys_disp_str(0, 0, "Config: ", LARGE);
    sys_disp_str(0, 3, "M_Num: ", SMALL);
    sys_disp_num(64, 3, AK_MOTOR_NUM, 1, SMALL);
    sys_disp_str(0, 4, "Grp_Ctrl: ", SMALL);
    #if AK_MOTOR_GROUP_CTRL
        sys_disp_char(64, 4, 'Y', SMALL);
    #else 
        sys_disp_char(64, 4, 'N', SMALL);
    #endif

    sys_disp_str(0, 5, "C_Once: ", SMALL);
    #if CTRL_MODE_ONCE
        sys_disp_char(64, 5, 'Y', SMALL);
    #else
        sys_disp_char(64, 5, 'N', SMALL);
    #endif
}
