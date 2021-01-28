/*
 * @Author: luoqi 
 * @Date: 2021-01-04 08:45:16 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-19 20:46:30
 */
#include "init.h"
#include "time_slice.h"
 
int main(void)
{
	sys_init();
	while(1)
	{
		time_slice_process();
	}
}
