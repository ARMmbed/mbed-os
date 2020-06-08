/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <stdlib.h>
#include <string.h>
#include "cmsis.h"
#include "mbed_boot.h"

extern uint32_t               Image$$ARM_LIB_STACK$$ZI$$Base[];
extern uint32_t               Image$$ARM_LIB_STACK$$ZI$$Length[];

#if !defined(__MICROLIB)

#include "mbed_critical.h"
#include <rt_misc.h>
#include "mbed_rtos_storage.h"
#include "cmsis_os2.h"

__value_in_regs struct __argc_argv __rt_lib_init(unsigned heapbase, unsigned heaptop);
void _platform_post_stackheap_init(void);


#if !defined(HEAP_START)
// Heap here is considered starting after ZI ends to Stack start
extern uint32_t               Image$$RW_IRAM1$$ZI$$Limit[];
#define HEAP_START            Image$$RW_IRAM1$$ZI$$Limit
#define HEAP_SIZE             ((uint32_t)((uint32_t)Image$$ARM_LIB_STACK$$ZI$$Base - (uint32_t)HEAP_START))
#endif

/*
 * Note - Overriding the function __cpp_initialize__aeabi_ for the Arm
 * standard library causes a crash when there are no global constructors.
 * To avoid this do not override __cpp_initialize__aeabi_ with the super$$
 * sub$$ mechanism for the Arm standard library. The uARM library is not
 * effected by this problem.
 */

/*
 * mbed entry point for the ARM standard toolchain
 *
 * Override the ARM standard library function __rt_entry to run code earlier in
 * the boot sequence. This is after scatter loading has taken place but before
 * the C library has been initialized.
 */
void __rt_entry(void)
{
    mbed_stack_isr_start = (unsigned char *) Image$$ARM_LIB_STACK$$ZI$$Base;
    mbed_stack_isr_size = (uint32_t) Image$$ARM_LIB_STACK$$ZI$$Length;
    mbed_heap_start = (unsigned char *) HEAP_START;
    mbed_heap_size = (uint32_t) HEAP_SIZE;

    mbed_init();

    _platform_post_stackheap_init();
    mbed_rtos_start();
}

void mbed_toolchain_init()
{
    /* Run the C++ global object constructors */
    __rt_lib_init((unsigned)mbed_heap_start, (unsigned)(mbed_heap_start + mbed_heap_size));
}

/* Move all code here from RTX code base (rtx_lib.c) and do some modifications:
 *
 * 1. _mutex_initialize/_mutex_free are re-implemented to meet Mbed.
 * 2. All _mutex_* functions are declared with '__USED' to avoid excluded by linker.
 */
#if defined(RTX_NO_MULTITHREAD_CLIB)

/* The single memory model is checking for stack collision at run time, verifying
   that the heap pointer is underneath the stack pointer.
   With the RTOS there is not only one stack above the heap, there are multiple
   stacks and some of them are underneath the heap pointer.
*/
#if defined (__ARMCC_VERSION)
__asm(".global __use_two_region_memory\n\t");
__asm(".global __use_no_semihosting\n\t");
#endif

#define LIBSPACE_SIZE 96

//lint -esym(714,__user_perthread_libspace,_mutex_*) "Referenced by C library"
//lint -esym(765,__user_perthread_libspace,_mutex_*) "Global scope"
//lint -esym(9003, os_libspace*) "variables 'os_libspace*' defined at module scope"

// Memory for libspace
static uint32_t os_libspace[OS_THREAD_LIBSPACE_NUM + 1][LIBSPACE_SIZE / 4] \
__attribute__((section(".bss.os.libspace")));

// Thread IDs for libspace
static osThreadId_t os_libspace_id[OS_THREAD_LIBSPACE_NUM] \
__attribute__((section(".bss.os.libspace")));

// Check if Kernel has been started
static uint32_t os_kernel_is_active(void)
{
    static uint8_t os_kernel_active = 0U;

    if (os_kernel_active == 0U) {
        if (osKernelGetState() > osKernelReady) {
            os_kernel_active = 1U;
        }
    }
    return (uint32_t)os_kernel_active;
}

// Provide libspace for current thread
void *__user_perthread_libspace(void)
{
    osThreadId_t id;
    uint32_t     n;

    if (os_kernel_is_active() != 0U) {
        id = osThreadGetId();
        for (n = 0U; n < (uint32_t)OS_THREAD_LIBSPACE_NUM; n++) {
            if (os_libspace_id[n] == NULL) {
                os_libspace_id[n] = id;
            }
            if (os_libspace_id[n] == id) {
                break;
            }
        }
        if (n == (uint32_t)OS_THREAD_LIBSPACE_NUM) {
            (void)osRtxErrorNotify(osRtxErrorClibSpace, id);
        }
    } else {
        n = OS_THREAD_LIBSPACE_NUM;
    }

    //lint -e{9087} "cast between pointers to different object types"
    return (void *)&os_libspace[n][0];
}

/* ARM toolchain requires dynamically created mutexes to enforce thread safety. There's
   up to 8 static mutexes, protecting atexit, signalinit, stdin, stdout, stderr, stream_list,
   fp_trap_init and the heap. Additionally for each call to fopen one extra mutex will be
   created.
   mbed OS provides a RTX pool for 8 mutexes, to satisfy the static requirements. All
   additional mutexes will be allocated on the heap. We can't use the heap allocation for
   all the required mutexes, as the heap operations also require a mutex. We don't need to
   worry about freeing the allocated memory as library mutexes are only freed when the
   application finishes executing.
 */

typedef void *mutex;
#define OS_MUTEX_STATIC_NUM   8
mutex _static_mutexes[OS_MUTEX_STATIC_NUM] = {NULL};
mbed_rtos_storage_mutex_t _static_mutexes_mem[OS_MUTEX_STATIC_NUM] = {NULL};

//lint -save "Function prototypes defined in C library"
//lint -e970 "Use of 'int' outside of a typedef"
//lint -e818 "Pointer 'm' could be declared as pointing to const"

/* Initialize mutex */
__USED int _mutex_initialize(mutex *m)
{
    osMutexAttr_t attr;
    memset(&attr, 0, sizeof(attr));
    attr.name = "ARM toolchain mutex";
    attr.attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust;

    mutex *slot = NULL;
    core_util_critical_section_enter();
    for (int i = 0; i < OS_MUTEX_STATIC_NUM; i++) {
        if (_static_mutexes[i] == NULL) {
            _static_mutexes[i] = (mutex) - 1; // dummy value to reserve slot
            slot = &_static_mutexes[i];
            //Use the static attrs
            attr.cb_size = sizeof(mbed_rtos_storage_mutex_t);
            attr.cb_mem = &_static_mutexes_mem[i];
            break;
        }
    }
    core_util_critical_section_exit();

    if (slot != NULL) {
        *m = osMutexNew(&attr);
        *slot = *m;
        if (*m != NULL) {
            return 1;
        }
    }

    /* Mutex pool exhausted, try using HEAP */
    attr.cb_size = sizeof(mbed_rtos_storage_mutex_t);
    attr.cb_mem = (void *)malloc(attr.cb_size);
    if (attr.cb_mem == NULL) {
        osRtxErrorNotify(osRtxErrorClibSpace, m);
        return 0;
    }

    *m = osMutexNew(&attr);
    if (*m == NULL) {
        osRtxErrorNotify(osRtxErrorClibMutex, m);
        return 0;
    }

    return 1;
}

/* Acquire mutex */
__USED void _mutex_acquire(mutex *m)
{
    if (os_kernel_is_active() != 0U) {
        (void)osMutexAcquire(*m, osWaitForever);
    }
}

/* Release mutex */
__USED void _mutex_release(mutex *m)
{
    if (os_kernel_is_active() != 0U) {
        (void)osMutexRelease(*m);
    }
}

/* Free mutex */
__USED void _mutex_free(mutex *m)
{
    mutex *slot = NULL;
    core_util_critical_section_enter();
    for (int i = 0; i < OS_MUTEX_STATIC_NUM; i++) {
        if (_static_mutexes[i] == *m) {
            slot = &_static_mutexes[i];
            break;
        }
    }
    core_util_critical_section_exit();

    osMutexDelete(*m);

    // if no slot reserved for mutex, must have been dynamically allocated
    if (!slot) {
        free(m);
    } else {
        *slot = NULL;
    }

}

#endif /* RTX_NO_MULTITHREAD_CLIB */
#else
#include <stdint.h>

extern uint32_t               Image$$ARM_LIB_HEAP$$ZI$$Base[];
extern uint32_t               Image$$ARM_LIB_HEAP$$ZI$$Length[];

/*
 * mbed entry point for the MICROLIB toolchain
 *
 * Override the microlib function _main_init to run code earlier in
 * the boot sequence. The function _main_init is responsible for invoking main.
 * This function must be placed in the ".ARM.Collect" section
 * or it won't get called.
 */
void _main_init(void) __attribute__((section(".ARM.Collect$$$$000000FF")));
void _main_init(void)
{
    /* microlib only supports the two region memory model */
    mbed_stack_isr_start = (unsigned char *) Image$$ARM_LIB_STACK$$ZI$$Base;
    mbed_stack_isr_size = (uint32_t) Image$$ARM_LIB_STACK$$ZI$$Length;

    mbed_heap_start = (unsigned char *) Image$$ARM_LIB_HEAP$$ZI$$Base;
    mbed_heap_size = (uint32_t) Image$$ARM_LIB_HEAP$$ZI$$Length;

    mbed_init();
    mbed_rtos_start();
}

void $Sub$$__cpp_initialize__aeabi_(void);
void $Super$$__cpp_initialize__aeabi_(void);
void $Sub$$__cpp_initialize__aeabi_(void)
{
    /* This should invoke C++ initializers but we keep
     * this empty and invoke them RTX is initialized.
     */
}

void mbed_toolchain_init()
{
    /* Run the C++ global object constructors */
    $Super$$__cpp_initialize__aeabi_();
}

#endif // !defined(__MICROLIB)
