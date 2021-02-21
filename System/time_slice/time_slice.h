/*
 * @Author: luoqi 
 * @Date: 2021-01-04 10:21:16 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-04 10:42:55
 */

#ifndef _TIME_SLICE_H
#define _TIME_SLICE_H
#include "config.h"
typedef struct TASK_COMPONENTS
{
    uint8_t TaskIsRunFlag;
    uint32_t TaskTimer;
    uint32_t TaskIntervalTime;
    void (*TaskHook)(void);
} TimeSliceInitTypedef;

void time_slice_init(void);
void time_slice_process(void);
void time_slice_tick(void);


#endif
