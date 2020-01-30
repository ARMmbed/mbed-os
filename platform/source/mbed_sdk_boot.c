/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#include "mbed_toolchain.h"
#include <stdlib.h>
#include <stdint.h>
#include "cmsis.h"
#include "hal/us_ticker_api.h"

/* This startup is for baremetal. There is no RTOS in baremetal,
 * therefore we protect this file with MBED_CONF_RTOS_PRESENT.
 * Note: The start-up code for mbed OS is in rtos/source/TARGET_CORTEX/mbed_boot code file.
 */
#if !defined(MBED_CONF_RTOS_PRESENT)

/* Heap limits - only used if set */
extern unsigned char *mbed_heap_start;
extern uint32_t mbed_heap_size;

/* Stack limits */
unsigned char *mbed_stack_isr_start = 0;
uint32_t mbed_stack_isr_size = 0;

/* mbed_main is a function that is called before main()
 * mbed_sdk_init() is also a function that is called before main(), but unlike
 * mbed_main(), it is not meant for user code, but for the SDK itself to perform
 * initializations before main() is called.
 */
MBED_WEAK void mbed_main(void)
{

}

/* This function can be implemented by the target to perform higher level target initialization
 */
MBED_WEAK void mbed_sdk_init(void)
{

}

MBED_WEAK void software_init_hook_rtos()
{
    // Nothing by default
}

void mbed_copy_nvic(void)
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

void mbed_init(void)
{
#ifdef MBED_DEBUG
    // Configs to make debugging easier
#ifdef SCnSCB_ACTLR_DISDEFWBUF_Msk
    // Disable write buffer to make BusFaults (eg write to ROM via NULL pointer) precise.
    // Possible on Cortex-M3 and M4, not on M0, M7 or M33.
    // Would be less necessary if ROM was write-protected in MPU to give a
    // precise MemManage exception.
    SCnSCB->ACTLR |= SCnSCB_ACTLR_DISDEFWBUF_Msk;
#endif
#endif
    mbed_copy_nvic();
    mbed_sdk_init();
#if DEVICE_USTICKER && MBED_CONF_TARGET_INIT_US_TICKER_AT_BOOT
    us_ticker_init();
#endif
}

/* Toolchain specific main code */

#if defined (__ARMCC_VERSION)

void $Sub$$__cpp_initialize__aeabi_(void);
void $Super$$__cpp_initialize__aeabi_(void);
void $Sub$$__cpp_initialize__aeabi_(void)
{
    /* This should invoke C++ initializers but we keep
     * this empty and invoke them in mbed_toolchain_init.
     */
}

void mbed_toolchain_init()
{
    /* Run the C++ global object constructors */
    $Super$$__cpp_initialize__aeabi_();
}

void _platform_post_stackheap_init(void)
{
    mbed_init();
}

extern uint32_t               Image$$ARM_LIB_STACK$$ZI$$Base[];
extern uint32_t               Image$$ARM_LIB_STACK$$ZI$$Length[];
extern uint32_t               Image$$ARM_LIB_HEAP$$ZI$$Base[];
extern uint32_t               Image$$ARM_LIB_HEAP$$ZI$$Length[];

int $Super$$main(void);
int $Sub$$main(void)
{
    mbed_stack_isr_start = (unsigned char *) Image$$ARM_LIB_STACK$$ZI$$Base;
    mbed_stack_isr_size = (uint32_t) Image$$ARM_LIB_STACK$$ZI$$Length;
    mbed_heap_start = (unsigned char *) Image$$ARM_LIB_HEAP$$ZI$$Base;
    mbed_heap_size = (uint32_t) Image$$ARM_LIB_HEAP$$ZI$$Length;

#if defined(__MICROLIB)
    // post stack/heap is not active in microlib
    // so call the function explicitly.
    _platform_post_stackheap_init();
#endif
    mbed_toolchain_init();
    mbed_main();
    mbed_error_initialize();
    return $Super$$main();
}




//Define an empty os_cb_sections to remove a RTX warning when building with no RTOS due
//to the --keep=os_cb_sections linker option
const uint32_t os_cb_sections[] __attribute__((section(".rodata"))) = {};

#elif defined (__GNUC__)

extern uint32_t             __StackLimit;
extern uint32_t             __StackTop;
extern uint32_t             __end__;
extern uint32_t             __HeapLimit;

extern int __real_main(void);

void software_init_hook(void)
{
    mbed_stack_isr_start = (unsigned char *) &__StackLimit;
    mbed_stack_isr_size = (uint32_t) &__StackTop - (uint32_t) &__StackLimit;
    mbed_heap_start = (unsigned char *) &__end__;
    mbed_heap_size = (uint32_t) &__HeapLimit - (uint32_t) &__end__;

    mbed_init();
    software_init_hook_rtos();
}


int __wrap_main(void)
{
    mbed_main();
    mbed_error_initialize();
    return __real_main();
}

#elif defined (__ICCARM__)

/* Defined by linker script */
#pragma section="CSTACK"
#pragma section="HEAP"

void __mbed_init(void)
{
    mbed_heap_start = (unsigned char *)__section_begin("HEAP");
    mbed_heap_size = (uint32_t)__section_size("HEAP");

    mbed_stack_isr_start = (unsigned char *)__section_begin("CSTACK");
    mbed_stack_isr_size = (uint32_t)__section_size("CSTACK");

    mbed_init();
    mbed_error_initialize();
}

#endif

#endif
