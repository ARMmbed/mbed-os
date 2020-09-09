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

#ifndef MBED_US_TICKER_DEFINES_H
#define MBED_US_TICKER_DEFINES_H

//  * US_TICKER_PERIOD_NUM, US_TICKER_PERIOD_DEN: These denote the ratio (numerator, denominator)
//  * of the ticker period to a microsecond. For example, an 8MHz ticker would have NUM = 1, DEN = 8;
//  * a 1MHz ticker would have NUM = 1, DEN = 1; a 250kHz ticker would have NUM = 4, DEN = 1.
//  * Both numerator and denominator must be 16 bits or less.
#define US_TICKER_FREQ AM_HAL_STIMER_HFRC_3MHZ
#define US_TICKER_PERIOD_NUM 1
#define US_TICKER_PERIOD_DEN 3

//  *
//  * US_TICKER_MASK: The value mask for the ticker - eg 0x07FFFFFF for a 27-bit ticker.
//  *
#define US_TICKER_MASK 0xFFFFFFFF

#define US_TICKER_COMPARE_INSTANCE A
#define US_TICKER_STIMER_INT_COMPARE_CONCATENATOR(a, b) a##b
#define US_TICKER_STIMER_INT_COMPARE_EVALUATOR(a, b) US_TICKER_STIMER_INT_COMPARE_CONCATENATOR(a, b)
#define US_TICKER_STIMER_INT_COMPARE US_TICKER_STIMER_INT_COMPARE_EVALUATOR(AM_HAL_STIMER_INT_COMPARE, US_TICKER_COMPARE_INSTANCE)

#endif
