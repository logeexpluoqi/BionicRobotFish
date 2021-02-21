/*
 * @Author: luoqi 
 * @Date: 2021-01-14 08:29:43 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-02-17 20:42:41
 */

#include "msg_distribute.h"
#include "msg_box.h"
#include "stm32f4xx_crc.h"

static uint8_t msg_cache[300];

void get_msg(uint8_t* msg, uint32_t msg_size)
{
    mem_cpy(msg, msg_cache, msg_size);
}

void msg_distribute()
{
    uint8_t datax_size = msg_cache[2];
    uint8_t mode = msg_cache[1];

    /* Check the data correctness and distribute this frame of data */
    if((msg_cache[0] == '{') && (msg_cache[datax_size + 5] == '}'))
    {
        
    }
} 
