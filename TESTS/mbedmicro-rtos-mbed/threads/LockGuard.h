/*
 * Copyright (c) 2013-2016, ARM Limited, All Rights Reserved
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MBEDMICRO_RTOS_MBED_THREADS_LOCK_GUARD
#define MBEDMICRO_RTOS_MBED_THREADS_LOCK_GUARD

#include <rtos.h>

/**
 * RAII mutex locker.
 * The mutex pass in the constructor will be locked for the lifetime of
 * the LockGuard instance.
 */
class LockGuard {
public:
    /**
     * Construct a LockGuard instance and ackire ownership of mutex in input.
     * @param mutex The mutex to ackire ownership of.
     */
    LockGuard(rtos::Mutex &mutex) : _mutex(mutex)
    {
        _mutex.lock();
    }

    /**
     * Destruct the lock and release the inner mutex.
     */
    ~LockGuard()
    {
        _mutex.unlock();
    }

private:
    LockGuard(const LockGuard &);
    LockGuard &operator=(const LockGuard &);
    rtos::Mutex &_mutex;
};
#endif /* MBEDMICRO_RTOS_MBED_THREADS_LOCK_GUARD */
