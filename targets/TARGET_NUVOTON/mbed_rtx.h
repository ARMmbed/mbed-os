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

#if defined(TARGET_NUMAKER_PFM_NUC472)

#ifndef OS_TASKCNT
#define OS_TASKCNT              14
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          256
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                84000000
#endif

#if defined(__CC_ARM)
    extern uint32_t               Image$$ARM_LIB_HEAP$$ZI$$Base[];
    extern uint32_t               Image$$ARM_LIB_HEAP$$ZI$$Length[];
    extern uint32_t               Image$$ARM_LIB_STACK$$ZI$$Base[];
    extern uint32_t               Image$$ARM_LIB_STACK$$ZI$$Length[];
    #define HEAP_START            ((unsigned char*) Image$$ARM_LIB_HEAP$$ZI$$Base)
    #define HEAP_SIZE             ((uint32_t) Image$$ARM_LIB_HEAP$$ZI$$Length)
    #define ISR_STACK_START       ((unsigned char*)Image$$ARM_LIB_STACK$$ZI$$Base)
    #define ISR_STACK_SIZE        ((uint32_t)Image$$ARM_LIB_STACK$$ZI$$Length)
#elif defined(__GNUC__)
    extern uint32_t               __StackTop[];
    extern uint32_t               __StackLimit[];
    extern uint32_t               __end__[];
    extern uint32_t               __HeapLimit[];
    #define HEAP_START            ((unsigned char*)__end__)
    #define HEAP_SIZE             ((uint32_t)((uint32_t)__HeapLimit - (uint32_t)HEAP_START))
    #define ISR_STACK_START       ((unsigned char*)__StackLimit)
    #define ISR_STACK_SIZE        ((uint32_t)((uint32_t)__StackTop - (uint32_t)__StackLimit))
#elif defined(__ICCARM__)
    /* No region declarations needed */
#else
    #error "no toolchain defined"
#endif

#elif defined(TARGET_NUMAKER_PFM_M453)

#ifndef OS_TASKCNT
#define OS_TASKCNT              14
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          256
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                72000000
#endif

#if defined(__CC_ARM)
    extern uint32_t               Image$$ARM_LIB_HEAP$$ZI$$Base[];
    extern uint32_t               Image$$ARM_LIB_HEAP$$ZI$$Length[];
    extern uint32_t               Image$$ARM_LIB_STACK$$ZI$$Base[];
    extern uint32_t               Image$$ARM_LIB_STACK$$ZI$$Length[];
    #define HEAP_START            ((unsigned char*) Image$$ARM_LIB_HEAP$$ZI$$Base)
    #define HEAP_SIZE             ((uint32_t) Image$$ARM_LIB_HEAP$$ZI$$Length)
    #define ISR_STACK_START       ((unsigned char*)Image$$ARM_LIB_STACK$$ZI$$Base)
    #define ISR_STACK_SIZE        ((uint32_t)Image$$ARM_LIB_STACK$$ZI$$Length)
#elif defined(__GNUC__)
    extern uint32_t               __StackTop[];
    extern uint32_t               __StackLimit[];
    extern uint32_t               __end__[];
    extern uint32_t               __HeapLimit[];
    #define HEAP_START            ((unsigned char*)__end__)
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
