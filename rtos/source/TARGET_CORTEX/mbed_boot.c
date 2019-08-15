/* mbed Microcontroller Library
 * Copyright (c) 2006-2016 ARM Limited
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
 * IAR Memory layout :
 * - Heap defined by "HEAP" region in .icf file
 * - Interrupt stack defined by "CSTACK" region in .icf file
 * - Value INITIAL_SP is ignored
 *
 * GCC Memory layout :
 * - Heap explicitly placed in linker script (*.ld file) and heap start (__end___) and heap end (__HeapLimit) should be defined in linker script
 * - Interrupt stack placed in linker script **.ld file) and stack start (__StackTop) and stack end (__StackLimit) should be defined in linker script
 *
 * ARM Memory layout :
 * - Heap can be explicitly placed by adding ARM_LIB_HEAP section in scatter file and defining both HEAP_START and HEAP_SIZE
 * - Interrupt stack placed in scatter files (*.sct) by adding ARM_LIB_STACK section
 *
 */

#include <stdlib.h>

#include "cmsis.h"
#include "hal/us_ticker_api.h"
#include "mbed_toolchain.h"
#include "mbed_boot.h"
#include "mbed_error.h"
#include "mbed_mpu_mgmt.h"

int main(void);
static void mbed_cpy_nvic(void);

/* Stack limits */
unsigned char *mbed_stack_isr_start = 0;
uint32_t mbed_stack_isr_size = 0;

void mbed_init(void)
{
    mbed_mpu_manager_init();
    mbed_cpy_nvic();
    mbed_sdk_init();
#if DEVICE_USTICKER && MBED_CONF_TARGET_INIT_US_TICKER_AT_BOOT
    us_ticker_init();
#endif
    mbed_rtos_init();
}

void mbed_start(void)
{
    mbed_toolchain_init();
    mbed_main();
    mbed_error_initialize();
    main();
}

MBED_WEAK void mbed_sdk_init(void)
{
    // Nothing by default
}

MBED_WEAK void software_init_hook_rtos()
{
    // Nothing by default
}

MBED_WEAK void mbed_main(void)
{
    // Nothing by default
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
    uint32_t *vectors = (uint32_t *)NVIC_RAM_VECTOR_ADDRESS;
    for (int i = 0; i < NVIC_NUM_VECTORS; i++) {
        vectors[i] = old_vectors[i];
    }
    SCB->VTOR = (uint32_t)NVIC_RAM_VECTOR_ADDRESS;
#endif /* NVIC_RAM_VECTOR_ADDRESS */
#endif /* !defined(__CORTEX_M0) && !defined(__CORTEX_A9) */
}
