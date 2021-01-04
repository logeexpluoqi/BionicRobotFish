/*
 * @Author: luoqi 
 * @Date: 2021-01-04 18:21:16 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-04 19:22:23
 */
/*
 * @Author: luoqi 
 * @Date: 2021-01-04 13:55:53 
 * @Last Modified by:   luoqi 
 * @Last Modified time: 2021-01-04 13:55:53 
 */

#include "led.h"
#include "stm32f4xx_gpio.h"

void led_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOC, ENABLE); 
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    //GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOD, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    //GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure); 

    GPIO_SetBits(GPIOD, GPIO_Pin_3);
    GPIO_SetBits(GPIOC, GPIO_Pin_0);
}

void led_set(Led led_num, LedState state)
{
    if(state == LED_ON)
    {
        switch (led_num)
        {
            case LED_0:
            {
                GPIO_ResetBits(GPIOD, GPIO_Pin_3);
                break;
            }
            case LED_1:
            {
                GPIO_ResetBits(GPIOC, GPIO_Pin_0);
                break;
            }
            default:
                break;
        }

    }
    else
    {
        switch (led_num)
        {
            case LED_0:
            {
                GPIO_SetBits(GPIOD, GPIO_Pin_3);
                break;
            }
            case LED_1:
            {   
                GPIO_SetBits(GPIOC, GPIO_Pin_0);
                break;
            }
            default:
                break;
        }
    }
}

void led_sys_run()
{
    static unsigned char state = 0;
    
    state = !state;
    if(state)
    {
        led_set(LED_0, LED_ON);
    }
    else
    {
        led_set(LED_0, LED_OFF);
    }
    
}
