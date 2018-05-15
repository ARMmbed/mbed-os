/* mbed Microcontroller Library - stackheap
 * Copyright (C) 2009-2011 ARM Limited. All rights reserved.
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

/* The single region memory model would check stack collision at run time, verifying that
 * the heap pointer is underneath the stack pointer. With two-region memory model/RTOS-less or
 * multiple threads(stacks)/RTOS, the check gets meaningless and we must disable it. */
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
__asm(".global __use_two_region_memory\n\t");
__asm(".global __use_no_semihosting\n\t");
#else
#pragma import(__use_two_region_memory)
#endif

extern char Image$$ARM_LIB_HEAP_$$Base[];
extern char Image$$ARM_LIB_HEAP_$$ZI$$Limit[];
extern __value_in_regs struct __initial_stackheap _mbed_user_setup_stackheap(uint32_t R0, uint32_t R1, uint32_t R2, uint32_t R3) {

    struct __initial_stackheap r;
    r.heap_base = (uint32_t)Image$$ARM_LIB_HEAP_$$Base;
    r.heap_limit = (uint32_t)Image$$ARM_LIB_HEAP_$$ZI$$Limit;
    return r;
}

#ifdef __cplusplus
}
#endif
