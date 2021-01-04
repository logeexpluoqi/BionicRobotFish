/*
 * @Author: luoqi 
 * @Date: 2021-01-04 10:21:16 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-04 10:42:55
 */

#ifndef _TIME_SLICE_H
#define _TIME_SLICE_H

typedef struct TASK_COMPONENTS
{
    unsigned char TaskIsRunFlag;
    unsigned int TaskTimer;
    unsigned int TaskIntervalTime;
    void (*TaskHook)(void);
} TimeSliceInitTypedef;

void time_slice_init(void);
void time_slice_process(void);
void time_slice_tick(void);


#endif
