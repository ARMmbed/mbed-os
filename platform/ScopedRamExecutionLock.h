/* mbed Microcontroller Library
 * Copyright (c) 2018-2019 ARM Limited
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
#ifndef MBED_SCOPEDRAMEXECUTIONLOCK_H
#define MBED_SCOPEDRAMEXECUTIONLOCK_H

#include "platform/mbed_mpu_mgmt.h"
#include "platform/NonCopyable.h"

namespace mbed {

/** \addtogroup platform-public-api */
/** @{*/

/** RAII object for disabling, then restoring RAM execute never mode
  * Usage:
  * @code
  *
  * void f() {
  *     // some code here
  *     {
  *         ScopedRamExecutionLock make_ram_executable;
  *         // Code in this block is allowed to call functions in RAM
  *     }
  *     // Execution from RAM is no longer allowed
  * }
  * @endcode
  */
class ScopedRamExecutionLock : private mbed::NonCopyable<ScopedRamExecutionLock> {
public:

    /**
     * Allow execution from RAM
     *
     * Increment the execute never lock to ensure code can
     * be executed from RAM. This class uses RAII to allow
     * execution from ram while it is in scope.
     */
    ScopedRamExecutionLock()
    {
        mbed_mpu_manager_lock_ram_execution();
    }

    /**
     * Restore previous execution from RAM settings
     *
     * Decrement the execute never lock to return execute from RAM
     * to its prior state.
     */
    ~ScopedRamExecutionLock()
    {
        mbed_mpu_manager_unlock_ram_execution();
    }
};

/**@}*/

}

#endif
