/*
 * @Author: luoqi 
 * @Date: 2021-01-06 20:20:08 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-07 14:28:06
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
        sys_disp_char(5, 7, 'H', SMALL);
    }
    else
    {
        sys_disp_char(5, 7, 'L', SMALL);
    }
    if(key.key_2 == KEY_H)
    {
        sys_disp_char(15, 7, 'H', SMALL);
    }
    else
    {
        sys_disp_char(15, 7, 'L', SMALL);
    }
}
