/*
 * @Author: luoqi 
 * @Date: 2021-01-08 14:51:00 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-19 21:09:21
 */

#include "ak_motor_ctrl_task.h"
#include "config.h"
#include "ak_motor.h"
#include "usart.h"

extern AkMotorCtrlTypedef ak_motor_ctrl_data;

void ak_motor_ctrl_task()
{
    ak_motor_ctrl(ak_motor_ctrl_data); 
}
