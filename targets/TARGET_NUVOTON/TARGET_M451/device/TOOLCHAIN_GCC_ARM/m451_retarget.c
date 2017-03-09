/******************************************************************************
 * @file     startup_NUC472_442.c
 * @version  V0.10
 * $Revision: 11 $
 * $Date: 15/09/02 10:02a $
 * @brief    CMSIS Cortex-M4 Core Peripheral Access Layer Source File for NUC472/442 MCU
 *
 * @note
 * Copyright (C) 2013~2015 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#include "M451Series.h"
#include <errno.h>
#include "nu_miscutil.h"

extern uint32_t __mbed_sbrk_start;
extern uint32_t __mbed_krbs_start;

#define NU_HEAP_ALIGN       32

/**
 * The default implementation of _sbrk() (in common/retarget.cpp) for GCC_ARM requires one-region model (heap and stack share one region), which doesn't
 * fit two-region model (heap and stack are two distinct regions), for example, NUMAKER-PFM-NUC472 locates heap on external SRAM. Define __wrap__sbrk() to
 * override the default _sbrk(). It is expected to get called through gcc hooking mechanism ('-Wl,--wrap,_sbrk') or in _sbrk().
 */
void *__wrap__sbrk(int incr)
{
    static uint32_t heap_ind = (uint32_t) &__mbed_sbrk_start;
    uint32_t heap_ind_old = NU_ALIGN_UP(heap_ind, NU_HEAP_ALIGN);
    uint32_t heap_ind_new = NU_ALIGN_UP(heap_ind_old + incr, NU_HEAP_ALIGN);
    
    if (heap_ind_new > &__mbed_krbs_start) {
        errno = ENOMEM;
        return (void *) -1;
    } 
    
    heap_ind = heap_ind_new;
    
    return (void *) heap_ind_old;
}
