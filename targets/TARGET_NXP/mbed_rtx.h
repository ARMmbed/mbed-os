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

#if defined(TARGET_LPC11U68)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x10008000UL)
#endif

#elif defined(TARGET_LPC11U24)        \
     || defined(TARGET_LPC11CXX)  \
     || defined(TARGET_LPC11U35_401)  \
     || defined(TARGET_LPC11U35_501)  \
     || defined(TARGET_LPCCAPPUCCINO)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x10002000UL)
#endif

#elif defined(TARGET_LPC1114)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x10001000UL)
#endif

#elif defined(TARGET_LPC1347)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x10002000UL)
#endif

#elif defined(TARGET_LPC1549)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x02009000UL)
#endif

#elif defined(TARGET_LPC1768) || defined(TARGET_LPC1769)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x10008000UL)
#endif

#elif defined(TARGET_LPC4088) || defined(TARGET_LPC4088_DM)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x10010000UL)
#endif

#elif defined(TARGET_LPC4330) || defined(TARGET_LPC4337)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x10008000UL)
#endif

#elif defined(TARGET_LPC812)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x10001000UL)
#endif

#elif defined(TARGET_LPC824) || defined(TARGET_SSCI824)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x10002000UL)
#endif

#elif defined(TARGET_LPC54114_M4)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x20010000UL)
#endif

#elif defined(TARGET_MCU_LPC546XX)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x20028000UL)
#endif

#elif defined(TARGET_MIMXRT1050_EVK)

#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
    extern uint32_t               Image$$RW_IRAM1$$ZI$$Base[];
    extern uint32_t               Image$$RW_IRAM1$$ZI$$Length[];
    extern uint32_t               Image$$ARM_LIB_STACK$$ZI$$Base[];
    extern uint32_t               Image$$ARM_LIB_STACK$$ZI$$Length[];
    #define HEAP_START            ((unsigned char*) Image$$RW_IRAM1$$ZI$$Base)
    #define HEAP_SIZE             ((uint32_t) Image$$RW_IRAM1$$ZI$$Length)
    #define ISR_STACK_START       ((unsigned char*)Image$$ARM_LIB_STACK$$ZI$$Base)
    #define ISR_STACK_SIZE        ((uint32_t)Image$$ARM_LIB_STACK$$ZI$$Length)
#elif defined(__GNUC__)
    extern uint32_t               __StackTop[];
    extern uint32_t               __StackLimit[];
    extern uint32_t               __end__;
    extern uint32_t               __HeapLimit[];
    #define HEAP_START            ((unsigned char*)&__end__)
    #define HEAP_SIZE             ((uint32_t)((uint32_t)__HeapLimit - (uint32_t)HEAP_START))
    #define ISR_STACK_START       ((unsigned char*)__StackLimit)
    #define ISR_STACK_SIZE        ((uint32_t)((uint32_t)__StackTop - (uint32_t)__StackLimit))
#elif defined(__ICCARM__)
    /* No region declarations needed */
#else
    #error "no toolchain defined"
#endif

#endif

#endif  // MBED_MBED_RTX_H
