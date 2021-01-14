/*
 * @Author: luoqi 
 * @Date: 2021-01-05 22:27:54 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-13 17:13:06
 */

#include "oled_task.h"
#include "stm32f4xx_exti.h"
#include "misc.h"
#include "oled.h"

OledDispMsgTypedef oled_disp;

void sys_disp_init()
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    oled_disp.refresh_flag = 0;

    OLED_Init();
    OLED_Clear();
    OLED_ShowString(30, 0, "WIAS_DSFT", FONT_LARGE);
    OLED_ShowString(72, 7, "by luoqi", FONT_SMALL);
    
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void sys_disp_char(unsigned char x, unsigned int y, unsigned char chr, OledDispFont font)
{
    oled_disp.refresh_flag = 1;
    oled_disp.type = TYPE_CHAR;
    oled_disp.pos_x = x;
    oled_disp.pos_y = y;
    oled_disp.disp_chr = chr;
    oled_disp.font = font;
    EXTI_GenerateSWInterrupt(EXTI_Line3);
}

void sys_disp_num(unsigned char x, unsigned char y, unsigned int num, unsigned char len, OledDispFont font)
{
    oled_disp.refresh_flag = 1;
    oled_disp.type = TYPE_NUM;
    oled_disp.pos_x = x;
    oled_disp.pos_y = y;
    oled_disp.disp_num = num;
    oled_disp.num_len = len;
    oled_disp.font = font;
    EXTI_GenerateSWInterrupt(EXTI_Line3);
}

void sys_disp_str(unsigned char x, unsigned char y, unsigned char *str, OledDispFont font)
{
    oled_disp.refresh_flag = 1;
    oled_disp.type = TYPE_STRING;
    oled_disp.pos_x = x;
    oled_disp.pos_y = y;
    oled_disp.disp_str = str;
    oled_disp.font = font;
    EXTI_GenerateSWInterrupt(EXTI_Line3);
}

void oled_sys_disp_task()
{ 
    if(oled_disp.refresh_flag == 1)
    {
        switch (oled_disp.type)
        {
        case TYPE_CHAR:
        {
            if (oled_disp.font == LARGE)
                OLED_ShowChar(oled_disp.pos_x, oled_disp.pos_y, oled_disp.disp_chr, FONT_LARGE); 
            else if (oled_disp.font == SMALL)
                OLED_ShowChar(oled_disp.pos_x, oled_disp.pos_y, oled_disp.disp_chr, FONT_SMALL); 
            break;
        }
        case TYPE_NUM:
        {
            if (oled_disp.font == LARGE)
                OLED_ShowNum(oled_disp.pos_x, oled_disp.pos_y, oled_disp.disp_num, oled_disp.num_len, FONT_LARGE); 
            else if (oled_disp.font == SMALL)
                OLED_ShowNum(oled_disp.pos_x, oled_disp.pos_y, oled_disp.disp_num, oled_disp.num_len, FONT_SMALL); 
            break;
        }
        case TYPE_STRING:
        {
            if (oled_disp.font == LARGE)
                OLED_ShowString(oled_disp.pos_x, oled_disp.pos_y, oled_disp.disp_str, FONT_LARGE);
            else
                OLED_ShowString(oled_disp.pos_x, oled_disp.pos_y, oled_disp.disp_str, FONT_SMALL);
            break;
        }
        default:
            break;
        }
        oled_disp.refresh_flag = 0;
    }
}

void EXTI3_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        oled_sys_disp_task();

        EXTI_ClearFlag(EXTI_Line3);
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}
