/*
 * @Author: luoqi 
 * @Date: 2021-01-14 08:29:43 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-19 20:41:04
 */

#include "msg_distribute.h"
#include "control_info.h"
#include "msg_codec.h"
#include "ak_motor.h"
#include "config.h"

extern AkMotorCtrlTypedef ak_motor_ctrl_data;

static unsigned char msg_cache[300];

void get_msg(unsigned char* msg, unsigned int msg_size)
{
    mem_cpy(msg, msg_cache, msg_size);
}

// void get_ak_motor_ctrl_data(AkMotorCtrlTypedef* ctrl_data)
// {
//     ctrl_data -> id     = msg_cache[1];
//     ctrl_data -> p_dst  = msg_char_to_float(msg_cache[2], msg_cache[3]);
//     ctrl_data -> v_dst  = msg_char_to_float(msg_cache[4], msg_cache[5]);
//     ctrl_data -> t_dst  = msg_char_to_float(msg_cache[6], msg_cache[7]);
//     ctrl_data -> kp     = msg_char_to_float(msg_cache[8], msg_cache[9]);
//     ctrl_data -> kd     = msg_char_to_float(msg_cache[10], msg_cache[11]);

// }

void msg_distribute()
{
#if AK_MOTOR_GROUP_CTRL
    
#else // AK_MOTOR_GROUP_CTRL
    switch (msg_cache[0])
    {
    case EN_MOTOR_MODE: ak_motor_mode_set(msg_cache[1], ENTER_MOTOR_CTRL); break;
    case EX_MOTOR_MODE: ak_motor_mode_set(msg_cache[1], QUIT_MOTOR_CTRL); break;
    case SET_MOTOR_ZERO: ak_motor_mode_set(msg_cache[1], SET_ZERO_POS); break;
    case CTRL_MOTOR:
    {
        ak_motor_ctrl_data.id    = msg_cache[1];
        ak_motor_ctrl_data.p_dst = msg_char_to_float(msg_cache[2], msg_cache[3]);
        ak_motor_ctrl_data.v_dst = msg_char_to_float(msg_cache[4], msg_cache[5]);
        ak_motor_ctrl_data.t_dst = msg_char_to_float(msg_cache[6], msg_cache[7]);
        ak_motor_ctrl_data.kp    = msg_char_to_float(msg_cache[8], msg_cache[9]);
        ak_motor_ctrl_data.kd    = msg_char_to_float(msg_cache[10], msg_cache[11]);

    #if CTRL_MODE_STROKE
		ak_motor_ctrl(ak_motor_ctrl_data);
	#endif
        break;
    }
    case 255: 
    {
        sys_reset();
    }

    default:
        break;
    }
#endif
}
