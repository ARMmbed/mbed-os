/*
 * Copyright (c) 2018-2019, Nuvoton Technology Corporation
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

#include "M2351.h"
#include "stddriver_secure.h"
#include "cmsis_nvic.h"
#include "partition_M2351.h"

/* Check relevant macro has been defined */
#if (! defined(NU_ROM_SIZE_S))
#error("NU_ROM_SIZE_S not defined")
#endif

/* Determine vector table management
 *
 * 1. Non-secure non-PSA/Non-secure PSA
 * 2. Secure non-PSA (TFM_LVL == 0)
 * 3. Secure PSA (TFM_LVL > 0)
 *
 * In Case 1/2, Mbed OS takes over vector table.
 * In Case 3, TFM takes over vector table.
 *
 * If TFM_LVL isn't defined, it is equivalent to TFM_LVL == 0.
 *
 * FIXME: Currently, we still go mbed relocating vector table sequence. When TFM (Secure PSA)
 *        updates to take over NVIC itself, we must disable this sequence:
 *        1. Do not allocate vector RAM (cmsis.h/cmsis_nvic.h/linker files):
 *           Image$$ER_IRAMVEC$$ZI$$Base
 *           __section_begin("IRAMVEC")
 *           __start_vector_table__
 *        2. Secure PSA may go mbed boot sequence. We must leave NVIC_RAM_VECTOR_ADDRESS
 *           undefined to make the mbed_copy_nvic() function dummy (cmsis_nvic.h).
 */

// Support linker-generated symbol as start of relocated vector table.
#if defined(__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050))
extern uint32_t Image$$ER_IRAMVEC$$ZI$$Base;
#elif defined(__ICCARM__)

#elif defined(__GNUC__)
extern uint32_t __start_vector_table__;
#endif

/* TZ_START_NS: Start address of non-secure application */
#ifndef TZ_START_NS
#define TZ_START_NS (NS_OFFSET + NU_ROM_SIZE_S)
#endif

#endif
