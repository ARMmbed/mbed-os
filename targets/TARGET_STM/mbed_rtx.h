/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#ifndef INITIAL_SP

#if (defined(TARGET_STM32L475VG) ||\
     defined(TARGET_STM32L476RG) ||\
     defined(TARGET_STM32L476JG) ||\
     defined(TARGET_STM32L476VG) ||\
     defined(TARGET_STM32L486RG) ||\
     defined(TARGET_STM32L471QG))
/* only GCC_ARM and IAR toolchains have the stack on SRAM2 */
#if (((defined(__GNUC__) && !defined(__CC_ARM)) ||\
       defined(__IAR_SYSTEMS_ICC__ )) &&\
       defined(TWO_RAM_REGIONS))
#define INITIAL_SP              (0x10008000UL)
#else
#define INITIAL_SP              (0x20018000UL)
#endif /* toolchains */

#elif (defined(TARGET_STM32F051R8) ||\
       defined(TARGET_STM32F100RB) ||\
       defined(TARGET_STM32L031K6) ||\
       defined(TARGET_STM32L053C8) ||\
       defined(TARGET_STM32L053R8))
#define INITIAL_SP              (0x20002000UL)

#elif (defined(TARGET_STM32F303K8) ||\
       defined(TARGET_STM32F334C8) ||\
       defined(TARGET_STM32F334R8))
#define INITIAL_SP              (0x20003000UL)

#elif (defined(TARGET_STM32F070RB) ||\
       defined(TARGET_STM32F072RB) ||\
       defined(TARGET_STM32F302R8))
#define INITIAL_SP              (0x20004000UL)

#elif (defined(TARGET_STM32F103RB) ||\
       defined(TARGET_STM32F103C8) ||\
       defined(TARGET_STM32L072CZ) ||\
       defined(TARGET_STM32L073RZ) ||\
       defined(TARGET_STM32L0x2xZ))
#define INITIAL_SP              (0x20005000UL)

#elif (defined(TARGET_STM32F091RC) ||\
       defined(TARGET_STM32F410RB) ||\
       defined(TARGET_STM32L151CBA)||\
       defined(TARGET_STM32L151CC) ||\
       defined(TARGET_STM32L151RC) ||\
       defined(TARGET_STM32L152RC))
#define INITIAL_SP              (0x20008000UL)

#elif defined(TARGET_STM32F303VC)
#define INITIAL_SP              (0x2000A000UL)

#elif defined(TARGET_STM32L443RC)
#define INITIAL_SP              (0x2000C000UL)

#elif (defined(TARGET_STM32F303RE) ||\
       defined(TARGET_STM32F303ZE) ||\
       defined(TARGET_STM32F401VC) ||\
       defined(TARGET_STM32L432KC) ||\
       defined(TARGET_STM32L433RC))
#define INITIAL_SP              (0x20010000UL)

#elif defined(TARGET_STM32L152RE)
#define INITIAL_SP              (0x20014000UL)

#elif (defined(TARGET_STM32F401RE) ||\
       defined(TARGET_STM32F401VE))
#define INITIAL_SP              (0x20018000UL)

#elif (defined(TARGET_STM32F207ZG) ||\
       defined(TARGET_STM32F405RG) ||\
       defined(TARGET_STM32F407VG) ||\
       defined(TARGET_STM32F407VE) ||\
       defined(TARGET_STM32F411RE) ||\
       defined(TARGET_STM32F446RE) ||\
       defined(TARGET_STM32F446VE) ||\
       defined(TARGET_STM32F446ZE) ||\
       defined(TARGET_STM32H743xI) ||\
       defined(TARGET_STM32H753xI))
#define INITIAL_SP              (0x20020000UL)

#elif (defined(TARGET_STM32F429ZI) ||\
       defined(TARGET_STM32F437VG) ||\
       defined(TARGET_STM32F439VI) ||\
       defined(TARGET_STM32F439ZI) ||\
       defined(TARGET_STM32WB55RG))
#define INITIAL_SP              (0x20030000UL)

#elif defined(TARGET_STM32F412ZG)
#define INITIAL_SP              (0x20040000UL)

#elif (defined(TARGET_STM32F413ZH) ||\
       defined(TARGET_STM32F469NI) ||\
       defined(TARGET_STM32F746NG) ||\
       defined(TARGET_STM32F746ZG) ||\
       defined(TARGET_STM32F756ZG) ||\
       defined(TARGET_STM32L496AG) ||\
       defined(TARGET_STM32L496ZG))
#define INITIAL_SP              (0x20050000UL)

#elif (defined(TARGET_STM32F767ZI) ||\
       defined(TARGET_STM32F767VI) ||\
       defined(TARGET_STM32F769NI))
#define INITIAL_SP              (0x20080000UL)

#elif defined(TARGET_STM32L4R5xI)
#define INITIAL_SP              (0x200A0000UL)

#elif defined(TARGET_STM32L4R9xI)
#define INITIAL_SP              (0x200A0000UL)

#else
#error "INITIAL_SP is not defined for this target in the mbed_rtx.h file"
#endif

#endif // INITIAL_SP

#if (defined(TARGET_STM32F070RB) || defined(TARGET_STM32F072RB))
#ifdef MBED_CONF_RTOS_MAIN_THREAD_STACK_SIZE
#undef MBED_CONF_RTOS_MAIN_THREAD_STACK_SIZE
#endif
#define MBED_CONF_RTOS_MAIN_THREAD_STACK_SIZE 3072
#endif

#if (defined(TARGET_STM32L475VG) || defined(TARGET_STM32L443RC))
#if defined(__ARMCC_VERSION)
extern uint32_t               Image$$ARM_LIB_HEAP$$ZI$$Base[];
extern uint32_t               Image$$ARM_LIB_HEAP$$ZI$$Length[];
#define HEAP_START            Image$$ARM_LIB_HEAP$$ZI$$Base
#define HEAP_SIZE             Image$$ARM_LIB_HEAP$$ZI$$Length
#endif
#endif

#endif  // MBED_MBED_RTX_H
