/*
 * @Author: luoqi 
 * @Date: 2021-01-13 22:46:10 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-13 22:49:44
 */

#include "msg_codec.h"

CodecTypedef codec;

/* Receive data decode 
 * a float data use 2byte, 
 */
void msg_char_to_float(unsigned char chr_1, unsigned char chr_0)
{
    short int num_int;
    
    if((chr_1 & 0x80) == 0x80)
        num_int = -(((chr_1 & 0x7f) << 8) | chr_0);
    else
        num_int = ((chr_1 & 0x7f) << 8) | chr_0;
    codec.num = (float)num_int / 100;
}

void msg_float_to_char(float num)
{
    short int num_int;

    num_int = (short int)(num * 100);
    if(num_int < 0)
    {   
        num_int = -num_int;
        codec.chr[1] = (num_int >> 8) | 0x80;
        codec.chr[0] = num_int & 0x00ff;
    }
    else
    {
        codec.chr[1] = num_int >> 8;
        codec.chr[0] = num_int & 0xff;
    }
}



