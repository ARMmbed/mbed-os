/* mbed Microcontroller Library
 *******************************************************************************
 * Copyright (c) 2019, STMicroelectronics
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *
 * You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 *
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 *
 * either express or implied.
 *
 * See the License for the specific language governing permissions and limitations under the License.
 */

#if MBED_MBED_RTX_H

#include <stdint.h>

#ifndef INITIAL_SP

#if (defined(TARGET_BLUENRG1))
#define INITIAL_SP              (0x20006000UL)

#elif (defined(TARGET_BLUENRG2))
#define INITIAL_SP              (0x20006000UL)  //TBC

#else
#error "INITIAL_SP is not defined for this target in the mbed_rtx.h file"
#endif

#endif // INITIAL_SP

#if (defined(TARGET_BLUENRG2))
#if (defined(__GNUC__) && !defined(__CC_ARM) && !defined(__ARMCC_VERSION))
extern uint32_t               __StackLimit;
extern uint32_t               __StackTop;
extern uint32_t               __end__;
extern uint32_t               __HeapLimit;
#define HEAP_START            ((unsigned char*) &__end__)
#define HEAP_SIZE             ((uint32_t)((uint32_t) &__HeapLimit - (uint32_t) HEAP_START))
#define ISR_STACK_START       ((unsigned char*) &__StackLimit)
#define ISR_STACK_SIZE        ((uint32_t)((uint32_t) &__StackTop - (uint32_t) &__StackLimit))
#endif

#ifdef MBED_CONF_RTOS_MAIN_THREAD_STACK_SIZE
#undef MBED_CONF_RTOS_MAIN_THREAD_STACK_SIZE
#endif
#define MBED_CONF_RTOS_MAIN_THREAD_STACK_SIZE 3072  //da rivedere

#endif

#endif // MBED_MBED_RTX_H
