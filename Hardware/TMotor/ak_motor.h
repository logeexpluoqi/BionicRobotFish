/*
 * @Author: luoqi 
 * @Date: 2021-01-08 09:15:07 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-02-23 18:52:39
 */

#ifndef _AK_MOTOR_H
#define _AK_MOTOR_H
#include "config.h"

typedef enum
{
    AK10_9,
    AK80_9
} AkMotorType;

typedef enum
{
    EN_AK_MOTOR_MODE   = 1,
    EX_AK_MOTOR_MODE   = 2,
    SET_AK_MOTOR_ZERO  = 3
} AkMotorCmd;

typedef struct
{
    uint8_t id;
    float p_dst;
    float v_dst;
    float t_dst;
    float kp;
    float kd;
} AkMotorCtrlTypedef;

typedef struct
{
    uint8_t id;
    float position;
    float velocity;
    float torque;
} AkMotorInfo;


uint8_t ak_motor_mode_set(uint8_t id, AkMotorCmd cmd);
uint8_t ak_motor_ctrl(AkMotorCtrlTypedef *ctrl_data, AkMotorInfo *motor_info);

#endif
