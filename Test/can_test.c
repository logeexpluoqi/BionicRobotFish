/*
 * @Author: luoqi 
 * @Date: 2021-01-07 10:00:56 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-07 19:50:49
 */

#include "can_test.h"
#include "can.h"
#include "keyboard.h"
#include "keyboard_task.h"
#include "oled.h"

extern KeyTypedef key; // define in: /Tasks/keyboard task.c

// extern CanMsgTypedef can1_msg;

void can_test_init()
{
    // can1_msg.send_data[0] = 0xff;
    // can1_msg.send_data[1] = 0xff;
    // can1_msg.send_data[2] = 0xff;
    // can1_msg.send_data[3] = 0xff;
    // can1_msg.send_data[4] = 0xff;
    // can1_msg.send_data[5] = 0xff;
    // can1_msg.send_data[6] = 0xff;
    // can1_msg.send_data[7] = 0xfc;

    // can1_msg.send_data[0] = 0x7f;
    // can1_msg.send_data[1] = 0xff;
    // can1_msg.send_data[2] = 0x8e;
    // can1_msg.send_data[3] = 0x30;
    // can1_msg.send_data[4] = 0x00;
    // can1_msg.send_data[5] = 0x3d;
    // can1_msg.send_data[6] = 0x68;
    // can1_msg.send_data[7] = 0x38;

    // can1_msg.receive_data[0] = ' ';
    // can1_msg.receive_data[1] = ' ';
    // can1_msg.receive_data[2] = ' ';
    // can1_msg.receive_data[3] = ' ';
    // can1_msg.receive_data[4] = ' ';
    // can1_msg.receive_data[5] = ' ';
    // can1_msg.receive_data[6] = ' ';
    // can1_msg.receive_data[7] = ' ';
	
    // can1_msg.std_id = 4;
    // can1_msg.rtr = 0;
    // can1_msg.ext_id = 0;
    // can1_msg.ide = 0;
    // can1_msg.dlc = 8; // a frame 8 byte data

	OLED_ShowString(0, 2, "S:", FONT_LARGE);
    OLED_ShowString(0, 4, "R:", FONT_LARGE);
}

void can_test()
{
    unsigned char is_send_fault;
    unsigned char receive_data_len;
    unsigned char i;
    
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

