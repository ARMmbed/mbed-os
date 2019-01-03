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

#if !defined(DOXYGEN_ONLY)

/**
 * Define a compile time range.
 * @tparam Min left-bound
 * @tparam Max right-bound
 */
template<uint32_t Min, uint32_t Max>
struct Range {
    static const uint32_t MIN = Min;
    static const uint32_t MAX = Max;
};

/**
 * Deduce default range for C++ basic integer types.
 *
 *
 * @tparam Rep The C++ integer type.
 */
template<typename Rep>
struct DefaultRange;

/**
 * DefaultRange specialization for uint8_t.
 */
template<>
struct DefaultRange<uint8_t> {
    typedef Range<0, 0xFF> type;
};

/**
 * DefaultRange specialization for uint16_t.
 */
template<>
struct DefaultRange<uint16_t > {
    typedef Range<0, 0xFFFF> type;
};

/**
 * DefaultRange specialization for uint32_t
 */
template<>
struct DefaultRange<uint32_t> {
    typedef Range<0, 0xFFFFFFFF> type;
};

/**
 * Represent an integral compile time value that can be used in Duration.
 *
 * @tparam T Type of the integral value.
 * @tparam V The integer value representing a never ending duration.
 */
template<typename T, T V>
struct Value {
    static const T VALUE = V;
};

#endif

/**
 * Model BLE durations.
 *
 * @tparam Rep The representation type of the duration.
 * @tparam TB The time base in micro seconds.
 * @tparam Range Closed interval of the duration
 * @tparam Forever The special value (if applicable) that represents a forever
 * duration.
 */
template<
    typename Rep,
    uint32_t TB,
    typename Range = typename DefaultRange<Rep>::type,
    typename Forever = void*
>
struct Duration {
    /**
     * Type of the actual representation.
     */
    typedef Rep representation_t;

    /**
     * Construct a default Duration.
     *
     * It is initialized with the minimum value acceptable.
     */
    Duration() : duration(Range::MIN)
    {
    }

    /**
     * Construct a Duration from an integer value.
     *
     * @param v The value of the duration in TIME_BASE units.
     */
    explicit Duration(Rep v) : duration(clamp(v))
    {
    }

    /**
     * Construct a Duration from another Duration.
     *
     * @note The operation fail at compile time if there is a loss of precision.
     *
     * @tparam OtherRep The type used to represent the other Duration.
     * @tparam OtherTB The time base in micro seconds of the other Duration.
     * @tparam OtherRange The range of the other Duration.
     * @tparam OtherF The forever value of the other type.
     *
     * @param other The Duration used to construct this object.
     */
    template<typename OtherRep, uint32_t OtherTB, typename OtherRange, typename OtherF>
    Duration(Duration<OtherRep, OtherTB, OtherRange, OtherF> other) :
        duration(clamp(other.value() * (OtherTB / TB)))
    {
        MBED_STATIC_ASSERT(OtherTB >= TB && (OtherTB % TB) == 0, "Incompatible units");
    }

    /**
     * Construct a new Duration from a Duration in milliseconds.
     *
     * @note The result of the conversion is rounded up.
     *
     * @tparam OtherRep The representation type used by other_ms.
     * @tparam OtherRange The range used by other_ms.
     * @tparam OtherF The forever value used by other_ms.
     *
     * @param other_ms The Duration in millisecond to convert.
     */
    template<typename OtherRep, typename OtherRange, typename OtherF>
    explicit Duration(Duration<OtherRep, 1000, OtherRange, OtherF> other_ms, void* = NULL) :
        duration(clamp(((other_ms.value() * 1000) + TB - 1) / TB))
    {
    }

    /**
     * Return the duration in TB units.
     *
     * @return The duration in TB units.
     */
    Rep value() const
    {
        return duration;
    }

    /**
     * Return the duration in milliseconds.
     *
     * @return The duration in milliseconds.
     */
    uint32_t valueInMs() const
    {
        return ((uint32_t)duration * TB) / 1000;
    }

    /**
     * The time base.
     */
    static const uint32_t TIME_BASE = TB;

    /**
     * Left-bound of the duration range.
     */
    static const Rep MIN = Range::MIN;

    /**
     * Right bound of the duration range.
     */
    static const Rep MAX = Range::MAX;

    /**
     * Return the minimum duration.
     *
     * @return The minimum duration.
     */
    static Duration min()
    {
        return Duration(MIN);
    }

    /**
     * Return the maximum duration.
     *
     * @return The maximum duration.
     */
    static Duration max()
    {
        return Duration(MAX);
    }

    /**
     * Return a pointer to the value of the duration.
     *
     * @return a pointer to the value of the duration.
     */
    const Rep* storage() const
    {
        return &duration;
    }

    /**
     * Return the Duration value meaning forever.
     * @return the Duration value meaning forever.
     */
    static Duration forever()
    {
        return Duration(Forever::VALUE);
    }

private:
    static Rep clamp(Rep in)
    {
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

/**
 * Type that represents micro seconds.
 */
typedef Duration<uint32_t, 1> microsecond_t;

/**
 * Type that represents milliseconds.
 */
typedef Duration<uint32_t, 1000 * microsecond_t::TIME_BASE> millisecond_t;

/**
 * Type that represents seconds.
 */
typedef Duration<uint32_t, 1000 * millisecond_t::TIME_BASE> second_t;

/**
 * Cast a duration to another.
 *
 * @tparam DurationOut Type of the Duration in output.
 * @tparam RepIn The representation type of duration.
 * @tparam TBIn The timebase of duration.
 * @tparam RangeIn The range of duration.
 * @tparam FIn The Forever value of duration.
 * @param duration The duration to convert.
 * @return The converted duration. It is rounded up if precision is lost.
 *
 * @related Duration
 */
template<typename DurationOut, typename RepIn, uint32_t TBIn, typename RangeIn, typename FIn>
DurationOut durationCast(Duration<RepIn, TBIn, RangeIn, FIn> duration)
{
    return DurationOut(((duration.value() * TBIn) + DurationOut::TIME_BASE - 1) / DurationOut::TIME_BASE);
}

/**
 * Add two durations together and return the result in microseconds.
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return The result of the addition of the two durations in microseconds.
 *
 * @related Duration
 */
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

/**
 * Add two durations together.
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return The addition of the two durations in input.
 *
 * @related Duration
 */
template<typename Rep, uint32_t TB, typename Range, typename F>
Duration<Rep, TB, Range, F> operator+(
    Duration<Rep, TB, Range, F> lhs,
    Duration<Rep, TB, Range, F> rhs
)
{
    return Duration<Rep, TB, Range, F>(lhs.value() + rhs.value());
}

/**
 * Multiply a duration and a positive integer.
 *
 * @param lhs The duration.
 * @param rhs The integer.
 *
 * @return A duration that represents the multiplication of lhs with rhs.
 *
 * @related Duration
 */
template<typename Rep, uint32_t TB, typename Range, typename F>
Duration<Rep, TB, Range, F> operator*(Duration<Rep, TB, Range, F> lhs, uint32_t rhs)
{
    return Duration<Rep, TB, Range, F>(lhs.value() * rhs);
}

/**
 * Multiply a duration and a positive integer.
 *
 * @param lhs The integer.
 * @param rhs The multiplication.
 *
 * @return A duration that represents the multiplication of lhs with rhs.
 *
 * @related Duration
 */
template<typename Rep, uint32_t TB, typename Range, typename F>
Duration<Rep, TB, Range, F> operator*(uint32_t lhs, Duration<Rep, TB, Range, F> rhs)
{
    return Duration<Rep, TB, Range, F>(lhs * rhs.value());
}

/**
 * Indicate if the duration lhs is less than the duration rhs.
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return true if lhs is less than rhs and false otherwise.
 *
 * @related Duration
 */
template<
    typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename FLHS,
    typename RepRHS, uint32_t TBRHS, typename RangeRHS, typename FRHS
>
bool operator<(Duration<RepLHS, TBLHS, RangeLHS, FLHS> lhs, Duration<RepRHS, TBRHS, RangeRHS, FRHS> rhs)
{
    return lhs.value() * lhs.TIME_BASE < rhs.value() * rhs.TIME_BASE;
}

/**
 * Indicate if the duration lhs is less than the duration rhs.
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return true if lhs is less than rhs and false otherwise.
 *
 * @related Duration
 */
template<typename Rep, uint32_t Us, typename Range, typename F>
bool operator<(Duration<Rep, Us, Range, F> lhs, Duration<Rep, Us, Range, F> rhs)
{
    return lhs.value() < rhs.value();
}

/**
 * Indicate if the duration lhs is less than or equal to the duration rhs.
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return true if lhs is less than or equal to rhs and false otherwise.
 *
 * @related Duration
 */
template<
    typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename FLHS,
    typename RepRHS, uint32_t TBRHS, typename RangeRHS, typename FRHS
>
bool operator<=(
    Duration<RepLHS, TBLHS, RangeLHS, FLHS> lhs,
    Duration<RepRHS, TBRHS, RangeRHS, FRHS> rhs
)
{
    return lhs.value() * lhs.TIME_BASE <= rhs.value() * rhs.TIME_BASE;
}

/**
 * Indicate if the duration lhs is less than or equal to the duration rhs.
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return true if lhs is less than or equal to rhs and false otherwise.
 *
 * @related Duration
 */
template<typename Rep, uint32_t Us, typename Range>
bool operator<=(Duration<Rep, Us, Range> lhs, Duration<Rep, Us, Range> rhs)
{
    return lhs.value() <= rhs.value();
}

/**
 * Indicate if the duration lhs is equal to the duration rhs.
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return true if lhs is equal to rhs and false otherwise.
 *
 * @related Duration
 */
template<
    typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename FLHS,
    typename RepRHS, uint32_t TBRHS, typename RangeRHS, typename FRHS
>
bool operator==(
    Duration<RepLHS, TBLHS, RangeLHS, FLHS> lhs,
    Duration<RepRHS, TBRHS, RangeRHS, FRHS> rhs
)
{
    return lhs.value() * lhs.TIME_BASE == rhs.value() * rhs.TIME_BASE;
}

/**
 * Indicate if the duration lhs is equal to the duration rhs.
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return true if lhs is equal to rhs and false otherwise.
 *
 * @related Duration
 */
template<typename Rep, uint32_t Us, typename Range, typename F>
bool operator==(Duration<Rep, Us, Range, F> lhs, Duration<Rep, Us, Range, F> rhs)
{
    return lhs.value() == rhs.value();
}

/**
 * Indicate if the duration lhs is not equal to the duration rhs.
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return true if lhs is not equal to rhs and false otherwise.
 *
 * @related Duration
 */
template<
    typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename FLHS,
    typename RepRHS, uint32_t TBRHS, typename RangeRHS, typename FRHS
>
bool operator!=(
    Duration<RepLHS, TBLHS, RangeLHS, FLHS> lhs,
    Duration<RepRHS, TBRHS, RangeRHS, FRHS> rhs
)
{
    return !(lhs == rhs);
}

/**
 * Indicate if the duration lhs is not equal to the duration rhs.
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return true if lhs is not equal to rhs and false otherwise.
 *
 * @related Duration
 */
template<typename Rep, uint32_t Us, typename Range, typename F>
bool operator!=(Duration<Rep, Us, Range, F> lhs, Duration<Rep, Us, Range, F> rhs)
{
    return !(lhs == rhs);
}

/**
 * Indicate if the duration lhs greater or equal to the duration rhs.
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return true if lhs is greater or equal to rhs and false otherwise.
 *
 * @related Duration
 */
template<
    typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename FLHS,
    typename RepRHS, uint32_t TBRHS, typename RangeRHS, typename FRHS
>
bool operator>=(
    Duration<RepLHS, TBLHS, RangeLHS, FLHS> lhs,
    Duration<RepRHS, TBRHS, RangeRHS, FRHS> rhs
)
{
    return rhs <= lhs;
}

/**
 * Indicate if the duration lhs greater or equal to the duration rhs.
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return true if lhs is greater or equal to rhs and false otherwise.
 *
 * @related Duration
 */
template<typename Rep, uint32_t Us, typename Range, typename F>
bool operator>=(Duration<Rep, Us, Range, F> lhs, Duration<Rep, Us, Range, F> rhs)
{
    return rhs <= lhs;
}

/**
 * Indicate if the duration lhs greater than the duration rhs.
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return true if lhs is greater than rhs and false otherwise.
 *
 * @related Duration
 */
template<
    typename RepLHS, uint32_t TBLHS, typename RangeLHS, typename FLHS,
    typename RepRHS, uint32_t TBRHS, typename RangeRHS, typename FRHS
>
bool operator>(
    Duration<RepLHS, TBLHS, RangeLHS, FLHS> lhs,
    Duration<RepRHS, TBRHS, RangeRHS, FRHS> rhs
)
{
    return rhs < lhs;
}

/**
 * Indicate if the duration lhs greater than the duration rhs.
 * @param lhs Left hand side operand.
 * @param rhs Right hand side operand.
 * @return true if lhs is greater than rhs and false otherwise.
 *
 * @related Duration
 */
template<typename Rep, uint32_t Us, typename Range, typename F>
bool operator>(Duration<Rep, Us, Range, F> lhs, Duration<Rep, Us, Range, F> rhs)
{
    return rhs < lhs;
}

/* ---------------------- Static variable initialization -------------------- */

#if !defined(DOXYGEN_ONLY)

template<uint32_t Min, uint32_t Max>
const uint32_t Range<Min, Max>::MIN;

template<uint32_t Min, uint32_t Max>
const uint32_t Range<Min, Max>::MAX;

template<typename T, T V>
const T Value<T, V>::VALUE;

#endif

template<typename Rep, uint32_t TB, typename Range, typename Forever>
const uint32_t Duration<Rep, TB, Range, Forever>::TIME_BASE;

template<typename Rep, uint32_t TB, typename Range, typename Forever>
const Rep Duration<Rep, TB, Range, Forever>::MIN;

template<typename Rep, uint32_t TB, typename Range, typename Forever>
const Rep Duration<Rep, TB, Range, Forever>::MAX;

}

#endif //BLE_COMMON_DURATION_H_
