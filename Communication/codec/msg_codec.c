/*
 * @Author: luoqi 
 * @Date: 2021-01-13 22:46:10 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-13 22:49:44
 */

#include "msg_codec.h"


/* Receive data decode 
 * a float data use 2byte, 
 * chr_1: higher 8bit, chr_0: lower 8bit
 */
float msg_char_to_float(unsigned char chr_1, unsigned char chr_0)
{
    short int num_int;
    
    if((chr_1 & 0x80) == 0x80)
        num_int = -(((chr_1 & 0x7f) << 8) | chr_0);
    else
        num_int = ((chr_1 & 0x7f) << 8) | chr_0;
    return (float)num_int / 100;
}

/*  
 * pdst[0]: lower 8bit
 * pdst[1]: higher 8bit
 */
void msg_float_to_char(float num, unsigned char *pdst)
{
    short int num_int;

    num_int = (short int)(num * 100);
    if(num_int < 0)
    {   
        num_int = -num_int;
        *(pdst + 1) = (num_int >> 8) | 0x80;
        *pdst = num_int & 0x00ff;
    }
    else
    {
        *(pdst + 1) = num_int >> 8;
        *pdst = num_int & 0xff;
    }
}



