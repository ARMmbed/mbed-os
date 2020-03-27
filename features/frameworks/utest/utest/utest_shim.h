
/** \addtogroup frameworks */
/** @{*/
/****************************************************************************
 * Copyright (c) 2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 ****************************************************************************
 */

#ifndef UTEST_SHIM_H
#define UTEST_SHIM_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "utest/utest_scheduler.h"

#ifndef __deprecated_message
#   if defined (__ICCARM__)
#       define __deprecated_message(msg)
#   else
#       define __deprecated_message(msg) __attribute__((deprecated(msg)))
#   endif
#endif

#ifndef UTEST_ENTER_CRITICAL_SECTION
#	define UTEST_ENTER_CRITICAL_SECTION utest_v1_enter_critical_section()
#endif
#ifndef UTEST_LEAVE_CRITICAL_SECTION
#	define UTEST_LEAVE_CRITICAL_SECTION utest_v1_leave_critical_section()
#endif

#ifdef __cplusplus
extern "C" {
#endif

/// must be implemented by the port
void utest_v1_enter_critical_section(void);
void utest_v1_leave_critical_section(void);

/// This is the default scheduler implementation used by the harness.
utest_v1_scheduler_t utest_v1_get_scheduler(void);

#ifdef __cplusplus
}
#endif

#endif // UTEST_SHIM_H

/** @}*/
