/*
 * @Author: luoqi 
 * @Date: 2021-01-19 14:10:53 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-19 21:07:44
 */

#ifndef _CONFIG_H
#define _CONFIG_H

/* The maximal quantity of motor we can control
*/
#define AK_MOTOR_NUM_MAX        8

/** 
 * Motor control mode
 * 1: group control mode
 * 0: stroke control mode
 */
#define AK_MOTOR_CTRL_MODE     0

/* Motor control mode:
 * 0: control in time_slice process;
 * 1: control once when receive a control command from pc;
 */
#define CTRL_MODE_STROKE        1

/* Data upload mode.
 * 1: continuous mode, upload data every control period;
 * 0: stroke mode, upload data while receive a control command.
 */
#define CONTINUOUS_UPLOAD       0

/* @breif: This function is used to clear structures.
 * @param: *mem, structure position;
 * @param: c, fill number.
 */
void mem_set(void* mem, unsigned char mem_val);
void mem_cpy(void* mem_src, void* mem_dst, unsigned int m_size);
void sys_reset(void);
void sys_config_display(void);

#endif
