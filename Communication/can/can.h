/*
 * @Author: luoqi 
 * @Date: 2021-01-04 21:21:24 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-07 17:34:26
 */

#ifndef _CAN_H
#define _CAN_H
#include "config.h"

typedef struct
{
    uint8_t send_data[8];
    uint8_t receive_data[8];
    uint8_t dlc;      // data length code
    uint8_t std_id;   // standard id
    uint8_t ext_id;   // extend id
    uint8_t ide;      // identifier extension
    uint8_t rtr;      // remote request substitution
} CanMsgTypedef;

void can1_init(void);
uint8_t can_send_msg(CanMsgTypedef msg);
uint8_t can_receive_msg(uint8_t *buf);

#endif
