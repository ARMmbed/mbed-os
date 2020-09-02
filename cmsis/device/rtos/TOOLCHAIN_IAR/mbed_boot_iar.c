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
#include "mbed_error.h"
#include "mbed_boot.h"
#if defined(__IAR_SYSTEMS_ICC__ ) && (__VER__ >= 8000000)
#include <DLib_Threads.h>
#endif
#include "mbed_rtos_storage.h"
#include "cmsis_os2.h"

/* Defined by linker script */
#pragma section="CSTACK"
#pragma section="HEAP"

extern void *__vector_table[];
extern int  __low_level_init(void);
extern void __iar_data_init3(void);
extern __weak void __iar_init_core(void);
extern __weak void __iar_init_vfp(void);
extern void __iar_dynamic_initialization(void);

/*
 * mbed entry point for the IAR toolchain
 *
 * Override __iar_program_start to run code before main.
 * This is the first function called after the low level
 * target initialization, so RAM has not been setup yet.
 *
 * Details on what this function are supposed to do can
 * be found by looking at the IAR source for this file.
 *
 * IAR source files for ARM instruction set:
 * <IAR>\arm\src\lib\arm\cstartup.s
 * <IAR>\arm\src\lib\arm\cmain.s
 *
 * IAR source files for THUMB instruction set:
 * <IAR>\arm\src\lib\thumb\cmain.s
 * <IAR>\arm\src\lib\thumb\cstartup_M.s
 */
#pragma required=__vector_table
void __iar_program_start(void)
{
    /* the calls below are normally made in IAR cstartup */
    __iar_init_core();
    __iar_init_vfp();

    /* the calls below are normally made in IAR cmain
     *
     * The function "__low_level_init" is a user overrideable hook
     * that does nothing by default. Returning zero means that
     * ram initialization should be skipped. Skipping ram initialization
     * is not allowed by mbed.
     *
     * The function "__iar_data_init3" is an IAR function which
     * initializes ram.
     *
     */
    __low_level_init();
    __iar_data_init3();

    /* mbed specific code */
    mbed_heap_start = (unsigned char *)__section_begin("HEAP");
    mbed_heap_size = (uint32_t)__section_size("HEAP");

    mbed_stack_isr_start = (unsigned char *)__section_begin("CSTACK");
    mbed_stack_isr_size = (uint32_t)__section_size("CSTACK");

    mbed_init();
    mbed_rtos_start();
}

void mbed_toolchain_init(void)
{
#if defined(__IAR_SYSTEMS_ICC__ ) && (__VER__ >= 8000000) && !defined(MBED_RTOS_SINGLE_THREAD)
    __iar_Initlocks();
#endif

    /* Run the C++ global object constructors */
    __iar_dynamic_initialization();
}

/* Thread safety */
static osMutexId_t               std_mutex_id_sys[_MAX_LOCK] = {0};
static mbed_rtos_storage_mutex_t std_mutex_sys[_MAX_LOCK] = {0};
#define _FOPEN_MAX 10
static osMutexId_t               std_mutex_id_file[_FOPEN_MAX] = {0};
static mbed_rtos_storage_mutex_t std_mutex_file[_FOPEN_MAX] = {0};

void __iar_system_Mtxinit(__iar_Rmtx *mutex) /* Initialize a system lock */
{
    osMutexAttr_t attr;
    uint32_t index;
    for (index = 0; index < _MAX_LOCK; index++) {
        if (0 == std_mutex_id_sys[index]) {
            attr.name = "system_mutex";
            attr.cb_mem = &std_mutex_sys[index];
            attr.cb_size = sizeof(std_mutex_sys[index]);
            attr.attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust;
            std_mutex_id_sys[index] = osMutexNew(&attr);
            *mutex = (__iar_Rmtx *)&std_mutex_id_sys[index];
            return;
        }
    }

    /* This should never happen */
    error("Not enough mutexes\n");
}

void __iar_system_Mtxdst(__iar_Rmtx *mutex) /* Destroy a system lock */
{
    osMutexDelete(*(osMutexId_t *)*mutex);
    *mutex = 0;
}

void __iar_system_Mtxlock(__iar_Rmtx *mutex) /* Lock a system lock */
{
    osMutexAcquire(*(osMutexId_t *)*mutex, osWaitForever);
}

void __iar_system_Mtxunlock(__iar_Rmtx *mutex) /* Unlock a system lock */
{
    osMutexRelease(*(osMutexId_t *)*mutex);
}

void __iar_file_Mtxinit(__iar_Rmtx *mutex) /* Initialize a file lock */
{
    osMutexAttr_t attr;
    uint32_t index;
    for (index = 0; index < _FOPEN_MAX; index++) {
        if (0 == std_mutex_id_file[index]) {
            attr.name = "file_mutex";
            attr.cb_mem = &std_mutex_file[index];
            attr.cb_size = sizeof(std_mutex_file[index]);
            attr.attr_bits = osMutexRecursive | osMutexPrioInherit | osMutexRobust;
            std_mutex_id_file[index] = osMutexNew(&attr);
            *mutex = (__iar_Rmtx *)&std_mutex_id_file[index];
            return;
        }
    }
    /* The variable _FOPEN_MAX needs to be increased */
    error("Not enough mutexes\n");
}

void __iar_file_Mtxdst(__iar_Rmtx *mutex) /* Destroy a file lock */
{
    osMutexDelete(*(osMutexId_t *)*mutex);
    *mutex = 0;
}

void __iar_file_Mtxlock(__iar_Rmtx *mutex) /* Lock a file lock */
{
    osMutexAcquire(*(osMutexId_t *)*mutex, osWaitForever);
}

void __iar_file_Mtxunlock(__iar_Rmtx *mutex) /* Unlock a file lock */
{
    osMutexRelease(*(osMutexId_t *)*mutex);
}
