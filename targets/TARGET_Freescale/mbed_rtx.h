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

#if defined(TARGET_K20D50M)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x10008000UL)
#endif
#ifndef OS_TASKCNT
#define OS_TASKCNT              14
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          256
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                96000000
#endif

#elif defined(TARGET_TEENSY3_1)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x20008000UL)
#endif
#ifndef OS_TASKCNT
#define OS_TASKCNT              14
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          256
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                96000000
#endif

#elif defined(TARGET_MCU_K22F)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x20010000UL)
#endif
#ifndef OS_TASKCNT
#define OS_TASKCNT              14
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          256
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                80000000
#endif

#elif defined(TARGET_K66F)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x20030000UL)
#endif
#ifndef OS_TASKCNT
#define OS_TASKCNT              14
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          256
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                120000000
#endif

#elif defined(TARGET_KL27Z)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x20003000UL)
#endif
#ifndef OS_TASKCNT
#define OS_TASKCNT              6
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          128
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                48000000
#endif

#elif defined(TARGET_KL43Z)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x20006000UL)
#endif
#ifndef OS_TASKCNT
#define OS_TASKCNT              14
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          256
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                48000000
#endif

#elif defined(TARGET_KL05Z)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x20000C00UL)
#endif
#ifndef OS_TASKCNT
#define OS_TASKCNT              6
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          128
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                48000000
#endif

#elif defined(TARGET_KL25Z)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x20003000UL)
#endif
#ifndef OS_TASKCNT
#define OS_TASKCNT              6
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          128
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                48000000
#endif

#elif defined(TARGET_KL26Z)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x20003000UL)
#endif
#ifndef OS_TASKCNT
#define OS_TASKCNT              6
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          128
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                48000000
#endif

#elif defined(TARGET_KL46Z)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x20006000UL)
#endif
#ifndef OS_TASKCNT
#define OS_TASKCNT              14
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          256
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                48000000
#endif

#elif defined(TARGET_KL82Z)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x20012000UL)
#endif
#ifndef OS_TASKCNT
#define OS_TASKCNT              14
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          256
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                72000000
#endif

#elif defined(TARGET_K64F)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x20030000UL)
#endif

#if defined(__CC_ARM) || defined(__GNUC__)
#define ISR_STACK_SIZE          (0x1000)
#endif

#ifndef OS_TASKCNT
#define OS_TASKCNT              14
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          256
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                120000000
#endif

#elif defined(TARGET_KW24D)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x20008000UL)
#endif
#ifndef OS_TASKCNT
#define OS_TASKCNT              14
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          256
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                48000000
#endif

#elif defined(TARGET_KW41Z)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x20018000UL)
#endif
#ifndef OS_TASKCNT
#define OS_TASKCNT              14
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          256
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                40000000
#endif

#elif defined(TARGET_K82F)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x20030000UL)
#endif
#ifndef OS_TASKCNT
#define OS_TASKCNT              14
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          256
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                120000000
#endif

#endif

#endif  // MBED_MBED_RTX_H
