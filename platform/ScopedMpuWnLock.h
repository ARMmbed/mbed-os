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
#ifndef MBED_SCOPEDMPUWNLOCK_H
#define MBED_SCOPEDMPUWNLOCK_H

#include "platform/mbed_mpu_mgmt.h"
#include "platform/NonCopyable.h"

namespace mbed {

/** \addtogroup platform */
/** @{*/

/** RAII object for disabling, then restoring ROM write never mode
  * Usage:
  * @code
  *
  * void f() {
  *     // some code here
  *     {
  *         ScopedMpuXnLock xn;
  *         // Code in this block is allowed to call functions in RAM
  *     }
  *     // Execution from RAM is no longer allowed
  * }
  * @endcode
  */
class ScopedMpuWnLock : private mbed::NonCopyable<ScopedMpuWnLock> {
public:

    /**
     * Allow execution from RAM
     *
     * Increment the execute never lock to ensure code can
     * be executed from RAM. This class uses RAII to allow
     * execution from ram while it is in scope.
     */
    ScopedMpuWnLock()
    {
        mbed_mpu_manager_lock_rom_wn();
    }

    /**
     * Restore previous execution from RAM settings
     *
     * Decrement the execute never lock to return execute from RAM
     * to its prior state.
     */
    ~ScopedMpuWnLock()
    {
        mbed_mpu_manager_unlock_rom_wn();
    }
};

/**@}*/

}

#endif
