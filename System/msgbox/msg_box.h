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

    SYS_DEBUG       = 254,
    SYS_RESET       = 255
};

typedef enum MSG_STATE
{
    MSG_OK,
    MSG_ERR
} msg_state_t;

typedef enum MSGBOX_TASK_EN
{
    TASK_ENABLE,
    TASK_DISABLE
} MsgboxTaskState;

typedef struct msgbox_akmotor
{
    /* motor exixt */
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
} msgbox_akmotor_t;

typedef struct msgbox
{
    uint8_t mode;
    msgbox_akmotor_t akmotor[AK_MOTOR_NUM_MAX];
} msgbox_t;

void msgbox_init(void);
void msgbox_task_en(MsgboxTaskState state);
uint8_t msgbox_get_task_en(void);
void msg_get(uint8_t* msg);

void msg_put_computer(uint8_t* msg, uint16_t msg_size);
void msg_put_akmotor_task(msgbox_akmotor_t** akmotor, uint8_t* mode);

#endif
