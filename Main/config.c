/*
 * @Author: luoqi 
 * @Date: 2021-01-19 19:56:32 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-19 21:06:54
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


void sys_reset()
{
    
}

void sys_config_display()
{
    sys_disp_str(0, 0, "SysInfo: ", SMALL);
    sys_disp_str(0, 2, "M_Num: ", SMALL);
    sys_disp_num(64, 2, AK_MOTOR_NUM, 1, SMALL);
    
    sys_disp_str(0, 3, "GrpCtrl: ", SMALL);
    #if AK_MOTOR_GROUP_CTRL
        sys_disp_char(64, 3, 'Y', SMALL);
    #else 
        sys_disp_char(64, 3, 'N', SMALL);
    #endif

    sys_disp_str(0, 4, "StrkCtrl: ", SMALL); // strock control
    #if CTRL_MODE_STROKE
        sys_disp_char(64, 4, 'Y', SMALL);
    #else
        sys_disp_char(64, 4, 'N', SMALL);
    #endif

    sys_disp_str(0, 5, "Upload: ", SMALL);
    #if CONTINUOUS_UPLOAD
        sys_disp_char(64, 5, 'C', SMALL);
    #else
        sys_disp_char(64, 5, 'S', SMALL);
    #endif
}
