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
#ifndef MBED_CALLCHAIN_H
#define MBED_CALLCHAIN_H

#include "platform/Callback.h"
#include "platform/mbed_toolchain.h"
#include "platform/NonCopyable.h"
#include <string.h>

namespace mbed {


typedef Callback<void()> *pFunctionPointer_t;
class CallChainLink;

/**
 * \defgroup platform_CallChain CallChain class
 * \ingroup platform-public-api
 * @{
 */

/** Group one or more functions in an instance of a CallChain, then call them in
 * sequence using CallChain::call(). Used mostly by the interrupt chaining code,
 * but can be used for other purposes.
 *
 * @deprecated Do not use this class. This class is not part of the public API of mbed-os and is being removed in the future.
 * @note Synchronization level: Not protected
 *
 * Example:
 * @code
 * #include "mbed.h"
 *
 * CallChain chain;
 *
 * void first(void) {
 *     printf("'first' function.\n");
 * }
 *
 * void second(void) {
 *     printf("'second' function.\n");
 * }
 *
 * class Test {
 * public:
 *     void f(void) {
 *         printf("A::f (class member).\n");
 *     }
 * };
 *
 * int main() {
 *     Test test;
 *
 *     chain.add(second);
 *     chain.add_front(first);
 *     chain.add(&test, &Test::f);
 *     chain.call();
 * }
 * @endcode
 */
class
    MBED_DEPRECATED("CallChain has been deprecated and will be removed.")
    CallChain : private NonCopyable<CallChain> {
public:
    /** Create an empty chain
     *  @deprecated
     *  Do not use this function, this class is not part of the public API of mbed-os and is being removed in the future.
     *
     *  @param size (optional) Initial size of the chain
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.6", "This class is not part of the "
                          "public API of mbed-os and is being removed in the future.")
    CallChain(int size = 4);

    /** Create an empty chain
     *  @deprecated
     *  Do not use this function, this class is not part of the public API of mbed-os and is being removed in the future.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.6", "This class is not part of the "
                          "public API of mbed-os and is being removed in the future.")
    virtual ~CallChain();

    /** Add a function at the end of the chain
     *
     *  @deprecated
     *  Do not use this function, this class is not part of the public API of mbed-os and is being removed in the future.
     *
     *  @param func A pointer to a void function
     *
     *  @returns
     *  The function object created for 'func'
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.6", "This class is not part of the "
                          "public API of mbed-os and is being removed in the future.")
    pFunctionPointer_t add(Callback<void()> func);

    /** Add a function at the end of the chain
     *
     *  @param obj pointer to the object to call the member function on
     *  @param method pointer to the member function to be called
     *
     *  @returns
     *  The function object created for 'obj' and 'method'
     *
     *  @deprecated
     *  The add function does not support cv-qualifiers. Replaced by
     *  add(callback(obj, method)).
     */
    template<typename T, typename M>
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
                          "The add function does not support cv-qualifiers. Replaced by "
                          "add(callback(obj, method)).")
    pFunctionPointer_t add(T *obj, M method)
    {
        return add(callback(obj, method));
    }

    /** Add a function at the beginning of the chain
     *  @deprecated
     *  Do not use this function, this class is not part of the public API of mbed-os and is being removed in the future.
     *
     *
     *  @param func A pointer to a void function
     *
     *  @returns
     *  The function object created for 'func'
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.6", "This class is not part of the "
                          "public API of mbed-os and is being removed in the future.")
    pFunctionPointer_t add_front(Callback<void()> func);

    /** Add a function at the beginning of the chain
     *
     *  @param obj pointer to the object to call the member function on
     *  @param method pointer to the member function to be called
     *
     *  @returns
     *  The function object created for the object and method pointers
     *
     *  @deprecated
     *  The add_front function does not support cv-qualifiers. Replaced by
     *  add_front(callback(obj, method)).
     */
    template<typename T, typename M>
    MBED_DEPRECATED_SINCE("mbed-os-5.1",
                          "The add_front function does not support cv-qualifiers. Replaced by "
                          "add_front(callback(obj, method)).")
    pFunctionPointer_t add_front(T *obj, M method)
    {
        return add_front(callback(obj, method));
    }

    /** Get the number of functions in the chain
     *  @deprecated
     *  Do not use this function, this class is not part of the public API of mbed-os and is being removed in the future.
     *
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.6", "This class is not part of the "
                          "public API of mbed-os and is being removed in the future.")
    int size() const;

    /** Get a function object from the chain
     *  @deprecated
     *  Do not use this function, this class is not part of the public API of mbed-os and is being removed in the future.
     *
     *  @param i function object index
     *
     *  @returns
     *  The function object at position 'i' in the chain
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.6", "This class is not part of the "
                          "public API of mbed-os and is being removed in the future.")
    pFunctionPointer_t get(int i) const;

    /** Look for a function object in the call chain
     *  @deprecated
     *  Do not use this function, this class is not part of the public API of mbed-os and is being removed in the future.
     *
     *  @param f the function object to search
     *
     *  @returns
     *  The index of the function object if found, -1 otherwise.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.6", "This class is not part of the "
                          "public API of mbed-os and is being removed in the future.")
    int find(pFunctionPointer_t f) const;

    /** Clear the call chain (remove all functions in the chain).
     *  @deprecated Do not use this function. This class is not part of the public API of mbed-os and is being removed in the future.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.6", "This class is not part of the "
                          "public API of mbed-os and is being removed in the future.")
    void clear();

    /** Remove a function object from the chain
     *  @deprecated
     *  Do not use this function, this class is not part of the public API of mbed-os and is being removed in the future.
     *
     *  @arg f the function object to remove
     *
     *  @returns
     *  true if the function object was found and removed, false otherwise.
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.6", "This class is not part of the "
                          "public API of mbed-os and is being removed in the future.")
    bool remove(pFunctionPointer_t f);

    /** Call all the functions in the chain in sequence
     *  @deprecated
     *  Do not use this function, this class is not part of the public API of mbed-os and is being removed in the future.
     *
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.6", "This class is not part of the "
                          "public API of mbed-os and is being removed in the future.")
    void call();

    /**
     *  @deprecated
     *  Do not use this function, this class is not part of the public API of mbed-os and is being removed in the future.
     *
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.6", "This class is not part of the "
                          "public API of mbed-os and is being removed in the future.")
    void operator()(void)
    {
        call();
    }

    /**
     *  @deprecated
     *  Do not use this function, this class is not part of the public API of mbed-os and is being removed in the future.
     *
     */
    MBED_DEPRECATED_SINCE("mbed-os-5.6", "This class is not part of the "
                          "public API of mbed-os and is being removed in the future.")
    pFunctionPointer_t operator [](int i) const
    {
        return get(i);
    }

private:
    CallChainLink *_chain;
};

/**@}*/

} // namespace mbed

#endif
