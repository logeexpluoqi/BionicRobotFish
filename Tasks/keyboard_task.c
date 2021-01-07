/*
 * @Author: luoqi 
 * @Date: 2021-01-06 20:20:08 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-07 14:28:06
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
        OLED_ShowChar(5,7,'H',FONT_SMALL);
    }
    else
    {
        OLED_ShowChar(5,7,'L',FONT_SMALL);
    }
    if(key.key_1 == KEY_H)
    {
        OLED_ShowChar(15,7,'H',FONT_SMALL);
    }
    else
    {
        OLED_ShowChar(15,7,'L',FONT_SMALL);
    }
}
