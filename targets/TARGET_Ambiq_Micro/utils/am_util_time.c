//*****************************************************************************
//
//! @file am_util_time.h
//!
//! @brief Functions useful for RTC, calendar, time, etc. computations.
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2019, Ambiq Micro
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
// This is part of revision v2.2.0-7-g63f7c2ba1 of the AmbiqSuite Development Package.
//
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include "am_util_time.h"

//*****************************************************************************
//
//! @brief Compute the day of the week given the month, day, and year.
//!
//! @param iYear  - The year of the desired date (e.g. 2016).  The smallest
//! allowed year is 2000.
//! @param iMonth - The month of the desired date (1-12).
//! @param iDay   - The day of the month of the desired date (1-31).
//!
//! This function is general in nature, but is designed to be used with the RTC.
//!
//! Note: This function is valid for the years 2000 - 2399 (it breaks at year
//! 2400 because the year 2400 is NOT a leap year).
//!
//! @returns An index value indicating the day of the week.
//! 0-6 indicate  Sun, Mon, Tue, Wed, Thu, Fri, Sat, respectively.
//! 7   indicates that the given date is invalid (e.g. 2/29/2015).
//
//*****************************************************************************
int
am_util_time_computeDayofWeek(int iYear, int iMonth, int iDay)
{
    int iDayCnt, iCnt;

    //
    // 1/1/2000 was a Saturday, which we'll use as a base date.
    // To get the day of the week of the given date, we'll compute the number
    //  of days from 1/1/2000, then offset that from Saturday (6).
    //

    //
    // Validate inputs.  Return 7 if any are out-of-bounds.
    //
    if ( (iMonth < 1) || (iMonth > 12) || (iYear < 2000) || (iYear >= 2400) || (iDay < 1) )
    {
        return 7;
    }

    if ( (iMonth == 2) && (iDay > 28) )
    {
        if ( (iDay > 29) || (iYear % 4) )
        {
            return 7;
        }
    }

    //
    // 0-base the month and date.
    //
    iMonth--;
    iDay--;

    //
    // First we'll count the number of days up to the the specified
    // month and date in the specified year.
    //
    iDayCnt = iDay;
    iCnt = 0;
    while ( iCnt < iMonth )
    {
        switch ( iCnt )
        {
            case 3:
            case 5:
            case 8:
            case 10:
                iDayCnt += 30;
                break;
            case 0:
            case 2:
            case 4:
            case 6:
            case 7:
            case 9:
            case 11:
                iDayCnt += 31;
                break;
            case 1:
                iDayCnt += (iYear % 4) ? 28 : 29;
                break;
        }
        iCnt++;
    }

    //
    // Now, add in the number of days in the intervening years between
    // 2000 and the specified year.
    //
    iCnt = 2000;
    while ( iCnt < iYear )
    {
        iDayCnt += (iCnt % 4) ? 365 : 366;
        iCnt++;
    }

    //
    // Offset the day count with the base day of the week.
    // For Saturday, this is 6.
    //
    return (iDayCnt + 6) % 7;
}

