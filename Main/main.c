/*
 * @Author: luoqi 
 * @Date: 2021-01-04 08:45:16 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-04 19:20:31
 */
#include "init.h"
#include "time_slice.h"
#include "can_test.h"

int main(void)
{ 		
	sys_init();
	can_test_init();
	
	while(1)
	{	
		can_test();
		time_slice_process();
	}
}
 
