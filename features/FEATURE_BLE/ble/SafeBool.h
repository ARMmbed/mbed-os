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

#ifndef BLE_API_SAFE_BOOL_H_
#define BLE_API_SAFE_BOOL_H_

/* Safe bool idiom, see: http://www.artima.com/cppsource/safebool.html */

/**
 * @file
 * @addtogroup ble
 * @{
 * @addtogroup common
 * @{
 */

/**
 *  Private namespace used to host details of the SafeBool implementation.
 */
namespace SafeBool_ {
/**
 * Base class of all SafeBool instances.
 *
 * This nontemplate base class exists to reduce the number of instantiation of
 * the trueTag function.
 */
class base {
  template<typename>
  friend class SafeBool;

protected:
    /**
     * The bool type is a pointer to method that can be used in boolean context.
     */
    typedef void (base::*BoolType_t)() const;

    /**
     * Nonimplemented call, use to disallow conversion between unrelated types.
     */
    void invalidTag() const;

    /**
     * Special member function that indicates a true value.
     */
    void trueTag() const {}
};


}

/**
 * Safe conversion of objects in boolean context.
 *
 * Classes wanting evaluation of their instances in boolean context must derive
 * publicly from this class rather than implementing the easy to misuse
 * operator bool().
 *
 * Descendant classes must implement the function bool toBool() const to enable
 * the safe conversion in boolean context.
 *
 * @tparam T Type of the derived class
 *
 * @code
 *
 * class A : public SafeBool<A> {
 * public:
 *
 *      // boolean conversion
 *      bool toBool() const {
 *
 *      }
 * };
 *
 * class B : public SafeBool<B> {
 * public:
 *
 *      // boolean conversion
 *      bool toBool() const {
 *
 *      }
 * };
 *
 * A a;
 * B b;
 *
 * // will compile
 * if(a) {
 *
 * }
 *
 * // compilation error
 * if(a == b) {
 *
 * }
 * @endcode
 */
template <typename T>
class SafeBool : public SafeBool_::base {
public:
    /**
     * Bool operator implementation, derived class must provide a bool
     * toBool() const function.
     */
    operator BoolType_t() const
    {
        return (static_cast<const T*>(this))->toBool()
            ? &SafeBool<T>::trueTag : 0;
    }
};

/**
 * Avoid conversion to bool between different classes.
 *
 * @important Will generate a compile time error if instantiated.
 */
template <typename T, typename U>
void operator==(const SafeBool<T>& lhs,const SafeBool<U>& rhs)
{
    lhs.invalidTag();
    // return false;
}

/**
 * Avoid conversion to bool between different classes.
 *
 * @important Will generate a compile time error if instantiated.
 */
template <typename T,typename U>
void operator!=(const SafeBool<T>& lhs,const SafeBool<U>& rhs)
{
    lhs.invalidTag();
    // return false;
}

/**
 * @}
 * @}
 */


#endif /* BLE_API_SAFE_BOOL_H_ */
