/*
 * @Author: luoqi 
 * @Date: 2021-01-08 14:51:00 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-03-12 13:31:43
 */

#include "ak_motor_ctrl_task.h"
#include "msg_box.h"
#include "msg_codec.h"
#include "ak_motor.h"
#include "delay.h"

static msgbox_akmotor_t* akmotor;
static uint8_t ak_motor_upload_cache[62] = {0};

state_t ak_motor_ctrl_task()
{
    uint8_t i, chr[2];
    uint8_t motor_num = 0;
    uint8_t mode;
    state_t state = OK;

    if(msgbox_get_task_en())
    {
        mem_set(ak_motor_upload_cache, 0, sizeof(ak_motor_upload_cache));
        msg_put_akmotor_task(&akmotor, &mode);
        for(i = 0; i < AK_MOTOR_NUM_MAX; i++)
        {
            if(akmotor[i].exist == 1)
            {
                switch(mode)
                {
                case EN_MOTOR_MODE: 
                {
                    ak_motor_upload_cache[3 + motor_num*2] = akmotor[motor_num].id_dst;
                    if(ak_motor_mode_set(akmotor[motor_num].id_dst, EN_AK_MOTOR_MODE) == 0)
                        ak_motor_upload_cache[4 + motor_num*2] = 1;
                    else
                        ak_motor_upload_cache[4 + motor_num*2] = 0;
                    break;
                }
                case EX_MOTOR_MODE: 
                {
                    ak_motor_upload_cache[3 + motor_num*2] = akmotor[motor_num].id_dst;
                    if(ak_motor_mode_set(akmotor[motor_num].id_dst, EX_AK_MOTOR_MODE) == 0)
                        ak_motor_upload_cache[4 + motor_num*2] = 1;
                    else
                        ak_motor_upload_cache[4 + motor_num*2] = 0;
                    break;
                }
                case SET_MOTOR_ZERO: 
                {
                    ak_motor_upload_cache[3 + motor_num*2] = akmotor[motor_num].id_dst;
                    if(ak_motor_mode_set(akmotor[motor_num].id_dst, SET_AK_MOTOR_ZERO) == 0)
                        ak_motor_upload_cache[4 + motor_num*2] = 1;
                    else
                        ak_motor_upload_cache[4 + motor_num*2] = 0;
                    break;
                }
                case CTRL_MOTOR: 
                {
                    AkMotorCtrlTypedef ak_motor_ctrl_data;
                    AkMotorInfo motor_feedback;

                    ak_motor_ctrl_data.id    = akmotor[motor_num].id_dst;
                    ak_motor_ctrl_data.p_dst = akmotor[motor_num].p_dst;
                    ak_motor_ctrl_data.v_dst = akmotor[motor_num].v_dst;
                    ak_motor_ctrl_data.t_dst = akmotor[motor_num].t_dst;
                    ak_motor_ctrl_data.kp    = akmotor[motor_num].kp;
                    ak_motor_ctrl_data.kd    = akmotor[motor_num].kd;

                    ak_motor_ctrl(&ak_motor_ctrl_data, &motor_feedback);

                    akmotor[motor_num].id_feedback = motor_feedback.id;
                    akmotor[motor_num].p_feedback  = motor_feedback.position;
                    akmotor[motor_num].v_feedback  = motor_feedback.velocity;
                    akmotor[motor_num].t_feedback  = motor_feedback.torque;
                    
                    ak_motor_upload_cache[3 + motor_num*7] = motor_feedback.id;
                    msg_float_to_char(motor_feedback.position, chr);
                    ak_motor_upload_cache[4 + motor_num*7] = chr[1];
                    ak_motor_upload_cache[5 + motor_num*7] = chr[0];
                    msg_float_to_char(motor_feedback.velocity, chr);
                    ak_motor_upload_cache[6 + motor_num*7] = chr[1];
                    ak_motor_upload_cache[7 + motor_num*7] = chr[0];
                    msg_float_to_char(motor_feedback.torque, chr);
                    ak_motor_upload_cache[8 + motor_num*7] = chr[1];
                    ak_motor_upload_cache[9 + motor_num*7] = chr[0];
                    break;
                }
                }
                akmotor[motor_num].exist = 0;
                motor_num = motor_num + 1;
                delay_us(330); // this delay is necessary for CAN bus transmmit
            }
        }
        msgbox_task_en(TASK_DISABLE);

        if(state == OK)
        {
            if(mode == SET_MOTOR_ZERO || mode == EX_MOTOR_MODE || mode == EN_MOTOR_MODE)
            {
                ak_motor_upload_cache[0] = '{';
                ak_motor_upload_cache[1] = mode;
                ak_motor_upload_cache[2] = motor_num * 2;
                ak_motor_upload_cache[5 + motor_num*2] = '}';
                msg_put_computer(ak_motor_upload_cache, 6 + motor_num*2);
            }
            else if(mode == CTRL_MOTOR)
            {
                ak_motor_upload_cache[0] = '{';
                ak_motor_upload_cache[1] = mode;
                ak_motor_upload_cache[2] = motor_num * 7;
                ak_motor_upload_cache[5 + motor_num*7] = '}';
                msg_put_computer(ak_motor_upload_cache, 6 + motor_num*7);
            }
        }
    }
    return state;
}
