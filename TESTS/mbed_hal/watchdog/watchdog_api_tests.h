/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
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

/** Test max_timeout is valid
 *
 * Given a device supporting watchdog HAL API
 * When @a hal_watchdog_get_platform_features() is called
 * Then max_timeout member of returned watchdog_features_t struct is greater than 1
 */
void test_max_timeout_is_valid();

/** Test stopped watchdog can be started again
 *
 * Given a device supporting watchdog HAL API
 * When the device supports the @a disable_watchdog feature
 * Then the device also supports @a update_config feature
 */
void test_restart_is_possible();

/** Test watchdog can be stopped
 *
 * Given a device without a support for the @a disable_watchdog feature
 * When @a hal_watchdog_stop() is called
 * Then WATCHDOG_STATUS_NOT_SUPPORTED is returned
 *
 * Given a device supporting @a disable_watchdog feature
 * When watchdog is not running and @a hal_watchdog_stop() is called
 * Then WATCHDOG_STATUS_OK is returned
 * When watchdog is running
 *     and @a hal_watchdog_stop() is called before timeout
 * Then WATCHDOG_STATUS_OK is returned
 *     and watchdog does not reset the device
 * When watchdog has already been stopped and @a hal_watchdog_stop() is called
 * Then WATCHDOG_STATUS_OK is returned
 */
void test_stop();

/** Test update config with multiple init calls
 *
 * Given @a max_timeout value returned by @a hal_watchdog_get_platform_features()
 *     and @a hal_watchdog_init() called multiple times with same @a config
 * When @a config.timeout_ms is set to WDG_CONFIG_DEFAULT
 * Then return value of hal_watchdog_init() is @a WATCHDOG_STATUS_OK
 *     and @a hal_watchdog_get_reload_value() returns WDG_CONFIG_DEFAULT
 * When @a config.timeout_ms is set to max_timeout-delta
 * Then return value of hal_watchdog_init() is @a WATCHDOG_STATUS_OK
 *     and @a hal_watchdog_get_reload_value() returns max_timeout-delta
 * When @a config.timeout_ms is set to max_timeout
 * Then return value of hal_watchdog_init() is @a WATCHDOG_STATUS_OK
 *     and @a hal_watchdog_get_reload_value() returns max_timeout
 */
void test_update_config();

/** Test init with a valid config
 *
 * Given a device supporting watchdog HAL API
 * When @a config.timeout_ms is set to value X within platform's timeout range
 * Then return value of hal_watchdog_init() is @a WATCHDOG_STATUS_OK
 *     and @a hal_watchdog_get_reload_value() returns X
 */
template<uint32_t timeout_ms>
void test_init();

/** Test init with a max_timeout
 *
 * Given @a max_timeout value returned by @a hal_watchdog_get_platform_features()
 * When @a config.timeout_ms is set to max_timeout
 * Then return value of hal_watchdog_init() is @a WATCHDOG_STATUS_OK
 *     and @a hal_watchdog_get_reload_value() returns max_timeout
 */
void test_init_max_timeout();

#endif

#endif

/** @}*/

