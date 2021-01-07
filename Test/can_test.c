/*
 * @Author: luoqi 
 * @Date: 2021-01-07 10:00:56 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-07 17:50:46
 */

#include "can_test.h"
#include "can.h"
#include "keyboard.h"
#include "keyboard_task.h"
#include "oled.h"

extern KeyTypedef key; // define in: /Tasks/keyboard task.c

CanMsgTypedef can1_msg;

void can_test_init()
{
    can1_msg.send_data[0] = 'A';
    can1_msg.send_data[1] = 'b';
    can1_msg.send_data[2] = 'C';
    can1_msg.send_data[3] = 'd';
    can1_msg.send_data[4] = 'E';
    can1_msg.send_data[5] = 'f';
    can1_msg.send_data[6] = 'G';
    can1_msg.send_data[7] = 'h';

    can1_msg.receive_data[0] = '_';
    can1_msg.receive_data[1] = '_';
    can1_msg.receive_data[2] = '_';
    can1_msg.receive_data[3] = '_';
    can1_msg.receive_data[4] = '_';
    can1_msg.receive_data[5] = '_';
    can1_msg.receive_data[6] = '_';
    can1_msg.receive_data[7] = '_';
	
	OLED_ShowString(0, 2, "S:", FONT_LARGE);
    OLED_ShowString(0, 4, "R:", FONT_LARGE);
}

void can_test()
{
    unsigned char is_send_fault;
    unsigned char receive_data_len;
    unsigned char i;

    can1_msg.dlc = 8; // a frame 8 byte data

    if(key.key_1 == KEY_H)
    {
        is_send_fault = can_send_msg(can1_msg);
        if(is_send_fault)
        {
            OLED_ShowChar(110, 2, 'X', FONT_LARGE);
        }
		else
		{
			OLED_ShowChar(110, 2, 'O', FONT_LARGE);
            for(i=0; i<can1_msg.dlc; i++)
            {
                OLED_ShowChar(20+8*i, 2, can1_msg.send_data[i], FONT_LARGE);
            }
		}
        receive_data_len = can_receive_msg(can1_msg.receive_data);
        if(receive_data_len != 0)
        {
            OLED_ShowChar(110, 4, 'O', FONT_LARGE);
            for(i=0; i < receive_data_len; i++)
            {
                OLED_ShowChar(20+8*i, 4, can1_msg.receive_data[i], FONT_LARGE);
            }
            receive_data_len = 0;
        }
        else
        {
            OLED_ShowChar(110, 4, 'X', FONT_LARGE);
        }
    }
    if(key.key_2 == KEY_H)
    {
        for(i=0; i<8; i++)
        {
            OLED_ShowChar(20+8*i, 2, ' ', FONT_LARGE);
            OLED_ShowChar(20+8*i, 4, ' ', FONT_LARGE);
        }
        OLED_ShowChar(110, 2, ' ', FONT_LARGE);
        OLED_ShowChar(110, 4, ' ', FONT_LARGE);
    }
}

