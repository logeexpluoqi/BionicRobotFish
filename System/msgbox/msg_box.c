/*
 * @Author: luoqi 
 * @Date: 2021-02-17 20:28:45 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-02-23 20:04:13
 */

#include "msg_box.h"
#include "usart.h"
#include "msg_codec.h"

#define NULL (void*) 0

static msgbox_t* msgbox;

static void msg_distribute(uint8_t* msg);
static uint8_t msg_verify(uint8_t* msg);

void msg_get(uint8_t* msg)
{
    msg_distribute(msg);
}

void msg_put(MsgLocation location, void* msg, uint16_t msg_size)
{
    switch(location)
    {
    case COMPUTER:
        usart1_dma_tx_data(msg, msg_size); break;
    case AKMOTOR_CTRL_TASK:
        msg = msgbox->msgbox_akmotor; break;
    }
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
 * Distribute messages to it's related location
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
            uint8_t akmotor_num = msg[2]/11;
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
