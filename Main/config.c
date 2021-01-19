/*
 * @Author: luoqi 
 * @Date: 2021-01-19 19:56:32 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-19 20:29:45
 */

#include "config.h"

/* @breif: clear memery, after clear, the value is 0. 
 * @param: *mem, memery position, &s_structure or array;
 * @param: m_size, memery size, use sizeof() function to get memery size.
 */
void mem_clear(void* mem, unsigned int m_size)
{
    unsigned char *p = mem;
    while(m_size > 0)
    {
        *p = 0;
        p ++;
        m_size --;
    }
}

