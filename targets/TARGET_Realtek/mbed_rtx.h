/* mbed Microcontroller Library
 * Copyright (c) 2013-2016 Realtek Semiconductor Corp.
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
#ifndef MBED_MBED_RTX_H
#define MBED_MBED_RTX_H

#if defined(TARGET_RTL8195A)

#include "rtl8195a.h"

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
    extern uint32_t             Image$$ARM_LIB_STACK$$ZI$$Base[];
    extern uint32_t             Image$$ARM_LIB_STACK$$ZI$$Length[];
    #define ISR_STACK_START     (unsigned char *)(Image$$ARM_LIB_STACK$$ZI$$Base)
    #define ISR_STACK_SIZE      (uint32_t)(Image$$ARM_LIB_STACK$$ZI$$Length)
    #define INITIAL_SP          (uint32_t)(Image$$ARM_LIB_STACK$$ZI$$Base)
#elif defined(__GNUC__)
    extern uint32_t             __StackTop[];
    extern uint32_t             __StackLimit[];
    extern uint32_t             __HeapLimit[];
    #define INITIAL_SP          (__StackTop)
#endif

#if defined(__GNUC__)
#ifndef ISR_STACK_SIZE
#define ISR_STACK_SIZE          (0x1000)
#endif
#endif

#endif
#endif
