/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

#if (defined(TARGET_STM32L475VG) || defined(TARGET_STM32L443RC))
#if defined(__ARMCC_VERSION)
extern uint32_t               Image$$ARM_LIB_HEAP$$ZI$$Base[];
extern uint32_t               Image$$ARM_LIB_HEAP$$ZI$$Length[];
#define HEAP_START            Image$$ARM_LIB_HEAP$$ZI$$Base
#define HEAP_SIZE             Image$$ARM_LIB_HEAP$$ZI$$Length
#endif
#endif

#endif  // MBED_MBED_RTX_H
