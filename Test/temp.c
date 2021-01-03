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
    msg->data[0] = p_int >> 8;                           //位置高8
    msg->data[1] = p_int & 0xFF;                         //位置低8
    msg->data[2] = v_int >> 4;                           //速度高8位
    msg->data[3] = ((v_int & 0xF) << 4) | (kp_int >> 8); //速度低4位   KP高4位
    msg->data[4] = kp_int & 0xFF;                        //KP低8位
    msg->data[5] = kd_int >> 4;                          //Kd高8位
    msg->data[6] = ((kd_int & 0xF) << 4) | (t_int >> 8); //KP低4位     扭矩高4位
    msg->data[7] = t_int & 0xff;                         //扭矩低8位
}

接收例程代码
void unpack_reply(CANMessage msg)
{
    /// unpack ints from can buffer ///
    int id = msg.data[0];                                 //驱动ID号
    int p_int = (msg.data[1] << 8) | msg.data[2];         //电机位置数据
    int v_int = (msg.data[3] << 4) | (msg.data[4] >> 4);  //电机速度数据
    int i_int = ((msg.data[4] & 0xF) << 8) | msg.data[5]; //电机扭矩数据
    /// convert ints to floats ///
    float p = uint_to_float(p_int, P_MIN, P_MAX, 16);
    float v = uint_to_float(v_int, V_MIN, V_MAX, 12);
    float i = uint_to_float(i_int, -I_MAX, I_MAX, 12);
    if (id == 1)
    {
        postion = p; //根据ID号  读取对应数据
        speed = v;
        torque = i;
    }
}
