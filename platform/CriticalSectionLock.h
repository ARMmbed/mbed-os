/*
 * PackageLicenseDeclared: Apache-2.0
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

#ifndef MBED_CRITICALSECTIONLOCK_H
#define MBED_CRITICALSECTIONLOCK_H

#include "platform/mbed_critical.h"
#include "platform/mbed_assert.h"

namespace mbed {

/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_CriticalSectionLock CriticalSectionLock functions
 * @{
 */

/** RAII object for disabling, then restoring, interrupt state
  * Usage:
  * @code
  *
  * void f() {
  *     // some code here
  *     {
  *         CriticalSectionLock lock;
  *         // Code in this block will run with interrupts disabled
  *     }
  *     // interrupts will be restored to their previous state
  * }
  * void f() {
  *     CriticalSectionLock cs(false);
  *     // some code here
  *
  *     cs.lock();
  *        // Code in this block will run with interrupts disabled
  *     cs.unlock();
  *     // interrupts will be restored to their previous state
  *
  *     cs.lock();
  *        // Code in this block will run with interrupts disabled
  *     cs.unlock();
  *     // interrupts will be restored to their previous state
  * }
  * @endcode
  */
class CriticalSectionLock {
public:
    /** Create and initialize a CriticalSectionLock object.
     * @param lock  critical section lock state after initialization
     */
    CriticalSectionLock(bool lock = true) : _locked(false)
    {
        if (lock) {
            core_util_critical_section_enter();
            _locked = true;
        }
    }

    ~CriticalSectionLock() 
    {
        if (_locked) {
            core_util_critical_section_exit();
        }
    }

    /** Mark the start of a critical section
     *     
     */
    void lock()
    {
        MBED_ASSERT(false == _locked);
        if (!_locked) {
            core_util_critical_section_enter();
            _locked = true;
        }
    }

    /** Mark the end of a critical section
     *     
     */
    void unlock()
    {
        MBED_ASSERT(true == _locked);
        if (_locked) {
            core_util_critical_section_exit();
            _locked = false;
        }
    }

private:
    bool _locked;
};

/**@}*/

/**@}*/

} // namespace mbed

#endif
