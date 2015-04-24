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
#include "CallbackPointer.h"

namespace mbed {

CallbackPointer::CallbackPointer(void (*function)(int event)): _function(),
                                                          _object(),
                                                          _membercaller() {
    attach(function);
}

void CallbackPointer::attach(void (*function)(int)) {
    _function = function;
    _object = 0;
}

void CallbackPointer::call(int event) {
    if (_function) {
        _function(event);
    } else if (_object) {
        _membercaller(_object, _member, event);
    }
}

#ifdef MBED_OPERATORS
void FunctionPointer::operator ()(void) {
    call();
}
#endif

} // namespace mbed
