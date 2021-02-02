/*
 * @Author: luoqi 
 * @Date: 2021-01-09 16:04:41 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-09 16:05:20
 */

#ifndef _CONTRL_INFO_H
#define _CONTRL_INFO_H

typedef struct ak_motor_ctrl_cache
{
    /* receive motor control data */
    unsigned char id_dst;
    float p_dst;
    float v_dst;
    float t_dst;
    float kp;
    float kd;

    /* upload motor control data */
    unsigned char id_upload;
    float p_upload;
    float v_upload;
    float t_upload;
} AkMotorCtrlInfoTypedef;


#endif
