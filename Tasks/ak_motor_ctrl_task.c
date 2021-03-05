/*
 * @Author: luoqi 
 * @Date: 2021-01-08 14:51:00 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-02-23 21:06:21
 */

#include "ak_motor_ctrl_task.h"
#include "msg_box.h"
#include "msg_codec.h"
#include "ak_motor.h"
#include "delay.h"
#include "oled_task.h"

static AkMotorInfo motor_info;
static AkMotorCtrlTypedef ak_motor_ctrl_data;
static msgbox_akmotor_t* akmotor;
static uint8_t ak_motor_upload_cache[62] = {0};

state_t ak_motor_ctrl_task()
{
    uint8_t i, chr[2];
    uint8_t ctrl_motor_num = 0;
    uint8_t mode;
    uint8_t err_cnt = 0;
    state_t state = OK;

    mem_set(ak_motor_upload_cache, 0, sizeof(ak_motor_upload_cache));
    if(msgbox_get_task_en())
    {
        msg_put_akmotor_task(&akmotor, &mode);
        for(i = 0; i < AK_MOTOR_NUM_MAX; i++)
        {
            if(akmotor[i].exist == 1)
            {
                delay_us(330); // this delay is necessary for CAN bus transmmit
                ak_motor_upload_cache[3 + i*2] = akmotor[i].id_dst;
                ctrl_motor_num  = ctrl_motor_num + 1;
                switch(mode)
                {
                case EN_MOTOR_MODE: 
                {
                    if(ak_motor_mode_set(akmotor[i].id_dst, EN_AK_MOTOR_MODE) == 0)
                        ak_motor_upload_cache[4 + i*2] = 1;
                    else
                        ak_motor_upload_cache[4 + i*2] = 0;
                    break;
                }
                case EX_MOTOR_MODE: 
                {
                    if(ak_motor_mode_set(akmotor[i].id_dst, EX_AK_MOTOR_MODE) == 0)
                        ak_motor_upload_cache[4 + i*2] = 1;
                    else
                        ak_motor_upload_cache[4 + i*2] = 0;
                    break;
                }
                case SET_MOTOR_ZERO: 
                {
                    if(ak_motor_mode_set(akmotor[i].id_dst, SET_AK_MOTOR_ZERO) == 0)
                        ak_motor_upload_cache[4 + i*2] = 1;
                    else
                        ak_motor_upload_cache[4 + i*2] = 0;
                    break;
                }
                case CTRL_MOTOR: 
                {
                    ak_motor_ctrl_data.id    = akmotor[i].id_dst;
                    ak_motor_ctrl_data.p_dst = akmotor[i].p_dst;
                    ak_motor_ctrl_data.v_dst = akmotor[i].v_dst;
                    ak_motor_ctrl_data.t_dst = akmotor[i].t_dst;
                    ak_motor_ctrl_data.kp    = akmotor[i].kp;
                    ak_motor_ctrl_data.kd    = akmotor[i].kd;

                    while(ak_motor_ctrl(&ak_motor_ctrl_data, &motor_info))
                    {
                        err_cnt ++;
                        if(err_cnt == 3)
                        {
                            state = ERR;
                            break;
                        }
                    }
                    
                    akmotor[i].id_upload = motor_info.id;
                    akmotor[i].p_upload  = motor_info.position;
                    akmotor[i].v_upload  = motor_info.velocity;
                    akmotor[i].t_upload  = motor_info.torque;
                    
                    ak_motor_upload_cache[3 + i*7] = motor_info.id;
                    msg_float_to_char(motor_info.position, chr);
                    ak_motor_upload_cache[4 + i*7] = chr[1];
                    ak_motor_upload_cache[5 + i*7] = chr[0];
                    msg_float_to_char(motor_info.velocity, chr);
                    ak_motor_upload_cache[6 + i*7] = chr[1];
                    ak_motor_upload_cache[7 + i*7] = chr[0];
                    msg_float_to_char(motor_info.torque, chr);
                    ak_motor_upload_cache[8 + i*7] = chr[1];
                    ak_motor_upload_cache[9 + i*7] = chr[0];
                    break;
                }
                }
            }
            akmotor[i].exist = 0;
        }

        if(state == OK)
        {
            if(mode == SET_MOTOR_ZERO || mode == EX_MOTOR_MODE || mode == EN_MOTOR_MODE)
            {
                ak_motor_upload_cache[0] = '{';
                ak_motor_upload_cache[1] = mode;
                ak_motor_upload_cache[2] = ctrl_motor_num * 2;
                ak_motor_upload_cache[5 + ctrl_motor_num*2]= '}';
                msg_put_computer(ak_motor_upload_cache, 6 + ctrl_motor_num*2);
            }
            else if(mode == CTRL_MOTOR)
            {
                ak_motor_upload_cache[0] = '{';
                ak_motor_upload_cache[1] = mode;
                ak_motor_upload_cache[2] = ctrl_motor_num * 7;
                ak_motor_upload_cache[5 + ctrl_motor_num*7]= '}';
                msg_put_computer(ak_motor_upload_cache, 6 + ctrl_motor_num*7);
            }
        }
    }
    msgbox_task_en(TASK_DISABLE);
    return state;
}
