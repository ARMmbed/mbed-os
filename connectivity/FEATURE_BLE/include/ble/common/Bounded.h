/* mbed Microcontroller Library
 * Copyright (c) 2006-2020 ARM Limited
 *
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

#ifndef BLE_COMMON_BOUNDED_H_
#define BLE_COMMON_BOUNDED_H_

#include <cstdint>

namespace ble {

/**
 * Restrict values of an integer type to a defined range.
 *
 * The range is a closed interval that includes its left-bound (Min) and
 * right-bound value (Max).
 *
 * @tparam Rep The C++ integer type used to represent the values.
 * @tparam Min Minimum value allowed.
 * @tparam Max Maximum value allowed.
 */
template<typename Rep, Rep Min, Rep Max>
struct Bounded {
    /**
     * Construct a bounded integer.
     *
     * If v is out of the range [Min : Max], then if it is less than Min, the
     * value of the bounded integer will be Min. If it greater than Max, then
     * the value of the bounded integer will be Max.
     *
     * @param v The value to store.
     */
    Bounded(Rep v) : _value(v)
    {
        if (v < Min) {
            _value = Min;
        } else if (v > Max) {
            _value = Max;
        }
    }

    /**
     * Access the inner value.
     *
     * @return The current value.
     */
    Rep value() const
    {
        return _value;
    }

    /**
     * The left-bound value.
     *
     * @return The lowest value that this type can represent
     */
    static Rep min()
    {
        return Min;
    }

    /**
     * The right-bound value.
     *
     * @return The highest value that this type can represent
     */
    static Rep max()
    {
        return Max;
    }

    /**
     * The left-bound value.
     */
    static const Rep MIN = Min;

    /**
     * The right-bound value.
     */
    static const Rep MAX = Max;

private:
    Rep _value;
};

/* ---------------------- Static variable initialization -------------------- */

template<typename T, T Min, T Max>
const T Bounded<T, Min, Max>::MIN;

template<typename T, T Min, T Max>
const T Bounded<T, Min, Max>::MAX;

} // namespace ble

#endif //BLE_COMMON_BOUNDED_H_
