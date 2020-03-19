/* mbed Microcontroller Library
 * Copyright (c) 2006-2019 ARM Limited
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
#ifndef SINGLETONPTR_H
#define SINGLETONPTR_H

#include <stdlib.h>
#include <stdint.h>
#include <new>
#include "platform/mbed_assert.h"
#include "platform/mbed_atomic.h"
#ifdef MBED_CONF_RTOS_PRESENT
#include "cmsis_os2.h"
#endif

#ifdef MBED_CONF_RTOS_PRESENT
extern osMutexId_t singleton_mutex_id;
#endif

/** \addtogroup platform-public-api */
/** @{*/

/**
 * \defgroup platform_SingletonPtr SingletonPtr class
 * @{
 */

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

/** Utility class for creating and using a singleton
 *
 * @note Synchronization level: Thread safe
 *
 * @note: This class is lazily initialized on first use.
 * This class has a constexpr default constructor so if it is
 * not used as a non-local variable it will be garbage collected.
 *
 * @note: This class would normally be used in a static standalone
 * context. It does not call the destructor of the wrapped object
 * when it is destroyed, effectively ensuring linker exclusion of the
 * destructor for static objects. If used in another context, such as
 * a member of a normal class wanting "initialize on first-use"
 * semantics on a member, care should be taken to call the destroy
 * method manually if necessary.
 *
 * @note: If used as a sub-object of a class, that class's own
 * constructor must be constexpr to achieve its exclusion by
 * the linker when unused. That will require explicit
 * initialization of its other members.
 *
 * @note: More detail on initialization: Formerly, SingletonPtr
 * had no constructor, so was "zero-initialized" when non-local.
 * So if enclosed in another class with no constructor, the whole
 * thing would be zero-initialized, and linker-excludable.
 * Having no constructor meant SingletonPtr was not constexpr,
 * which limited applicability in other contexts. With its new
 * constexpr constructor, it is now "constant-initialized" when
 * non-local. This achieves the same effect as a standalone
 * non-local object, but as a sub-object linker exclusion is
 * now only achieved if the outer object is itself using a
 * constexpr constructor to get constant-initialization.
 * Otherwise, the outer object will be neither zero-initialized
 * nor constant-initialized, so will be "dynamic-initialized",
 * and likely to be left in by the linker.
 */
template <class T>
struct SingletonPtr {

    // Initializers are required to make default constructor constexpr
    // This adds no overhead as a static object - the compiler and linker can
    // figure out that we are effectively zero-init, and either place us in
    // ".bss", or exclude us if unused.
    constexpr SingletonPtr() noexcept : _ptr(), _data() { }

    /** Get a pointer to the underlying singleton
     *
     * @returns
     *   A pointer to the singleton
     */
    T *get() const
    {
        T *p = core_util_atomic_load(&_ptr);
        if (p == NULL) {
            singleton_lock();
            p = _ptr;
            if (p == NULL) {
                p = new (_data) T();
                core_util_atomic_store(&_ptr, p);
            }
            singleton_unlock();
        }
        // _ptr was not zero initialized or was
        // corrupted if this assert is hit
        MBED_ASSERT(p == reinterpret_cast<T *>(&_data));
        return p;
    }

    /** Get a pointer to the underlying singleton
     *
     * @returns
     *   A pointer to the singleton
     */
    T *operator->() const
    {
        return get();
    }

    /** Get a reference to the underlying singleton
     *
     * @returns
     *   A reference to the singleton
     */
    T &operator*() const
    {
        return *get();
    }

    /** Get a pointer to the underlying singleton
     *
     * Gets a pointer without initialization - can be
     * used as an optimization when it is known that
     * initialization must have already occurred.
     *
     * @returns
     *   A pointer to the singleton, or NULL if not
     *   initialized.
     */
    T *get_no_init() const
    {
        return _ptr;
    }

    /** Destroy the underlying singleton
     *
     * The underlying singleton is never automatically destroyed;
     * this is a potential optimization to avoid destructors
     * being pulled into an embedded image on the exit path,
     * which should never occur. The destructor can be
     * manually invoked via this call.
     *
     * Unlike construction, this is not thread-safe. After this call,
     * no further operations on the object are permitted.
     *
     * Is a no-op if the object has not been constructed.
     */
    void destroy()
    {
        if (_ptr) {
            _ptr->~T();
        }
    }

    mutable T *_ptr;
#if __cplusplus >= 201103L
    // Align data appropriately
    alignas(T) mutable char _data[sizeof(T)];
#else
    // Force data to be 8 byte aligned
    mutable uint64_t _data[(sizeof(T) + sizeof(uint64_t) - 1) / sizeof(uint64_t)];
#endif
};

#endif
/**@}*/

/**@}*/
