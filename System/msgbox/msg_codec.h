/*
 * @Author: luoqi 
 * @Date: 2021-01-13 22:45:26 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-13 22:46:03
 */

#ifndef _MSG_CODEC_H
#define _MSG_CODEC_H
#include "config.h"

float msg_char_to_float(uint8_t chr_1, uint8_t chr_0);
void msg_float_to_char(float num, uint8_t *pdst);


#endif
