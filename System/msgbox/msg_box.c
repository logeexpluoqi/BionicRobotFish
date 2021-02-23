/*
 * @Author: luoqi 
 * @Date: 2021-02-23 09:53:29 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-02-23 09:57:35
 */
#include "msg_box.h"
#include "ak_motor.h"
#include "ak_motor_ctrl_task.h"
#include "config.h"

static unsigned char msg_cache[300];

void get_msg(unsigned char* msg, unsigned int msg_size)
{
    mem_cpy(msg, msg_cache, msg_size);
}

void msg_distribute()
{
#if AK_MOTOR_CTRL_MODE == 1
    switch(msg_cache[0])
    {
    }
    
#else // strok control mode
    switch (msg_cache[0])
    {
    case EN_MOTOR_MODE:  ak_motor_mode_set(msg_cache[1], ENTER_MOTOR_CTRL); break;
    case EX_MOTOR_MODE:  ak_motor_mode_set(msg_cache[1], QUIT_MOTOR_CTRL); break;
    case SET_MOTOR_ZERO: ak_motor_mode_set(msg_cache[1], SET_ZERO_POS); break;
    case CTRL_MOTOR:
    {
    #if AK_MOTOR_CTRL_MODE == 0
        ak_motor_stroke_ctrl(msg_cache + 1);
	#endif
        break;
    }
    case 255: 
    {
        sys_reset();
    }

    default:
        break;
    }
#endif
}
