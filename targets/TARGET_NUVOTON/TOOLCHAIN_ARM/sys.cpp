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

extern char Image$$ARM_LIB_STACK$$ZI$$Limit[];
extern char Image$$ARM_LIB_HEAP$$Base[];
extern char Image$$ARM_LIB_HEAP$$ZI$$Limit[];
extern __value_in_regs struct __initial_stackheap _mbed_user_setup_stackheap(uint32_t R0, uint32_t R1, uint32_t R2, uint32_t R3) {

    struct __initial_stackheap r;
    r.heap_base = (uint32_t)Image$$ARM_LIB_HEAP$$Base;
    r.heap_limit = (uint32_t)Image$$ARM_LIB_HEAP$$ZI$$Limit;
    return r;
}

#if !defined(MBED_CONF_RTOS_PRESENT) || !MBED_CONF_RTOS_PRESENT

/* The single region memory model would check stack collision at run time, verifying that
 * the heap pointer is underneath the stack pointer. With two-region memory model/RTOS-less or
 * multiple threads(stacks)/RTOS, the check gets meaningless and we must disable it. */
#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
__asm(".global __use_two_region_memory\n\t");
__asm(".global __use_no_semihosting\n\t");
#else
#pragma import(__use_two_region_memory)
#endif

/* Fix __user_setup_stackheap and ARM_LIB_STACK/ARM_LIB_HEAP cannot co-exist in RTOS-less build
 *
 * According AN241 (http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dai0241b/index.html),
 * __rt_entry has the following call sequence:
 * 1. _platform_pre_stackheap_init
 * 2. __user_setup_stackheap or setup the Stack Pointer (SP) by another method
 * 3. _platform_post_stackheap_init
 * 4. __rt_lib_init
 * 5. _platform_post_lib_init
 * 6. main()
 * 7. exit()
 *
 * Per our check, when __user_setup_stackheap and ARM_LIB_STACK/ARM_LIB_HEAP co-exist, neither
 * does __user_setup_stackheap get called and nor is ARM_LIB_HEAP used to get heap base/limit,
 * which are required to pass to __rt_lib_init later. To fix the issue, by subclass'ing
 * __rt_lib_init, heap base/limit are replaced with Image$$ARM_LIB_HEAP$$ZI$$Base/Limit if
 * ARM_LIB_HEAP region is defined in scatter file.
 *
 * The overriding __rt_lib_init is needed only for rtos-less code. For rtos code, __rt_entry is
 * overridden and the overriding __rt_lib_init here gets meaningless.
 */
extern __value_in_regs struct __argc_argv $Super$$__rt_lib_init(unsigned heapbase, unsigned heaptop);

__value_in_regs struct __argc_argv $Sub$$__rt_lib_init (unsigned heapbase, unsigned heaptop)
{
    return $Super$$__rt_lib_init((unsigned) Image$$ARM_LIB_HEAP$$Base, (unsigned) Image$$ARM_LIB_HEAP$$ZI$$Limit);
}

#endif

#ifdef __cplusplus
}
#endif
