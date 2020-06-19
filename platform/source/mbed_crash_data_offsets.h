/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#ifndef MBED_CRASH_DATA_INFO_H
#define MBED_CRASH_DATA_INFO_H

#include "platform/internal/mbed_fault_handler.h"
#include "platform/mbed_error.h"

#ifdef __cplusplus
extern "C" {
#endif

#if MBED_CONF_PLATFORM_CRASH_CAPTURE_ENABLED
/** \ingroup mbed-os-internal */
/** \addtogroup platform-internal-api */
/** @{*/
// Any changes here must be reflected in except.S if they affect the fault handler.
// The fault context is first to keep it simple for the assembler.
typedef struct mbed_crash_data {
    union {
        mbed_fault_context_t context;
        int pad[32];
    } fault;
    union {
        mbed_error_ctx context;
        int pad[32];
    } error;
} mbed_crash_data_t;

#if defined(__ARMCC_VERSION)
#define MBED_CRASH_DATA Image$$RW_m_crash_data$$ZI$$Base
#elif defined(__ICCARM__)
#define MBED_CRASH_DATA __CRASH_DATA_RAM_START__
#elif defined(__GNUC__)
#define MBED_CRASH_DATA __CRASH_DATA_RAM_START__
#endif

extern mbed_crash_data_t MBED_CRASH_DATA;
/**@}*/
#endif

#ifdef __cplusplus
}
#endif

#endif
