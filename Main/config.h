/*
 * @Author: luoqi 
 * @Date: 2021-01-19 14:10:53 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-02-28 10:13:49
 */

#ifndef _CONFIG_H
#define _CONFIG_H

typedef signed char 		int8_t;
typedef signed short int 	int16_t;
typedef signed int 			int32_t;

typedef unsigned char 		uint8_t;
typedef unsigned short int 	uint16_t;
typedef unsigned int 		uint32_t;

typedef enum sys_disp 
{
    SYS_DISP_ENABLE,
    SYS_DISP_DISABLE
} SysDispState;

typedef enum state
{
    OK,
    ERR,
    BUSY,
    IDLE,
    PENDING,
    
} state_t;

/* The maximal quantity of motor we can control
*/
#define AK_MOTOR_NUM_MAX        8

/* Enable or disable debuge mode
 * 1: enable
 * 0: disable 
 */
#define USING_SYS_DEBUG         0

/* Using oled system display function 
 * 1: enable
 * 0: disnable
 */
#define USING_SYS_DISP          1

void mem_set(void* mem, uint8_t mem_val, uint32_t mem_size);
void mem_cpy(void* mem_src, void* mem_dst, uint32_t mem_size);

void sys_reset(void);
void sys_config_display(void);
void sys_disp_config(SysDispState state);

void sys_clk_init(void);
void sys_clk_start(void);
void sys_clk_stop(void);

#endif
