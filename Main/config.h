/*
 * @Author: luoqi 
 * @Date: 2021-01-19 14:10:53 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-19 20:02:34
 */

#ifndef _CONFIG_H
#define _CONFIG_H

/* The quantity of motor we want to control 
*/
#define AK_MOTOR_NUM            8

/* 
 *
 */
#define AK_MOTOR_GROUP_CTRL     0

/* Motor control mode:
 * 0: control in time_slice process;
 * 1: control once when receive a control command from pc;
 */
#define CTRL_MODE_ONCE          0

/* @breif: This function is used to clear structures.
 * @param: *p_s, structure position;
 * @param: s_size, the structure size, use sizeof(s_name).
 */
void struct_clear(void* p_s, unsigned int s_size);

#endif
