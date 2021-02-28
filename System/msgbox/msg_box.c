/*
 * @Author: luoqi 
 * @Date: 2021-02-17 20:28:45 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-02-23 20:04:13
 */

#include "msg_box.h"
#include "usart.h"
#include "msg_codec.h"

#if USING_SYS_DEBUG
 #include "sys_debug.h"
#endif

#define NULL (void*) 0

static msgbox_t msgbox;
static uint8_t task_en = 0;

static void msg_distribute(uint8_t* msg);
static uint8_t msg_verify(uint8_t* msg);

void msg_get(uint8_t* msg)
{
    msg_distribute(msg);
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
    mem_set(msgbox.akmotor, 0);
    msg_put_computer("System initialize success!", 26);
}

void msgbox_task_en(MsgboxTaskState state)
{
    if(state == TASK_ENABLE)
        task_en = 1;
    else if(state == TASK_DISABLE)
        task_en = 0;
}

uint8_t msgbox_get_task_en()
{
    return task_en;
}

/* 
 * Distribute messages to it's related location
 */
void msg_distribute(uint8_t* msg)
{
    uint8_t i;

    msgbox.mode = msg[1];
    if(msg_verify(msg) == OK)
    {
        switch(msgbox.mode)
        {
        case EN_MOTOR_MODE:
        { 
            uint8_t akmotor_num = msg[2];
            for(i = 0; i < akmotor_num; i++)
            {
                msgbox.akmotor[i].exist  = 1;
                msgbox.akmotor[i].id_dst = msg[3+i];
            }
            break;
        }
        case EX_MOTOR_MODE:
        { 
            uint8_t akmotor_num = msg[2];
            for(i = 0; i < akmotor_num; i++)
            {
                msgbox.akmotor[i].exist  = 1;
                msgbox.akmotor[i].id_dst = msg[3+i];
            }
            break;
        }
        case SET_MOTOR_ZERO: 
        {
            uint8_t akmotor_num = msg[2];
            for(i = 0; i < akmotor_num; i++)
            {
                msgbox.akmotor[i].exist  = 1;
                msgbox.akmotor[i].id_dst = msg[3+i];
            }
            break;
        }
        case CTRL_MOTOR: 
        {
            uint8_t akmotor_num = msg[2]/11;
            for(i = 0; i < akmotor_num; i++)
            {
                msgbox.akmotor[i].exist  = 1;
                msgbox.akmotor[i].id_dst = msg[3 + i*11];
                msgbox.akmotor[i].p_dst  = msg_char_to_float(msg[4 + i*11], msg[5 + i*11]);
                msgbox.akmotor[i].v_dst  = msg_char_to_float(msg[6 + i*11], msg[7 + i*11]);
                msgbox.akmotor[i].t_dst  = msg_char_to_float(msg[8 + i*11], msg[9 + i*11]);
                msgbox.akmotor[i].kp     = msg_char_to_float(msg[10 + i*11], msg[11 + i*11]);
                msgbox.akmotor[i].kd     = msg_char_to_float(msg[12 + i*11], msg[13 + i*11]);
            }
            break;
        }
        #if USING_SYS_DEB
         case SYS_DEBUG: sys_debug(msg) break;
        #endif
        case SYS_RESET: sys_reset(); break;
        default: break;
        }
        msgbox_task_en(TASK_ENABLE);
    }
    else
        msg_put_computer("Message error !", 15);
}

void msg_put_computer(uint8_t* msg, uint16_t msg_size)
{
    usart1_dma_tx_data(msg, msg_size);
}

void msg_put_akmotor_task(msgbox_akmotor_t** akmotor, uint8_t* mode)
{
    *akmotor = msgbox.akmotor;
    *mode = msgbox.mode;
}
