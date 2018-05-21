/* mbed Microcontroller Library - sbrkwrap
 * Copyright (C) 2009-2017 ARM Limited. All rights reserved.
 *
 * A wrap implementation of _sbrk()
 */

#if defined(TWO_RAM_REGIONS)
#include <stdint.h>
#include <errno.h>

extern uint32_t __sbrk_start;
extern uint32_t __krbs_start;

/* Overide _sbrk() to support two region model */
void *__wrap__sbrk(int incr)
{
    static uint32_t heap_ind = (uint32_t)(&__sbrk_start);
    uint32_t heap_ind_pre = heap_ind;
    uint32_t heap_ind_new = (heap_ind_pre + incr + 0x07) & ~0x07;
    if (heap_ind_new > (uint32_t)(&__krbs_start)) {
        errno = ENOMEM;
        return (void *)(-1);
    }
    heap_ind = heap_ind_new;
    return (void *) heap_ind_pre;
}
#endif
