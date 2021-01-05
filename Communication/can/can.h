/*
 * @Author: luoqi 
 * @Date: 2021-01-04 21:21:24 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-05 13:55:49
 */

#ifndef _CAN_H
#define _CAN_H

#include "type_def.h"

u8 can_init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode);
u8 can_send_msg(u8* msg, u8 len);
u8 can_receive_msg(u8* buf);

#endif
