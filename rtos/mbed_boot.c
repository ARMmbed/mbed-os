/* mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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

/* mbed OS boot sequence
 *
 * Most of mbed supported targets use default ARM Cortex M boot approach, where the core starts executing reset vector
 * after power up. Reset ISR is defined for each target by the vendor (basing on CMSIS template). Reset vector is
 * responsible for low level platform init and then calling in libc (__main). Depending on compiler and version of C
 * library, predefined function will be called which is implemented by mbed OS.
 *
 * There's number of functions, vendor and users can provide to setup the platform and/or inject a code to be executed
 *  before main():
 *      * Reset vector and SystemInit: Reset vector should do low level core and board initialization.
 *      * mbed_sdk_init: Higher level board init and making sure the board is ready for the mbed OS.
 *      * mbed_main: User's code to be executed before main().
 *      * main: Standard application code.
 *
 * Detailed boot procedures:
 *
 * For ARMCC:
 * ==========
 *
 * Reset (TARGET)
 *     -> SystemInit (TARGET)
 *     -> __main (LIBC)
 *         -> __rt_entry (MBED: rtos/mbed_boot.c)
 *             -> __user_setup_stackheap (LIBC)
 *             -> set_stack_heap (MBED: rtos/mbed_boot.c)
 *             -> __rt_lib_init (LIBC)
 *             -> mbed_sdk_init (TARGET)
 *             -> _platform_post_stackheap_init (RTX)
 *                 -> osKernelInitialize (RTX)
 *             -> mbed_start_main (MBED: rtos/mbed_boot.c)
 *                 -> mbed_cpy_nvic (MBED: rtos/mbed_boot.c)
 *                 -> osThreadNew (RTX)
 *                     -> pre_main(MBED: rtos/mbed_boot.c)
 *                         -> $Sub$$main (MBED: rtos/mbed_boot.c)
 *                             -> mbed_main (MBED: rtos/mbed_boot.c)
 *                             -> main (APP)
 *                 -> osKernelStart (RTX)
 *
 * In addition to the above, libc will use functions defined by RTX: __user_perthread_libspace, _mutex_initialize,
 * _mutex_acquire, _mutex_release, _mutex_free for details consult: ARM C and C++ Libraries and Floating-Point
 * Support User Guide.
 *
 * For GCC:
 * ========
 *
 * Reset (TARGET)
 *     -> SystemInit (TARGET)
 *     -> __main (LIBC)
 *         -> software_init_hook (MBED: rtos/mbed_boot.c)
 *             -> set_stack_heap (MBED: rtos/mbed_boot.c)
 *             -> mbed_sdk_init (TARGET)
 *             -> osKernelInitialize (RTX)
 *             -> mbed_start_main (MBED: rtos/mbed_boot.c)
 *                 -> mbed_cpy_nvic (MBED: rtos/mbed_boot.c)
 *                 -> osThreadNew (RTX)
 *                     -> pre_main(MBED: rtos/mbed_boot.c)
 *                     -> __libc_init_array (LIBC)
 *                         -> __wrap_main (MBED: rtos/mbed_boot.c)
 *                             -> mbed_main (MBED: rtos/mbed_boot.c)
 *                             -> __real_main (APP)
 *                 -> osKernelStart (RTX)
 *
 *
 * Other notes:
 *
 *    * In addition to the above, libc will use functions defined in mbed_boot.c: __rtos_malloc_lock/unlock,
 *      __rtos_env_lock/unlock.
 *
 *    * First step after the execution is passed to mbed, software_init_hook for GCC and __rt_entry for ARMC is to
 *      initialize heap.
 *
 * Memory layout notes:
 * ====================
 *
 * IAR Default Memory layout notes:
 * -Heap defined by "HEAP" region in .icf file
 * -Interrupt stack defined by "CSTACK" region in .icf file
 * -Value INITIAL_SP is ignored
 *
 * IAR Custom Memory layout notes:
 * -There is no custom layout available for IAR - everything must be defined in
 *      the .icf file and use the default layout
 *
 *
 * GCC Default Memory layout notes:
 * -Block of memory from symbol __end__ to define INITIAL_SP used to setup interrupt
 *      stack and heap in the function set_stack_heap()
 * -ISR_STACK_SIZE can be overridden to be larger or smaller
 *
 * GCC Custom Memory layout notes:
 * -Heap can be explicitly placed by defining both HEAP_START and HEAP_SIZE
 * -Interrupt stack can be explicitly placed by defining both ISR_STACK_START and ISR_STACK_SIZE
 *
 *
 * ARM Memory layout
 * -Block of memory from end of region "RW_IRAM1" to define INITIAL_SP used to setup interrupt
 *      stack and heap in the function set_stack_heap()
 * -ISR_STACK_SIZE can be overridden to be larger or smaller
 *
 * ARM Custom Memory layout notes:
 * -Heap can be explicitly placed by defining both HEAP_START and HEAP_SIZE
 * -Interrupt stack can be explicitly placed by defining both ISR_STACK_START and ISR_STACK_SIZE
 *
 */

#include "cmsis.h"
#include "mbed_rtx.h"
#include "rtx_lib.h"
#include "cmsis_os2.h"
#include "platform/toolchain.h"

// Heap limits - only used if set
unsigned char *mbed_heap_start = 0;
uint32_t mbed_heap_size = 0;

unsigned char *mbed_stack_isr_start = 0;
uint32_t mbed_stack_isr_size = 0;

extern int main(int argc, char **argv);
WEAK void mbed_main(void);
void pre_main (void);

osThreadAttr_t _main_thread_attr;
char _main_stack[4096] __ALIGNED(8);
char _main_obj[sizeof(os_thread_t)];

osMutexId_t   singleton_mutex_id;
os_mutex_t    singleton_mutex_obj;
osMutexAttr_t singleton_mutex_attr;

/*
 * Sanity check values
 */
#if defined(__ICCARM__) &&                                  \
    (defined(HEAP_START) || defined(HEAP_SIZE) ||           \
     defined(ISR_STACK_START) && defined(ISR_STACK_SIZE))
    #error "No custom layout allowed for IAR. Use .icf file instead"
#endif
#if defined(HEAP_START) && !defined(HEAP_SIZE)
    #error "HEAP_SIZE must be defined if HEAP_START is defined"
#endif
#if defined(ISR_STACK_START) && !defined(ISR_STACK_SIZE)
    #error "ISR_STACK_SIZE must be defined if ISR_STACK_START is defined"
#endif
#if defined(HEAP_SIZE) && !defined(HEAP_START)
    #error "HEAP_START must be defined if HEAP_SIZE is defined"
#endif

#if !defined(INITIAL_SP) && !defined(HEAP_START)
    #error "no target defined"
#endif

/* Define heap region if it has not been defined already */
#if !defined(HEAP_START)
    #if defined(__ICCARM__)
        #error "Heap should already be defined for IAR"
    #elif defined(__CC_ARM)
        extern uint32_t          Image$$RW_IRAM1$$ZI$$Limit[];
        #define HEAP_START      ((unsigned char*)Image$$RW_IRAM1$$ZI$$Limit)
        #define HEAP_SIZE       ((uint32_t)((uint32_t)INITIAL_SP - (uint32_t)HEAP_START))
    #elif defined(__GNUC__)
        extern uint32_t         __end__[];
        #define HEAP_START      ((unsigned char*)__end__)
        #define HEAP_SIZE       ((uint32_t)((uint32_t)INITIAL_SP - (uint32_t)HEAP_START))
    #endif
#endif

/* Define stack sizes if they haven't been set already */
#if !defined(ISR_STACK_SIZE)
    #define ISR_STACK_SIZE ((uint32_t)4096)
#endif

/*
 * mbed_set_stack_heap purpose is to set the following variables:
 * -mbed_heap_start
 * -mbed_heap_size
 * -mbed_stack_isr_start
 * -mbed_stack_isr_size
 */
void mbed_set_stack_heap(void) {

    unsigned char *free_start = HEAP_START;
    uint32_t free_size = HEAP_SIZE;

#ifdef ISR_STACK_START
    /* Interrupt stack explicitly specified */
    mbed_stack_isr_size = ISR_STACK_SIZE;
    mbed_stack_isr_start = ISR_STACK_START;
#else
    /* Interrupt stack -  reserve space at the end of the free block */
    mbed_stack_isr_size = ISR_STACK_SIZE;
    mbed_stack_isr_start = free_start + free_size - mbed_stack_isr_size;
    free_size -= mbed_stack_isr_size;
#endif

    /* Heap - everything else */
    mbed_heap_size = free_size;
    mbed_heap_start = free_start;
}

static void mbed_cpy_nvic(void)
{
    /* If vector address in RAM is defined, copy and switch to dynamic vectors. Exceptions for M0 which doesn't have
    VTOR register and for A9 for which CMSIS doesn't define NVIC_SetVector; in both cases target code is
    responsible for correctly handling the vectors.
    */
#if !defined(__CORTEX_M0) && !defined(__CORTEX_A9)
#ifdef NVIC_RAM_VECTOR_ADDRESS
    uint32_t *old_vectors = (uint32_t *)SCB->VTOR;
    uint32_t *vectors = (uint32_t*)NVIC_RAM_VECTOR_ADDRESS;
    for (int i = 0; i < NVIC_NUM_VECTORS; i++) {
        vectors[i] = old_vectors[i];
    }
    SCB->VTOR = (uint32_t)NVIC_RAM_VECTOR_ADDRESS;
#else /* NVIC_RAM_VECTOR_ADDRESS */
#error NVIC_RAM_VECTOR_ADDRESS not defined!
#endif /* NVIC_RAM_VECTOR_ADDRESS */
#endif /* !defined(__CORTEX_M0) && !defined(__CORTEX_A9) */
}

// mbed_main is a function that is called before main()
// mbed_sdk_init() is also a function that is called before main(), but unlike
// mbed_main(), it is not meant for user code, but for the SDK itself to perform
// initializations before main() is called.
WEAK void mbed_main(void) {

}

WEAK void mbed_sdk_init(void);
WEAK void mbed_sdk_init(void) {
}

void mbed_start_main(void)
{
    mbed_cpy_nvic();

    _main_thread_attr.stack_mem = _main_stack;
    _main_thread_attr.stack_size = sizeof(_main_stack);
    _main_thread_attr.cb_size = sizeof(_main_obj);
    _main_thread_attr.cb_mem = _main_obj;
    _main_thread_attr.priority = osPriorityNormal;
    _main_thread_attr.name = "MAIN";
    osThreadNew((osThreadFunc_t)pre_main, NULL, &_main_thread_attr);    // Create application main thread

    osKernelStart();                                                      // Start thread execution
}

/******************** Toolchain specific code ********************/

#if defined (__CC_ARM) /******************** ARMCC ********************/

int $Super$$main(void);
int $Sub$$main(void) {
    mbed_main();
    return $Super$$main();
}

/* If present, RTX version of this function will be called */
WEAK void _platform_post_stackheap_init (void) {
}

void pre_main (void)
{
    singleton_mutex_attr.attr_bits = osMutexRecursive;
    singleton_mutex_attr.cb_size = sizeof(singleton_mutex_obj);
    singleton_mutex_attr.cb_mem = &singleton_mutex_obj;
    singleton_mutex_id = osMutexNew(&singleton_mutex_attr);

    __rt_lib_init((unsigned)mbed_heap_start, (unsigned)(mbed_heap_start + mbed_heap_size));

    main(0, NULL);
}

/* The single memory model is checking for stack collision at run time, verifing
   that the heap pointer is underneath the stack pointer.
   With the RTOS there is not only one stack above the heap, there are multiple
   stacks and some of them are underneath the heap pointer.
*/
#pragma import(__use_two_region_memory)

/* Called by the C library */
void __rt_entry (void) {
    __user_setup_stackheap();
    mbed_set_stack_heap();
    mbed_sdk_init();
    _platform_post_stackheap_init();
    mbed_start_main();
}

#elif defined (__GNUC__) /******************** GCC ********************/

extern void __libc_init_array (void);
extern int __real_main(void);

osMutexId_t   malloc_mutex_id;
os_mutex_t    malloc_mutex_obj;
osMutexAttr_t malloc_mutex_attr;

osMutexId_t   env_mutex_id;
os_mutex_t    env_mutex_obj;
osMutexAttr_t env_mutex_attr;

#ifdef  FEATURE_UVISOR
#include "uvisor-lib/uvisor-lib.h"
#endif/* FEATURE_UVISOR */

int __wrap_main(void) {
    mbed_main();
    return __real_main();
}

void pre_main(void)
{
    singleton_mutex_attr.attr_bits = osMutexRecursive;
    singleton_mutex_attr.cb_size = sizeof(singleton_mutex_obj);
    singleton_mutex_attr.cb_mem = &singleton_mutex_obj;
    singleton_mutex_id = osMutexNew(&singleton_mutex_attr);

    malloc_mutex_attr.attr_bits = osMutexRecursive;
    malloc_mutex_attr.cb_size = sizeof(malloc_mutex_obj);
    malloc_mutex_attr.cb_mem = &malloc_mutex_obj;
    malloc_mutex_id = osMutexNew(&malloc_mutex_attr);

    env_mutex_attr.attr_bits = osMutexRecursive;
    env_mutex_attr.cb_size = sizeof(env_mutex_obj);
    env_mutex_attr.cb_mem = &env_mutex_obj;
    env_mutex_id = osMutexNew(&env_mutex_attr);

    __libc_init_array();

    main(0, NULL);
}

void software_init_hook(void)
{
#ifdef   FEATURE_UVISOR
    int return_code;

    return_code = uvisor_lib_init();
    if (return_code) {
        mbed_die();
    }
#endif/* FEATURE_UVISOR */
    mbed_set_stack_heap();
    mbed_sdk_init();
    osKernelInitialize();
    mbed_start_main();
}

// Opaque declaration of _reent structure
struct _reent;

void __rtos_malloc_lock( struct _reent *_r )
{
    osMutexAcquire(malloc_mutex_id, osWaitForever);
}

void __rtos_malloc_unlock( struct _reent *_r )
{
    osMutexRelease(malloc_mutex_id);
}

void __rtos_env_lock( struct _reent *_r )
{
    osMutexAcquire(env_mutex_id, osWaitForever);
}

void __rtos_env_unlock( struct _reent *_r )
{
    osMutexRelease(env_mutex_id);
}

#endif

#if defined(TOOLCHAIN_IAR) /******************** IAR ********************/

// IAR doesn't have the $Super/$Sub mechanism of armcc, nor something equivalent
// to ld's --wrap. It does have a --redirect, but that doesn't help, since redirecting
// 'main' to another symbol looses the original 'main' symbol. However, its startup
// code will call a function to setup argc and argv (__iar_argc_argv) if it is defined.
// Since mbed doesn't use argc/argv, we use this function to call our mbed_main.
void __iar_argc_argv() {
    mbed_main();
}

#endif
