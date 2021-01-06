/*
 * @Author: luoqi 
 * @Date: 2021-01-06 20:20:08 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-06 20:59:49
 */

#include "keyboard_task.h"
#include "keyboard.h"
#include "oled.h"

KeyTypedef key;

void key_scan()
{
    key = keyboard_scan();
    if(key.key_2 == KEY_H)
    {
        OLED_ShowChar(0,3,'Y',FONT_LARGE);
    }
    else
    {
        OLED_ShowChar(0,3,'N',FONT_LARGE);
    }
    if(key.key_1 == KEY_H)
    {
        OLED_ShowChar(10,3,'Y',FONT_LARGE);
    }
    else
    {
        OLED_ShowChar(10,3,'N',FONT_LARGE);
    }
}
