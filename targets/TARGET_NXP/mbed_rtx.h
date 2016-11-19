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

#if defined(TARGET_LPC11U68)

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
#define OS_CLOCK                48000000
#endif

#elif defined(TARGET_LPC11U24)        \
     || defined(TARGET_LPC11U35_401)  \
     || defined(TARGET_LPC11U35_501)  \
     || defined(TARGET_LPCCAPPUCCINO)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x10002000UL)
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

#elif defined(TARGET_LPC1114)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x10001000UL)
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

#elif defined(TARGET_LPC1347)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x10002000UL)
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

#elif defined(TARGET_LPC1549)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x02009000UL)
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

#elif defined(TARGET_LPC1768)

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

#elif defined(TARGET_LPC2368)

/* FIXME: INITIAL_SP is undefined for this target */
#ifndef OS_TASKCNT
#define OS_TASKCNT              14
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          256
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                96000000
#endif
#ifndef OS_SCHEDULERSTKSIZE
#define OS_SCHEDULERSTKSIZE    (136*2)
#endif

#elif defined(TARGET_LPC2460)

extern unsigned char            __usr_stack_top__[];
#ifndef INITIAL_SP
#define INITIAL_SP              (__usr_stack_top__)
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
#ifndef OS_SCHEDULERSTKSIZE
#define OS_SCHEDULERSTKSIZE     (136*2)
#endif

#elif defined(TARGET_LPC4088) || defined(TARGET_LPC4088_DM)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x10010000UL)
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

#elif defined(TARGET_LPC4330) || defined(TARGET_LPC4337)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x10008000UL)
#endif
#define OS_TASKCNT              14
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          256
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                204000000
#endif

#elif defined(TARGET_LPC812)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x10001000UL)
#endif
#ifndef OS_TASKCNT
#define OS_TASKCNT              6
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          128
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                36000000
#endif

#elif defined(TARGET_LPC824) || defined(TARGET_SSCI824)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x10002000UL)
#endif
#ifndef OS_TASKCNT
#define OS_TASKCNT              6
#endif
#ifndef OS_MAINSTKSIZE
#define OS_MAINSTKSIZE          128
#endif
#ifndef OS_CLOCK
#define OS_CLOCK                30000000
#endif

#endif

#endif  // MBED_MBED_RTX_H
