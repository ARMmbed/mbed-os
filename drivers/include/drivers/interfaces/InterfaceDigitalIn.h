/*
 * Mbed-OS Microcontroller Library
 * Copyright (c) 2021 Embedded Planet
 * Copyright (c) 2021 ARM Limited
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
 * limitations under the License
 */

#ifndef MBED_INTERFACE_DIGITALIN_H_
#define MBED_INTERFACE_DIGITALIN_H_

#include "PinNames.h"

namespace mbed {

class DigitalIn;

namespace interface {

#ifdef FEATURE_EXPERIMENTAL_API

// TODO - move method doxygen comments to interface once this polymorphism is mainstream

// Pure interface definition for DigitalIn
struct DigitalIn {
    virtual ~DigitalIn() = default;
    virtual int read() = 0;
    virtual void mode(PinMode pull) = 0;
    virtual int is_connected() = 0;

    operator int()
    {
        // Underlying implementation is responsible for thread-safety
        return read();
    }

};
#else
using DigitalIn = ::mbed::DigitalIn;
#endif /* FEATURE_EXPERIMENTAL_API */

} // namespace interface
} // namespace mbed

#endif /* MBED_INTERFACE_DIGITALIN_H_ */
