/*
 * @Author: luoqi 
 * @Date: 2021-01-05 22:27:09 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-05 22:34:41
 */

#ifndef _OLED_TASK_H
#define _OLED_TASK_H 
#include "config.h"

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
    uint8_t  pos_x;
    uint8_t  pos_y;
    uint8_t  disp_chr;
    uint8_t  *disp_str;
    uint32_t disp_num;
    uint8_t  num_len;
    OledDispFont font;
    OledDispType type;
    uint8_t refresh_flag;
} OledDispMsgTypedef;

void sys_disp_init(void);
void oled_sys_disp_task(void);
void sys_disp_close(void);
void sys_disp_open(void);

void sys_disp_char(uint8_t x, uint32_t y, uint8_t chr, OledDispFont font);
void sys_disp_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, OledDispFont font);
void sys_disp_str(uint8_t x, uint8_t y, uint8_t *str, OledDispFont font);

#endif
