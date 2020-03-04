/* mbed Microcontroller Library
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
 * @addtogroup hal_sleep_manager_tests
 * @{
 */

#ifndef MBED_HAL_SLEEP_MANAGER_API_RACECONDITION_TESTS_H
#define MBED_HAL_SLEEP_MANAGER_API_RACECONDITION_TESTS_H

/** Test lock/unlock is thread safe
 *
 * Given a device with sleep mode support
 * When multiple threads are using the sleep manager API
 * Then lock/unlock calls are thread safe
 */
void sleep_manager_multithread_test();

/** Test lock/unlock is IRQ safe
 *
 * Given a device with sleep mode support
 * When the sleep manager API is used from IRQ and the main thread concurrently
 * Then lock/unlock calls are IRQ safe
 */
void sleep_manager_irq_test();

#endif

/** @}*/
