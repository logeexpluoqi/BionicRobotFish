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
	TIM3_Int_Init(10-1,8400-1); // 1kHz
}

void time_slice_process()
{
	unsigned char i;
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
	unsigned char i;
	
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


