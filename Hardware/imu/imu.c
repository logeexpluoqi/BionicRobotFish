/*
 * @Author: luoqi 
 * @Date: 2021-03-12 13:22:42 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-03-12 13:41:04
 */

#include "imu.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"


static void imu_pin_init(void);

void imu_init()
{
    imu_pin_init();
    imu_stop();
}

void imu_pin_init()
{
    
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;      
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
}

void imu_start()
{
    GPIO_SetBits(GPIOB, GPIO_Pin_15);
}

void imu_stop()
{
    GPIO_ResetBits(GPIOB, GPIO_Pin_15);
}
