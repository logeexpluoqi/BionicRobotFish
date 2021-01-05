/*
 * @Author: luoqi 
 * @Date: 2021-01-04 21:22:57 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-05 17:29:23
 */
/* 
 * modified from 正点原子，by luoqi
 */

#include "can.h"
#include "stm32f4xx_can.h"


/* CAN initialize
 * tsjw: Resynchronize jump time unit. @ref CAN_synchronisation_jump_width;  
 *       Rage: CAN_SJW_1tq~ CAN_SJW_4tq;
 * tbs2: A time unit for time period 2. @ref CAN_time_quantum_in_bit_segment_2; 
 *       Rage:CAN_BS2_1tq~CAN_BS2_8tq;
 * tbs1: A time unit for time period 1. @ref CAN_time_quantum_in_bit_segment_1; 
 *       Rage:CAN_BS1_1tq~CAN_BS1_16tq;
 * brp: Baud rate divider; Rage: 1-1024; tq = (brp)*tpclk1;
 * mode: @ref CAN_operating_mode; Rage: CAN_Mode_Normal, CAN_Mode_LoopBack;
 * 
 * fpclk is 36MHz when this clock initialized, if set CAN_Normal_Init(CAN_SJW_1tq,CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack)
 * buad rate is 42MHz / ((1+6+7) * 6) = 500Kbps
 * 
 * return value: 0, initial is ok; 1: fault
 */
u8 can_init(u8 tsjw, u8 tbs2, u8 tbs1, u16 brp, u8 mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    CAN_InitTypeDef CAN_InitStructure;
    CAN_FilterInitTypeDef CAN_FilterInitStructure;
#if CAN1_RX0_INT_ENABLE
    NVIC_InitTypeDef NVIC_InitStructure;
#endif
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE); 

    //初始化GPIO
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);

    /* Configure CAN mode */
    CAN_InitStructure.CAN_TTCM = DISABLE;  // 非时间触发通信模式
    CAN_InitStructure.CAN_ABOM = DISABLE;  // 软件自动离线管理
    CAN_InitStructure.CAN_AWUM = DISABLE;  // 睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
    CAN_InitStructure.CAN_NART = ENABLE;   // 禁止报文自动传送
    CAN_InitStructure.CAN_RFLM = DISABLE;  // 报文不锁定,新的覆盖旧的
    CAN_InitStructure.CAN_TXFP = DISABLE;  // 优先级由报文标识符决定
    CAN_InitStructure.CAN_Mode = mode;     // 模式设置
    CAN_InitStructure.CAN_SJW = tsjw;      // 重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
    CAN_InitStructure.CAN_BS1 = tbs1;      // Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
    CAN_InitStructure.CAN_BS2 = tbs2;      // Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
    CAN_InitStructure.CAN_Prescaler = brp; // 分频系数(Fdiv)为brp+1
    CAN_Init(CAN1, &CAN_InitStructure);    // 初始化CAN1

    /* Configure CAN filter */
    CAN_FilterInitStructure.CAN_FilterNumber = 0; // filter 0
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; 
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;               //32bit ID
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000; //32bit MASK
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

#if CAN1_RX0_INT_ENABLE

    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE); //FIFO0消息挂号中断允许.

    NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 主优先级为1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;        // 次优先级为0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
#endif
    return 0;
}

u8 can_send_msg(u8 *msg, u8 len)
{
    u8 mbox;
    u16 i = 0;
    CanTxMsg TxMessage;
    TxMessage.StdId = 0x12; // 标准标识符为0
    TxMessage.ExtId = 0x12; // 设置扩展标示符（29位）
    TxMessage.IDE = 0;      // 使用扩展标识符
    TxMessage.RTR = 0;      // 消息类型为数据帧，一帧8位
    TxMessage.DLC = len;    // 发送两帧信息
    for (i = 0; i < len; i++)
    {
        TxMessage.Data[i] = msg[i]; // 第一帧信息
    }
    mbox = CAN_Transmit(CAN1, &TxMessage);
    i = 0;
    while ((CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Failed) && (i < 0XFFF))
    {
        i++; //等待发送结束
    }
    if (i >= 0XFFF)
    {
        return 1;
    }

    return 0;
}

u8 can_receive_msg(u8 *buf)
{
    u32 i;
    CanRxMsg RxMessage;
    if (CAN_MessagePending(CAN1, CAN_FIFO0) == 0)
        return 0;                             //没有接收到数据,直接退出
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage); //读取数据
    for (i = 0; i < RxMessage.DLC; i++)
        buf[i] = RxMessage.Data[i];
    return RxMessage.DLC;
}

#if CAN1_RX0_INT_ENABLE //使能RX0中断
//中断服务函数
void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg RxMessage;
    int i = 0;
    CAN_Receive(CAN1, 0, &RxMessage);
    for (i = 0; i < 8; i++)
        printf("rxbuf[%d]:%d\r\n", i, RxMessage.Data[i]);
}
#endif
