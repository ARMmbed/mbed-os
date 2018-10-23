/* Copyright (c) 2017 ARM Limited
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
 * To prevent generation of copy constructor and copy assignment operator simply 
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
 * Instances of polymorphic classes are not meant to be copied. Unfortunately, 
 * the C++ standards generates a default copy constructor and copy assignment 
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
 * There is a subtile bug in this code, the function get_connection returns a 
 * reference to a Connection which is captured by value instead of reference. 
 * 
 * When the reference returned by get_connection is copied into connection, the 
 * vtable and others members defined in Connection are copied but members defined
 * in SerialConnection are left apart. This can cause severe crashes or bugs if 
 * the virtual functions captured uses members not present in the base 
 * declaration. 
 * 
 * To solve that problem, the copy constructor and assignment operator have to 
 * be declared (but doesn't need to be defined) in the private section of the 
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
 * While manually declaring private copy constructor and assignment functions 
 * works, it is not ideal as these declarations are usually not immediately 
 * visible, easy to forget and may be obscure for uninformed programmer. 
 * 
 * Using the NonCopyable class reduce the boilerplate required and express 
 * clearly the intent as class inheritance appears right after the class name
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
 * be applied and therefore ensure that the cost of the NonCopyable semantic 
 * sugar is null.
 * 
 * As an example, the empty base optimization is prohibited if one of the empty
 * base class is also a base type of the first non static data member:
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
 * // B shall have a different address. In that case, with the alignment
 * // sizeof(C) == 2* sizeof(int)
 * @endcode
 *
 * The solution to that problem is to templatize the empty class to makes it
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
 * @tparam T The type that should be made non copyable. 
 *
 * @note Compile time errors are disabled if the develop or the release profile
 * is used. To override this behavior and force compile time errors in all profile
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
     * A compile time warning is issued when this function is used and a runtime
     * warning is printed when the copy construction of the non copyable happens.
     *
     * If you see this warning, your code is probably doing something unspecified.
     * Copy of non copyable resources can lead to resource leak and random error.
     */
    MBED_DEPRECATED("Invalid copy construction of a NonCopyable resource.")
    NonCopyable(const NonCopyable &)
    {
        debug("Invalid copy construction of a NonCopyable resource: %s\r\n", MBED_PRETTY_FUNCTION);
    }

    /**
     * NonCopyable copy assignment operator.
     *
     * A compile time warning is issued when this function is used and a runtime
     * warning is printed when the copy construction of the non copyable happens.
     *
     * If you see this warning, your code is probably doing something unspecified.
     * Copy of non copyable resources can lead to resource leak and random error.
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
     * Declare copy constructor as private, any attempt to copy construct
     * a NonCopyable will fail at compile time.
     */
    NonCopyable(const NonCopyable &);

    /**
     * Declare copy assignment operator as private, any attempt to copy assign
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
