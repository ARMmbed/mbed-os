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

#ifndef MBED_HAL_WATCHDOG_API_TESTS_H
#define MBED_HAL_WATCHDOG_API_TESTS_H

#if DEVICE_WATCHDOG

/** Test max_timeout validity
 *
 * Given a device supporting Watchdog HAL API,
 * when @a hal_watchdog_get_platform_features() is called,
 * then max_timeout member of returned watchdog_features_t struct is greater than 1.
 */
void test_max_timeout_is_valid();

/** Test Watchdog features if a stopped Watchdog can be started again
 *
 * Given a device supporting Watchdog HAL API,
 * when the device supports the @a disable_watchdog feature,
 * then the device also supports @a update_config feature.
 */
void test_restart_is_possible();

/** Test Watchdog stop
 *
 * Given a device without a support for the @a disable_watchdog feature,
 * when @a hal_watchdog_stop() is called,
 * then WATCHDOG_STATUS_NOT_SUPPORTED is returned.
 *
 * Otherwise, given the device with @a disable_watchdog feature support:
 *
 * Given the Watchdog is *NOT* running,
 * when @a hal_watchdog_stop() is called,
 * then WATCHDOG_STATUS_OK is returned.
 *
 * Given the Watchdog is running,
 * when @a hal_watchdog_stop() is called before the timeout expires,
 * then WATCHDOG_STATUS_OK is returned and the device is not restarted.
 *
 * Given the Watchdog is *NOT* running (it has already been stopped),
 * when @a hal_watchdog_stop() is called,
 * then WATCHDOG_STATUS_OK is returned.
 */
void test_stop();

/** Test Watchdog init multiple times
 *
 * Given a set of unique timeout values,
 * when @a config.timeout_ms is set to each of these values (T),
 * then, for every value T, @a hal_watchdog_init() returns @a WATCHDOG_STATUS_OK
 *     and @a hal_watchdog_get_reload_value() returns a reload value R
 *     and T <= R < 2 * T.
 */
void test_update_config();

/** Test Watchdog init with a valid config
 *
 * Given @a config.timeout_ms is set to T ms,
 *     which is within supported Watchdog timeout range,
 * when @a hal_watchdog_init() is called,
 * then @a WATCHDOG_STATUS_OK is returned
 *     and @a hal_watchdog_get_reload_value() returns a reload value R
 *     and T <= R < 2 * T.
 */
template<uint32_t timeout_ms>
void test_init();

/** Test Watchdog init with a max_timeout
 *
 * Given @a config.timeout_ms is set to max_timeout,
 *     which is a value returned by @a hal_watchdog_get_platform_features(),
 * when @a hal_watchdog_init() is called,
 * then @a WATCHDOG_STATUS_OK is returned
 *     and @a hal_watchdog_get_reload_value() returns max_timeout.
 */
void test_init_max_timeout();

#endif

#endif

/** @}*/

