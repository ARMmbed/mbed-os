/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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
#ifndef MBED_BOOT_H
#define MBED_BOOT_H

#include "mbed_toolchain.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "mbed_rtx.h"

/** \addtogroup rtos-internal-api */
/** @{*/

/**
 * \defgroup boot Boot sequence
 * Boot sequence overview
 *
 * 1. Setup target
 *      - Configure clocks
 *      - Configure watchdog (if applicable)
 *      - Turn on RAM (if applicable)
 *      - Jump to setup toolchain
 * 2. Setup toolchain
 *      - Initialize RAM
 *      - Initialize standard library
 *      - Call mbed_init
 *      - jump to start rtos
 * 3. Start RTOS
 *      - Create main thread
 *      - Start scheduler
 *      - main thread calls start mbed
 * 4. Start mbed
 *      - Call mbed_main
 *      - Call main
 *
 * @{
 */

/* Heap limits - only used if set */
extern unsigned char *mbed_heap_start;
extern uint32_t mbed_heap_size;

/* Stack limits */
extern unsigned char *mbed_stack_isr_start;
extern uint32_t mbed_stack_isr_size;

/**
 * Perform low level init of mbed
 *
 * The toolchain calls this function as part of the boot
 * sequence. This function does the following:
 * - Sets up NVIC so interrupts can be used
 * - Calls into the vendor SDK so it can be used
 * - Initializes the RTOS so it can be used
 *
 * Preconditions:
 * - Target is initialized
 * - Ram is initialized
 * - value set for mbed_heap_start
 * - value set for mbed_heap_size
 * - value set for mbed_stack_isr_start
 * - value set for mbed_stack_isr_size
 *
 * The following events must not have happened yet:
 * - global constructors must not be called yet
 *
 */
void mbed_init(void);

/**
 * Start the main mbed application
 *
 * This is the last stage of the boot sequence. This function must be
 * called only after the RTOS has been fully initialized.
 * This function does the following:
 * - Initialize the toolchain
 * - Run mbed_main
 * - Run main
 *
 * Preconditions:
 * - The RTOS has been started by a call to mbed_rtos_start
 *
 */
void mbed_start(void);

/**
 * Perform low level initialization of the RTOS
 *
 * Set the RTOS to a known state but don't start the scheduler. After
 * the RTOS has been initialized it is safe to create RTOS primitives
 * for file locks or other purposes.
 *
 * Preconditions:
 * - Ram is initialized
 * - NVIC is setup
 * - Vendor SDK must be initialized by a call to mbed_sdk_init
 */
void mbed_rtos_init(void);

/**
 * Start the RTOS
 *
 * Start the RTOS scheduler and call mbed_start on the
 * main thread. This function does not return.
 *
 * Preconditions:
 * - RTOS has been initialized by a call to mbed_rtos_init
 */
MBED_NORETURN void mbed_rtos_start(void);

/**
 * Perform toolchain specific initialization
 *
 * Initialize locks if this has not been done already
 * and call global C++ constructors.
 *
 * Preconditions:
 * - The RTOS has been started by a call to mbed_rtos_start
 */
void mbed_toolchain_init(void);

/**
 * SDK hook for running code before ctors or OS
 *
 * This is a weak function which can be overridden by a target's
 * SDK to allow code to run after ram is initialized but before
 * the OS has been started or constructors have run.
 *
 * Preconditions:
 * - Ram is initialized
 * - NVIC is setup
 */
void mbed_sdk_init(void);

/**
 * Application hook for running code before main
 *
 * This is a weak function which can be overridden by an application
 * to allow code to run before main is called.
 *
 * Preconditions:
 * - The RTOS has been started by a call to mbed_rtos_start
 * - The toolchain has been initialized by a call to mbed_toolchain_init
 */
void mbed_main(void);

/**@}*/
/**@}*/

#ifdef __cplusplus
}
#endif

#endif /* MBED_BOOT_H */
