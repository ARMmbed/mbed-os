//*****************************************************************************
//
//  am_hal_wdt.c
//! @file
//!
//! @brief Hardware abstraction layer for the Watchdog Timer module.
//!
//! @addtogroup wdt3 Watchdog Timer (WDT)
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
// Adjacency check
//
// This is related to the timer read workaround. This macro checks to see if
// the two supplied count values are within one "tick" of eachother. It should
// still pass in the event of a timer rollover. The "B" read is assumed to
// follow the "A" read.  The macro returns "TRUE" when the adjacent timer reads
// can be used.
//
//*****************************************************************************
#define adjacent(A, B)      (((A) == (B)) || (((A) + 1) == (B)) || ((B) == 0))

//*****************************************************************************
//
//! @brief Configure the watchdog timer.
//!
//! @param psConfig - pointer to a configuration structure containing the
//! desired watchdog settings.
//!
//! This function will set the watchdog configuration register based on the
//! user's desired settings listed in the structure referenced by psConfig. If
//! the structure indicates that watchdog interrupts are desired, this function
//! will also set the interrupt enable bit in the configuration register.
//!
//! @note In order to actually receive watchdog interrupt and/or watchdog reset
//! events, the caller will also need to make sure that the watchdog interrupt
//! vector is enabled in the ARM NVIC, and that watchdog resets are enabled in
//! the reset generator module. Otherwise, the watchdog-generated interrupt and
//! reset events will have no effect.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_wdt_init(const am_hal_wdt_config_t *psConfig)
{
    uint32_t ui32ConfigVal;
    uint16_t ui16IntCount, ui16ResetCount;
    bool bResetEnabled = psConfig->ui32Config & AM_HAL_WDT_ENABLE_RESET;
    bool bInterruptEnabled = psConfig->ui32Config & AM_HAL_WDT_ENABLE_INTERRUPT;

    //
    // Read the desired settings from the psConfig structure.
    //
    ui16IntCount = psConfig->ui16InterruptCount;
    ui16ResetCount = psConfig->ui16ResetCount;

    //
    // Write the interrupt and reset count values to a temporary variable.
    //
    // Accept the passed Config value, but clear the Counts that we are about to set.
    ui32ConfigVal = psConfig->ui32Config & ~(WDT_CFG_INTVAL_Msk | WDT_CFG_RESVAL_Msk);
    ui32ConfigVal |= _VAL2FLD(WDT_CFG_INTVAL, ui16IntCount);
    ui32ConfigVal |= _VAL2FLD(WDT_CFG_RESVAL, ui16ResetCount);

    //
    // If interrupts should be enabled, set the appropriate bit in the
    // temporary variable. Also, enable the interrupt in INTEN register in the
    // watchdog module.
    //
    if ( bInterruptEnabled )
    {
        //
        // Enable the watchdog interrupt if the configuration calls for them.
        //
        WDT->INTEN |= WDT_INTEN_WDTINT_Msk;
    }
    else
    {
        //
        // Disable the watchdog interrupt if the configuration doesn't call for
        // watchdog interrupts.
        //
        WDT->INTEN &= ~WDT_INTEN_WDTINT_Msk;
    }

    //
    // If resets should be enabled, set the appropriate bit in the temporary
    // variable.
    //
    if ( bResetEnabled )
    {
        //
        // Also enable watchdog resets in the reset module.
        //
        RSTGEN->CFG |= RSTGEN_CFG_WDREN_Msk;
    }
    else
    {
        //
        // Disable watchdog resets in the reset module.
        //
        RSTGEN->CFG &= ~RSTGEN_CFG_WDREN_Msk;
    }

    //
    // Check for a user specified clock select. If none specified then
    // set 128Hz.
    //
    if ( !(psConfig->ui32Config & WDT_CFG_CLKSEL_Msk) )
    {
        ui32ConfigVal |= _VAL2FLD(WDT_CFG_CLKSEL, WDT_CFG_CLKSEL_128HZ);
    }

    //
    // Write the saved value to the watchdog configuration register.
    //
    WDT->CFG = ui32ConfigVal;
} // am_hal_wdt_init()

//*****************************************************************************
//
//! @brief Starts the watchdog timer.
//!
//! Enables the watchdog timer tick using the 'enable' bit in the watchdog
//! configuration register.  This function does not perform any locking of the
//! watchdog timer, so it can be disabled or reconfigured later.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_wdt_start(void)
{
    //
    // Make sure the watchdog timer is in the "reset" state, and then set the
    // enable bit to start counting.
    //
    WDT->CFG   |= WDT_CFG_WDTEN_Msk;
    WDT->RSTRT  = WDT_RSTRT_RSTRT_KEYVALUE;
} // am_hal_wdt_start()

//*****************************************************************************
//
//! @brief Stops the watchdog timer.
//!
//! Disables the watchdog timer tick by clearing the 'enable' bit in the
//! watchdog configuration register.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_wdt_halt(void)
{
    //
    // Clear the watchdog enable bit.
    //
    WDT->CFG &= ~WDT_CFG_WDTEN_Msk;
} // am_hal_wdt_halt()

//*****************************************************************************
//
//! @brief Locks the watchdog configuration and starts the watchdog timer.
//!
//! This function sets the watchdog "lock" register, which prevents software
//! from re-configuring the watchdog. This action will also set the enable bit
//! for the watchdog timer, so it will start counting immediately.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_wdt_lock_and_start(void)
{
    //
    // Write the 'key' value to the watchdog lock register.
    //
    WDT->LOCK = WDT_LOCK_LOCK_KEYVALUE;
} // am_hal_wdt_lock_and_start()

//*****************************************************************************
//
//! @brief Read the state of the wdt interrupt status.
//!
//! @param bEnabledOnly - return the status of only the enabled interrupts.
//!
//! This function extracts the interrupt status bits and returns the enabled or
//! raw based on bEnabledOnly.
//!
//! @return WDT interrupt status.
//
//*****************************************************************************
uint32_t
am_hal_wdt_int_status_get(bool bEnabledOnly)
{
    if ( bEnabledOnly )
    {
        uint32_t ui32RetVal;
        AM_CRITICAL_BEGIN
        ui32RetVal = WDT->INTSTAT;
        ui32RetVal &= WDT->INTEN;
        AM_CRITICAL_END
        return ui32RetVal;
    }
    else
    {
        return WDT->INTSTAT;
    }
} // am_hal_wdt_int_status_get()

//*****************************************************************************
//
//! @brief Set the state of the wdt interrupt status bit.
//!
//! This function sets the interrupt bit.
//!
//! @return None
//
//*****************************************************************************
void
am_hal_wdt_int_set(void)
{
    WDT->INTSET = WDT_INTSET_WDTINT_Msk;
} // am_hal_wdt_int_set()

//*****************************************************************************
//
//! @brief Clear the state of the wdt interrupt status bit.
//!
//! This function clear the interrupt bit.
//!
//! @return None
//
//*****************************************************************************
void
am_hal_wdt_int_clear(void)
{
    WDT->INTCLR = WDT_INTCLR_WDTINT_Msk;
} // am_hal_wdt_int_clear()

//*****************************************************************************
//
//! @brief Enable the wdt interrupt.
//!
//! This function enable the interrupt.
//!
//! @return None
//
//*****************************************************************************
void
am_hal_wdt_int_enable(void)
{
    WDT->INTEN |= WDT_INTEN_WDTINT_Msk;
} // am_hal_wdt_int_enable()

//*****************************************************************************
//
//! @brief Return the enabled WDT interrupts.
//!
//! This function returns the enabled WDT interrupts.
//!
//! @return enabled WDT interrupts.
//
//*****************************************************************************
uint32_t
am_hal_wdt_int_enable_get(void)
{
    return WDT->INTEN;
} // am_hal_wdt_int_enable_get()

//*****************************************************************************
//
//! @brief Disable the wdt interrupt.
//!
//! This function disablee the interrupt.
//!
//! @return None
//
//*****************************************************************************
void
am_hal_wdt_int_disable(void)
{
    WDT->INTEN &= ~WDT_INTEN_WDTINT_Msk;
} // am_hal_wdt_int_disable()

//*****************************************************************************
//
//! @brief Get the wdt counter value.
//!
//! This function reads the current value of watch dog timer counter register.
//!
//! @return None
//
//*****************************************************************************
uint32_t
am_hal_wdt_counter_get(void)
{
    uint32_t ui32Values[3] = {0};
    uint32_t ui32Value;

    //
    // Start a critical section.
    //
    uint32_t ui32InterruptState = am_hal_interrupt_master_disable();

    //
    // First, go read the value from the counter register 3 times
    // back to back in assembly language.
    //
    am_hal_triple_read( AM_REGADDR(WDT, COUNT), ui32Values );

    //
    // Mask out the COUNT field from the 3 read values.
    //
    ui32Values[0] = _VAL2FLD(WDT_COUNT_COUNT, ui32Values[0]);
    ui32Values[1] = _VAL2FLD(WDT_COUNT_COUNT, ui32Values[1]);
    ui32Values[2] = _VAL2FLD(WDT_COUNT_COUNT, ui32Values[2]);

    //
    // Now, we'll figure out which of the three values is the correct time.
    //
    if (ui32Values[0] == ui32Values[1])
    {
        //
        // If the first two values match, then neither one was a bad read.
        // We'll take this as the current time.
        //
        ui32Value = ui32Values[1];
    }
    else
    {
        //
        // If the first two values didn't match, then one of them might be bad.
        // If one of the first two values is bad, then the third one should
        // always be correct. We'll take the third value as the correct count.
        //
        ui32Value = ui32Values[2];

        //
        // If all of the statements about the architecture are true, the third
        // value should be correct, and it should always be within one count of
        // either the first or the second value.
        //
        // Just in case, we'll check against the previous two values to make
        // sure that our final answer was reasonable. If it isn't, we will
        // flag it as a "bad read", and fail this assert statement.
        //
        // This shouldn't ever happen, and it hasn't ever happened in any of
        // our tests so far.
        //
        am_hal_debug_assert_msg((adjacent(ui32Values[1], ui32Values[2]) ||
                                 adjacent(ui32Values[0], ui32Values[2])),
                                "Bad CDT read");
    }

    //
    // End the critical section.
    //
    am_hal_interrupt_master_set(ui32InterruptState);

    return ui32Value;
} // am_hal_wdt_counter_get()

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
