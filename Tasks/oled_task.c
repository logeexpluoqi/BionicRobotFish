/*
 * @Author: luoqi 
 * @Date: 2021-01-05 22:27:54 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-07 17:48:15
 */

#include "oled_task.h"
#include "oled.h"

void oled_disp_test()
{
    static unsigned int i;

    i = i + 1;
    OLED_ShowNum(28, 7, i, 5, FONT_SMALL);
}
