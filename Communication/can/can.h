/*
 * @Author: luoqi 
 * @Date: 2021-01-04 21:21:24 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-07 17:34:26
 */

#ifndef _CAN_H
#define _CAN_H
typedef struct 
{
    unsigned char tsjw;
    unsigned char tbs2;
    unsigned char tbs1;
    unsigned short brp;
    unsigned char mode;
} CanInitTypedef;

typedef struct
{
    unsigned char send_data[8];
    unsigned char receive_data[8];
    unsigned char dlc;      // data length code
    unsigned char std_id;   // standard id
    unsigned char ext_id;   // extend id
    unsigned char ide;      // identifier extension
    unsigned char rtr;      // remote request substitution
} CanMsgTypedef;

void can_init(void);
unsigned char can1_mode_init(CanInitTypedef* can_init_data);
unsigned char can_send_msg(CanMsgTypedef msg);
unsigned char can_receive_msg(unsigned char *buf);

#endif
