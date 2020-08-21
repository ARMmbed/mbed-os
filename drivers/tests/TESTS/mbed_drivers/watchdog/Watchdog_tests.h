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

#ifndef MBED_DRIVERS_WATCHDOG_TESTS_H
#define MBED_DRIVERS_WATCHDOG_TESTS_H

#if DEVICE_WATCHDOG

/** Test Watchdog max_timeout validity
 *
 * Given a device supporting Watchdog driver API,
 * when @a Watchdog::get_max_timeout() is called,
 * then the returned value is greater than 1.
 */
void test_max_timeout_is_valid();

/** Test Watchdog stop
 *
 * Given a device without a support for the @a disable_watchdog feature,
 * when @a Watchdog::stop() is called,
 * then false is returned.
 *
 * Otherwise, given the device with @a disable_watchdog feature support:
 *
 * Given the Watchdog is *NOT* running,
 * when @a Watchdog::stop() is called,
 * then false is returned.
 *
 * Given the Watchdog is running,
 * when @a Watchdog::stop() is called before the timeout expires,
 * then true is returned and the device is not restarted.
 *
 * Given the Watchdog is *NOT* running (it has already been stopped),
 * when @a Watchdog::stop() is called,
 * then false is returned.
 */
void test_stop();

/** Test Watchdog start multiple times
 *
 * Given a set of unique timeout values,
 * when Watchdog::start(T) is called for each value T,
 * then, for every T, Watchdog::start() returns true
 *     and Watchdog::get_timeout() returns an actual timeout value R
 *     and T <= R < 2 * T.
 */
void test_restart();

/** Test Watchdog start with a valid config
 *
 * Given a value of T ms which is within supported Watchdog timeout range,
 * when Watchdog::start(T) is called,
 * then true is returned
 *     and Watchdog::get_timeout() returns an actual timeout value R
 *     and T <= R < 2 * T.
 */
template<uint32_t timeout_ms>
void test_start();

/** Test Watchdog start with max_timeout
 *
 * Given max_timeout value returned by @a Watchdog::get_max_timeout(),
 * when @a Watchdog::start(max_timeout) is called,
 * then true is returned
 *     and @a Watchdog::get_timeout() returns max_timeout.
 */
void test_start_max_timeout();

#endif

#endif
