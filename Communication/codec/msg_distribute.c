/*
 * @Author: luoqi 
 * @Date: 2021-01-14 08:29:43 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-19 20:41:04
 */

#include "msg_distribute.h"
#include "ak_motor.h"
#include "control_info.h"
#include "config.h"
#include "msg_codec.h"
#include "usart.h"

extern AkMotorCtrlTypedef ak_motor_ctrl_data;
extern AkMotorCtrlInfoTypedef ak_motor_ctrl_cache[AK_MOTOR_NUM];

void msg_distribute(unsigned char *msg)
{
#if ! AK_MOTOR_GROUP_CTRL
    switch (msg[0])
    {
    case EN_MOTOR_MODE: ak_motor_mode_set(msg[1], ENTER_MOTOR_CTRL); break;
    case EX_MOTOR_MODE: ak_motor_mode_set(msg[1], QUIT_MOTOR_CTRL); break;
    case SET_MOTOR_ZERO: ak_motor_mode_set(msg[1], SET_ZERO_POS); break;
    case CTRL_MOTOR:
    {
        ak_motor_ctrl_data.id = msg[1];
        ak_motor_ctrl_data.p_dst = msg_char_to_float(msg[2], msg[3]);
        ak_motor_ctrl_data.v_dst = msg_char_to_float(msg[4], msg[5]);
        ak_motor_ctrl_data.t_dst = msg_char_to_float(msg[6], msg[7]);
        ak_motor_ctrl_data.kp    = msg_char_to_float(msg[8], msg[9]);
        ak_motor_ctrl_data.kd    = msg_char_to_float(msg[10], msg[11]);

    #if CTRL_MODE_STROKE
		ak_motor_ctrl(ak_motor_ctrl_data);
	#endif
        break;
    }
    case 255: 
    {
    }

    default:
        break;
    }
#else // AK_MOTOR_GROUP_CCTRL


#endif
}
