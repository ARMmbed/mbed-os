/* Copyright (c) 2017-2018 ARM Limited
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

#ifndef __MBED_SPM_PANIC_H__
#define __MBED_SPM_PANIC_H__

/** @addtogroup SPM
 * @{
 */

#include "mbed_toolchain.h"
#include "mbed_assert.h"
#include "mbed_error.h"


#ifdef __cplusplus
extern "C" {
#endif


#ifdef SPM_SPE_BUILD

#error "Trying to build for SPE with SPM_SPE_BUILD flag - Not supported"

#else

/**
 * Generate a system panic
 *
 * @param[in] format The format string to output on panic
 * @param[in] ...    (Additional arguments) Depending on the format string
 */
#define SPM_PANIC(format, ...)  error("%s %u: " format, __func__, __LINE__, ##__VA_ARGS__)


/**
 * Assert on condition (debug build only)
 *
 * @param[in] expr Condition to be asserted
 */
#define SPM_ASSERT(expr) MBED_ASSERT(expr)

#endif


#ifdef __cplusplus
}
#endif

/** @}*/ // end of SPM group

#endif  // __MBED_SPM_PANIC_H__
