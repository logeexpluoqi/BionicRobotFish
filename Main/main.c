/*
 * @Author: luoqi 
 * @Date: 2021-01-04 08:45:16 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-08 16:40:34
 */
#include "init.h"
#include "time_slice.h"

#include "ak_motor.h"
#include "keyboard.h"

extern AkMotorCtrl ak_motor_ctrl_data;
extern KeyTypedef key;

void motor_ctrl_test()
{
	ak_motor_ctrl_data.id = 4;
	if(key.key_1 == KEY_H)
	{
		ak_motor_mode_set(ENTER_MOTOR_CTRL);
	}
	if(key.key_2 == KEY_H)
	{
		ak_motor_mode_set(QUIT_MOTOR_CTRL);
	}
}

int main(void)
{ 		
	sys_init();
	
	while(1)
	{	
		motor_ctrl_test();
		time_slice_process();
	}
}
 
