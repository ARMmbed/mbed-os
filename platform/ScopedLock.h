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
#ifndef MBED_SCOPEDLOCK_H
#define MBED_SCOPEDLOCK_H

#include "platform/NonCopyable.h"

namespace mbed {

/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_ScopedLock ScopedLock functions
 * @{
 */

/** RAII-style mechanism for owning a lock of Lockable object for the duration of a scoped block
 *
 * @tparam Lockable The type implementing BasicLockable concept
 *
 * @note For type Lockable to be BasicLockable, the following conditions have to be satisfied:
 *        - has public member function @a lock which blocks until a lock can be obtained for the current execution context
 *        - has public member function @a unlock which releases the lock
 *
 * Usage:
 *
 * Example with rtos::Mutex
 *
 * @code
 * void foo(Mutex &m) {
 *     ScopedLock<Mutex> lock(m);
 *     // Mutex lock protects code in this block
 * }
 * @endcode
 *
 *
 * More generic example
 *
 * @code
 * template<typename Lockable>
 * void foo(Lockable& lockable) {
 *     ScopedLock<Lockable> lock(lockable);
 *     // Code in this block runs under lock
 * }
 * @endcode
 */
template <typename Lockable>
class ScopedLock : private NonCopyable<ScopedLock<Lockable> > {
public:
    /** Locks given locable object
     *
     * @param lockable reference to the instance of Lockable object
     * @note lockable object should outlive the ScopedLock object
     */
    ScopedLock(Lockable &lockable): _lockable(lockable)
    {
        _lockable.lock();
    }

    ~ScopedLock()
    {
        _lockable.unlock();
    }
private:
    Lockable &_lockable;
};

/**@}*/

/**@}*/

} // embed

#endif // MBED_SCOPEDLOCK_H
