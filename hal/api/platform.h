/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
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
#ifndef MBED_PLATFORM_H
#define MBED_PLATFORM_H

#define MBED_OPERATORS    1

#include "device.h"
#include "PinNames.h"
#include "PeripheralNames.h"
#include "critical.h"

#include <cstddef>
#include <cstdlib>
#include <cstdio>
#include <cstring>

#ifdef MBED_CONF_RTOS_PRESENT
#include "Mutex.h"
typedef rtos::Mutex PlatformMutex;
#else
/** A stub mutex for when an RTOS is not present
*/
class PlatformMutex {
public:
    PlatformMutex() {
        // Stub

    }
    ~PlatformMutex() {
        // Stub
    }

    void lock() {
        // Do nothing
    }

    void unlock() {
        // Do nothing
    }
};

#endif

/** The static version of a PlatformMutex
 *
 * This class must only be used in a static context -
 * this class must never be allocated or created on the
 * stack.
 *
 * This class is lazily initialized on first use.
 * This class is a POD type so if it is not used it will
 * be garbage collected.
 */
struct PlatformMutexStatic {
    PlatformMutex* _mutex;

    void _init() {
        PlatformMutex* current = _mutex;
        PlatformMutex* new_mutex;

        if (NULL == current) {
            bool done = false;
            new_mutex = new PlatformMutex;
            while (!done) {
                done = core_util_atomic_cas_ptr((void**)&_mutex, (void**)&current, (void*)new_mutex);
                if (current != NULL) {
                    // Mutex was created on another thread first
                    // so delete ours
                    delete new_mutex;
                    break;
                }
            }
        }
    }

    /** Wait until this Mutex becomes available.
     */
    void lock() {
        if (NULL == _mutex) {
            _init();
        }
        _mutex->lock();
    }

    /** Unlock the mutex that has previously been locked by the same thread
     */
    void unlock() {
        if (NULL == _mutex) {
            _init();
        }
        _mutex->unlock();
    }
};

#endif
