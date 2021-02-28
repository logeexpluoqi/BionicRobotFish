/*
 * @Author: luoqi 
 * @Date: 2021-01-06 20:20:08 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-19 20:44:59
 */

#include "keyboard_task.h"
#include "keyboard.h"
#include "oled_task.h"

extern OledDispMsgTypedef oled_disp;

KeyTypedef key;

void key_scan()
{
    key = keyboard_scan();
    if(key.key_1 == KEY_H)
    {
        sys_disp_char(0, 7, 'H', SMALL);
    }
    else
    {
        sys_disp_char(0, 7, 'L', SMALL);
    }
    if(key.key_2 == KEY_H)
    {
        sys_disp_char(8, 7, 'H', SMALL);
    }
    else
    {
        sys_disp_char(8, 7, 'L', SMALL);
    }
}

void key_edge_action()
{
	static uint8_t flag = 0;

    /* rising edge action */
	if(key.key_1 == KEY_H && flag == 0)
	{
		flag = 1;
	}
    /* falling edge action */
	if(key.key_1 == KEY_NULL && flag == 1)
	{
		flag = 2;
	}
    /* rising edge action */
	if(key.key_1 == KEY_H && flag == 2)
	{
		flag = 3;
	}
    /* falling edge action */
	if(key.key_1 == KEY_NULL && flag == 3)
	{
		flag = 0;
	}
}
