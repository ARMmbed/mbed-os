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

#ifndef MBED_HAL_WATCHDOG_TIMING_TESTS_H
#define MBED_HAL_WATCHDOG_TIMING_TESTS_H

#if DEVICE_WATCHDOG

/** Test watchdog timing accuracy
 *
 * Phase 1.
 * Given a watchdog timer started with a timeout value of X ms,
 * when the time of X ms elapses,
 * then the device is restarted by the watchdog.
 *
 * Phase 2.
 * Given a device restarted by the watchdog timer,
 * when the device receives time measurement T from the host,
 * then X <= T < 2 * X.
 */
template<uint32_t timeout_ms, uint32_t delta_ms>
void test_timing();

/** Test Watchdog timeout
 *
 * Given a device with a Watchdog started,
 * when the Watchdog timeout doesn't expire,
 * then the device restart is not performed.
 * When the Watchdog timeout does expire,
 * then the device is restarted after the timeout and before twice the timeout value.
 */
void test_timeout_lower_limit();

#endif

#endif

/** @}*/

