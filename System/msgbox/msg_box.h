/*
 * @Author: luoqi 
 * @Date: 2021-02-17 20:27:52 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-02-17 20:45:42
 */

#ifndef _MSG_BOX_H
#define _MSG_BOX_H
#include "config.h"
typedef struct msg_box_akmotor_t
{
    /* motor control mode */
    uint8_t mode;

    /* receive motor control data */
    uint8_t id_dst;
    float p_dst;
    float v_dst;
    float t_dst;
    float kp;
    float kd;

    /* upload motor control data */
    uint8_t id_upload;
    float p_upload;
    float v_upload;
    float t_upload;
} MsgBoxAkmotor;

#endif
