/*
 * @Author: luoqi 
 * @Date: 2021-01-14 08:28:57 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-14 08:39:00
 */

#ifndef _MSG_DISTRIBUTE_H
#define _MSG_DISTRIBUTE_H

typedef enum cmd 
{
    NORMAL          = 0,
    EN_MOTOR_MODE   = 1,
    EX_MOTOR_MODE   = 2,
    SET_MOTOR_ZERO  = 3
} MsgCmd;

void msg_distribute(unsigned char msg[28]);

#endif
