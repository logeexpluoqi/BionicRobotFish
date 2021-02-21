/*
 * @Author: luoqi 
 * @Date: 2021-02-17 20:28:45 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-02-17 20:29:39
 */

#include "msg_box.h"
#include "usart.h"

#define NULL (void*) 0

static uint8_t msg_rcv_cache[300];
static uint8_t msg_send_cache[300];

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

void msg_distribute(uint8_t* msg)
{
    uint8_t mode = msg[1];

    if(msg_verify(msg) == OK)
    {
        switch(mode)
        {
        case EN_MOTOR_MODE: break;
        case EX_MOTOR_MODE: break;
        case SET_MOTOR_ZERO: break;
        case CTRL_MOTOR: break;

        case SYS_RESET: break;
        default:
            break;
        }
    }
    else
    {

    }
}
