/*
 * Copyright (c) 2018-2019 Arm Limited and affiliates.
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
 * @addtogroup hal_watchdog_tests
 * @{
 */

#ifndef MBED_HAL_WATCHDOG_RESET_TESTS_H
#define MBED_HAL_WATCHDOG_RESET_TESTS_H

#if DEVICE_WATCHDOG

/** Test watchdog reset
 *
 * Given a device with a watchdog started,
 * when a watchdog timeout expires,
 * then the device is restarted.
 */
void test_simple_reset();

/** Test watchdog reset in sleep mode
 *
 * Given a device with a watchdog started,
 * when the watchdog timeout expires while the device is in sleep mode,
 * then the device is restarted.
 */
void test_sleep_reset();

/** Test watchdog reset in deepsleep mode
 *
 * Given a device with a watchdog started,
 * when the watchdog timeout expires while the device is in deepsleep mode,
 * then the device is restarted.
 */
void test_deepsleep_reset();

/** Test watchdog reset after watchdog restart
 *
 * Given a device with a watchdog started,
 * when the watchdog is stopped before its timeout expires,
 * then the device is not restarted.
 * When the watchdog is started again and its timeout expires,
 * then the device is restarted.
 */
void test_restart_reset();

/** Test watchdog kick
 *
 * Given a device with a watchdog started,
 * when the watchdog is kicked before its timeout expires,
 * then the device restart is prevented.
 * When the watchdog is *NOT* kicked again before next timeout expires,
 * then the device is restarted.
 */
void test_kick_reset();

#endif

#endif

/** @}*/

