
/** \addtogroup frameworks */
/** @{*/
/****************************************************************************
 * Copyright (c) 2015, ARM Limited, All Rights Reserved
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

#ifndef UTEST_UNITY_ASSERT_FAILURE_H
#define UTEST_UNITY_ASSERT_FAILURE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/// this function is called from the unity module when an assertion failed.
void utest_unity_assert_failure(void);

/// this function is called from the unity module when an assertion failed, but is ignored.
void utest_unity_ignore_failure(void);

void utest_safe_putc(int chr);

#ifdef __cplusplus
}
#endif

#endif // UTEST_UNITY_ASSERT_FAILURE_H

/** @}*/
