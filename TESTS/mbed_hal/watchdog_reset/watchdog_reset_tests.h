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

#ifndef MBED_HAL_WATCHDOG_RESET_TESTS_H
#define MBED_HAL_WATCHDOG_RESET_TESTS_H

#if DEVICE_WATCHDOG

/** Test watchdog reset
 *
 * Given a device with a watchdog started
 * When a watchdog timeout expires
 * Then the device is restarted
 */
void test_simple_reset();

/** Test watchdog reset in sleep mode
 *
 * Given a device supporting sleep mode, with a watchdog started
 * When the device is in sleep mode and watchdog timeout expires
 * Then the device is restarted
 */
void test_sleep_reset();

/** Test watchdog reset in deepsleep mode
 *
 * Given a device supporting deepsleep mode, with watchdog started
 * When the device is in deepsleep mode and watchdog timeout expires
 * Then the device is restarted
 */
void test_deepsleep_reset();

/** Test stopped watchdog can be started again and reset the device
 *
 * Given a device supporting 'disable_watchdog' feature, with watchdog started
 * When the watchdog is stopped before timeout expires
 * Then the device is not restarted
 * When the watchdog is started again and it's timeout expires
 * Then the device is restarted
 */
void test_restart_reset();

/** Test kicking the watchdog prevents reset
 *
 * Given a device with watchdog started
 * When the watchdog is kicked before timeout expires
 * Then the device restart is prevented
 * When the watchdog is *NOT* kicked again before next timeout expires
 * Then the device is restarted
 *
 */
void test_kick_reset();

#endif

#endif

/** @}*/

