/* mbed Microcontroller Library - stackheap
 * Copyright (C) 2009-2018 ARM Limited. All rights reserved.
 *
 * Setup a fixed single stack/heap memory model,
 *  between the top of the RW/ZI region and the stackpointer
 */

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#include <arm_compat.h>
#endif

#include <rt_misc.h>
#include <stdint.h>

extern char Image$$ARM_LIB_HEAP$$ZI$$Base[];
extern char Image$$ARM_LIB_HEAP$$ZI$$Length[];

extern __value_in_regs struct __initial_stackheap _mbed_user_setup_stackheap(uint32_t R0, uint32_t R1, uint32_t R2, uint32_t R3)
{
    uint32_t hp_base  = (uint32_t)Image$$ARM_LIB_HEAP$$ZI$$Base;
    uint32_t hp_limit = (uint32_t)Image$$ARM_LIB_HEAP$$ZI$$Length + hp_base;
    struct __initial_stackheap r;

    hp_base = (hp_base + 7) & ~0x7;    // ensure hp_base is 8-byte aligned
    r.heap_base = hp_base;
    r.heap_limit = hp_limit;
    return r;
}

#ifdef __cplusplus
}
#endif
