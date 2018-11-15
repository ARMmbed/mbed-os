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

#ifndef BLE_COMMON_DURATION_H_
#define BLE_COMMON_DURATION_H_

#include <stdint.h>
#include <stddef.h>
#include "platform/mbed_assert.h"

namespace ble {

template<uint32_t Min, uint32_t Max>
struct Range {
    enum {
        MIN = Min,
        MAX = Max
    };
};

template<typename Rep>
struct DefaultRange;

template<>
struct DefaultRange<uint8_t> {
    typedef Range<0, 0xFF> type;
};

template<>
struct DefaultRange<uint16_t > {
    typedef Range<0, 0xFFFF> type;
};

template<>
struct DefaultRange<uint32_t> {
    typedef Range<0, 0xFFFFFFFF> type;
};


template<typename Rep, uint32_t TB, typename Range = typename DefaultRange<Rep>::type >
struct Duration {
    Duration() : duration() { }

    explicit Duration(Rep v) : duration(clamp(v)) { }

    template<typename OtherRep, uint32_t OtherTB, typename OtherRange>
    Duration(Duration<OtherRep, OtherTB, OtherRange> other) :
        duration(clamp(other.value() * (OtherTB / TB)))
    {
        MBED_STATIC_ASSERT(OtherTB >= TB && (OtherTB % TB) == 0, "Incompatible units");
    }

    template<typename OtherRep>
    explicit Duration(Duration<OtherRep, 1000> other_ms, void* = NULL) :
        duration(clamp(((other_ms.value() * 1000) + TB - 1) / TB))
    { }

    Rep value() {
        return duration;
    }

    enum {
        TIME_BASE = TB,
        MIN = Range::MIN,
        MAX = Range::MAX
    };

    static Duration min()
    {
        return Duration(MIN);
    }

    static Duration max()
    {
        return Duration(MAX);
    }

    const Rep* storage() const
    {
        return &duration;
    }

private:
    static Rep clamp(Rep in) {
        if (in < MIN) {
            return MIN;
        } else if (in > MAX) {
            return MAX;
        } else {
            return in;
        }
    }

    Rep duration;
};

typedef Duration<uint32_t, 1> microsecond_t;
typedef Duration<uint32_t, 1000 * microsecond_t::TIME_BASE> millisecond_t;
typedef Duration<uint32_t, 1000 * millisecond_t::TIME_BASE> second_t;

template<typename DurationOut, typename RepIn, uint32_t TBIn, typename RangeIn>
DurationOut durationCast(Duration<RepIn, TBIn, RangeIn> duration) {
    return DurationOut(((duration.value() * TBIn) + DurationOut::TIME_BASE - 1) / DurationOut::TIME_BASE);
}

// ADDITION OPERATOR
template<typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename RepRHS, uint32_t TBRHS, typename RangeRHS>
microsecond_t  operator+(Duration<RepLHS, TBLHS, RangeLHS> lhs, Duration<RepRHS, TBRHS, RangeRHS> rhs) {
    return microsecond_t((lhs.value() * lhs.TIME_BASE) + (rhs.value() * rhs.TIME_BASE));
}

template<typename Rep, uint32_t TB, typename Range>
Duration<Rep, TB, Range> operator+(Duration<Rep, TB, Range> lhs, Duration<Rep, TB> rhs) {
    return Duration<Rep, TB, Range>(lhs.value() + rhs.value());
}

// MULTIPLICATION OPERATOR

template<typename Rep, uint32_t TB, typename Range>
Duration<Rep, TB, Range> operator*(Duration<Rep, TB, Range> lhs, uint32_t rhs) {
    return Duration<Rep, TB, Range>(lhs.value() * rhs);
}

template<typename Rep, uint32_t TB, typename Range>
Duration<Rep, TB, Range> operator*(uint32_t lhs, Duration<Rep, TB, Range> rhs) {
    return Duration<Rep, TB, Range>(lhs * rhs.value());
}

// LESS THAN

template<typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename RepRHS, uint32_t TBRHS, typename RangeRHS>
bool operator<(Duration<RepLHS, TBLHS, RangeLHS> lhs, Duration<RepRHS, TBRHS, RangeRHS> rhs) {
    return lhs.value() * lhs.TIME_BASE < rhs.value() * rhs.TIME_BASE;
}

template<typename Rep, uint32_t Us, typename Range>
bool operator<(Duration<Rep, Us, Range> lhs, Duration<Rep, Us, Range> rhs) {
    return lhs.value() < rhs.value();
}

// LESS OR EQUAL TO

template<typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename RepRHS, uint32_t TBRHS, typename RangeRHS>
bool operator<=(Duration<RepLHS, TBLHS, RangeLHS> lhs, Duration<RepRHS, TBRHS, RangeRHS> rhs) {
    return lhs.value() * lhs.TIME_BASE <= rhs.value() * rhs.TIME_BASE;
}

template<typename Rep, uint32_t Us, typename Range>
bool operator<=(Duration<Rep, Us, Range> lhs, Duration<Rep, Us, Range> rhs) {
    return lhs.value() <= rhs.value();
}

// EQUAL

template<typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename RepRHS, uint32_t TBRHS, typename RangeRHS>
bool operator==(Duration<RepLHS, TBLHS, RangeLHS> lhs, Duration<RepRHS, TBRHS, RangeRHS> rhs) {
    return lhs.value() * lhs.TIME_BASE == rhs.value() * rhs.TIME_BASE;
}

template<typename Rep, uint32_t Us, typename Range>
bool operator==(Duration<Rep, Us, Range> lhs, Duration<Rep, Us, Range> rhs) {
    return lhs.value() == rhs.value();
}

// NOT EQUAL

template<typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename RepRHS, uint32_t TBRHS, typename RangeRHS>
bool operator!=(Duration<RepLHS, TBLHS, RangeLHS> lhs, Duration<RepRHS, TBRHS, RangeRHS> rhs) {
    return !(lhs == rhs);
}

template<typename Rep, uint32_t Us, typename Range>
bool operator!=(Duration<Rep, Us, Range> lhs, Duration<Rep, Us, Range> rhs) {
    return !(lhs == rhs);
}

// GREATER OR EQUAL

template<typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename RepRHS, uint32_t TBRHS, typename RangeRHS>
bool operator>=(Duration<RepLHS, TBLHS, RangeLHS> lhs, Duration<RepRHS, TBRHS, RangeRHS> rhs) {
    return rhs <= lhs;
}

template<typename Rep, uint32_t Us, typename Range>
bool operator>=(Duration<Rep, Us, Range> lhs, Duration<Rep, Us, Range> rhs) {
    return rhs <= lhs;
}

// GREATER THAN

template<typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename RepRHS, uint32_t TBRHS, typename RangeRHS>
bool operator>(Duration<RepLHS, TBLHS, RangeLHS> lhs, Duration<RepRHS, TBRHS, RangeRHS> rhs) {
    return rhs < lhs;
}

template<typename Rep, uint32_t Us, typename Range>
bool operator>(Duration<Rep, Us, Range> lhs, Duration<Rep, Us, Range> rhs) {
    return rhs < lhs;
}

}

#endif //BLE_COMMON_DURATION_H_
