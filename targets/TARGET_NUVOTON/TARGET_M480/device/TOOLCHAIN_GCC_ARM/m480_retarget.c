/******************************************************************************
 * @file     startup_NUC472_442.c
 * @version  V0.10
 * $Revision: 11 $
 * $Date: 15/09/02 10:02a $
 * @brief    CMSIS Cortex-M4 Core Peripheral Access Layer Source File for M480 MCU
 *
 * @note
 * Copyright (C) 2013~2015 Nuvoton Technology Corp. All rights reserved.
*****************************************************************************/

#include "M480.h"
#include <errno.h>
#include "nu_miscutil.h"

extern uint32_t __mbed_sbrk_start;
extern uint32_t __mbed_krbs_start;

#define NU_HEAP_ALIGN       32

/* Support heap with two-region model
 *
 * The default implementation of _sbrk() (in mbed_retarget.cpp) for GCC_ARM requires one-region
 * model (heap and stack share one region), which doesn't fit two-region model (heap and stack
 * are two distinct regions), e.g., stack in internal SRAM/heap in external SRAM on NUMAKER_PFM_NUC472.
 * Hence, override _sbrk() here to support heap with two-region model.
 */
void *_sbrk(int incr)
{
    static uint32_t heap_ind = (uint32_t) &__mbed_sbrk_start;
    uint32_t heap_ind_old = NU_ALIGN_UP(heap_ind, NU_HEAP_ALIGN);
    uint32_t heap_ind_new = NU_ALIGN_UP(heap_ind_old + incr, NU_HEAP_ALIGN);
    
    if (heap_ind_new > (uint32_t) &__mbed_krbs_start) {
        errno = ENOMEM;
        return (void *) -1;
    } 
    
    heap_ind = heap_ind_new;
    
    return (void *) heap_ind_old;
}
