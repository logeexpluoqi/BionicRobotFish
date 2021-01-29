/*
 * @Author: luoqi 
 * @Date: 2021-01-19 14:10:53 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-19 21:07:44
 */

#ifndef _CONFIG_H
#define _CONFIG_H

/* The quantity of motor we want to control 
*/
#define AK_MOTOR_NUM            8

/* When mcu received all motor control data, then 
 * control all the data at same time. (in practice, 
 * these motors are controlled one by one in a short 
 * time)
 */
#define AK_MOTOR_GROUP_CTRL     0

/* Motor control mode:
 * 0: control in time_slice process;
 * 1: control once when receive a control command from pc;
 */
#define CTRL_MODE_STROKE        1

/* Data upload mode.
 * 1: continuous mode, upload data every control period;
 * 0: stroke mode, upload data while receive a control command.
 */
#define CONTINUOUS_UPLOAD       0

/* Use usart tx dma send data 
 * 1: use usart tx dma
 * 0: use usart tx normal
 */
#define USING_USART_DMA_TX      1

/* Use usart rx dma receive data 
 * 1: use usart rx dma ;
 * 0: use usart interrupt rx.
 */
#define USING_USART_DMA_RX      1

/* @breif: This function is used to clear structures.
 * @param: *mem, structure position;
 * @param: c, fill number.
 */
void mem_set(void* mem, unsigned char c);
void sys_reset(void);
void sys_config_display(void);

#endif
