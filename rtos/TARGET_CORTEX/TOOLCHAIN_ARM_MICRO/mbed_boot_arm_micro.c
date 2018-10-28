/* mbed Microcontroller Library
 * Copyright (c) 2018-2018 ARM Limited
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

#include "mbed_boot.h"
#include "mbed_assert.h"

/* Symbols that are typically defined in startup_<mcu>.S */
extern uint32_t __initial_sp[];
extern uint32_t __heap_base[];
extern uint32_t __heap_limit[];

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

    mbed_heap_start = (unsigned char*)__heap_base;
    mbed_heap_size = (uint32_t)__heap_base - (uint32_t)__heap_limit;

    mbed_stack_isr_start = (unsigned char*)((uint32_t)__initial_sp - ISR_STACK_SIZE);
    mbed_stack_isr_size = ISR_STACK_SIZE;

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

MBED_WEAK void __aeabi_assert(const char *expr, const char *file, int line)
{
    mbed_assert_internal(expr, file, line);
}
