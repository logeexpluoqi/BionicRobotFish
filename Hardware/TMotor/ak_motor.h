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
    unsigned char ctrl_en_flag;
    unsigned char id;
    float p_dst;
    float v_dst;
    float t_dst;
    float kp;
    float kd;
} AkMotorCtrl;

typedef struct
{
    float position;
    float velocity;
    float torque;
    float current;
} AkMotorInfo;

float p_limit(float p, AkMotorType m_type);
float v_limit(float v, AkMotorType m_type);
float t_limit(float t, AkMotorType m_type);
float kp_limit(float kp, AkMotorType m_type);
float kd_limit(float kd, AkMotorType m_type);

AkMotorType motor_type_detect(unsigned char id);
unsigned int float2uint(float x, float x_min, float x_max, unsigned char bits);
float unit2float(unsigned int x, float x_min, float x_max, unsigned char bits);

void clear_send_data(void);

void ak_motor_ctrl_init(void);
unsigned char ak_motor_mode_set(AkMotorCmd cmd);
unsigned char ak_motor_ctrl(AkMotorCtrl motor);
unsigned char ak_motor_info_receive(AkMotorInfo* motor_info);

#endif
