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

#ifndef MBED_DRIVERS_WATCHDOG_RESET_TESTS_H
#define MBED_DRIVERS_WATCHDOG_RESET_TESTS_H

#if DEVICE_WATCHDOG

/** Test Watchdog reset
 *
 * Given a device with a Watchdog started,
 * when a Watchdog timeout expires,
 * then the device is restarted.
 */
void test_simple_reset();

/** Test Watchdog reset in sleep mode
 *
 * Given a device with a Watchdog started,
 * when the Watchdog timeout expires while the device is in sleep mode,
 * then the device is restarted.
 */
void test_sleep_reset();

/** Test Watchdog reset in deepsleep mode
 *
 * Given a device with a Watchdog started,
 * when the Watchdog timeout expires while the device is in deepsleep mode,
 * then the device is restarted.
 */
void test_deepsleep_reset();

/** Test Watchdog reset after Watchdog restart
 *
 * Given a device with a Watchdog started,
 * when the Watchdog is stopped before its timeout expires,
 * then the device is not restarted.
 * When the Watchdog is started again and its timeout expires,
 * then the device is restarted.
 */
void test_restart_reset();

/** Test Watchdog kick
 *
 * Given a device with a Watchdog started,
 * when the Watchdog is kicked before its timeout expires,
 * then the device restart is prevented.
 * When the Watchdog is *NOT* kicked again before next timeout expires,
 * then the device is restarted.
 */
void test_kick_reset();

#endif

#endif
