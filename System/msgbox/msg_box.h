/*
 * @Author: luoqi 
 * @Date: 2021-02-17 20:27:52 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-02-17 20:45:42
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

typedef enum MSG_SRC
{
    COMPUTER,
    TASKS
} MsgSrc;

typedef struct msg_box_akmotor_t
{
    /* motor control mode */
    uint8_t mode;

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
} MsgBoxAkmotor;

void mem_set(void* mem, uint8_t mem_val);
void mem_cpy(void* mem_src, void* mem_dst, uint32_t m_size);

void msg_get(MsgSrc src, uint8_t* msg, uint16_t msg_size);
uint8_t msg_verify(uint8_t* msg);
void msg_send(uint8_t* msg, uint16_t len);

#endif
