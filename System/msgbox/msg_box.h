/*
 * @Author: luoqi 
 * @Date: 2021-02-17 20:27:52 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-02-23 20:05:11
 */

#ifndef _MSG_BOX_H
#define _MSG_BOX_H
#include "config.h"

enum MODE
{
    EN_MOTOR_MODE   = 1,
    EX_MOTOR_MODE   = 2,
    SET_MOTOR_ZERO  = 3,
    CTRL_MOTOR      = 4,

    SYS_RESET       = 255
};

enum MSG_STATE
{
    OK,
    ERR
};

typedef enum MSGBOX_TASK_EN
{
    TASK_ENABLE,
    TASK_DISABLE
} MsgboxTaskState;

typedef enum MSG_LOCATION
{
    COMPUTER,
    AKMOTOR_CTRL_TASK
} MsgLocation;

struct msg_box_akmotor_t
{
    /* motor control mode */
    uint8_t mode;
    uint8_t exist;

    /* receive motor control data */
    uint8_t id_dst;
    float p_dst;
    float v_dst;
    float t_dst;
    float kp;
    float kd;

    /* upload motor control data */
    uint8_t id_upload;
    float p_upload;
    float v_upload;
    float t_upload;
};

typedef struct msgbox
{
    struct msg_box_akmotor_t msgbox_akmotor[AK_MOTOR_NUM_MAX];
} msgbox_t;

void msgbox_init(void);
void msgbox_task_en(MsgboxTaskState state);
uint8_t msgbox_get_task_en(void);
void msg_get(uint8_t* msg);
void msg_put(MsgLocation location, void* msg, uint16_t msg_size);

#endif
