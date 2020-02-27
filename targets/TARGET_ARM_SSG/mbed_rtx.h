/* mbed Microcontroller Library
 * Copyright (c) 2016-2020 Arm Limited
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

#elif defined(TARGET_MPS2_M0) || defined(TARGET_MPS2_M0P) || \
	      defined(TARGET_MPS2_M3) || defined(TARGET_MPS2_M4) || \
      defined(TARGET_MPS2_M7)

#ifndef INITIAL_SP
#define INITIAL_SP              (0x20400000UL)
#endif

#elif defined(TARGET_CM3DS_MPS2)

#include "memory_zones.h"

#ifndef INITIAL_SP
#define INITIAL_SP              (ZBT_SSRAM23_START + ZBT_SSRAM23_SIZE)
#endif

#elif defined(TARGET_MUSCA_A1) || defined(TARGET_MUSCA_B1)

#if defined(__ARMCC_VERSION)
    extern uint32_t               Image$$ARM_LIB_HEAP$$ZI$$Base[];
    extern uint32_t               Image$$ARM_LIB_HEAP$$ZI$$Length[];
    #define HEAP_START            Image$$ARM_LIB_HEAP$$ZI$$Base
    #define HEAP_SIZE             Image$$ARM_LIB_HEAP$$ZI$$Length
#elif defined(__GNUC__)
    /* No region declarations needed */
#elif defined(__ICCARM__)
    /* No region declarations needed */
#else
    #error "no toolchain defined"
#endif

#endif  /* defined(TARGET_MUSCA_A1) || defined(TARGET_MUSCA_B1) */

#endif  /* MBED_MBED_RTX_H */
