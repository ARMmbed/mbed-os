/*
 * Copyright (c) 2017-2019 ARM Limited
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

#ifndef MBED_CRITICALSECTIONLOCK_H
#define MBED_CRITICALSECTIONLOCK_H

#include "platform/mbed_toolchain.h"

namespace mbed {
/** \addtogroup platform-public-api */
/** @{*/
/**
 * \defgroup platform_CriticalSectionLock CriticalSectionLock functions
 * @{
 */

/** RAII object for disabling, then restoring, interrupt state
  * Usage:
  * @code
  *
  * // RAII style usage
  * unsigned int atomic_counter_increment(unsigned int &counter) {
  *     CriticalSectionLock lock;
  *     // Code in this block will run with interrupts disabled
  *     // Interrupts will be restored to their previous state automatically
  *     // at the end of function scope
  *     return ++counter;
  * }
  *
  * // free locking usage
  * unsigned int atomic_counter_decrement(unsigned int &counter) {
  *     CriticalSectionLock::enable();
  *     // Code in this block will run with interrupts disabled
  *     counter--;
  *     CriticalSectionLock::disable(); // need explicitly to disable critical section lock
  *     // interrupts will be restored to their previous state here
  *     return counter;
  * }
  *
  * @endcode
  */
class CriticalSectionLock {
public:
    CriticalSectionLock();

    ~CriticalSectionLock();

    /** Mark the start of a critical section
     *  @deprecated This function is inconsistent with RAII and is being removed in the future. Replaced by static function CriticalSectionLock::enable.
     *
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.8",
                          "This function is inconsistent with RAII and is being removed in the future."
                          "Replaced by static function CriticalSectionLock::enable.")
    void lock();

    /** Mark the end of a critical section
     *  @deprecated This function is inconsistent with RAII and is being removed in the future. Replaced by static function CriticalSectionLock::enable.
     *
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.8",
                          "This function is inconsistent with RAII and is being removed in the future."
                          "Replaced by static function CriticalSectionLock::disable.")
    void unlock();

    /** Mark the start of a critical section
     */
    static void enable();

    /** Mark the end of a critical section
     */
    static void disable();
};

/**@}*/

/**@}*/

} // namespace mbed

#endif
