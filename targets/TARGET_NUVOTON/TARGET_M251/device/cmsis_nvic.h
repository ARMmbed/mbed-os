/*
 * Copyright (c) 2019, Nuvoton Technology Corporation
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

#ifndef MBED_CMSIS_NVIC_H
#define MBED_CMSIS_NVIC_H

#include "cmsis.h"

#define NVIC_USER_IRQ_OFFSET 16
#define NVIC_USER_IRQ_NUMBER 64
#define NVIC_NUM_VECTORS     (NVIC_USER_IRQ_OFFSET + NVIC_USER_IRQ_NUMBER)

#if defined(__ARMCC_VERSION)
#   define NVIC_RAM_VECTOR_ADDRESS  ((uint32_t) &Image$$ER_IRAMVEC$$ZI$$Base)
#elif defined(__ICCARM__)
#   pragma section = "IRAMVEC"
#   define NVIC_RAM_VECTOR_ADDRESS  ((uint32_t) __section_begin("IRAMVEC"))
#elif defined(__GNUC__)
#   define NVIC_RAM_VECTOR_ADDRESS  ((uint32_t) &__start_vector_table__)
#endif

#endif
