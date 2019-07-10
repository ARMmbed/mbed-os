/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
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
#ifndef __move_h
#define __move_h

namespace std
{
template <typename T>
struct remove_reference { using type = T; };

template <typename T>
struct remove_reference<T &> { using type = T; };

template <typename T>
struct remove_reference<T &&> { using type = T; };

template <typename T>
using remove_reference_t = typename remove_reference<T>::type;

// Note that these implementations do not function as
// constant expressions in ARM C 5, so are not marked constexpr.
// This makes them C++11 compliant, but not C++14.

// [forward]
template <typename _TypeT>
_TypeT &&forward(remove_reference_t<_TypeT> &__t) noexcept
{
    return static_cast<_TypeT &&>(__t);
}

template <typename _TypeT>
_TypeT &&forward(remove_reference_t<_TypeT> &&__t) noexcept
{
    return static_cast<_TypeT &&>(__t);
}

template <typename _TypeT>
remove_reference_t<_TypeT> &&move(_TypeT &&__t) noexcept
{
    return static_cast<remove_reference_t<_TypeT> &&>(__t);
}

// [utility.exchange]
template <typename _TypeT, typename _TypeU = _TypeT>
_TypeT exchange(_TypeT &__obj, _TypeU &&__new_val)
{
    _TypeT __old_val = std::move(__obj);
    __obj = std::forward<_TypeU>(__new_val);
    return __old_val;
}

} // namespace std

#endif /* __move_h */
