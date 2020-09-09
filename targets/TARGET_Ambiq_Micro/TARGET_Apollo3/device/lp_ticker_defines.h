/*
 * Copyright (c) 2020 SparkFun Electronics
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
// #ifndef MBED_US_TICKER_DEFINES_H
// #define MBED_US_TICKER_DEFINES_H

#include "am_mcu_apollo.h"

#define LP_TICKER_FREQ 32768
#define LP_TICKER_AM_HAL_CTIMER_SRC AM_HAL_CTIMER_XT_32_768KHZ

#define LP_TICKER_AM_HAL_CTIMER_SEGMENT_TIME_KEEPER AM_HAL_CTIMER_TIMERA
#define LP_TICKER_AM_HAL_CTIMER_SEGMENT_INT_COUNTER AM_HAL_CTIMER_TIMERB
#define LP_TICKER_AM_HAL_CTIMER_NUMBER 7
#define LP_TICKER_AM_HAL_CTIMER_TIME_KEEPER_FN AM_HAL_CTIMER_FN_CONTINUOUS
#define LP_TICKER_AM_HAL_CTIMER_INT_COUNTER_FN AM_HAL_CTIMER_FN_ONCE
#define LP_TICKER_AM_HAL_CTIMER_CMPR_REG 0 // CMPR0 reg used with CTIMER_FN_CONTINUOUS mode
#define LP_TICKER_AM_HAL_CTIMER_CMPR_INT AM_HAL_CTIMER_INT_TIMERB7C0
//#define LP_TICKER_AM_HAL_CTIMER_CMPR_INT1 AM_HAL_CTIMER_INT_TIMERA7C1

// Automatic configuration

#if (LP_TICKER_AM_HAL_CTIMER_SEGMENT == AM_HAL_CTIMER_BOTH)
#define LP_TICKER_BITS 32
#else
#define LP_TICKER_BITS 16
#endif // (LP_TICKER_AM_HAL_CTIMER_SEGMENT == AM_HAL_CTIMER_BOTH)

// #endif // MBED_US_TICKER_DEFINES_H
