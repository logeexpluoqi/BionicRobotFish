/*
 * @Author: luoqi 
 * @Date: 2021-02-17 20:28:45 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-02-17 20:29:39
 */

#include "msg_box.h"
#include "usart.h"
#include "msg_codec.h"

#define NULL (void*) 0

static uint8_t msg_rcv_cache[300];
static uint8_t msg_send_cache[300];

static msgbox_t* msgbox;

/* @breif: clear memery, after clear, the value is mem_val 
 * @param: *mem, memery position, &s_structure or array;
 * @param: mem_val, set the memery value, uint8_t type.
 */
void mem_set(void* mem, uint8_t mem_val)
{
    uint8_t *p = mem;
    uint32_t m_size = sizeof(p);
    while(m_size > 0)
    {
        *p = mem_val;
        p ++;
        m_size --;
    }
}

void mem_cpy(void* mem_src, void* mem_dst, uint32_t m_size)
{
    int8_t* p_src = (int8_t*)mem_src;
    int8_t* p_dst = (int8_t*)mem_dst;
    while(m_size --)
        *p_dst++ = *p_src++;
}

void msg_get(MsgSrc src, uint8_t* msg, uint16_t msg_size)
{
    if(src == COMPUTER)
        mem_cpy(msg, msg_rcv_cache, msg_size);
    else if(src == TASKS)
        mem_cpy(msg, msg_send_cache, msg_size);
}

void msg_send(uint8_t* msg, uint16_t len)
{
    usart1_dma_tx_data(msg, len);
}

/* Verification receive message
 * Msg format: SOF | MODE | SIZE | DATA0 - DATAx | FCS_h | FCS_l | EOF
 * retval: 0, OK; 1, Error
 */
uint8_t msg_verify(uint8_t* msg)
{
    uint8_t err = OK;
    uint8_t datax_size = msg[2];

    /* Check the data correctness and distribute this frame of data */
    if((msg[0] == '{') && (msg[datax_size + 5] == '}'))
    {
        // crc verification
    }
    else
        err = ERR;
    
    return err;
}

void msgbox_init()
{
    mem_set(msgbox, 0);
}

/* 
 * Distribute messages to it's related task
 */
void msg_distribute(uint8_t* msg)
{
    uint8_t mode = msg[1];
    uint8_t i;

    if(msg_verify(msg) == OK)
    {
        switch(mode)
        {
        case EN_MOTOR_MODE:
        { 
            uint8_t akmotor_num = msg[2];
            for(i = 0; i < akmotor_num; i++)
            {
                msgbox->msgbox_akmotor[i].exist  = 1;
                msgbox->msgbox_akmotor[i].mode   = EN_MOTOR_MODE;
                msgbox->msgbox_akmotor[i].id_dst = msg[3+i];
            }
            break;
        }
        case EX_MOTOR_MODE:
        { 
            uint8_t akmotor_num = msg[2];
            for(i = 0; i < akmotor_num; i++)
            {
                msgbox->msgbox_akmotor[i].exist  = 1;
                msgbox->msgbox_akmotor[i].mode   = EX_MOTOR_MODE;
                msgbox->msgbox_akmotor[i].id_dst = msg[3+i];
            }
            break;
        }
        case SET_MOTOR_ZERO: 
        {
            uint8_t akmotor_num = msg[2];
            for(i = 0; i < akmotor_num; i++)
            {
                msgbox->msgbox_akmotor[i].exist  = 1;
                msgbox->msgbox_akmotor[i].mode   = SET_MOTOR_ZERO;
                msgbox->msgbox_akmotor[i].id_dst = msg[3+i];
            }
            break;
        }
        case CTRL_MOTOR: 
        {
            uint8_t akmotor_num = (uint8_t)(msg[2]/11);
            for(i = 0; i < akmotor_num; i++)
            {
                msgbox->msgbox_akmotor[i].exist  = 1;
                msgbox->msgbox_akmotor[i].id_dst = msg[3 + i*11];
                msgbox->msgbox_akmotor[i].p_dst  = msg_char_to_float(msg[4 + i*11], msg[5 + i*11]);
                msgbox->msgbox_akmotor[i].v_dst  = msg_char_to_float(msg[6 + i*11], msg[7 + i*11]);
                msgbox->msgbox_akmotor[i].t_dst  = msg_char_to_float(msg[8 + i*11], msg[9 + i*11]);
                msgbox->msgbox_akmotor[i].kp     = msg_char_to_float(msg[10 + i*11], msg[11 + i*11]);
                msgbox->msgbox_akmotor[i].kd     = msg_char_to_float(msg[12 + i*11], msg[13 + i*11]);
            }
            break;
        }
        case SYS_RESET: 
        {
            sys_reset();
            break;
        }
        default:
            break;
        }
    }
    else
    {
        
    }
}
