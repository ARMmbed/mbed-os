/* mbed Microcontroller Library
 * Copyright (c) 2016 ARM Limited
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

#include <stdint.h>

#if defined(TARGET_RZ_A1H) || defined(TARGET_VK_RZ_A1H) || defined(TARGET_GR_LYCHEE)

#if defined(__CC_ARM)
    extern char Image$$ARM_LIB_STACK$$Base[];
    extern char Image$$ARM_LIB_STACK$$ZI$$Limit[];
    extern char Image$$ARM_LIB_HEAP$$Base[];
    #define ISR_STACK_START       ((unsigned char*)Image$$ARM_LIB_STACK$$Base)
    #define ISR_STACK_SIZE        ((uint32_t)((uint32_t)Image$$ARM_LIB_STACK$$ZI$$Limit - (uint32_t)Image$$ARM_LIB_STACK$$Base))
    #define INITIAL_SP            (Image$$ARM_LIB_STACK$$ZI$$Limit)
    #define HEAP_START            ((unsigned char*)Image$$ARM_LIB_HEAP$$Base)
    #define HEAP_SIZE             ((uint32_t)((uint32_t)ISR_STACK_START - (uint32_t)HEAP_START))
#elif defined(__GNUC__)
    extern uint32_t               __StackTop;
    extern uint32_t               __StackLimit;
    extern uint32_t               __end__;
    #define ISR_STACK_START       ((unsigned char*)&__StackLimit)
    #define ISR_STACK_SIZE        ((uint32_t)((uint32_t)&__StackTop - (uint32_t)&__StackLimit))
    #define INITIAL_SP            (&__StackTop)
    #define HEAP_START            ((unsigned char*)&__end__)
    #define HEAP_SIZE             ((uint32_t)((uint32_t)ISR_STACK_START - (uint32_t)HEAP_START))
#elif defined(__ICCARM__)
    /* No region declarations needed */
#else
    #error "no toolchain defined"
#endif
#endif

#endif  // MBED_MBED_RTX_H
