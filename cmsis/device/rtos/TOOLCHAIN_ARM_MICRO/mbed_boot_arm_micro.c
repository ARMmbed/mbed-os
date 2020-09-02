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
#include <stdint.h>

#include "mbed_boot.h"
#include "mbed_assert.h"

extern uint32_t               Image$$ARM_LIB_STACK$$ZI$$Base[];
extern uint32_t               Image$$ARM_LIB_STACK$$ZI$$Length[];

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
