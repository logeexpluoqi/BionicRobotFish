/*
 * @Author: luoqi 
 * @Date: 2021-01-05 22:27:09 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-05 22:34:41
 */

#ifndef _OLED_TASK_H
#define _OLED_TASK_H 

typedef enum disp_type
{
    TYPE_CHAR,
    TYPE_NUM,
    TYPE_STRING
} OledDispType;

typedef enum
{
    LARGE,
    SMALL
} OledDispFont;
typedef struct oled_disp_msg 
{
    unsigned char pos_x;
    unsigned char pos_y;
    unsigned char disp_chr;
    unsigned char *disp_str;
    unsigned int disp_num;
    unsigned char num_len;
    OledDispFont font;
    OledDispType type;
    unsigned char refresh_flag;
} OledDispMsgTypedef;

void sys_disp_init(void);
void oled_sys_disp_task(void);
void sys_disp_close(void);
void sys_disp_open(void);

void sys_disp_char(unsigned char x, unsigned int y, unsigned char chr, OledDispFont font);
void sys_disp_num(unsigned char x, unsigned char y, unsigned int num, unsigned char len, OledDispFont font);
void sys_disp_str(unsigned char x, unsigned char y, unsigned char *str, OledDispFont font);

#endif
