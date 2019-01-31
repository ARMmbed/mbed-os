/* mbed Microcontroller Library
 * Copyright (c) 2006-2018 ARM Limited
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

#ifndef __SHAREDPTR_H__
#define __SHAREDPTR_H__

#include <stdlib.h>

#include <stdint.h>
#include <stddef.h>

#include "platform/mbed_critical.h"

namespace mbed {

/** Shared pointer class.
  *
  * A shared pointer is a "smart" pointer that retains ownership of an object using
  * reference counting across all smart pointers referencing that object.
  *
  * @code
  * #include "platform/SharedPtr.h"
  *
  * void test() {
  *     struct MyStruct { int a; };
  *
  *     // Create shared pointer
  *     SharedPtr<MyStruct> ptr( new MyStruct );
  *
  *     // Increase reference count
  *     SharedPtr<MyStruct> ptr2( ptr );
  *
  *     ptr = NULL; // Reference to the struct instance is still held by ptr2
  *
  *     ptr2 = NULL; // The raw pointer is freed
  * }
  * @endcode
  *
  *
  * It is similar to the std::shared_ptr class introduced in C++11;
  * however, this is not a compatible implementation (no weak pointer, no make_shared, no custom deleters and so on.)
  *
  * Usage: SharedPtr<Class> ptr(new Class())
  *
  * When ptr is passed around by value, the copy constructor and
  * destructor manages the reference count of the raw pointer.
  * If the counter reaches zero, delete is called on the raw pointer.
  *
  * To avoid loops, use "weak" references by calling the original
  * pointer directly through ptr.get().
  */

template <class T>
class SharedPtr {
public:
    /**
     * @brief Create empty SharedPtr not pointing to anything.
     * @details Used for variable declaration.
     */
    SharedPtr(): _ptr(NULL), _counter(NULL)
    {
    }

    /**
     * @brief Create new SharedPtr
     * @param ptr Pointer to take control over
     */
    SharedPtr(T *ptr): _ptr(ptr), _counter(NULL)
    {
        // Allocate counter on the heap, so it can be shared
        if (_ptr != NULL) {
            _counter = new uint32_t;
            *_counter = 1;
        }
    }

    /**
     * @brief Destructor.
     * @details Decrement reference counter, and delete object if no longer pointed to.
     */
    ~SharedPtr()
    {
        decrement_counter();
    }

    /**
     * @brief Copy constructor.
     * @details Create new SharedPtr from other SharedPtr by
     *          copying pointer to original object and pointer to counter.
     * @param source Object being copied from.
     */
    SharedPtr(const SharedPtr &source): _ptr(source._ptr), _counter(source._counter)
    {
        // Increment reference counter
        if (_ptr != NULL) {
            core_util_atomic_incr_u32(_counter, 1);
        }
    }

    /**
     * @brief Assignment operator.
     * @details Cleanup previous reference and assign new pointer and counter.
     * @param source Object being assigned from.
     * @return Object being assigned.
     */
    SharedPtr operator=(const SharedPtr &source)
    {
        if (this != &source) {
            // Clean up by decrementing counter
            decrement_counter();

            // Assign new values
            _ptr = source.get();
            _counter = source.get_counter();

            // Increment new counter
            if (_ptr != NULL) {
                core_util_atomic_incr_u32(_counter, 1);
            }
        }

        return *this;
    }

    /**
     * @brief Replaces the managed pointer with a new unmanaged pointer.
     * @param[in] ptr the new raw pointer to manage.
     */
    void reset(T *ptr)
    {
        // Clean up by decrementing counter
        decrement_counter();

        _ptr = ptr;
        if (ptr != NULL) {
            // Allocate counter on the heap, so it can be shared
            _counter = new uint32_t;
            *_counter = 1;
        } else {
            _counter = NULL;
        }
    }

    /**
     * @brief Replace the managed pointer with a NULL pointer.
     */
    void reset()
    {
        reset(NULL);
    }

    /**
     * @brief Raw pointer accessor.
     * @details Get raw pointer to object pointed to.
     * @return Pointer.
     */
    T *get() const
    {
        return _ptr;
    }

    /**
     * @brief Reference count accessor.
     * @return Reference count.
     */
    uint32_t use_count() const
    {
        if (_ptr != NULL) {
            core_util_critical_section_enter();
            uint32_t current_counter = *_counter;
            core_util_critical_section_exit();
            return current_counter;
        } else {
            return 0;
        }
    }

    /**
     * @brief Dereference object operator.
     * @details Override to return the object pointed to.
     */
    T &operator*() const
    {
        return *_ptr;
    }

    /**
     * @brief Dereference object member operator.
     * @details Override to return return member in object pointed to.
     */
    T *operator->() const
    {
        return _ptr;
    }

    /**
     * @brief Boolean conversion operator.
     * @return Whether or not the pointer is NULL.
     */
    operator bool() const
    {
        return (_ptr != NULL);
    }

private:
    /**
     * @brief Get pointer to reference counter.
     * @return Pointer to reference counter.
     */
    uint32_t *get_counter() const
    {
        return _counter;
    }

    /**
     * @brief Decrement reference counter.
     * @details If count reaches zero, free counter and delete object pointed to.
     * Does not modify our own pointers - assumption is they will be overwritten
     * or destroyed immediately afterwards.
     */
    void decrement_counter()
    {
        if (_ptr != NULL) {
            uint32_t new_value = core_util_atomic_decr_u32(_counter, 1);
            if (new_value == 0) {
                delete _counter;
                delete _ptr;
            }
        }
    }

private:
    // Pointer to shared object
    T *_ptr;

    // Pointer to shared reference counter
    uint32_t *_counter;
};

/** Non-member relational operators.
  */
template <class T, class U>
bool operator== (const SharedPtr<T> &lhs, const SharedPtr<U> &rhs)
{
    return (lhs.get() == rhs.get());
}

template <class T, typename U>
bool operator== (const SharedPtr<T> &lhs, U rhs)
{
    return (lhs.get() == (T *) rhs);
}

template <class T, typename U>
bool operator== (U lhs, const SharedPtr<T> &rhs)
{
    return ((T *) lhs == rhs.get());
}

/** Non-member relational operators.
  */
template <class T, class U>
bool operator!= (const SharedPtr<T> &lhs, const SharedPtr<U> &rhs)
{
    return (lhs.get() != rhs.get());
}

template <class T, typename U>
bool operator!= (const SharedPtr<T> &lhs, U rhs)
{
    return (lhs.get() != (T *) rhs);
}

template <class T, typename U>
bool operator!= (U lhs, const SharedPtr<T> &rhs)
{
    return ((T *) lhs != rhs.get());
}

} /* namespace mbed */

#ifndef MBED_NO_GLOBAL_USING_DIRECTIVE
using mbed::SharedPtr;
#endif

#endif // __SHAREDPTR_H__
