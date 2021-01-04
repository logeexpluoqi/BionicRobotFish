/*
 * @Author: luoqi 
 * @Date: 2021-01-04 13:55:04 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-04 18:48:21
 */

#ifndef _LED_H
#define _LED_H

typedef enum LED_STATE 
{
    LED_OFF = 0,
    LED_ON = 1
} LedState;

typedef enum LED
{
    LED_0 = 0,
    LED_1 = 1
} Led;

void led_init(void);
void led_set(Led led_num, LedState state);

#endif
