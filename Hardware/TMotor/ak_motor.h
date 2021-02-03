/*
 * @Author: luoqi 
 * @Date: 2021-01-08 09:15:07 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-09 09:39:08
 */

#ifndef _AK_MOTOR_H
#define _AK_MOTOR_H

typedef enum
{
    AK10_9,
    AK80_9
} AkMotorType;

typedef enum
{
    ENTER_MOTOR_CTRL = 1,
    QUIT_MOTOR_CTRL = 2,
    SET_ZERO_POS = 3
} AkMotorCmd;

typedef struct
{
    unsigned char ctrl_en; // 1: enable, 0: disable
    unsigned char id;
    float p_dst;
    float v_dst;
    float t_dst;
    float kp;
    float kd;
} AkMotorCtrlTypedef;

typedef struct
{
    unsigned char id;
    float position;
    float velocity;
    float torque;
} AkMotorInfo;


unsigned char ak_motor_mode_set(unsigned char id, AkMotorCmd cmd);
unsigned char ak_motor_ctrl(AkMotorCtrlTypedef *ctrl_data);
unsigned char ak_motor_info_receive(AkMotorInfo *motor_info);

#endif
