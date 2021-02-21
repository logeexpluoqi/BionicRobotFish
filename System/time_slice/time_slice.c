/*
 * @Author: luoqi 
 * @Date: 2021-01-04 10:34:53 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-04 10:55:07
 */

#include "time_slice.h"
#include "task.h"
#include "timer.h"

extern TimeSliceInitTypedef Tasks[TASK_NUM];

void time_slice_init()
{
	tim3_int_init(10-1,8400-1); // 1kHz
}

void time_slice_process()
{
	uint8_t i;
	for(i=0; i<TASK_NUM; i++)
	{
		if(Tasks[i].TaskIsRunFlag == 1)
		{
			Tasks[i].TaskHook();
			Tasks[i].TaskIsRunFlag = 0;
		}
	}
}

void time_slice_tick()
{
	uint8_t i;
	
	for(i=0; i<TASK_NUM; i++)
	{
		if(Tasks[i].TaskTimer != 0)
		{
			Tasks[i].TaskTimer --;
			if(Tasks[i].TaskTimer == 0)
			{
				Tasks[i].TaskIsRunFlag = 1;
				Tasks[i].TaskTimer = Tasks[i].TaskIntervalTime;
			}
		}
	}
}


