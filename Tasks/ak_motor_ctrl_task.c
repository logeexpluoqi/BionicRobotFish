/*
 * @Author: luoqi 
 * @Date: 2021-01-08 14:51:00 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-08 16:35:19
 */

#include "ak_motor_ctrl_task.h"
#include "ak_motor.h"

extern AkMotorCtrl ak_motor_ctrl_data;

void ak_motor_ctrl_task()
{
    ak_motor_ctrl(ak_motor_ctrl_data);  
}

void ak_motor_receive_data_task()
{
    
}
