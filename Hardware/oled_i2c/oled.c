/*
 * @Author: luoqi 
 * @Date: 2021-01-04 08:49:52 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-04 09:37:15
 */
/* 
 * @brief: This file is created by 正点原子, modified by luoqi
 */
#include "oled.h"
#include "myiic.h"
#include "oledfont.h"
#include "stdlib.h"
/* //OLED's display RAM, the data format:
* [0]0 1 2 3 ... 127
*  ....
*  ....
* [7]0 1 2 3 ... 127 */

/* I2C write command */
void Write_IIC_Command(unsigned char IIC_Command)
{
	IIC_Start();
	IIC_Send_Byte(0x78); //Slave address,SA0=0
	IIC_Wait_Ack();
	IIC_Send_Byte(0x00); //write command
	IIC_Wait_Ack();
	IIC_Send_Byte(IIC_Command);
	IIC_Wait_Ack();
	IIC_Stop();
}
/* I2C write data */
void Write_IIC_Data(unsigned char IIC_Data)
{
	IIC_Start();
	IIC_Send_Byte(0x78); //D/C#=0; R/W#=0
	IIC_Wait_Ack();
	IIC_Send_Byte(0x40); //write data
	IIC_Wait_Ack();
	IIC_Send_Byte(IIC_Data);
	IIC_Wait_Ack();
	IIC_Stop();
}

void OLED_WR_Byte(unsigned dat, unsigned cmd)
{
	if (cmd)
	{
		Write_IIC_Command(dat);
	}
	else
	{
		Write_IIC_Data(dat);
	}
}

/*  Fill_Picture */
void fill_picture(unsigned char fill_Data)
{
	unsigned char m, n;
	for (m = 0; m < 8; m++)
	{
		OLED_WR_Byte(0xb0 + m, 0); //page0-page1
		OLED_WR_Byte(0x00, 0);	   //low column start address
		OLED_WR_Byte(0x10, 0);	   //high column start address
		for (n = 0; n < 128; n++)
		{
			OLED_WR_Byte(fill_Data, 1);
		}
	}
}

/* Oled draw a point */
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
	OLED_WR_Byte(0xb0 + y, OLED_CMD);
	OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
	OLED_WR_Byte((x & 0x0f), OLED_CMD);
}

/* Oled display ON */
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC命令
	OLED_WR_Byte(0X14, OLED_CMD); //DCDC ON
	OLED_WR_Byte(0XAF, OLED_CMD); //DISPLAY ON
}

/* oled display OFF */
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D, OLED_CMD); //SET DCDC命令
	OLED_WR_Byte(0X10, OLED_CMD); //DCDC OFF
	OLED_WR_Byte(0XAE, OLED_CMD); //DISPLAY OFF
}

/* Clear oled, all pix is OFF */
void OLED_Clear(void)
{
	u8 i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); // set page addr(0-7)
		OLED_WR_Byte(0x00, OLED_CMD);	  // set display position - column lower addr
		OLED_WR_Byte(0x10, OLED_CMD);	  // set display position - column higher addr 
		for (n = 0; n < 128; n++)
		{
			OLED_WR_Byte(0, OLED_DATA);
		}
	} // update display
}
void OLED_On(void)
{
	u8 i, n;
	for (i = 0; i < 8; i++)
	{
		OLED_WR_Byte(0xb0 + i, OLED_CMD); // set page addr(0-7)
		OLED_WR_Byte(0x00, OLED_CMD);	  // set display position - column lower addr
		OLED_WR_Byte(0x10, OLED_CMD);	  // set display position - column higher addr 
		for (n = 0; n < 128; n++)
		{
			OLED_WR_Byte(1, OLED_DATA);
		}
	} //update display
}

/* Display a char at position (x,y)
 * x: 0 - 127
 * y: 0 - 63
 * Char_Size: 16/12
 */

void OLED_ShowChar(u8 x, u8 y, u8 chr, FontSize f_size)
{
	unsigned char c = 0, i = 0;
	c = chr - ' '; //得到偏移后的值
	if (x > Max_Column - 1)
	{
		x = 0;
		y = y + 2;
	}
	if (f_size == FONT_LARGE)
	{
		OLED_Set_Pos(x, y);
		for (i = 0; i < 8; i++)
		{
			OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
		}
		OLED_Set_Pos(x, y + 1);
		for (i = 0; i < 8; i++)
		{
			OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
		}
	}
	else
	{
		OLED_Set_Pos(x, y);
		for (i = 0; i < 6; i++)
		{
			OLED_WR_Byte(F6x8[c][i], OLED_DATA);
		}
	}
}

//m^n function
u32 oled_pow(u8 m, u8 n)
{
	u32 result = 1;
	while (n--)
	{
		result *= m;
	}
	return result;
}

/* Display integer numbers 
 * x,y: start position
 * len: number digits
 * size: font size
 * num: integer number
 */
void OLED_ShowNum(u8 x, u8 y, u32 num, u8 len, FontSize f_size)
{
	u8 t, temp;
	u8 enshow = 0;
	for (t = 0; t < len; t++)
	{
		temp = (num / oled_pow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				OLED_ShowChar(x + (f_size / 2) * t, y, ' ', f_size);
				continue;
			}
			else
				enshow = 1;
		}
		OLED_ShowChar(x + (f_size / 2) * t, y, temp + '0', f_size);
	}
}

/* Display a string */
void OLED_ShowString(u8 x, u8 y, u8 *chr, FontSize f_size)
{
	unsigned char j = 0;
	while (chr[j] != '\0')
	{
		OLED_ShowChar(x, y, chr[j], f_size);
		x += 8;
		if (x > 120)
		{
			x = 0;
			y += 2;
		}
		j++;
	}
}

/* Display a Chinese */
void OLED_ShowCHinese(u8 x, u8 y, u8 no)
{
	u8 t, adder = 0;
	OLED_Set_Pos(x, y);
	for (t = 0; t < 16; t++)
	{
		OLED_WR_Byte(Hzk[2 * no][t], OLED_DATA);
		adder += 1;
	}
	OLED_Set_Pos(x, y + 1);
	for (t = 0; t < 16; t++)
	{
		OLED_WR_Byte(Hzk[2 * no + 1][t], OLED_DATA);
		adder += 1;
	}
}

/* Display a max 128x64 BMP picture, start position is (x,y) */
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
	unsigned int j = 0;
	unsigned char x, y;

	if (y1 % 8 == 0)
		y = y1 / 8;
	else
		y = y1 / 8 + 1;
	for (y = y0; y < y1; y++)
	{
		OLED_Set_Pos(x0, y);
		for (x = x0; x < x1; x++)
		{
			OLED_WR_Byte(BMP[j++], OLED_DATA);
		}
	}
}

//初始化SSD1306
void OLED_Init(void)
{
	delay_init(168);
	delay_ms(100);
	IIC_Init();
	OLED_WR_Byte(0xAE, OLED_CMD); //--display off
	OLED_WR_Byte(0x00, OLED_CMD); //---set low column address
	OLED_WR_Byte(0x10, OLED_CMD); //---set high column address
	OLED_WR_Byte(0x40, OLED_CMD); //--set start line address
	OLED_WR_Byte(0xB0, OLED_CMD); //--set page address
	OLED_WR_Byte(0x81, OLED_CMD); // contract control
	OLED_WR_Byte(0xFF, OLED_CMD); //--128
	OLED_WR_Byte(0xA1, OLED_CMD); //set segment remap
	OLED_WR_Byte(0xA6, OLED_CMD); //--normal / reverse
	OLED_WR_Byte(0xA8, OLED_CMD); //--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F, OLED_CMD); //--1/32 duty
	OLED_WR_Byte(0xC8, OLED_CMD); //Com scan direction
	OLED_WR_Byte(0xD3, OLED_CMD); //-set display offset
	OLED_WR_Byte(0x00, OLED_CMD); //
	OLED_WR_Byte(0xD5, OLED_CMD); //set osc division
	OLED_WR_Byte(0x80, OLED_CMD); //
	OLED_WR_Byte(0xD8, OLED_CMD); //set area color mode off
	OLED_WR_Byte(0x05, OLED_CMD); //
	OLED_WR_Byte(0xD9, OLED_CMD); //Set Pre-Charge Period
	OLED_WR_Byte(0xF1, OLED_CMD); //
	OLED_WR_Byte(0xDA, OLED_CMD); //set com pin configuartion
	OLED_WR_Byte(0x12, OLED_CMD); //
	OLED_WR_Byte(0xDB, OLED_CMD); //set Vcomh
	OLED_WR_Byte(0x30, OLED_CMD); //
	OLED_WR_Byte(0x8D, OLED_CMD); //set charge pump enable
	OLED_WR_Byte(0x14, OLED_CMD); //
	OLED_WR_Byte(0xAF, OLED_CMD); //--turn on oled panel
}
