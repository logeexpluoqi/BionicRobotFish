/*
 * @Author: luoqi 
 * @Date: 2021-01-08 09:16:00 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-08 14:31:00
 */

#include "ak_motor.h"
#include "ak80_9.h"
#include "ak10_9.h"
#include "can.h"
#include "oled.h"

CanMsgTypedef can1_msg;
AkMotorInfo motor;

void ak_motor_ctrl_init()
{
    can1_msg.rtr = 0;
    can1_msg.ext_id = 0;
    can1_msg.ide = 0;
    can1_msg.dlc = 8; // a frame 8 byte data

    OLED_ShowString(0, 2, "S:", FONT_LARGE);
    OLED_ShowString(0, 4, "R:", FONT_LARGE);
}

unsigned char ak_motor_ctrl(AkMotorInfo motor)
{
    AkMotorType motor_type;
    unsigned int p_dst, v_dst, t_dst;
    unsigned int kp, kd;
    float p_min, p_max;
    float v_min, v_max;
    float t_min, t_max;
    float kp_min, kp_max;
    float kd_min, kd_max;

    can1_msg.std_id = motor.id;
    motor_type = motor_type_detect(motor.id);

    if(motor_type == AK10_9)
    {
        p_min = AK10_9_P_MIN; p_max = AK10_9_P_MAX;
        v_min = AK10_9_V_MIN; v_max = AK10_9_V_MAX;
        t_min = AK10_9_T_MIN; t_max = AK10_9_T_MAX;
        kp_min = AK10_9_KP_MIN; kp_max = AK10_9_KP_MAX;
        kd_min = AK10_9_KD_MIN; kd_max = AK10_9_KD_MAX;
    }
    else if(motor_type == AK80_9)
    {
        p_min = AK80_9_P_MIN; p_max = AK80_9_P_MAX;
        v_min = AK80_9_V_MIN; v_max = AK80_9_V_MAX;
        t_min = AK80_9_T_MIN; t_max = AK80_9_T_MAX;
        kp_min = AK80_9_KP_MIN; kp_max = AK80_9_KP_MAX;
        kd_min = AK80_9_KD_MIN; kd_max = AK80_9_KD_MAX;
    }

    motor.p_dst = p_limit(motor.p_dst, motor_type);
    motor.v_dst = v_limit(motor.v_dst, motor_type);
    motor.t_dst = t_limit(motor.t_dst, motor_type);
    motor.kp = kp_limit(motor.kp, motor_type);
    motor.kd = kp_limit(motor.kd, motor_type);

    p_dst = float2uint(motor.p_dst, p_min, p_max, 16);
    v_dst = float2uint(motor.v_dst, v_min, v_max, 12);
    t_dst = float2uint(motor.t_dst, t_min, t_max, 12);
    kp = float2uint(motor.kp, kp_min, kp_max, 12);
    kd = float2uint(motor.kd, kd_min, kd_max, 12);

    can1_msg.send_data[0] = p_dst >> 8;
    can1_msg.send_data[1] = p_dst & 0xff;
    can1_msg.send_data[2] = v_dst >> 4;
    can1_msg.send_data[3] = ((v_dst & 0x0f) << 4) | (kp >> 8);
    can1_msg.send_data[4] = kp & 0xff;
    can1_msg.send_data[5] = kd >> 4;
    can1_msg.send_data[6] = ((kd & 0x0f) << 4) | (t_dst >> 8);
    can1_msg.send_data[7] = t_dst & 0xff;

    if(can_send_msg(can1_msg)) 
        return 1;
    else
        return 0;
}

unsigned char motor_mode_set(AkMotorCmd cmd)
{
    switch(cmd)
    {
    case ENTER_MOTOR_CTRL:
    {
        can1_msg.send_data[0] = 0xff;
        can1_msg.send_data[1] = 0xff;
        can1_msg.send_data[2] = 0xff;
        can1_msg.send_data[3] = 0xff;
        can1_msg.send_data[4] = 0xff;
        can1_msg.send_data[5] = 0xff;
        can1_msg.send_data[6] = 0xff;
        can1_msg.send_data[7] = 0xfc;
        break;
    }
    case QUIT_MOTOR_CTRL:
    {
        can1_msg.send_data[0] = 0xff;
        can1_msg.send_data[1] = 0xff;
        can1_msg.send_data[2] = 0xff;
        can1_msg.send_data[3] = 0xff;
        can1_msg.send_data[4] = 0xff;
        can1_msg.send_data[5] = 0xff;
        can1_msg.send_data[6] = 0xff;
        can1_msg.send_data[7] = 0xfd;
        break;
    }
    case SET_ZERO_POS:
    {
        can1_msg.send_data[0] = 0xff;
        can1_msg.send_data[1] = 0xff;
        can1_msg.send_data[2] = 0xff;
        can1_msg.send_data[3] = 0xff;
        can1_msg.send_data[4] = 0xff;
        can1_msg.send_data[5] = 0xff;
        can1_msg.send_data[6] = 0xff;
        can1_msg.send_data[7] = 0xfe;
        break;
    }
    default:
        break;
    }

    return (can_send_msg(can1_msg));
}

unsigned int float2uint(float x, float x_min, float x_max, unsigned char bits)
{
    float span = x_max - x_min;
    float offset = x_min;

    return (unsigned int)((x - offset) * ((float)((1 << bits) - 1)) / span); 
}

AkMotorType motor_type_detect(unsigned char id)
{
	AkMotorType type;
    if(id == 1)
        type = AK10_9;
    else if(id == 2)
        type = AK80_9;
	
	return type;
}

float p_limit(float p, AkMotorType m_type)
{
    float ret_data;

    if(m_type == AK10_9)
    {
        if(p < AK10_9_P_MIN)        
            ret_data = AK10_9_P_MIN;
        else if(p > AK10_9_P_MAX)   
            ret_data = AK10_9_P_MAX;
        else                        
            ret_data = p ;
    }
    else if(m_type == AK80_9)
    {
        if(p < AK80_9_P_MIN) 
            ret_data = AK80_9_P_MIN;
        else if(p > AK80_9_P_MAX) 
            ret_data = AK80_9_P_MAX;
        else
            ret_data = p;
    }

    return ret_data;
}

float v_limit(float v, AkMotorType m_type)
{
    float ret_data;
    
    if(m_type == AK10_9)
    {
        if(v < AK10_9_V_MIN) 
            ret_data = AK10_9_V_MIN;
        else if(v > AK10_9_V_MAX) 
            ret_data = AK10_9_V_MAX;
        else
            ret_data = v;
    }
    else if(m_type == AK80_9)
    {
        if(v < AK80_9_V_MIN) 
            ret_data = AK80_9_V_MIN;
        else if(v > AK80_9_V_MAX) 
            ret_data = AK80_9_V_MAX;
        else
            ret_data = v ;
    }

    return ret_data;
}

float t_limit(float t, AkMotorType m_type)
{
    float ret_data;

    if(m_type == AK10_9)
    {
        if(t < AK10_9_T_MIN) 
            ret_data = AK10_9_T_MIN;
        else if(t > AK10_9_T_MAX) 
            ret_data = AK10_9_T_MAX;
        else
            ret_data = t;
    }
    else if(m_type == AK80_9)
    {
        if(t < AK80_9_T_MIN) 
            ret_data = AK80_9_T_MIN;
        else if(t > AK80_9_T_MAX) 
            ret_data = AK80_9_T_MAX;
        else
            ret_data = t ;
    }

    return ret_data;
}

float kp_limit(float kp, AkMotorType m_type)
{
    float ret_data;

    if(m_type == AK10_9)
    {
        if(kp < AK10_9_KP_MIN) 
            ret_data = AK10_9_KP_MIN;
        else if(kp > AK10_9_KP_MAX) 
            ret_data = AK10_9_KP_MAX;
        else
            ret_data = kp;
    }
    else if(m_type == AK80_9)
    {
        if(kp < AK80_9_KP_MIN) 
            ret_data = AK80_9_KP_MIN;
        else if(kp > AK80_9_KP_MAX) 
            ret_data = AK80_9_KP_MAX;
        else
            ret_data = kp ;
    }

    return ret_data;
}

float kd_limit(float kd, AkMotorType m_type)
{
    float ret_data;

    if(m_type == AK10_9)
    {
        if(kd < AK10_9_KD_MIN) 
            ret_data = AK10_9_KD_MIN;
        else if(kd > AK10_9_KD_MAX) 
            ret_data = AK10_9_KD_MAX;
        else
            ret_data = kd;
    }
    else if(m_type == AK80_9)
    {
        if(kd < AK80_9_KD_MIN) 
            ret_data = AK80_9_KD_MIN;
        else if(kd > AK80_9_KD_MAX) 
            ret_data = AK80_9_KD_MAX;
        else
            ret_data = kd ;
    }

    return ret_data;
}
