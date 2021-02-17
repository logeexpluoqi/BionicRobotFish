/*
 * @Author: luoqi 
 * @Date: 2021-01-08 14:51:00 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-02-17 20:30:00
 */

#include "ak_motor_ctrl_task.h"
#include "msg_box.h"
#include "ak_motor.h"
#include "msg_codec.h"
#include "config.h"
#include "usart.h"

static AkMotorCtrlTypedef ak_motor_ctrl_data = {
    .ctrl_en = 0,
    .p_dst   = 0,
    .v_dst   = 0,
    .t_dst   = 0,
    .kp      = 0,
    .kd      = 0
};

static AkMotorInfo motor_info = {
    .id       = 0,
    .position = 0,
    .velocity = 0,
    .torque   = 0
};

void ak_motor_ctrl_task()
{
    AkMotorInfo motor_info;
    ak_motor_ctrl(&ak_motor_ctrl_data, &motor_info); 
}
