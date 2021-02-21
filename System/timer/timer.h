/*
 * @Author: luoqi 
 * @Date: 2021-01-04 09:46:01 
 * @Last Modified by:   luoqi 
 * @Last Modified time: 2021-01-04 09:46:01 
 */
#ifndef _TIMER_H
#define _TIMER_H
#include "config.h"

void tim3_int_init(uint16_t arr, uint16_t psc);
void tim5_int_init(void);

#endif
