/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
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
#ifndef MBED_FUNCTIONPOINTER_H
#define MBED_FUNCTIONPOINTER_H

#include "Callback.h"
#include <string.h>
#include <stdint.h>

namespace mbed {


// Declarations for backwards compatibility
// To be foward compatible, code should adopt the Callback class
template <typename R, typename A1>
class FunctionPointerArg1 : public Callback<R(A1)> {
public:
    FunctionPointerArg1(R (*function)(A1) = 0)
        : Callback<R(A1)>(function) {}

    template<typename T>
    FunctionPointerArg1(T *object, R (T::*member)(A1))
        : Callback<R(A1)>(object, member) {}

    R (*get_function())(A1) {
        return *reinterpret_cast<R (**)(A1)>(this);
    }
};

template <typename R>
class FunctionPointerArg1<R, void> : public Callback<R()> {
public:
    FunctionPointerArg1(R (*function)() = 0)
        : Callback<R()>(function) {}

    template<typename T>
    FunctionPointerArg1(T *object, R (T::*member)())
        : Callback<R()>(object, member) {}

    R (*get_function())() {
        return *reinterpret_cast<R (**)()>(this);
    }
};

typedef FunctionPointerArg1<void, void> FunctionPointer;


} // namespace mbed

#endif
