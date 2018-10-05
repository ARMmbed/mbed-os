
/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_SingletonPtr SingletonPtr class
 * @{
 */
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
#ifndef SINGLETONPTR_H
#define SINGLETONPTR_H

#include <stdint.h>
#include <new>
#include "platform/mbed_assert.h"
#ifdef MBED_CONF_RTOS_PRESENT
#include "cmsis_os2.h"
#endif

#ifdef MBED_CONF_RTOS_PRESENT
extern osMutexId_t singleton_mutex_id;
#endif

/** Lock the singleton mutex
 *
 * This function is typically used to provide
 * exclusive access when initializing a
 * global object.
 */
inline static void singleton_lock(void)
{
#ifdef MBED_CONF_RTOS_PRESENT
    if (!singleton_mutex_id) {
        // RTOS has not booted yet so no mutex is needed
        return;
    }
    osMutexAcquire(singleton_mutex_id, osWaitForever);
#endif
}

/** Unlock the singleton mutex
 *
 * This function is typically used to provide
 * exclusive access when initializing a
 * global object.
 */
inline static void singleton_unlock(void)
{
#ifdef MBED_CONF_RTOS_PRESENT
    if (!singleton_mutex_id) {
        // RTOS has not booted yet so no mutex is needed
        return;
    }
    osMutexRelease(singleton_mutex_id);
#endif
}

/** Utility class for creating an using a singleton
 *
 * @note Synchronization level: Thread safe
 *
 * @note: This class must only be used in a static context -
 * this class must never be allocated or created on the
 * stack.
 *
 * @note: This class is lazily initialized on first use.
 * This class is a POD type so if it is not used it will
 * be garbage collected.
 */
template <class T>
struct SingletonPtr {

    /** Get a pointer to the underlying singleton
     *
     * @returns
     *   A pointer to the singleton
     */
    T *get()
    {
        if (NULL == _ptr) {
            singleton_lock();
            if (NULL == _ptr) {
                _ptr = new (_data) T();
            }
            singleton_unlock();
        }
        // _ptr was not zero initialized or was
        // corrupted if this assert is hit
        MBED_ASSERT(_ptr == (T *)&_data);
        return _ptr;
    }

    /** Get a pointer to the underlying singleton
     *
     * @returns
     *   A pointer to the singleton
     */
    T *operator->()
    {
        return get();
    }

    // This is zero initialized when in global scope
    T *_ptr;
    // Force data to be 4 byte aligned
    uint32_t _data[(sizeof(T) + sizeof(uint32_t) - 1) / sizeof(uint32_t)];
};

#endif
/**@}*/

/**@}*/
