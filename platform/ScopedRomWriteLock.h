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
#ifndef MBED_SCOPEDROMWRITELOCK_H
#define MBED_SCOPEDROMWRITELOCK_H

#include "platform/mbed_mpu_mgmt.h"
#include "platform/NonCopyable.h"

namespace mbed {

/** \addtogroup platform-public-api */
/** @{*/

/** RAII object for disabling, then restoring ROM write never mode
  * Usage:
  * @code
  *
  * void f() {
  *     // some code here
  *     {
  *         ScopedRomWriteLock make_ram_executable;
  *         // Code in this block is allowed to write to ROM
  *     }
  *     // Writing to ROM is no longer allowed
  * }
  * @endcode
  */
class ScopedRomWriteLock : private mbed::NonCopyable<ScopedRomWriteLock> {
public:

    /**
     * Allow writing to ROM
     *
     * Increment the ROM write lock to ensure code can
     * write to ROM. This class uses RAII to allow
     * writing to ROM while it is in scope.
     */
    ScopedRomWriteLock()
    {
        mbed_mpu_manager_lock_rom_write();
    }

    /**
     * Restore previous write to ROM settings
     *
     * Decrement the ROM write lock to return ROM write
     * to its prior state.
     */
    ~ScopedRomWriteLock()
    {
        mbed_mpu_manager_unlock_rom_write();
    }
};

/**@}*/

}

#endif
