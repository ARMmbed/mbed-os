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
 * @addtogroup hal_watchdog_tests
 * @{
 */

#ifndef MBED_HAL_WATCHDOG_TIMING_TESTS_H
#define MBED_HAL_WATCHDOG_TIMING_TESTS_H

#if DEVICE_WATCHDOG

/** Test watchdog timing accuracy
 *
 * Phase 1.
 * Given a watchdog timer started with a timeout value of X ms
 * When given time of X ms elapses
 * Then the device is restarted by the watchdog
 *
 * Phase 2.
 * Given a device restarted by the watchdog timer
 * When the device receives time measurement from the host
 * Then time measured by host equals X ms
 */
template<uint32_t timeout_ms, uint32_t delta_ms>
void test_timing();

#endif

#endif

/** @}*/

