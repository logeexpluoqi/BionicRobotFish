/*
 * @Author: luoqi 
 * @Date: 2021-01-04 08:45:16 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-12 17:32:30
 */
#include "init.h"
#include "time_slice.h"

#include "ak_motor.h"
#include "keyboard.h"

extern AkMotorCtrl ak_motor_ctrl_data;
extern KeyTypedef key;

void motor_ctrl_test()
{
	static unsigned char flag = 0;

	ak_motor_ctrl_data.id = 4;
	if(key.key1_edge == KEY_RISE_EDGE && flag == 0)
	{
		ak_motor_mode_set(ENTER_MOTOR_CTRL);
		flag = 1;
	}
	if(key.key1_edge == KEY_RISE_EDGE && flag == 1)
	{
		ak_motor_mode_set(QUIT_MOTOR_CTRL);
		flag = 0;
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
 
