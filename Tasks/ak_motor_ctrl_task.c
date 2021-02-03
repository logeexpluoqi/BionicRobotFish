/*
 * @Author: luoqi 
 * @Date: 2021-01-08 14:51:00 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-19 21:09:21
 */

#include "ak_motor_ctrl_task.h"
#include "config.h"
#include "ak_motor.h"

AkMotorCtrlTypedef ak_motor_ctrl_data = {
    .ctrl_en = 0,
    .p_dst   = 0,
    .v_dst   = 0,
    .t_dst   = 0,
    .kp      = 0,
    .kd      = 0
};

void ak_motor_ctrl_task()
{
    ak_motor_ctrl(&ak_motor_ctrl_data); 
}

// void ak_motor_stroke_ctrl(AkMotorCtrlTypedef *ctrl_data, )
// {

// }
