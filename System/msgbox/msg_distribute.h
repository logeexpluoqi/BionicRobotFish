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
    EN_MOTOR_MODE   = 1,
    EX_MOTOR_MODE   = 2,
    SET_MOTOR_ZERO  = 3,
    CTRL_MOTOR      = 4
} MsgCmd;

void get_msg(unsigned char* msg, unsigned int msg_size);
void msg_distribute(void);

#endif
