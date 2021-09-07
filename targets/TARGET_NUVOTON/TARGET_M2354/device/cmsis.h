/*
 * Copyright (c) 2020, Nuvoton Technology Corporation
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

#ifndef MBED_CMSIS_H
#define MBED_CMSIS_H

#include "M2354.h"
#include "cmsis_nvic.h"
#if TARGET_TFM
#include "partition_M2354.h"
#include "platform_extra_secure.h"
#endif

// Support linker-generated symbol as start of relocated vector table.
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
extern uint32_t Image$$ER_IRAMVEC$$ZI$$Base;
#elif defined(__ICCARM__)

#elif defined(__GNUC__)
extern uint32_t __start_vector_table__;
#endif

#endif
