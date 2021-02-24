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

static uint8_t ak_motor_upload_cache[62];

void ak_motor_ctrl_task()
{
    uint8_t i, chr[2];
    uint8_t ctrl_motor_num = 0;
    AkMotorInfo motor_info;
    AkMotorCtrlTypedef ak_motor_ctrl_data;
    struct msg_box_akmotor_t msg_box_akmotor[AK_MOTOR_NUM_MAX];

    uint8_t start[] = "{MOTOR UNLOCK}";
    uint8_t exit[]  = "{MOTOR LOCKED}";
    uint8_t zero[]  = "{SET ZERO    }";
    uint8_t error[] = "{CAN SEND ERR}";

    if(msgbox_get_task_en())
    {
        msg_put(AKMOTOR_CTRL_TASK, msg_box_akmotor, 0);
        for(i = 0; i < AK_MOTOR_NUM_MAX; i++)
        {
            if(msg_box_akmotor[i].exist == 1)
            {
                ctrl_motor_num  = ctrl_motor_num + 1;
                switch(msg_box_akmotor[i].mode)
                {
                case EN_MOTOR_MODE: 
                {
                    if(ak_motor_mode_set(msg_box_akmotor[i].id_dst, EN_AK_MOTOR_MODE) == 0)
                        msg_put(COMPUTER, start, 14);
                    else
                        msg_put(COMPUTER, error, 14);
                    break;
                }
                case EX_MOTOR_MODE: 
                {
                    if(ak_motor_mode_set(msg_box_akmotor[i].id_dst, EX_AK_MOTOR_MODE) == 0)
                        msg_put(COMPUTER, exit, 14);
                    else
                        msg_put(COMPUTER, error, 14);
                    break;
                }
                case SET_MOTOR_ZERO: 
                {
                    if(ak_motor_mode_set(msg_box_akmotor[i].id_dst, SET_AK_MOTOR_ZERO) == 0)
                        msg_put(COMPUTER, zero, 14);
                    else
                        msg_put(COMPUTER, error, 14);
                    break;
                }
                case CTRL_MOTOR: 
                {
                    ak_motor_ctrl_data.id    = msg_box_akmotor[i].id_dst;
                    ak_motor_ctrl_data.p_dst = msg_box_akmotor[i].p_dst;
                    ak_motor_ctrl_data.v_dst = msg_box_akmotor[i].v_dst;
                    ak_motor_ctrl_data.t_dst = msg_box_akmotor[i].t_dst;
                    ak_motor_ctrl_data.kp    = msg_box_akmotor[i].kp;
                    ak_motor_ctrl_data.kd    = msg_box_akmotor[i].kd;

                    ak_motor_ctrl(&ak_motor_ctrl_data, &motor_info);
                    
                    msg_box_akmotor[i].id_upload = motor_info.id;
                    msg_box_akmotor[i].p_upload  = motor_info.position;
                    msg_box_akmotor[i].v_upload  = motor_info.velocity;
                    msg_box_akmotor[i].t_upload  = motor_info.torque;
                    
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
            msg_box_akmotor[i].exist = 0;
        }
        ak_motor_upload_cache[0] = '{';
        ak_motor_upload_cache[1] = 4;
        ak_motor_upload_cache[2] = ctrl_motor_num;
        ak_motor_upload_cache[5 + ctrl_motor_num*7]= '}';
        msg_put(COMPUTER, ak_motor_upload_cache, 6 + ctrl_motor_num*7);
    }
    msgbox_task_en(TASK_DISABLE);
}
