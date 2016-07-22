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

#include "NUC472_442.h"
#include <errno.h>

extern uint32_t __mbed_sbrk_start;
extern uint32_t __mbed_krbs_start;

// NOTE: The implementation of _sbrk (in common/retarget.cpp) for GCC_ARM requires one-region model for heap and stack, which doesn't
//       meet the layout of e.g. Nu-mbed-NUC472 board where heap is located on external SRAM. 
//       Because the symbol is not weak and cannot be overwritten, wrap calls to it by hooking command line linker with '-Wl,--wrap,_sbrk'.
void *__wrap__sbrk(int incr)
{
    static uint32_t heap_ind = (uint32_t) &__mbed_sbrk_start;
    uint32_t heap_ind_old = heap_ind;
    uint32_t heap_ind_new = (heap_ind_old + incr + 7) & ~7;
    
    if (heap_ind_new > &__mbed_krbs_start) {
        errno = ENOMEM;
        return (void *) -1;
    } 
    
    heap_ind = heap_ind_new;
    
    return (void *) heap_ind_old;
}
