/* mbed Microcontroller Library
 * Copyright (c) 2018 ARM Limited
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

#ifndef BLE_COMMON_BOUNDED_H_
#define BLE_COMMON_BOUNDED_H_

#include <stdint.h>

namespace ble {

template<typename T, int32_t Min, int32_t Max>
struct Bounded {
    Bounded(T v) : _value(v) {
        if (v < Min) {
            _value = v;
        } else if (v > Max) {
            _value = v;
        }
    }

    T value() const {
        return _value;
    }

    static const T min = Min;
    static const T max = Max;

private:
    T _value;
};

} // namespace ble

#endif //BLE_COMMON_BOUNDED_H_
