/*
 * Copyright (c) 2019, Arm Limited and affiliates.
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

#include <cmath>
#include <cstdarg>
#include "gtest/gtest.h"

/* "mbed_printf_implementation.h" does not declare it as extern "C", so pull it in manually */
extern "C" int mbed_minimal_formatted_string(char *buffer, size_t length, const char *format, va_list arguments, FILE *stream);

namespace
{
    std::string format(const char *fmt, ...)
    {
        va_list arguments;
        va_start(arguments, fmt);

        constexpr int buf_size = 128;
        char buffer[buf_size];
        int n = mbed_minimal_formatted_string(buffer, buf_size, fmt, arguments, nullptr);
        va_end(arguments); 
        
        if (n >= buf_size) {
            return "overflow";
        }

        return std::string(buffer);
    }
}

TEST(minimal_printf, floats)
{
    /* std::nextafter is used for the values below to make sure the numbers are 
       floating point representations of at least the value we want to test. 
       This hopefully mitigates false failures due to different floating point
       implementations */

    /* Positive numbers */
    float value = std::nextafter(1.5f, 0.0f); /* Something like 1.499999, i.e just below 1.5 */
    EXPECT_EQ("1", format("%.0f", value));
    EXPECT_EQ("1.5", format("%.1f", value));
    EXPECT_EQ("1.50", format("%.2f", value));
    EXPECT_EQ("1.500", format("%.3f", value));
    EXPECT_EQ("1.5000", format("%.4f", value));
    EXPECT_EQ("1.50000", format("%.5f", value));
    EXPECT_EQ("1.500000", format("%.6f", value));

    value = std::nextafter(1.5f, 2.0f); /* Something like 1.500001, i.e just above 1.5 */
    EXPECT_EQ("2", format("%.0f", value));
    EXPECT_EQ("1.5", format("%.1f", value));
    EXPECT_EQ("1.50", format("%.2f", value));
    EXPECT_EQ("1.500", format("%.3f", value));
    EXPECT_EQ("1.5000", format("%.4f", value));
    EXPECT_EQ("1.50000", format("%.5f", value));
    EXPECT_EQ("1.500000", format("%.6f", value));

    value = std::nextafter(2.0f, 0.0f); /* Something like 1.999999, i.e just below 2.0 */
    EXPECT_EQ("2", format("%.0f", value));
    EXPECT_EQ("2.0", format("%.1f", value));
    EXPECT_EQ("2.00", format("%.2f", value));
    EXPECT_EQ("2.000", format("%.3f", value));
    EXPECT_EQ("2.0000", format("%.4f", value));
    EXPECT_EQ("2.00000", format("%.5f", value));
    EXPECT_EQ("2.000000", format("%.6f", value));

    /* Negative numbers */
    value = std::nextafter(-1.5f, 0.0f); /* Something like -1.499999, i.e just above -1.5 */
    EXPECT_EQ("-1", format("%.0f", value));
    EXPECT_EQ("-1.5", format("%.1f", value));
    EXPECT_EQ("-1.50", format("%.2f", value));
    EXPECT_EQ("-1.500", format("%.3f", value));
    EXPECT_EQ("-1.5000", format("%.4f", value));
    EXPECT_EQ("-1.50000", format("%.5f", value));
    EXPECT_EQ("-1.500000", format("%.6f", value));

    value = std::nextafter(-1.5f, -2.0f); /* Something like -1.500001, i.e just below -1.5 */
    EXPECT_EQ("-2", format("%.0f", value));
    EXPECT_EQ("-1.5", format("%.1f", value));
    EXPECT_EQ("-1.50", format("%.2f", value));
    EXPECT_EQ("-1.500", format("%.3f", value));
    EXPECT_EQ("-1.5000", format("%.4f", value));
    EXPECT_EQ("-1.50000", format("%.5f", value));
    EXPECT_EQ("-1.500000", format("%.6f", value));

    value = std::nextafter(-2.0f, 0.0f);  /* Something like -1.999999, i.e just above -2.0 */
    EXPECT_EQ("-2", format("%.0f", value));
    EXPECT_EQ("-2.0", format("%.1f", value));
    EXPECT_EQ("-2.00", format("%.2f", value));
    EXPECT_EQ("-2.000", format("%.3f", value));
    EXPECT_EQ("-2.0000", format("%.4f", value));
    EXPECT_EQ("-2.00000", format("%.5f", value));
    EXPECT_EQ("-2.000000", format("%.6f", value));

    // And some near zero stuff
    EXPECT_EQ("-0", format("%.0f", std::nextafter(0.0f, -1.0f)));
    EXPECT_EQ("0", format("%.0f", 0.0));
    EXPECT_EQ("0", format("%.0f", std::nextafter(0.0f, 1.0f)));
}

TEST(minimal_printf, padding)
{
    // Right aligned integers
    EXPECT_EQ("12", format("%1d", 12));
    EXPECT_EQ("12", format("%2d", 12));
    EXPECT_EQ(" 12", format("%3d", 12));
    EXPECT_EQ("  12", format("%4d", 12));

    EXPECT_EQ("-12", format("%1d", -12));
    EXPECT_EQ("-12", format("%3d", -12));
    EXPECT_EQ(" -12", format("%4d", -12));
    EXPECT_EQ("  -12", format("%5d", -12));

    EXPECT_EQ("0012", format("%04d", 12));
    EXPECT_EQ("-012", format("%04d", -12));

    // Right aligned floats
    EXPECT_EQ("12.3", format("%1.1f", 12.345f));
    EXPECT_EQ("12.3", format("%4.1f", 12.345));
    EXPECT_EQ(" 12.3", format("%5.1f", 12.345));
    EXPECT_EQ("  12.3", format("%6.1f", 12.345));

    EXPECT_EQ("-12.3", format("%1.1f", -12.345f));
    EXPECT_EQ("-12.3", format("%4.1f", -12.345));
    EXPECT_EQ(" -12.3", format("%6.1f", -12.345));
    EXPECT_EQ("  -12.3", format("%7.1f", -12.345));

    EXPECT_EQ("012.3", format("%05.1f", 12.345));
    EXPECT_EQ("-012.3", format("%06.1f", -12.345));
}

