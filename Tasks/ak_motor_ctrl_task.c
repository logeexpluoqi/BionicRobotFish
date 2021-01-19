/*
 * @Author: luoqi 
 * @Date: 2021-01-08 14:51:00 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-08 16:35:19
 */

#include "ak_motor_ctrl_task.h"
#include "ak_motor.h"
#include "usart.h"

extern AkMotorCtrl ak_motor_ctrl_data;
extern AkMotorInfo ak_motor_info[20];

void ak_motor_ctrl_task()
{
    ak_motor_ctrl(ak_motor_ctrl_data);  
}
