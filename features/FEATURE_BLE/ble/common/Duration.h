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

template<uint32_t V>
struct Forever {
    static const uint32_t VALUE = V;
};


template<
    typename Rep,
    uint32_t TB,
    typename Range = typename DefaultRange<Rep>::type,
    typename Forever = void*
>
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

    static Duration forever()
    {
        return Duration(Forever::VALUE);
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

template<typename DurationOut, typename RepIn, uint32_t TBIn, typename RangeIn, typename FIn>
DurationOut durationCast(Duration<RepIn, TBIn, RangeIn, FIn> duration) {
    return DurationOut(((duration.value() * TBIn) + DurationOut::TIME_BASE - 1) / DurationOut::TIME_BASE);
}

// ADDITION OPERATOR
template<
    typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename FLHS,
    typename RepRHS, uint32_t TBRHS, typename RangeRHS, typename FRHS>
microsecond_t  operator+(
    Duration<RepLHS, TBLHS, RangeLHS, FLHS> lhs,
    Duration<RepRHS, TBRHS, RangeRHS, FRHS> rhs
)
{
    return microsecond_t((lhs.value() * lhs.TIME_BASE) + (rhs.value() * rhs.TIME_BASE));
}

template<typename Rep, uint32_t TB, typename Range, typename F>
Duration<Rep, TB, Range, F> operator+(
    Duration<Rep, TB, Range, F> lhs,
    Duration<Rep, TB, Range, F> rhs
) {
    return Duration<Rep, TB, Range, F>(lhs.value() + rhs.value());
}

// MULTIPLICATION OPERATOR

template<typename Rep, uint32_t TB, typename Range, typename F>
Duration<Rep, TB, Range, F> operator*(Duration<Rep, TB, Range, F> lhs, uint32_t rhs) {
    return Duration<Rep, TB, Range, F>(lhs.value() * rhs);
}

template<typename Rep, uint32_t TB, typename Range, typename F>
Duration<Rep, TB, Range, F> operator*(uint32_t lhs, Duration<Rep, TB, Range, F> rhs) {
    return Duration<Rep, TB, Range, F>(lhs * rhs.value());
}

// LESS THAN

template<
    typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename FLHS,
    typename RepRHS, uint32_t TBRHS, typename RangeRHS, typename FRHS
>
bool operator<(Duration<RepLHS, TBLHS, RangeLHS, FLHS> lhs, Duration<RepRHS, TBRHS, RangeRHS, FRHS> rhs) {
    return lhs.value() * lhs.TIME_BASE < rhs.value() * rhs.TIME_BASE;
}

template<typename Rep, uint32_t Us, typename Range, typename F>
bool operator<(Duration<Rep, Us, Range, F> lhs, Duration<Rep, Us, Range, F> rhs) {
    return lhs.value() < rhs.value();
}

// LESS OR EQUAL TO

template<
    typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename FLHS,
    typename RepRHS, uint32_t TBRHS, typename RangeRHS, typename FRHS
>
bool operator<=(
    Duration<RepLHS, TBLHS, RangeLHS, FLHS> lhs,
    Duration<RepRHS, TBRHS, RangeRHS, FRHS> rhs
) {
    return lhs.value() * lhs.TIME_BASE <= rhs.value() * rhs.TIME_BASE;
}

template<typename Rep, uint32_t Us, typename Range>
bool operator<=(Duration<Rep, Us, Range> lhs, Duration<Rep, Us, Range> rhs) {
    return lhs.value() <= rhs.value();
}

// EQUAL

template<
    typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename FLHS,
    typename RepRHS, uint32_t TBRHS, typename RangeRHS, typename FRHS
>
bool operator==(
    Duration<RepLHS, TBLHS, RangeLHS, FLHS> lhs,
    Duration<RepRHS, TBRHS, RangeRHS, FRHS> rhs
) {
    return lhs.value() * lhs.TIME_BASE == rhs.value() * rhs.TIME_BASE;
}

template<typename Rep, uint32_t Us, typename Range, typename F>
bool operator==(Duration<Rep, Us, Range, F> lhs, Duration<Rep, Us, Range, F> rhs) {
    return lhs.value() == rhs.value();
}

// NOT EQUAL

template<
    typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename FLHS,
    typename RepRHS, uint32_t TBRHS, typename RangeRHS, typename FRHS
>
bool operator!=(
    Duration<RepLHS, TBLHS, RangeLHS, FLHS> lhs,
    Duration<RepRHS, TBRHS, RangeRHS, FRHS> rhs
) {
    return !(lhs == rhs);
}

template<typename Rep, uint32_t Us, typename Range, typename F>
bool operator!=(Duration<Rep, Us, Range, F> lhs, Duration<Rep, Us, Range, F> rhs) {
    return !(lhs == rhs);
}

// GREATER OR EQUAL

template<
    typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename FLHS,
    typename RepRHS, uint32_t TBRHS, typename RangeRHS, typename FRHS
>
bool operator>=(
    Duration<RepLHS, TBLHS, RangeLHS, FLHS> lhs,
    Duration<RepRHS, TBRHS, RangeRHS, FRHS> rhs
) {
    return rhs <= lhs;
}

template<typename Rep, uint32_t Us, typename Range, typename F>
bool operator>=(Duration<Rep, Us, Range, F> lhs, Duration<Rep, Us, Range, F> rhs) {
    return rhs <= lhs;
}

// GREATER THAN

template<
    typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename FLHS,
    typename RepRHS, uint32_t TBRHS, typename RangeRHS, typename FRHS
>
bool operator>(
    Duration<RepLHS, TBLHS, RangeLHS, FLHS> lhs,
    Duration<RepRHS, TBRHS, RangeRHS, FRHS> rhs
) {
    return rhs < lhs;
}

template<typename Rep, uint32_t Us, typename Range, typename F>
bool operator>(Duration<Rep, Us, Range, F> lhs, Duration<Rep, Us, Range, F> rhs) {
    return rhs < lhs;
}

/* ---------------------- Static variable initialization -------------------- */


template<uint32_t V>
const uint32_t Forever<V>::VALUE;

}

#endif //BLE_COMMON_DURATION_H_
