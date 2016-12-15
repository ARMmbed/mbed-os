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

#include "cmsis.h"
#include "mbed_rtx.h"
#include "rtx_os.h"
#include "cmsis_os2.h"
#include "cmsis_compiler.h"

extern unsigned char *mbed_heap_start;
extern uint32_t mbed_heap_size;

unsigned char *mbed_stack_isr_start = 0;
uint32_t mbed_stack_isr_size = 0;

extern int main(int argc, char **argv);

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

/*
 * set_stack_heap purpose is to set the following variables:
 * -mbed_heap_start
 * -mbed_heap_size
 * -mbed_stack_isr_start
 * -mbed_stack_isr_size
 *
 * Along with setting up os_thread_def_main
 */
void set_stack_heap(void) {

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

/* Define stack sizes if they haven't been set already */
#if !defined(ISR_STACK_SIZE)
    #define ISR_STACK_SIZE ((uint32_t)4096)
#endif

#if defined (__CC_ARM)

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

__asm void __rt_entry (void) {

  IMPORT  __user_setup_stackheap
  IMPORT  _platform_post_stackheap_init
  IMPORT  os_thread_def_main
  IMPORT  osKernelInitialize
  IMPORT  set_stack_heap
  IMPORT  osKernelStart
  IMPORT  osThreadCreate

  /* __user_setup_stackheap returns:
   * - Heap base in r0 (if the program uses the heap).
   * - Stack base in sp.
   * - Heap limit in r2 (if the program uses the heap and uses two-region memory).
   *
   * More info can be found in:
   * ARM Compiler ARM C and C++ Libraries and Floating-Point Support User Guide
   */
  BL      __user_setup_stackheap
  /* Ignore return value of __user_setup_stackheap since
   * this will be setup by set_stack_heap
   */
  BL      _platform_post_stackheap_init
  BL      osKernelInitialize
#ifdef __MBED_CMSIS_RTOS_CM
  BL      set_stack_heap
#endif
  LDR     R0,=os_thread_def_main
  MOVS    R1,#0
  BL      osThreadCreate
  BL      osKernelStart
  /* osKernelStart should not return */
  B       .

  ALIGN
}

#elif defined (__GNUC__)

extern void __libc_init_array (void);

osMutexId_t   malloc_mutex_id;
os_mutex_t    malloc_mutex_obj;
osMutexAttr_t malloc_mutex_attr;

osMutexId_t   env_mutex_id;
os_mutex_t    env_mutex_obj;
osMutexAttr_t env_mutex_attr;


osThreadAttr_t _main_thread_attr;
char _main_stack[4096] __ALIGNED(8);
char _main_obj[sizeof(os_thread_t)];

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

void mbed_start_main(void)
{
    mbed_cpy_nvic();

    set_stack_heap();

    _main_thread_attr.stack_mem = _main_stack;
    _main_thread_attr.stack_size = sizeof(_main_stack);
    _main_thread_attr.cb_size = sizeof(_main_obj);
    _main_thread_attr.cb_mem = _main_obj;
    _main_thread_attr.priority = osPriorityNormal;
    _main_thread_attr.name = "MAIN";
    osThreadNew((os_thread_func_t)pre_main, NULL, &_main_thread_attr);    // Create application main thread

    osKernelStart();                                                      // Start thread execution
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
