/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:07:39 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-04 10:11:52
 */
/* 
 * @brief: This file is created by 正点原子, modified by luoqi
 */
#ifndef _OLED_H
#define _OLED_H

#include "delay.h"
#include "oled.h"
#include "stdlib.h"
#include "sys.h"

#define OLED_MODE 0
#define SIZE 8
#define XLevelL 0x00
#define XLevelH 0x10
#define Max_Column 128
#define Max_Row 64
#define Brightness 0xFF
#define X_WIDTH 128
#define Y_WIDTH 64

#define OLED_CMD 1  // write command
#define OLED_DATA 0 // write data

typedef enum CHAR_SIZE 
{
    FONT_LARGE = 16,
    FONT_SMALL = 12
} FontSize;

//OLED control function
void OLED_WR_Byte(unsigned dat, unsigned cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(u8 x, u8 y, u8 chr, FontSize f_size);
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, FontSize f_size);
void OLED_ShowString(u8 x, u8 y, u8 *p, FontSize f_size);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x, u8 y, u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
void fill_picture(unsigned char fill_Data);

#endif
