/*
 * @Author: luoqi 
 * @Date: 2021-01-14 08:29:43 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-14 08:40:58
 */

#include "msg_distribute.h"
#include "ak_motor.h"
#include "msg_codec.h"
#include "usart.h"
#include "usart.h"

extern AkMotorCtrl ak_motor_ctrl_data;

void msg_distribute(unsigned char *msg)
{
    ak_motor_ctrl_data.id = msg[1];
    switch (msg[0])
    {
    case NORMAL:
    {
        ak_motor_ctrl_data.p_dst = msg_char_to_float(msg[2], msg[3]);
        ak_motor_ctrl_data.v_dst = msg_char_to_float(msg[4], msg[5]);
        ak_motor_ctrl_data.t_dst = msg_char_to_float(msg[6], msg[7]);
        ak_motor_ctrl_data.kp    = msg_char_to_float(msg[8], msg[9]);
        ak_motor_ctrl_data.kd    = msg_char_to_float(msg[10], msg[11]);
        break;
    }
    case EN_MOTOR_MODE: ak_motor_mode_set(ENTER_MOTOR_CTRL); break;
    case EX_MOTOR_MODE: ak_motor_mode_set(QUIT_MOTOR_CTRL); break;
    case SET_MOTOR_ZERO: ak_motor_mode_set(SET_ZERO_POS); break;

    default:
        break;
    }
}
