/* Copyright (c) 2017 ARM Limited
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

#ifndef MBED_NONCOPYABLE_H_
#define MBED_NONCOPYABLE_H_

#if (!defined(MBED_DEBUG) && (MBED_CONF_PLATFORM_FORCE_NON_COPYABLE_ERROR == 0))
#include "mbed_toolchain.h"
#include "mbed_debug.h"
#endif

namespace mbed {

/** \addtogroup platform */
/** @{*/
/**
 * \defgroup platform_NonCopyable NonCopyable class
 * @{
 */

/**
 * Prevents generation of copy constructor and copy assignment operator in
 * derived classes.
 *
 * @par Usage
 *
 * To prevent generation of copy constructor and copy assignment operator,
 * inherit privately from the NonCopyable class.
 *
 * @code
 * class Resource : NonCopyable<Resource> { };
 *
 * Resource r;
 * // generates compile time error:
 * Resource r2 = r;
 * @endcode
 *
 * @par Background information
 *
 * Instances of polymorphic classes are not meant to be copied. The
 * C++ standards generate a default copy constructor and copy assignment
 * function if these functions have not been defined in the class.
 *
 * Consider the following example:
 *
 * @code
 * // base class representing a connection
 * struct Connection {
 *     Connection();
 *     virtual ~Connection();
 *     virtual void open() = 0;
 * }
 *
 * class SerialConnection : public Connection {
 * public:
 *     SerialConnection(Serial*);
 *
 * private:
 *     Serial* _serial;
 * };
 *
 * Connection& get_connection() {
 *     static SerialConnection serial_connection;
 *     return serial_connection;
 * }
 *
 * Connection connection = get_connection();
 * @endcode
 *
 * There is a subtle bug in this code, the function get_connection returns a
 * reference to a Connection which is captured by value instead of reference.
 *
 * When `get_connection` returns a reference to serial_connection it is copied into
 * the local variable connection. The vtable and others members defined in Connection
 * are copied, but members defined in SerialConnection are left apart. This can cause
 * severe crashes or bugs if the virtual functions captured use members not present
 * in the base declaration.
 *
 * To solve that problem, the copy constructor and assignment operator have to
 * be declared (but don't need to be defined) in the private section of the
 * Connection class:
 *
 * @code
 * struct Connection {
 * private:
 *     Connection(const Connection&);
 *     Connection& operator=(const Connection&);
 * }
 * @endcode
 *
 * Although manually declaring private copy constructor and assignment functions
 * works, it is not ideal. These declarations are usually easy to forget,
 * not immediately visible, and may be obscure to uninformed programmers.
 *
 * Using the NonCopyable class reduces the boilerplate required and expresses
 * the intent because class inheritance appears right after the class name
 * declaration.
 *
 * @code
 * struct Connection : private NonCopyable<Connection> {
 *      // regular declarations
 * }
 * @endcode
 *
 *
 * @par Implementation details
 *
 * Using a template type prevents cases where the empty base optimization cannot
 * be applied and therefore ensures that the cost of the NonCopyable semantic
 * sugar is null.
 *
 * As an example, the empty base optimization is prohibited if one of the empty
 * base classes is also a base type of the first nonstatic data member:
 *
 * @code
 * struct A { };
 * struct B : A {
 *    int foo;
 * };
 * // thanks to empty base optimization, sizeof(B) == sizeof(int)
 *
 * struct C : A {
 *   B b;
 * };
 *
 * // empty base optimization cannot be applied here because A from C and A from
 * // B have a different address. In that case, with the alignment
 * // sizeof(C) == 2* sizeof(int)
 * @endcode
 *
 * The solution to that problem is to templatize the empty class to make it
 * unique to the type it is applied to:
 *
 * @code
 * template<typename T>
 * struct A<T> { };
 * struct B : A<B> {
 *    int foo;
 * };
 * struct C : A<C> {
 *   B b;
 * };
 *
 * // empty base optimization can be applied B and C does not refer to the same
 * // kind of A. sizeof(C) == sizeof(B) == sizeof(int).
 * @endcode
 *
 * @tparam T The type that should be made noncopyable.
 *
 * @note Compile time errors are disabled if you use the develop or release profile.
 * To override this behavior and force compile time errors in all profiles,
 * set the configuration parameter "platform.force-non-copyable-error" to true.
 */
template<typename T>
class NonCopyable {
#ifndef DOXYGEN_ONLY
protected:
    /**
     * Disallow construction of NonCopyable objects from outside of its hierarchy.
     */
    NonCopyable() { }
    /**
     * Disallow destruction of NonCopyable objects from outside of its hierarchy.
     */
    ~NonCopyable() { }

#if (!defined(MBED_DEBUG) && (MBED_CONF_PLATFORM_FORCE_NON_COPYABLE_ERROR == 0))
    /**
     * NonCopyable copy constructor.
     *
     * A compile time warning is issued when this function is used, and a runtime
     * warning is printed when the copy construction of the noncopyable happens.
     *
     * If you see this warning, your code is probably doing something unspecified.
     * Copying of noncopyable resources can lead to resource leak and random error.
     */
    MBED_DEPRECATED("Invalid copy construction of a NonCopyable resource.")
    NonCopyable(const NonCopyable &)
    {
        debug("Invalid copy construction of a NonCopyable resource: %s\r\n", MBED_PRETTY_FUNCTION);
    }

    /**
     * NonCopyable copy assignment operator.
     *
     * A compile time warning is issued when this function is used, and a runtime
     * warning is printed when the copy construction of the noncopyable happens.
     *
     * If you see this warning, your code is probably doing something unspecified.
     * Copying of noncopyable resources can lead to resource leak and random error.
     */
    MBED_DEPRECATED("Invalid copy assignment of a NonCopyable resource.")
    NonCopyable &operator=(const NonCopyable &)
    {
        debug("Invalid copy assignment of a NonCopyable resource: %s\r\n", MBED_PRETTY_FUNCTION);
        return *this;
    }

#else
private:
    /**
     * Declare copy constructor as private. Any attempt to copy construct
     * a NonCopyable will fail at compile time.
     */
    NonCopyable(const NonCopyable &);

    /**
     * Declare copy assignment operator as private. Any attempt to copy assign
     * a NonCopyable will fail at compile time.
     */
    NonCopyable &operator=(const NonCopyable &);
#endif
#endif
};

/**@}*/

/**@}*/

} // namespace mbed

#endif /* MBED_NONCOPYABLE_H_ */
