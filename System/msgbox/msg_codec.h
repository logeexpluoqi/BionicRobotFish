/*
 * @Author: luoqi 
 * @Date: 2021-01-13 22:45:26 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-13 22:46:03
 */

#ifndef _MSG_CODEC_H
#define _MSG_CODEC_H

float msg_char_to_float(unsigned char chr_1, unsigned char chr_0);
void msg_float_to_char(float num, unsigned char *pdst);


#endif
