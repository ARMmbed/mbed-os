/* Copyright (c) 2017-2019 ARM Limited
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

namespace mbed {

/** \addtogroup platform-public-api */
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
 * be defined as deleted:
 *
 * @code
 * struct Connection {
 *     Connection(const Connection &) = delete;
 *     Connection &operator=(const Connection &) = delete;
 * }
 * @endcode
 *
 * Although manually defining deleted copy constructor and assignment functions
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
    NonCopyable() = default;
    /**
     * Disallow destruction of NonCopyable objects from outside of its hierarchy.
     */
    ~NonCopyable() = default;

public:
    /**
     * Define copy constructor as deleted. Any attempt to copy construct
     * a NonCopyable will fail at compile time.
     */
    NonCopyable(const NonCopyable &) = delete;

    /**
     * Define copy assignment operator as deleted. Any attempt to copy assign
     * a NonCopyable will fail at compile time.
     */
    NonCopyable &operator=(const NonCopyable &) = delete;
#endif
};

/**@}*/

/**@}*/

} // namespace mbed

#endif /* MBED_NONCOPYABLE_H_ */
