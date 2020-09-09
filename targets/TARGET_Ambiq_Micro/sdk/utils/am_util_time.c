//*****************************************************************************
//
//! @file am_util_time.h
//!
//! @brief Functions useful for RTC, calendar, time, etc. computations.
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2020, Ambiq Micro
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision 2.4.2 of the AmbiqSuite Development Package.
//
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause
#include <stdint.h>
#include <stdbool.h>
#include "am_util_time.h"

//*****************************************************************************
//
// Macro definitions.
//
//*****************************************************************************
#define AM_UTIL_TIME_IS_LEAP_YEAR(year) \
    (year % 4 == 0 && ((year % 100 != 0) || (year % 400 != 0)))

//*****************************************************************************
//
// Local variables.
//
//*****************************************************************************

//
// Numer of days in each month in a standard year.
//
const static uint32_t g_iDaysPerMonth[] =
    {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

//
// Weekday drift numbers for each month.
//
const static int g_iMonthOffsets[] =
    {4, 0, 0, 3, 5, 1, 3, 6, 2, 4, 0, 2};

//*****************************************************************************
//
//! @brief Compute the day of the week given the month, day, and year.
//!
//! @param iYear  - The year of the desired date (e.g. 2016).
//! @param iMonth - The month of the desired date (1-12).
//! @param iDay   - The day of the month of the desired date (1-31).
//!
//! This function is general in nature, but is designed to be used with the RTC.
//!
//! @returns An index value indicating the day of the week.
//! 0-6 indicate  Sun, Mon, Tue, Wed, Thu, Fri, Sat, respectively.
//! 7   indicates that the given date is invalid (e.g. 2/29/2015).
//
//*****************************************************************************
int
am_util_time_computeDayofWeek(int iYear, int iMonth, int iDay)
{
    bool bInvalidDay;
    int iYearOffset;
    int iMonthOffset;
    int iWeekday;

    int iLeapYearOffset = 0;

    //
    // Validate inputs.  Return 7 if any are out-of-bounds.
    //
    if ( (iMonth < 1) || (iMonth > 12) || (iYear < 0) || (iDay < 1) )
    {
        return 7;
    }

    //
    // Make sure this day actually exists in this month. Make sure to include
    // an exception for leap years.
    //
    if (iDay > g_iDaysPerMonth[iMonth - 1])
    {
        if (iMonth == 2 && AM_UTIL_TIME_IS_LEAP_YEAR(iYear) && iDay == 29)
        {
            bInvalidDay = false;
        }
        else
        {
            bInvalidDay = true;
        }
    }
    else
    {
        bInvalidDay = false;
    }

    if (bInvalidDay)
    {
        return 7;
    }

    iYearOffset = 2 + iYear + iYear / 4 - iYear / 100 + iYear / 400;
    iMonthOffset = g_iMonthOffsets[iMonth - 1];

    if (AM_UTIL_TIME_IS_LEAP_YEAR(iYear) && (iMonth < 3))
    {
        iLeapYearOffset = -1;
    }

    iWeekday = iDay + iYearOffset + iMonthOffset + iLeapYearOffset;

    return iWeekday % 7;
}
