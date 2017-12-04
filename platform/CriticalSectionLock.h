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
  * @endcode
  */
class CriticalSectionLock {
public:
    CriticalSectionLock() 
    {
        core_util_critical_section_enter();
    }

    ~CriticalSectionLock() 
    {
        core_util_critical_section_exit();
    }

    /** Mark the start of a critical section
     *     
     */
    void lock()
    {
        core_util_critical_section_enter();
    }

    /** Mark the end of a critical section
     *     
     */
    void unlock()
    {
        core_util_critical_section_exit();
    }
};

/**@}*/

/**@}*/

} // namespace mbed

#endif
