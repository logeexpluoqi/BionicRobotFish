/*
 * @Author: luoqi 
 * @Date: 2021-01-19 14:10:53 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-19 21:07:44
 */

#ifndef _CONFIG_H
#define _CONFIG_H

typedef enum sys_disp 
{
    SYS_DISP_ENABLE,
    SYS_DISP_DISABLE
} SysDispState;

/* The maximal quantity of motor we can control
*/
#define AK_MOTOR_NUM_MAX        8

/** 
 * Motor control mode
 * 1: group control mode
 * 0: stroke control mode
 */
#define AK_MOTOR_CTRL_MODE      0


/* Using oled system display function 
 * 1: enable
 * 0: disnable
 */
#define USING_SYS_DISP          0

/* @breif: This function is used to clear structures.
 * @param: *mem, structure position;
 * @param: c, fill number.
 */
void mem_set(void* mem, unsigned char mem_val);
void mem_cpy(void* mem_src, void* mem_dst, unsigned int m_size);
void sys_reset(void);
void sys_config_display(void);
void sys_disp_config(SysDispState state);

#endif
