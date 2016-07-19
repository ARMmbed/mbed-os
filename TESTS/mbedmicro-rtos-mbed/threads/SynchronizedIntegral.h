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

#ifndef MBEDMICRO_RTOS_MBED_THREADS_SYNCHRONIZED_INTEGRAL
#define MBEDMICRO_RTOS_MBED_THREADS_SYNCHRONIZED_INTEGRAL

#include <rtos.h>
#include "LockGuard.h"

/**
 * Thread safe wrapper for integral types.
 * @tparam T type of the integral
 */
template<typename T>
class SynchronizedIntegral {
public:
    SynchronizedIntegral(T value) : _mutex(), _value(value) { }

    // preincrement operator
    T operator++() {
        LockGuard lock(_mutex);
        return ++_value;
    }

    // predecrement operator
    T operator--() {
        LockGuard lock(_mutex);
        return --_value;
    }

    // post increment operator
    T operator++(int) {
        LockGuard lock(_mutex);
        return _value++;
    }

    // post decrement operator
    T operator--(int) {
        LockGuard lock(_mutex);
        return _value--;
    }

    // conversion operator, used also for <,>,<=,>=,== and !=
    operator T()  const {
        LockGuard lock(_mutex);
        return _value;
    }

    // access to the internal mutex
    rtos::Mutex& internal_mutex() {
        return _mutex;
    }

private:
    mutable rtos::Mutex _mutex;
    T _value;
};

#endif /* MBEDMICRO_RTOS_MBED_THREADS_SYNCHRONIZED_INTEGRAL */
