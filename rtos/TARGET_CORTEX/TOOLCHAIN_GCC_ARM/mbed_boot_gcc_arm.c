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

#include "cmsis.h"
#include "mbed_toolchain.h"
#include "mbed_boot.h"
#include "mbed_rtos_storage.h"
#include "cmsis_os2.h"

static osMutexId_t               malloc_mutex_id;
static mbed_rtos_storage_mutex_t malloc_mutex_obj;
static osMutexAttr_t             malloc_mutex_attr;

static osMutexId_t               env_mutex_id;
static mbed_rtos_storage_mutex_t env_mutex_obj;
static osMutexAttr_t             env_mutex_attr;

extern uint32_t             __StackLimit;
extern uint32_t             __StackTop;
extern uint32_t             __end__;
extern uint32_t             __HeapLimit;

extern void __libc_init_array(void);

/*
 * mbed entry point for the GCC toolchain
 *
 * Override gcc boot hook software_init_hook to run code before main.
 */
void software_init_hook(void)
{
    mbed_stack_isr_start = (unsigned char *) &__StackLimit;
    mbed_stack_isr_size = (uint32_t) &__StackTop - (uint32_t) &__StackLimit;
    mbed_heap_start = (unsigned char *) &__end__;
    mbed_heap_size = (uint32_t) &__HeapLimit - (uint32_t) &__end__;

    mbed_init();
    mbed_rtos_start();
}

void mbed_toolchain_init()
{
    malloc_mutex_attr.name = "malloc_mutex";
    malloc_mutex_attr.attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust;
    malloc_mutex_attr.cb_size = sizeof(malloc_mutex_obj);
    malloc_mutex_attr.cb_mem = &malloc_mutex_obj;
    malloc_mutex_id = osMutexNew(&malloc_mutex_attr);

    env_mutex_attr.name = "env_mutex";
    env_mutex_attr.attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust;
    env_mutex_attr.cb_size = sizeof(env_mutex_obj);
    env_mutex_attr.cb_mem = &env_mutex_obj;
    env_mutex_id = osMutexNew(&env_mutex_attr);

    /* Run the C++ global object constructors */
    __libc_init_array();
}

extern int __real_main(void);
int __wrap_main(void)
{
    /* For backwards compatibility */
    return __real_main();
}

/* Opaque declaration of _reent structure */
struct _reent;

void __rtos_malloc_lock(struct _reent *_r)
{
    osMutexAcquire(malloc_mutex_id, osWaitForever);
}

void __rtos_malloc_unlock(struct _reent *_r)
{
    osMutexRelease(malloc_mutex_id);
}

void __rtos_env_lock(struct _reent *_r)
{
    osMutexAcquire(env_mutex_id, osWaitForever);
}

void __rtos_env_unlock(struct _reent *_r)
{
    osMutexRelease(env_mutex_id);
}
