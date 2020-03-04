/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

/** \addtogroup hal_critical
 *  @{
 *  \defgroup hal_critical_test Tests
 *  Tests definitions of the HAL Critical module.
 *  @{
 */

#ifndef MBED_CRITICAL_SECTION_TEST_H
#define MBED_CRITICAL_SECTION_TEST_H

/** Template for HAL critical section tests
 *
 * Test critical section
 * Given a critical section HAL mechanism
 * When before critical section
 * Then interrupts are enabled
 * When inside critical section
 * Then interrupts are disabled
 * When after critical section
 * Then interrupts are enabled again
 *
 * Test critical section - nested lock
 * Given a critical section HAL mechanism
 * When before critical section
 * Then interrupts are enabled
 * When inside nested critical section
 * Then interrupts are disabled
 * When after nested critical section
 * Then interrupts are enabled again
 *
 */
template <int N>
void test_critical_section();

/**@}*/
/**@}*/

#endif // MBED_CRITICAL_SECTION_TEST_H
