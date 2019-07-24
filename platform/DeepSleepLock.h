/* mbed Microcontroller Library
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
#ifndef MBED_DEEPSLEEPLOCK_H
#define MBED_DEEPSLEEPLOCK_H

#include <limits.h>
#include <stdint.h>

namespace mbed {

/** \ingroup mbed-os-public */
/** \addtogroup platform-public-api */
/** @{*/
/**
 * \defgroup platform_DeepSleepLock DeepSleepLock functions
 * @{
 */

/** RAII object for disabling, then restoring the deep sleep mode
  * Usage:
  * @code
  *
  * void f() {
  *     // some code here
  *     {
  *         DeepSleepLock lock;
  *         // Code in this block will run with the deep sleep mode locked
  *     }
  *     // deep sleep mode will be restored to their previous state
  * }
  * @endcode
  */
class DeepSleepLock {
private:
    uint16_t _lock_count;

public:
    DeepSleepLock();

    ~DeepSleepLock();

    /** Mark the start of a locked deep sleep section
     */
    void lock();

    /** Mark the end of a locked deep sleep section
     */
    void unlock();
};

/**@}*/

/**@}*/


}

#endif
