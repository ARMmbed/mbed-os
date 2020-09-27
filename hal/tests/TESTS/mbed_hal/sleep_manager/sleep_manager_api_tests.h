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

/**
 * @addtogroup hal_sleep_manager_tests
 * @{
 */

#ifndef MBED_HAL_SLEEP_MANAGER_API_TESTS_H
#define MBED_HAL_SLEEP_MANAGER_API_TESTS_H

#if DEVICE_SLEEP

/** Test lock/unlock
 *
 * Given no prior calls to lock/unlock
 * When the deep sleep status is checked
 * Then the deep sleep is allowed
 *
 * When the lock function is called
 * Then the deep sleep is not allowed
 *
 * When the unlock function is called
 * Then the deep sleep is allowed again
 */
void test_lock_unlock();

/** Test lock USHRT_MAX times
 *
 * Given a device with sleep mode support
 * When deep sleep mode is locked USHRT_MAX times
 * Then the deep sleep mode is locked
 *
 * When unlock is called repeatedly
 * Then deep sleep mode stays locked until the number
 *     of unlock calls is equal to number of lock calls
 */
void test_lock_eq_ushrt_max();

/** Test sleep_auto calls sleep and deep sleep based on lock
 *
 * Given a device with sleep mode support
 * When the deep sleep mode is locked
 * Then sleep_auto uses sleep mode
 *
 * When the deep sleep mode is unlocked
 * Then sleep_auto uses deep sleep mode
 */
void test_sleep_auto();

/** Test lock/unlock test_check fun
 *
 * Given the deep sleep has not been locked
 * When the deep sleep status is checked
 * Then sleep_manager_can_deep_sleep() returns true
 *     and sleep_manager_can_deep_sleep_test_check() returns true instantly.
 *
 * When the deep sleep mode is locked
 * Then sleep_manager_can_deep_sleep() returns false
 *     and sleep_manager_can_deep_sleep_test_check() returns false with 2 ms delay.
 *
 * When the deep sleep mode is unlocked with a 1 ms delay
 * Then sleep_manager_can_deep_sleep() returns false
 *     and sleep_manager_can_deep_sleep_test_check() returns true with 1 ms delay
 *     and sleep_manager_can_deep_sleep() returns true when checked again.
 */
void test_lock_unlock_test_check();

#endif

#endif

/** @}*/
