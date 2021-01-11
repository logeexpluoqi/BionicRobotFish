/*
 * @Author: luoqi 
 * @Date: 2021-01-06 19:23:23 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-06 20:37:29
 */

#include "keyboard.h"
#include "stm32f4xx_gpio.h"

void keyboard_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOF, ENABLE); 
 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}

KeyTypedef keyboard_scan()
{
	static unsigned char scan_filter = 0;
    KeyTypedef key;
	
    key.key_1 = KEY_NULL;
    key.key_2 = KEY_NULL;

    if(scan_filter == 1)
    {
        if(rkey_1 == 1)
        {
            key.key_1 = KEY_H;
        } 
        else 
        {
            key.key_1 = KEY_NULL;
        }

        if(rkey_2 == 1)
        {
            key.key_2 = KEY_H;
        }
        else
        {
            key.key_2 = KEY_NULL;
        }

        scan_filter = 0;
    }

    if(rkey_1 == 1 | rkey_2 == 1)
    {
        scan_filter = 1;
    }

    return key;
}
