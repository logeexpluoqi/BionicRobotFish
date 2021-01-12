/*
 * @Author: luoqi 
 * @Date: 2021-01-06 19:22:47 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-12 19:09:56
 */

#ifndef _KEYBOARD_H
#define _KEYBOARD_H

#define rkey_1 GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define rkey_2 GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_11)

typedef enum 
{
    KEY_L, // logic 0
    KEY_H, // logic 1
    KEY_NULL // no action
} KeyState;

typedef struct Key
{
    KeyState key_1;
    KeyState key_2;
} KeyTypedef;

void keyboard_init(void);
KeyTypedef keyboard_scan(void);

#endif
