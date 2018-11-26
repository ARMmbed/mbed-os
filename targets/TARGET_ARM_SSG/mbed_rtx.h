/* mbed Microcontroller Library
 * Copyright (c) 2016-2018 Arm Limited
 *
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

#ifndef MBED_MBED_RTX_H
#define MBED_MBED_RTX_H

#include <stdint.h>

#if defined(TARGET_BEETLE)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x20020000UL)
#endif

#elif defined(TARGET_CM3DS_MPS2)

#include "memory_zones.h"

#ifndef INITIAL_SP
#define INITIAL_SP              (ZBT_SSRAM23_START + ZBT_SSRAM23_SIZE)
#endif

#elif defined(TARGET_MUSCA_A1)

/******************** ARMC ********************/
#if defined (__CC_ARM) || (defined (__ARMCC_VERSION) && \
   (__ARMCC_VERSION >= 6010050))
#ifndef INITIAL_SP
extern const uint32_t  __initial_sp;
#define INITIAL_SP     ((uint32_t)&__initial_sp)
#endif

/******************** GCC ********************/
#elif defined (__GNUC__)
#ifndef INITIAL_SP
extern const uint32_t __StackTop; /* Defined in the linker script */
#define INITIAL_SP    ((uint32_t)&__StackTop)
#endif

#endif

#endif  /* TARGET_MUSCA_A1 */

#endif  /* MBED_MBED_RTX_H */

