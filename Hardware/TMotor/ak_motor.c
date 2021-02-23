/*
 * @Author: luoqi 
 * @Date: 2021-01-08 09:16:00 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-02-23 14:17:19
 */

#include "ak_motor.h"
#include "ak80_9.h"
#include "ak10_9.h"
#include "can.h"
#include "usart.h"

static CanMsgTypedef can1_msg = {
    .rtr             = 0,
    .ext_id          = 0,
    .ide             = 0,
    .dlc             = 8
};

/* Built-in functions */
static uint8_t ak_motor_info_receive(AkMotorInfo *motor_info);
static void ak_motor_data_encode(uint8_t* motor_data, uint32_t P, uint32_t V, uint32_t T, uint32_t Kp, uint32_t Kd);
static float p_limit(float p, AkMotorType m_type);
static float v_limit(float v, AkMotorType m_type);
static float t_limit(float t, AkMotorType m_type);
static float kp_limit(float kp, AkMotorType m_type);
static float kd_limit(float kd, AkMotorType m_type);
static AkMotorType motor_type_detect(uint8_t id);
static uint32_t float2uint(float x, float x_min, float x_max, uint8_t bits);
static float unit2float(uint32_t x, float x_min, float x_max, uint8_t bits);

/**
 *  Notation: if motor id changed, there also need to change !!!
 */
AkMotorType motor_type_detect(uint8_t id)
{
	AkMotorType type;
    if(id == 9 || id == 16 || id == 17 || id == 18)
        type = AK10_9;
    else if(id == 1 || id == 2 || id == 3 || id == 4 || id == 5)
        type = AK80_9;
	
	return type;
}


uint8_t ak_motor_ctrl(AkMotorCtrlTypedef *ctrl_data, AkMotorInfo *motor_info)
{
    AkMotorType motor_type;
    uint8_t state = 0;
    uint8_t err_cnt = 0;
    uint32_t p_dst, v_dst, t_dst;
    uint32_t kp, kd;
    float p_min, p_max;
    float v_min, v_max;
    float t_min, t_max;
    float kp_min, kp_max;
    float kd_min, kd_max;

    sys_disp_config(SYS_DISP_DISABLE);
    can1_msg.std_id = ctrl_data -> id;
    motor_type = motor_type_detect(ctrl_data -> id);

    if(motor_type == AK10_9)
    {
        p_min  = AK10_9_P_MIN;  p_max  = AK10_9_P_MAX;
        v_min  = AK10_9_V_MIN;  v_max  = AK10_9_V_MAX;
        t_min  = AK10_9_T_MIN;  t_max  = AK10_9_T_MAX;
        kp_min = AK10_9_KP_MIN; kp_max = AK10_9_KP_MAX;
        kd_min = AK10_9_KD_MIN; kd_max = AK10_9_KD_MAX;
    }
    else if(motor_type == AK80_9)
    {
        p_min  = AK80_9_P_MIN;  p_max  = AK80_9_P_MAX;
        v_min  = AK80_9_V_MIN;  v_max  = AK80_9_V_MAX;
        t_min  = AK80_9_T_MIN;  t_max  = AK80_9_T_MAX;
        kp_min = AK80_9_KP_MIN; kp_max = AK80_9_KP_MAX;
        kd_min = AK80_9_KD_MIN; kd_max = AK80_9_KD_MAX;
    }

    ctrl_data -> p_dst = p_limit(ctrl_data -> p_dst, motor_type);
    ctrl_data -> v_dst = v_limit(ctrl_data -> v_dst, motor_type);
    ctrl_data -> t_dst = t_limit(ctrl_data -> t_dst, motor_type);
    ctrl_data -> kp    = kp_limit(ctrl_data -> kp, motor_type);
    ctrl_data -> kd    = kd_limit(ctrl_data -> kd, motor_type);

    p_dst = float2uint(ctrl_data -> p_dst, p_min, p_max, 16);
    v_dst = float2uint(ctrl_data -> v_dst, v_min, v_max, 12);
    t_dst = float2uint(ctrl_data -> t_dst, t_min, t_max, 12);
    kp    = float2uint(ctrl_data -> kp, kp_min, kp_max, 12);
    kd    = float2uint(ctrl_data -> kd, kd_min, kd_max, 12);
    ak_motor_data_encode(can1_msg.send_data, p_dst, v_dst, t_dst, kp, kd);

    while(can_send_msg(can1_msg))
    {
        err_cnt ++;
        if(err_cnt == 3)
        {
            state = 1;
            break;
        }
        else 
            state = 0;
    }

    ak_motor_info_receive(motor_info); 
    sys_disp_config(SYS_DISP_ENABLE); 
    return state;
}

uint8_t ak_motor_info_receive(AkMotorInfo *motor_info)
{
    uint8_t motor_type;
    uint8_t len;
    uint32_t position, velocity, torque;
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
        torque   = ((can1_msg.receive_data[4] & 0x0f) << 8) | can1_msg.receive_data[5];
    }
    motor_info -> id       = can1_msg.receive_data[0];
    motor_info -> position = unit2float(position, p_min, p_max, 16);
    motor_info -> velocity = unit2float(velocity, v_min, v_max, 12);
    motor_info -> torque   = unit2float(torque, t_min, t_max, 12);

    return 0;
}

void ak_motor_data_encode(uint8_t* motor_data, uint32_t P, uint32_t V, uint32_t T, uint32_t Kp, uint32_t Kd)
{
    motor_data[0] = (P >> 8) & 0xff;
    motor_data[1] = P & 0xff;
    motor_data[2] = (V >> 4) & 0xff;
    motor_data[3] = ((V & 0x0f) << 4) | ((Kp >> 8) & 0x0f);
    motor_data[4] = Kp & 0xff;
    motor_data[5] = (Kd >> 4) & 0xff;
    motor_data[6] = ((Kd & 0x0f) << 4) | ((T >> 8) & 0x0f);
    motor_data[7] = T & 0xff;
}

uint8_t ak_motor_mode_set(uint8_t id, AkMotorCmd cmd)
{
    uint8_t ret;
    uint8_t start[] = "{MOTOR UNLOCK}";
    uint8_t exit[]  = "{MOTOR LOCKED}";
    uint8_t zero[]  = "{SET ZERO    }";
    uint8_t error[] = "{CAN SEND ERR}";
    uint8_t *upload;

    can1_msg.std_id = id;

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
        upload = start;
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
        upload = exit;
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
        upload = zero;
        break;
    }
    default:
        break;
    }

    ret = can_send_msg(can1_msg);
    if(ret == 1)
    {
        upload = error;
        usart1_dma_tx_data(upload, 14);
    }
    else
        usart1_dma_tx_data(upload, 14);

    return ret;
}

float unit2float(uint32_t x, float x_min, float x_max, uint8_t bits)
{
    float span = x_max - x_min;
    float offset = x_min;

    return ((float)x) * span / ((float)((1 << bits) - 1)) + offset;
}

uint32_t float2uint(float x, float x_min, float x_max, uint8_t bits)
{
    float span = x_max - x_min;
    float offset = x_min;

    return (uint32_t)((x - offset) * ((float)((1 << bits) - 1)) / span); 
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
