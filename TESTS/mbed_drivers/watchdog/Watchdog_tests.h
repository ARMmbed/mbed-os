/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

/** Test max_timeout is valid
 *
 * Given a device supporting Watchdog driver API
 * When @a Watchdog::max_timeout() is called
 * Then the returned value is greater than 1
 */
void test_max_timeout_is_valid();

/** Test watchdog can be stopped
 *
 * Given a platform with a support for disabling a running watchdog
 * When watchdog is not running and @a Watchdog::stop() is called
 * Then WATCHDOG_STATUS_OK is returned
 * When watchdog is running
 *     and @a Watchdog::stop() is called before timeout
 * Then WATCHDOG_STATUS_OK is returned
 *     and watchdog does not reset the device
 * When watchdog has already been stopped and @a Watchdog::stop() is called
 * Then WATCHDOG_STATUS_OK is returned
 */
void test_stop();

/** Test restart watchdog multiple times
 *
 * Given @a max_timeout value returned by @a Watchdog::max_timeout()
 *     and @a Watchdog::start() called multiple times
 * When @a timeout is set to max_timeout - delta
 * Then return value of @a Watchdog::start() is @a WATCHDOG_STATUS_OK
 *     and @a Watchdog::reload_value() returns max_timeout - delta
 * When @a timeout is set to max_timeout
 * Then return value of @a Watchdog::start() is @a WATCHDOG_STATUS_OK
 *     and @a Watchdog::reload_value() returns max_timeout
 * When @a timeout is set to max_timeout + delta
 * Then return value of @a Watchdog::start() is @a WATCHDOG_STATUS_INVALID_ARGUMENT
 *     and @a Watchdog::reload_value() returns previously set value (max_timeout)
 * When @a timeout is set to 0
 * Then return value of @a Watchdog::start() is @a WATCHDOG_STATUS_INVALID_ARGUMENT
 *     and @a Watchdog::reload_value() returns previously set value (max_timeout)
 */
void test_restart();

/** Test start with 0 ms
 *
 * Given a device supporting Watchdog driver API
 * When @a timeout is set to 0 ms
 * Then return value of Watchdog::start() is @a WATCHDOG_STATUS_INVALID_ARGUMENT
 */
void test_start_zero();

/** Test start with a valid config
 *
 * Given a device supporting Watchdog driver API
 * When @a timeout is set to value X within platform's timeout range
 * Then return value of Watchdog::start() is @a WATCHDOG_STATUS_OK
 *     and @a Watchdog::reload_value() returns X
 */
template<uint32_t timeout_ms>
void test_start();

/** Test start with a max_timeout
 *
 * Given @a max_timeout value returned by @a Watchdog::max_timeout()
 * When @a timeout is set to max_timeout
 * Then return value of Watchdog::start() is @a WATCHDOG_STATUS_OK
 *     and @a Watchdog::reload_value() returns max_timeout
 */
void test_start_max_timeout();

/** Test start with a timeout value exceeding max_timeout
 *
 * Given a device supporting Watchdog driver API
 * When @a timeout is set to max_timeout + 1
 * Then return value of Watchdog::start() is @a WATCHDOG_STATUS_INVALID_ARGUMENT
 */
void test_start_max_timeout_exceeded();

#endif

#endif

/** @}*/

