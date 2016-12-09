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

extern void __libc_init_array (void);
extern int main(int argc, char **argv);

osMutexId_t singleton_mutex_id;
osMutexAttr_t singleton_mutex_attr;

osMutexId_t malloc_mutex_id;
osMutexAttr_t malloc_mutex_attr;

osMutexId_t env_mutex_id;
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
    singleton_mutex_id = osMutexNew(&singleton_mutex_attr);

    malloc_mutex_attr.attr_bits = osMutexRecursive;
    malloc_mutex_id = osMutexNew(&malloc_mutex_attr);

    env_mutex_attr.attr_bits = osMutexRecursive;
    env_mutex_id = osMutexNew(&env_mutex_attr);

    __libc_init_array();

    main(0, NULL);
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

