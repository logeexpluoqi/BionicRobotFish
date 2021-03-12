/*
 * @Author: luoqi 
 * @Date: 2021-01-04 18:21:16 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-03-12 13:27:55
 */
/*
 * @Author: luoqi 
 * @Date: 2021-01-04 13:55:53 
 * @Last Modified by:   luoqi 
 * @Last Modified time: 2021-01-04 13:55:53 
 */

#include "led.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"

void led_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    //GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    //GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
    GPIO_Init(GPIOC, &GPIO_InitStructure); 

    GPIO_SetBits(GPIOA, GPIO_Pin_1);
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
                GPIO_ResetBits(GPIOA, GPIO_Pin_1);
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
                GPIO_SetBits(GPIOA, GPIO_Pin_1);
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

