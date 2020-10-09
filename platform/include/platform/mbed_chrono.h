
/*
 * Copyright (c) 2015-2019, ARM Limited, All Rights Reserved
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

#ifndef __MBED_CHRONO_H__
#define __MBED_CHRONO_H__

#include "mbed_toolchain.h"
#include <cstdint>
#include <cassert>
#include <ratio>
#include <chrono>

/** \addtogroup platform-public-api */
/** @{*/

/**
 * \defgroup platform_chrono chrono utilities
 *
 * Additions and variations of std::chrono
 *
 * - unsigned 32-bit variants of standard signed 64-bit duration types
 * - centiseconds and deciseconds
 * @{
 */
namespace mbed {

/* Extensions declared in mbed::chrono, following pattern of std::chrono */
namespace chrono {

/* Add deciseconds and centiseconds - may be
 * useful to use lower precision when not messing with templating.
 */
using deciseconds = std::chrono::duration<long long, std::deci>;
using centiseconds = std::chrono::duration<long long, std::centi>;

/** 32-bit microsecond duration type
 *
 * Standard std::chrono::microseconds is signed 64-bit. For some purposes
 * it's more efficient to process small times as 32-bit. And when doing so,
 * as we likely need to worry about wrapping, use of an unsigned
 * value to process modulo 2**32 is appropriate.
 */
using microseconds_u32 = std::chrono::duration<std::uint32_t, std::micro>;

/** 32-bit millisecond duration type
 *
 * Standard std::chrono::milliseconds is signed 64-bit. For some purposes
 * it's more efficient to process times as 32-bit. And when doing so,
 * as we likely need to worry about wrapping, use of an unsigned
 * value to process modulo 2**32 is appropriate.
 */
using milliseconds_u32 = std::chrono::duration<std::uint32_t, std::milli>;

} // namespace chrono

inline namespace literals {

inline namespace chrono_literals {

/** User-defined literal for deciseconds (1/10 of a second)
 *
 * Useful in case we might change kernel tick frequency to be slower - with tick frequency 1000Hz, it is
 * possible to assign 500ms to a KernelClock::duration, but that would fail at slower rates.
 *
 * Example use:
 *
 *     using namespace mbed::chrono_literals;
 *
 *     ThisThread::sleep_for(5_ds);
 */
constexpr chrono::deciseconds operator "" _ds(unsigned long long x)
{
    chrono::deciseconds::rep val = static_cast<chrono::deciseconds::rep>(x);
    assert(val >= 0 && static_cast<unsigned long long>(val) == x);
    return chrono::deciseconds(val);
}

/** User-defined literal for centiseconds (1/100 of a second)
 *
 * Useful in case we might change kernel tick frequency to be slower - with tick frequency 1000Hz, it is
 * possible to assign 500ms to a KernelClock::duration, but that would fail at slower rates.
 *
 * Example use:
 *
 *     using namespace mbed::chrono_literals;
 *
 *     ThisThread::sleep_for(1_cs);
 */
constexpr chrono::centiseconds operator "" _cs(unsigned long long x)
{
    chrono::centiseconds::rep val = static_cast<chrono::centiseconds::rep>(x);
    assert(val >= 0 && static_cast<unsigned long long>(val) == x);
    return chrono::centiseconds(val);
}

} // inline namespace chrono_literals

} // inline namespace literals

namespace chrono {

using namespace chrono_literals;

} // namespace chrono

} // namespace mbed

/**@}*/

/**@}*/

#endif // __MBED_CHRONO_H__

