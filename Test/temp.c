void pack_cmd(CANMessage *msg, float p_des, float v_des, float kp, float kd, float t_ff)
{
    // limit data to be within bounds ///
    p_des = fminf(fmaxf(P_MIN, p_des), P_MAX);
    v_des = fminf(fmaxf(V_MIN, v_des), V_MAX);
    kp = fminf(fmaxf(KP_MIN, kp), KP_MAX);
    kd = fminf(fmaxf(KD_MIN, kd), KD_MAX);
    t_ff = fminf(fmaxf(T_MIN, t_ff), T_MAX);
    // convert floats to unsigned ints ///
    int p_int = float_to_uint(p_des, P_MIN, P_MAX, 16);
    int v_int = float_to_uint(v_des, V_MIN, V_MAX, 12);
    int kp_int = float_to_uint(kp, KP_MIN, KP_MAX, 12);
    int kd_int = float_to_uint(kd, KD_MIN, KD_MAX, 12);
    int t_int = float_to_uint(t_ff, T_MIN, T_MAX, 12);
    // pack ints into the can buffer ///
    msg->data[0] = p_int >> 8;                           //λ�ø�8
    msg->data[1] = p_int & 0xFF;                         //λ�õ�8
    msg->data[2] = v_int >> 4;                           //�ٶȸ�8λ
    msg->data[3] = ((v_int & 0xF) << 4) | (kp_int >> 8); //�ٶȵ�4λ   KP��4λ
    msg->data[4] = kp_int & 0xFF;                        //KP��8λ
    msg->data[5] = kd_int >> 4;                          //Kd��8λ
    msg->data[6] = ((kd_int & 0xF) << 4) | (t_int >> 8); //KP��4λ     Ť�ظ�4λ
    msg->data[7] = t_int & 0xff;                         //Ť�ص�8λ
}

�������̴���
void unpack_reply(CANMessage msg)
{
    /// unpack ints from can buffer ///
    int id = msg.data[0];                                 //����ID��
    int p_int = (msg.data[1] << 8) | msg.data[2];         //���λ������
    int v_int = (msg.data[3] << 4) | (msg.data[4] >> 4);  //����ٶ�����
    int i_int = ((msg.data[4] & 0xF) << 8) | msg.data[5]; //���Ť������
    /// convert ints to floats ///
    float p = uint_to_float(p_int, P_MIN, P_MAX, 16);
    float v = uint_to_float(v_int, V_MIN, V_MAX, 12);
    float i = uint_to_float(i_int, -I_MAX, I_MAX, 12);
    if (id == 1)
    {
        postion = p; //����ID��  ��ȡ��Ӧ����
        speed = v;
        torque = i;
    }
}
