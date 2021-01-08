/*
 * @Author: luoqi 
 * @Date: 2021-01-08 09:15:07 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-08 14:25:45
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
    ENTER_MOTOR_CTRL,
    QUIT_MOTOR_CTRL,
    SET_ZERO_POS
} AkMotorCmd;

typedef struct
{
    unsigned char id;
    float p_dst;
    float v_dst;
    float t_dst;
    float kp;
    float kd;
} AkMotorInfo;

float p_limit(float p, AkMotorType m_type);
float v_limit(float v, AkMotorType m_type);
float t_limit(float t, AkMotorType m_type);
float kp_limit(float kp, AkMotorType m_type);
float kd_limit(float kd, AkMotorType m_type);

AkMotorType motor_type_detect(unsigned char id);
unsigned int float2uint(float x, float x_min, float x_max, unsigned char bits);

void ak_motor_ctrl_init(void);
unsigned char motor_mode_set(AkMotorCmd cmd);
unsigned char ak_motor_ctrl(AkMotorInfo motor);

#endif
