/* Mbed Microcontroller Library
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
 * @addtogroup drivers_watchdog_tests
 * @{
 */

#ifndef MBED_DRIVERS_WATCHDOG_TESTS_H
#define MBED_DRIVERS_WATCHDOG_TESTS_H

#if DEVICE_WATCHDOG

/** Test Watchdog max_timeout validity
 *
 * Given a device supporting Watchdog driver API,
 * when @a Watchdog::max_timeout() is called,
 * then the returned value is greater than 1.
 */
void test_max_timeout_is_valid();

/** Test Watchdog stop
 *
 * Given the Watchdog is *NOT* running,
 * when @a Watchdog::stop() is called,
 * then WATCHDOG_STATUS_OK is returned.
 *
 * Given the Watchdog is running,
 * when @a Watchdog::stop() is called before the timeout expires,
 * then WATCHDOG_STATUS_OK is returned and the device is not restarted.
 *
 * Given the Watchdog is *NOT* running (it has already been stopped),
 * when @a Watchdog::stop() is called,
 * then WATCHDOG_STATUS_OK is returned.
 */
void test_stop();

/** Test Watchdog start multiple times
 *
 * Given @a max_timeout value returned by @a Watchdog::max_timeout(),
 *
 * when @a Watchdog::start(max_timeout - delta) is called,
 * then @a WATCHDOG_STATUS_OK is returned
 *     and @a Watchdog::reload_value() returns max_timeout - delta;
 *
 * when @a Watchdog::start(max_timeout) is called,
 * then @a WATCHDOG_STATUS_OK is returned
 *     and @a Watchdog::reload_value() returns max_timeout;
 *
 * when @a Watchdog::start(max_timeout + delta) is called,
 * then @a WATCHDOG_STATUS_INVALID_ARGUMENT is returned
 *     and @a Watchdog::reload_value() returns previously set value (max_timeout);
 *
 * when @a Watchdog::start(0) is called,
 * then @a WATCHDOG_STATUS_INVALID_ARGUMENT is returned
 *     and @a Watchdog::reload_value() returns previously set value (max_timeout).
 */
void test_restart();

/** Test Watchdog start with 0 ms timeout
 *
 * Given a device supporting Watchdog driver API,
 * when @a Watchdog::start() is called with @a timeout set to 0 ms,
 * then @a WATCHDOG_STATUS_INVALID_ARGUMENT is returned.
 */
void test_start_zero();

/** Test Watchdog start
 *
 * Given a value of X ms which is within supported Watchdog timeout range,
 * when @a Watchdog::start() is called with @a timeout set to X ms,
 * then @a WATCHDOG_STATUS_OK is returned
 *     and @a Watchdog::reload_value() returns X.
 */
template<uint32_t timeout_ms>
void test_start();

/** Test Watchdog start with max_timeout
 *
 * Given @a max_timeout value returned by @a Watchdog::max_timeout(),
 * when @a Watchdog::start() is called with @a timeout set to max_timeout,
 * then @a WATCHDOG_STATUS_OK is returned
 *     and @a Watchdog::reload_value() returns max_timeout.
 */
void test_start_max_timeout();

/** Test Watchdog start with a timeout value greater than max_timeout
 *
 * Given @a max_timeout value returned by @a Watchdog::max_timeout(),
 * when @a Watchdog::start() is called with @a timeout set to max_timeout + 1,
 * then @a WATCHDOG_STATUS_INVALID_ARGUMENT is retuned.
 */
void test_start_max_timeout_exceeded();

#endif

#endif

/** @}*/

