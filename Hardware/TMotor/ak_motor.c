/*
 * @Author: luoqi 
 * @Date: 2021-01-08 09:16:00 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-19 20:51:22
 */

#include "ak_motor.h"
#include "config.h"
#include "ak80_9.h"
#include "ak10_9.h"
#include "msg_codec.h"
#include "can.h"
#include "usart.h"

CanMsgTypedef can1_msg;
AkMotorCtrl ak_motor_ctrl_data;
AkMotorInfo ak_motor_info[20];

void ak_motor_ctrl_init()
{
    can1_msg.rtr = 0;
    can1_msg.ext_id = 0;
    can1_msg.ide = 0;
    can1_msg.dlc = 8; // a frame 8 byte data

    ak_motor_ctrl_data.ctrl_en = 0;
    ak_motor_ctrl_data.p_dst = 0;
    ak_motor_ctrl_data.v_dst = 0;
    ak_motor_ctrl_data.t_dst = 0;
    ak_motor_ctrl_data.kp = 0;
    ak_motor_ctrl_data.kd = 0;
}

unsigned char ak_motor_ctrl(AkMotorCtrl ctrl_data)
{
    AkMotorType motor_type;
    unsigned char err_cnt = 3;
    unsigned char err_state = 0;
    unsigned int p_dst, v_dst, t_dst;
    unsigned int kp, kd;
    float p_min, p_max;
    float v_min, v_max;
    float t_min, t_max;
    float kp_min, kp_max;
    float kd_min, kd_max;

    can1_msg.std_id = ctrl_data.id;
    motor_type = motor_type_detect(ctrl_data.id);

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

    ctrl_data.p_dst = p_limit(ctrl_data.p_dst, motor_type);
    ctrl_data.v_dst = v_limit(ctrl_data.v_dst, motor_type);
    ctrl_data.t_dst = t_limit(ctrl_data.t_dst, motor_type);
    ctrl_data.kp = kp_limit(ctrl_data.kp, motor_type);
    ctrl_data.kd = kp_limit(ctrl_data.kd, motor_type);

    p_dst = float2uint(ctrl_data.p_dst, p_min, p_max, 16);
    v_dst = float2uint(ctrl_data.v_dst, v_min, v_max, 12);
    t_dst = float2uint(ctrl_data.t_dst, t_min, t_max, 12);
    kp = float2uint(ctrl_data.kp, kp_min, kp_max, 12);
    kd = float2uint(ctrl_data.kd, kd_min, kd_max, 12);

    can1_msg.send_data[0] = (p_dst >> 8) & 0xff;
    can1_msg.send_data[1] = p_dst & 0xff;
    can1_msg.send_data[2] = (v_dst >> 4) & 0xff;
    can1_msg.send_data[3] = ((v_dst & 0x0f) << 4) | ((kp >> 8) & 0x0f);
    can1_msg.send_data[4] = kp & 0xff;
    can1_msg.send_data[5] = (kd >> 4) & 0xff;
    can1_msg.send_data[6] = ((kd & 0x0f) << 4) | ((t_dst >> 8) & 0x0f);
    can1_msg.send_data[7] = t_dst & 0xff;

    while(can_send_msg(can1_msg))
    {
        if(err_cnt == 0)
            err_state = 1;
        err_cnt --;
    }
    ak_motor_info_receive(ak_motor_info);
    return err_state;
}

unsigned char ak_motor_mode_set(AkMotorCmd cmd)
{
    unsigned char ret;

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

    ret = can_send_msg(can1_msg);

    return ret;
}

unsigned char ak_motor_info_receive(AkMotorInfo* motor_info)
{
    unsigned char chr[2];
    unsigned char msg_upload[9]; // float to char, up to usart transmmit
    unsigned char motor_type;
    unsigned char len;
    unsigned int position, velocity, torque;
    float p_min, p_max;
    float v_min, v_max;
    float t_min, t_max;

    len = can_receive_msg(can1_msg.receive_data);
    if(len == 0)
    {
        return 1;
    }
    motor_type = motor_type_detect(can1_msg.receive_data[0]);

    if(motor_type == AK10_9)
    {
        p_min = AK10_9_P_MIN; p_max = AK10_9_P_MAX;
        v_min = AK10_9_V_MIN; v_max = AK10_9_V_MAX;
        t_min = AK10_9_T_MIN; t_max = AK10_9_T_MAX;
    }
    else if(motor_type == AK80_9)
    {
        p_min = AK80_9_P_MIN; p_max = AK80_9_P_MAX;
        v_min = AK80_9_V_MIN; v_max = AK80_9_V_MAX;
        t_min = AK80_9_T_MIN; t_max = AK80_9_T_MAX;
    }

    if(len != 0)
    {
        position = (can1_msg.receive_data[1] << 8) | can1_msg.receive_data[2];
        velocity = (can1_msg.receive_data[3] << 4) | (can1_msg.receive_data[4] >> 4);
        torque = ((can1_msg.receive_data[4] & 0x0f) << 8) | can1_msg.receive_data[5];
    }
    ak_motor_info[can1_msg.receive_data[0]].position = unit2float(position, p_min, p_max, 16);
    ak_motor_info[can1_msg.receive_data[0]].velocity = unit2float(velocity, v_min, v_max, 12);
    ak_motor_info[can1_msg.receive_data[0]].torque = unit2float(torque, t_min, t_max, 12);
    msg_upload[1] = can1_msg.receive_data[0];
    msg_float_to_char(ak_motor_info[can1_msg.receive_data[0]].position, chr);
    msg_upload[2] = chr[1];
    msg_upload[3] = chr[0];
    msg_float_to_char(ak_motor_info[can1_msg.receive_data[0]].velocity, chr);
    msg_upload[4] = chr[1];
    msg_upload[5] = chr[0];
    msg_float_to_char(ak_motor_info[can1_msg.receive_data[0]].torque, chr);
    msg_upload[6] = chr[1];
    msg_upload[7] = chr[0];
    msg_upload[0] = '{'; // SOF
    msg_upload[8] = '}'; // EOF

    usart1_dma_tx_data(msg_upload, 9);

    return 0;
}

float unit2float(unsigned int x, float x_min, float x_max, unsigned char bits)
{
    float span = x_max - x_min;
    float offset = x_min;

    return ((float)x) * span / ((float)((1 << bits) - 1)) + offset;
}

unsigned int float2uint(float x, float x_min, float x_max, unsigned char bits)
{
    float span = x_max - x_min;
    float offset = x_min;

    return (unsigned int)((x - offset) * ((float)((1 << bits) - 1)) / span); 
}

/* @Notation:
 * If motor id changed, there also need to change !!!
 */
AkMotorType motor_type_detect(unsigned char id)
{
	AkMotorType type;
    if(id == 9 || id == 16 || id == 17 || id == 18)
        type = AK10_9;
    else if(id == 1 || id == 2 || id == 4 || id == 5)
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
