//*****************************************************************************
//
//  am_hal_rtc.c
//! @file
//!
//! @brief Functions for interfacing with the Real-Time Clock (RTC).
//!
//! @addtogroup rtc3 Real-Time Clock (RTC)
//! @ingroup apollo3hal
//! @{
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
#include "am_mcu_apollo.h"

//*****************************************************************************
//
// Converts a Binary Coded Decimal (BCD) byte to its Decimal form.
//
//*****************************************************************************
static uint8_t
bcd_to_dec(uint8_t ui8BCDByte)
{
    return (((ui8BCDByte & 0xF0) >> 4) * 10) + (ui8BCDByte & 0x0F);
}

//*****************************************************************************
//
// Converts a Decimal byte to its Binary Coded Decimal (BCD) form.
//
//*****************************************************************************
static uint8_t
dec_to_bcd(uint8_t ui8DecimalByte)
{
    return (((ui8DecimalByte / 10) << 4) | (ui8DecimalByte % 10));
}

//*****************************************************************************
//
//! @brief Selects the clock source for the RTC.
//!
//! @param ui32OSC the clock source for the RTC.
//!
//! This function selects the clock source for the RTC.
//!
//! Valid values for ui32OSC are:
//!
//!     AM_HAL_RTC_OSC_XT
//!
//! @return None
//!
//! @note After selection of the RTC oscillator, a 2 second delay occurs before
//! the new setting is reflected in status. Therefore the CLKGEN.STATUS.OMODE
//! bit will not reflect the new status until after the 2s wait period.
//!
//
//*****************************************************************************
void
am_hal_rtc_osc_select(uint32_t ui32OSC)
{
    if ( ui32OSC == AM_HAL_RTC_OSC_XT )
    {
        // Clear bit to 0 for XTAL
        CLKGEN->OCTRL &= ~CLKGEN_OCTRL_OSEL_Msk;
    }
}

//*****************************************************************************
//
//! @brief Enable/Start the RTC oscillator.
//!
//! Starts the RTC oscillator.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_rtc_osc_enable(void)
{
    //
    // Start the RTC Oscillator.
    //
    RTC->RTCCTL_b.RSTOP = 0;

}

//*****************************************************************************
//
//! @brief Disable/Stop the RTC oscillator.
//!
//! Stops the RTC oscillator.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_rtc_osc_disable(void)
{
    //
    // Stop the RTC Oscillator.
    //
    RTC->RTCCTL_b.RSTOP = 1;
}

//*****************************************************************************
//
//! @brief Configures the RTC for 12 or 24 hour time keeping.
//!
//! @param b12Hour - A 'true' configures the RTC for 12 hour time keeping.
//!
//! Configures the RTC for 12 (true) or 24 (false) hour time keeping.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_rtc_time_12hour(bool b12Hour)
{
    //
    // Set the 12/24 hour bit.
    //
    RTC->RTCCTL_b.HR1224 = b12Hour;
}

//*****************************************************************************
//
//! @brief Enable selected RTC interrupts.
//!
//! @param ui32Interrupt - desired interrupts
//!
//! Enables the RTC interrupts.
//!
//! ui32Interrupt should be the following:
//!
//!     AM_HAL_RTC_INT_ALM
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_rtc_int_enable(uint32_t ui32Interrupt)
{
    //
    // Enable the interrupts.
    //
    RTC->INTEN |= ui32Interrupt;
}

//*****************************************************************************
//
//! @brief Return the enabled RTC interrupts.
//!
//! Returns the enabled RTC interrupts.
//!
//! @return enabled RTC interrupts. Return is 0 or:
//!
//!     AM_HAL_RTC_INT_ALM
//
//*****************************************************************************
uint32_t
am_hal_rtc_int_enable_get(void)
{
    //
    // Read the RTC interrupt enable register, and return its contents.
    //
    return RTC->INTEN;
}

//*****************************************************************************
//
//! @brief Disable selected RTC interrupts.
//!
//! @param ui32Interrupt - desired interrupts
//!
//! Disables the RTC interrupts.
//!
//! ui32Interrupt should be the following:
//!
//!     AM_HAL_RTC_INT_ALM
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_rtc_int_disable(uint32_t ui32Interrupt)
{
    //
    // Disable the interrupts.
    //
    RTC->INTEN &= ~ui32Interrupt;

}

//*****************************************************************************
//
//! @brief Sets the selected RTC interrupts.
//!
//! @param ui32Interrupt - desired interrupts
//!
//! Sets the RTC interrupts causing them to immediately trigger.
//!
//! ui32Interrupt should be the following:
//!
//!     AM_HAL_RTC_INT_ALM
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_rtc_int_set(uint32_t ui32Interrupt)
{
    //
    // Set the interrupts.
    //
    RTC->INTSET = ui32Interrupt;
}

//*****************************************************************************
//
//! @brief Clear selected RTC interrupts.
//!
//! @param ui32Interrupt - desired interrupts
//!
//! Clears the RTC interrupts.
//!
//! ui32Interrupt should be the following:
//!
//!     AM_HAL_RTC_INT_ALM
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_rtc_int_clear(uint32_t ui32Interrupt)
{
    //
    // Clear the interrupts.
    //
    RTC->INTCLR = ui32Interrupt;
}

//*****************************************************************************
//
//! @brief Returns the RTC interrupt status.
//!
//! @param bEnabledOnly - return the status of only the enabled interrupts.
//!
//! Returns the RTC interrupt status.
//!
//! @return Bitwise representation of the current interrupt status.
//!
//! The return value will be 0 or the following:
//!
//!     AM_HAL_RTC_INT_ALM
//
//*****************************************************************************
uint32_t
am_hal_rtc_int_status_get(bool bEnabledOnly)
{
    //
    // Get the interrupt status.
    //
    if ( bEnabledOnly )
    {
        uint32_t u32RetVal;
        u32RetVal  = RTC->INTSTAT;
        u32RetVal &= RTC->INTEN;
        return u32RetVal & (AM_HAL_RTC_INT_ALM);
    }
    else
    {
        return RTC->INTSTAT & (AM_HAL_RTC_INT_ALM);
    }
}

//*****************************************************************************
//
//! @brief Set the Real Time Clock counter registers.
//!
//! @param *pTime - A pointer to the time structure.
//!
//! Sets the RTC counter registers to the supplied values.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_rtc_time_set(am_hal_rtc_time_t *pTime)
{
    //
    // Enable writing to the counters.
    //
    RTC->RTCCTL_b.WRTC = RTC_RTCCTL_WRTC_EN;

    //
    // Write the RTCLOW register.
    //
    RTC->CTRLOW =
        _VAL2FLD(RTC_CTRLOW_CTRHR,  dec_to_bcd(pTime->ui32Hour))         |
        _VAL2FLD(RTC_CTRLOW_CTRMIN, dec_to_bcd(pTime->ui32Minute))       |
        _VAL2FLD(RTC_CTRLOW_CTRSEC, dec_to_bcd(pTime->ui32Second))       |
        _VAL2FLD(RTC_CTRLOW_CTR100, dec_to_bcd(pTime->ui32Hundredths));

    //
    // Write the RTCUP register.
    //
    RTC->CTRUP =
        _VAL2FLD(RTC_CTRUP_CEB,     (pTime->ui32CenturyEnable))          |
        _VAL2FLD(RTC_CTRUP_CB,      (pTime->ui32Century))                |
        _VAL2FLD(RTC_CTRUP_CTRWKDY, (pTime->ui32Weekday))                |
        _VAL2FLD(RTC_CTRUP_CTRYR,   dec_to_bcd((pTime->ui32Year)))       |
        _VAL2FLD(RTC_CTRUP_CTRMO,   dec_to_bcd((pTime->ui32Month)))      |
        _VAL2FLD(RTC_CTRUP_CTRDATE, dec_to_bcd((pTime->ui32DayOfMonth)));

    //
    // Disable writing to the counters.
    //
    RTC->RTCCTL_b.WRTC = RTC_RTCCTL_WRTC_DIS;
}

//*****************************************************************************
//
//! @brief Get the Real Time Clock current time.
//!
//! @param *pTime - A pointer to the time structure to store the current time.
//!
//! Gets the RTC's current time
//!
//! @return 0 for success and 1 for error.
//
//*****************************************************************************
uint32_t
am_hal_rtc_time_get(am_hal_rtc_time_t *pTime)
{
    uint32_t ui32RTCLow, ui32RTCUp, ui32Value;

    //
    // Read the upper and lower RTC registers.
    //
    ui32RTCLow = RTC->CTRLOW;
    ui32RTCUp  = RTC->CTRUP;

    //
    // Break out the lower word.
    //
    ui32Value =
        ((ui32RTCLow & RTC_CTRLOW_CTRHR_Msk) >> RTC_CTRLOW_CTRHR_Pos);
    pTime->ui32Hour = bcd_to_dec(ui32Value);

    ui32Value =
        ((ui32RTCLow & RTC_CTRLOW_CTRMIN_Msk) >> RTC_CTRLOW_CTRMIN_Pos);
    pTime->ui32Minute = bcd_to_dec(ui32Value);

    ui32Value =
        ((ui32RTCLow & RTC_CTRLOW_CTRSEC_Msk) >> RTC_CTRLOW_CTRSEC_Pos);
    pTime->ui32Second = bcd_to_dec(ui32Value);

    ui32Value =
        ((ui32RTCLow & RTC_CTRLOW_CTR100_Msk) >> RTC_CTRLOW_CTR100_Pos);
    pTime->ui32Hundredths = bcd_to_dec(ui32Value);

    //
    // Break out the upper word.
    //
    pTime->ui32ReadError =
        ((ui32RTCUp & RTC_CTRUP_CTERR_Msk) >> RTC_CTRUP_CTERR_Pos);

    pTime->ui32CenturyEnable =
        ((ui32RTCUp & RTC_CTRUP_CEB_Msk) >> RTC_CTRUP_CEB_Pos);

    pTime->ui32Century =
        ((ui32RTCUp & RTC_CTRUP_CB_Msk) >> RTC_CTRUP_CB_Pos);

    ui32Value =
        ((ui32RTCUp & RTC_CTRUP_CTRWKDY_Msk) >> RTC_CTRUP_CTRWKDY_Pos);
    pTime->ui32Weekday = bcd_to_dec(ui32Value);

    ui32Value =
        ((ui32RTCUp & RTC_CTRUP_CTRYR_Msk) >> RTC_CTRUP_CTRYR_Pos);
    pTime->ui32Year = bcd_to_dec(ui32Value);

    ui32Value =
        ((ui32RTCUp & RTC_CTRUP_CTRMO_Msk) >> RTC_CTRUP_CTRMO_Pos);
    pTime->ui32Month = bcd_to_dec(ui32Value);

    ui32Value =
        ((ui32RTCUp & RTC_CTRUP_CTRDATE_Msk) >> RTC_CTRUP_CTRDATE_Pos);

    pTime->ui32DayOfMonth = bcd_to_dec(ui32Value);

    //
    // Was there a read error?
    //
    if (pTime->ui32ReadError)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

//*****************************************************************************
//
//! @brief Sets the alarm repeat interval.
//!
//! @param ui32RepeatInterval the desired repeat interval.
//!
//! Sets the alarm repeat interval.
//!
//! Valid values for ui32RepeatInterval:
//!
//!     AM_HAL_RTC_ALM_RPT_DIS
//!     AM_HAL_RTC_ALM_RPT_YR
//!     AM_HAL_RTC_ALM_RPT_MTH
//!     AM_HAL_RTC_ALM_RPT_WK
//!     AM_HAL_RTC_ALM_RPT_DAY
//!     AM_HAL_RTC_ALM_RPT_HR
//!     AM_HAL_RTC_ALM_RPT_MIN
//!     AM_HAL_RTC_ALM_RPT_SEC
//!     AM_HAL_RTC_ALM_RPT_10TH
//!     AM_HAL_RTC_ALM_RPT_100TH
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_rtc_alarm_interval_set(uint32_t ui32RepeatInterval)
{
    uint32_t ui32RptInt, ui32Alm100, ui32Value;

    switch(ui32RepeatInterval)
    {
        //
        // If repeat every 10th set RPT and ALM100 field accordinly
        //
        case AM_HAL_RTC_ALM_RPT_10TH:
            ui32RptInt = AM_HAL_RTC_ALM_RPT_SEC;
            ui32Alm100 = AM_HAL_RTC_ALM100_10TH;
            break;
        //
        // If repeat every 100th set RPT and ALM100 field accordinly
        //
        case AM_HAL_RTC_ALM_RPT_100TH:
            ui32RptInt = AM_HAL_RTC_ALM_RPT_SEC;
            ui32Alm100 = AM_HAL_RTC_ALM100_100TH;
            break;
        //
        // Otherwise set RPT as value passed.  ALM100 values need to be 0xnn
        // in this setting where n = 0-9.
        //
        default:
            //
            // Get the current value of the ALM100 field.
            //
            ui32Value = RTC->ALMLOW_b.ALM100;

            //
            // If ALM100 was previous EVERY_10TH or EVERY_100TH reset to zero
            // otherwise keep previous setting.
            //
            ui32Alm100 = ui32Value >= 0xF0 ? 0 : ui32Value;

            //
            // Set RPT value to value passed.
            //
            ui32RptInt = ui32RepeatInterval;
            break;
    }


    //
    // Write the interval to the register.
    //
    RTC->RTCCTL_b.RPT = ui32RptInt;

    //
    // Write the Alarm 100 bits in the ALM100 register.
    //
    RTC->ALMLOW_b.ALM100 = ui32Alm100;
}

//*****************************************************************************
//
//! @brief Sets the RTC's Alarm.
//!
//! @param *pTime - A pointer to the time structure.
//! @param ui32RepeatInterval - the desired alarm repeat interval.
//!
//! Set the Real Time Clock Alarm Parameters.
//!
//! Valid values for ui32RepeatInterval:
//!
//!     AM_HAL_RTC_ALM_RPT_DIS
//!     AM_HAL_RTC_ALM_RPT_YR
//!     AM_HAL_RTC_ALM_RPT_MTH
//!     AM_HAL_RTC_ALM_RPT_WK
//!     AM_HAL_RTC_ALM_RPT_DAY
//!     AM_HAL_RTC_ALM_RPT_HR
//!     AM_HAL_RTC_ALM_RPT_MIN
//!     AM_HAL_RTC_ALM_RPT_SEC
//!     AM_HAL_RTC_ALM_RPT_10TH
//!     AM_HAL_RTC_ALM_RPT_EVERY_100TH
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_rtc_alarm_set(am_hal_rtc_time_t *pTime, uint32_t ui32RepeatInterval)
{
    uint8_t ui8Value = 0;

    //
    // Write the interval to the register.
    //
    RTC->RTCCTL |= _VAL2FLD(RTC_RTCCTL_RPT, (ui32RepeatInterval > 0x7 ? 0x7 : ui32RepeatInterval));

    //
    // Check if the interval is 10th or every 100th and track it in ui8Value.
    //
    if (ui32RepeatInterval == AM_HAL_RTC_ALM_RPT_10TH)
    {
        ui8Value = 0xF0;
    }
    else if (ui32RepeatInterval == AM_HAL_RTC_ALM_RPT_100TH)
    {
        ui8Value = 0xFF;
    }

    //
    // Write the ALMUP register.
    //
    RTC->ALMUP =
        _VAL2FLD(RTC_ALMUP_ALMWKDY, (pTime->ui32Weekday))                   |
        _VAL2FLD(RTC_ALMUP_ALMMO,   dec_to_bcd((pTime->ui32Month)))         |
        _VAL2FLD(RTC_ALMUP_ALMDATE, dec_to_bcd((pTime->ui32DayOfMonth)));

    //
    // Write the ALMLOW register.
    //
    RTC->ALMLOW =
        _VAL2FLD(RTC_ALMLOW_ALMHR,  dec_to_bcd(pTime->ui32Hour))            |
        _VAL2FLD(RTC_ALMLOW_ALMMIN, dec_to_bcd(pTime->ui32Minute))          |
        _VAL2FLD(RTC_ALMLOW_ALMSEC, dec_to_bcd(pTime->ui32Second))          |
        _VAL2FLD(RTC_ALMLOW_ALM100, dec_to_bcd(pTime->ui32Hundredths) | ui8Value);
}

//*****************************************************************************
//
//! @brief Get the Real Time Clock Alarm Parameters
//!
//! @param *pTime - A pointer to the time structure to store the current alarm.
//!
//! Gets the RTC's Alarm time
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_rtc_alarm_get(am_hal_rtc_time_t *pTime)
{
    uint32_t ui32ALMLow, ui32ALMUp, ui32Value;

    //
    // Read the upper and lower RTC registers.
    //
    ui32ALMLow = RTC->ALMLOW;
    ui32ALMUp  = RTC->ALMUP;

    //
    // Break out the lower word.
    //
    ui32Value = ((ui32ALMLow & RTC_ALMLOW_ALMHR_Msk) >> RTC_ALMLOW_ALMHR_Pos);
    pTime->ui32Hour = bcd_to_dec(ui32Value);

    ui32Value = ((ui32ALMLow & RTC_ALMLOW_ALMMIN_Msk) >> RTC_ALMLOW_ALMMIN_Pos);
    pTime->ui32Minute = bcd_to_dec(ui32Value);

    ui32Value = ((ui32ALMLow & RTC_ALMLOW_ALMSEC_Msk) >> RTC_ALMLOW_ALMSEC_Pos);
    pTime->ui32Second = bcd_to_dec(ui32Value);

    ui32Value = ((ui32ALMLow & RTC_ALMLOW_ALM100_Msk) >> RTC_ALMLOW_ALM100_Pos);
    pTime->ui32Hundredths = bcd_to_dec(ui32Value);

    //
    // Break out the upper word.
    //
    pTime->ui32ReadError = 0;
    pTime->ui32CenturyEnable = 0;
    pTime->ui32Century = 0;

    ui32Value = ((ui32ALMUp & RTC_ALMUP_ALMWKDY_Msk) >> RTC_ALMUP_ALMWKDY_Pos);
    pTime->ui32Weekday = bcd_to_dec(ui32Value);

    pTime->ui32Year = 0;

    ui32Value = ((ui32ALMUp & RTC_ALMUP_ALMMO_Msk) >> RTC_ALMUP_ALMMO_Pos);
    pTime->ui32Month = bcd_to_dec(ui32Value);

    ui32Value = ((ui32ALMUp & RTC_ALMUP_ALMDATE_Msk) >> RTC_ALMUP_ALMDATE_Pos);
    pTime->ui32DayOfMonth = bcd_to_dec(ui32Value);
}

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
