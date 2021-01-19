/*
 * @Author: luoqi 
 * @Date: 2021-01-19 19:56:32 
 * @Last Modified by: luoqi
 * @Last Modified time: 2021-01-19 21:06:54
 */

#include "config.h"

/* @breif: clear memery, after clear, the value is c 
 * @param: *mem, memery position, &s_structure or array;
 * @param: c, set the memery value, unsigned char type.
 */
void mem_set(void* mem, unsigned char c)
{
    unsigned char *p = mem;
    unsigned int m_size = sizeof(p);
    while(m_size > 0)
    {
        *p = c;
        p ++;
        m_size --;
    }
}

