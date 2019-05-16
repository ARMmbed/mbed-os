/* mbed Microcontroller Library
 * Copyright (c) 2019-2019 ARM Limited
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

/** \addtogroup hal_rtc_tests
 *  @{
 */

#ifndef MBED_STACK_SIZE_UNIFICATION_H
#define MBED_STACK_SIZE_UNIFICATION_H

#ifdef __cplusplus
extern "C" {
#endif

/** Test sizes of ISR stack, main thread stack, default user thread stack.
 *
 *  Given is Mbed OS configuration.
 *  When ISR stack, main thread stack, default user thread stack sizes are defined.
 *  Then ISR stack size is equal to 1 KB,
 *       main thread stack size is equal to 4 KB,
 *       default user thread stack size is equal to 4 KB.
 *
 * NOTE:
 * It is impossible to verify RTOS-less thread stack size since all tests are build with RTOS.
 */
void stack_size_unification_test(void);

/**@}*/

#ifdef __cplusplus
}
#endif

#endif

/** @}*/
