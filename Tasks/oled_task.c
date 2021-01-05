/*
 * @Author: luoqi 
 * @Date: 2021-01-05 22:27:54 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-05 22:35:29
 */

#include "oled_task.h"
#include "oled.h"

void oled_disp_test()
{
    static unsigned int i;

    i = i + 1;
    OLED_ShowString(0,3,"Task test: ",16);
    OLED_ShowNum(80,3,i,5,16);
}
