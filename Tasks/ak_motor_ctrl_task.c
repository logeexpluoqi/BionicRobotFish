/*
 * @Author: luoqi 
 * @Date: 2021-01-08 14:51:00 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-19 21:09:21
 */

#include "ak_motor_ctrl_task.h"
#include "ak_motor.h"
#include "msg_codec.h"
#include "config.h"
#include "usart.h"

AkMotorCtrlTypedef ak_motor_ctrl_data = {
    .ctrl_en = 0,
    .p_dst   = 0,
    .v_dst   = 0,
    .t_dst   = 0,
    .kp      = 0,
    .kd      = 0
};

void ak_motor_ctrl_task()
{
    AkMotorInfo motor_info;
    ak_motor_ctrl(&ak_motor_ctrl_data, &motor_info); 
}

void ak_motor_stroke_ctrl(unsigned char *ctrl_data)
{
    AkMotorInfo motor_info;
    unsigned char chr[2];
    unsigned char msg_upload[9]; // float to char, up to usart transmmit

    ak_motor_ctrl_data.id    = ctrl_data[0];
    ak_motor_ctrl_data.p_dst = msg_char_to_float(ctrl_data[1], ctrl_data[2]);
    ak_motor_ctrl_data.v_dst = msg_char_to_float(ctrl_data[3], ctrl_data[4]);
    ak_motor_ctrl_data.t_dst = msg_char_to_float(ctrl_data[5], ctrl_data[6]);
    ak_motor_ctrl_data.kp    = msg_char_to_float(ctrl_data[7], ctrl_data[8]);
    ak_motor_ctrl_data.kd    = msg_char_to_float(ctrl_data[9], ctrl_data[10]);

    ak_motor_ctrl(&ak_motor_ctrl_data, &motor_info);

    msg_upload[1] = motor_info.id;
    msg_float_to_char(motor_info.position, chr);
    msg_upload[2] = chr[1];
    msg_upload[3] = chr[0];
    msg_float_to_char(motor_info.velocity, chr);
    msg_upload[4] = chr[1];
    msg_upload[5] = chr[0];
    msg_float_to_char(motor_info.torque, chr);
    msg_upload[6] = chr[1];
    msg_upload[7] = chr[0];
    msg_upload[0] = '{'; // SOF
    msg_upload[8] = '}'; // EOF

    if(get_usart_tx_flag(USART_1) == 1)
    {
        usart1_dma_tx_data(msg_upload, 9);
        usart_clear_tx_flag(USART_1);
    }
}
