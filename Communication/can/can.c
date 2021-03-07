/*
 * @Author: luoqi 
 * @Date: 2021-01-04 21:22:57 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-15 21:15:14
 */
/* 
 * modified from 正点原子，by luoqi
 */

#include "can.h"
#include "stm32f4xx_can.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

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
 * fpclk is 36MHz when this clock initialized, if set CAN_Normal_Init(CAN_SJW_1tq,
 * CAN_BS2_6tq,CAN_BS1_7tq,6,CAN_Mode_LoopBack) buad rate is 42MHz / ((1+6+7) * 6) = 500Kbps
 * 
 * return value: 0, initial is ok; 1: fault
 */
void can1_init()
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
    CAN_InitStructure.CAN_TTCM      = DISABLE;  // 非时间触发通信模式
    CAN_InitStructure.CAN_ABOM      = DISABLE;  // 软件自动离线管理
    CAN_InitStructure.CAN_AWUM      = DISABLE;  // 睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
    CAN_InitStructure.CAN_NART      = ENABLE;   // 禁止报文自动传送
    CAN_InitStructure.CAN_RFLM      = ENABLE;   // 报文不锁定,新的覆盖旧的
    CAN_InitStructure.CAN_TXFP      = DISABLE;  // 优先级由报文标识符决定
    CAN_InitStructure.CAN_Mode      = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW       = CAN_SJW_1tq;      // Tsjw = tsjw+1, CAN_SJW_1tq~CAN_SJW_4tq
    CAN_InitStructure.CAN_BS1       = CAN_BS1_7tq;      // Tbs1: CAN_BS1_1tq ~CAN_BS1_16tq
    CAN_InitStructure.CAN_BS2       = CAN_BS2_6tq;      // Tbs2: CAN_BS2_1tq ~	CAN_BS2_8tq
    CAN_InitStructure.CAN_Prescaler = 3;                // f_div = brp+1
    CAN_Init(CAN1, &CAN_InitStructure);

    /* Configure CAN filter */
    CAN_FilterInitStructure.CAN_FilterNumber         = 0; // filter 0
    CAN_FilterInitStructure.CAN_FilterMode           = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale          = CAN_FilterScale_32bit; 
    CAN_FilterInitStructure.CAN_FilterIdHigh         = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow          = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh     = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow      = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation     = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
}

uint8_t can_send_msg(CanMsgTypedef msg)
{
    uint8_t mbox;
    uint8_t i = 0;
    CanTxMsg TxMessage;
    TxMessage.StdId = msg.std_id; // standard ID identify 
    TxMessage.ExtId = msg.ext_id; // extend ID identify 
    TxMessage.IDE   = msg.ide;      // indentifier extension
    TxMessage.RTR   = msg.rtr;      // remote transmission request
    TxMessage.DLC   = msg.dlc;      // data length code

    for (i = 0; i < msg.dlc; i++)
    {
        TxMessage.Data[i] = msg.send_data[i]; // send a frame
    }
    mbox = CAN_Transmit(CAN1, &TxMessage);
    i = 0;
    while ((CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Failed) && (i < 500))
    {
        i++; // wait for send finish
    }
    if (i >= 500)
    {
        return 1;
    }

    return 0;
}

uint8_t can_receive_msg(uint8_t *msg)
{
    uint32_t i;
    CanRxMsg RxMessage;
    if (CAN_MessagePending(CAN1, CAN_FIFO0) == 0)
    {
        return 0; // if no data received, quit 
    }
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage); // read receive data from fifo
    for (i = 0; i < RxMessage.DLC; i++)
    {
        msg[i] = RxMessage.Data[i];
    }
    CAN_FIFORelease(CAN1, CAN_FIFO0);
    return RxMessage.DLC;
}

