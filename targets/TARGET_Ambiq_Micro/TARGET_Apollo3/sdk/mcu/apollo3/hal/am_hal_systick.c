//*****************************************************************************
//
//  am_hal_systick.c
//! @file
//!
//! @brief Functions for interfacing with the SYSTICK
//!
//! @addtogroup systick3 System Timer (SYSTICK)
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
// Macro definitions
//
//*****************************************************************************
#define SYSTICK_MAX_TICKS   ((1 << 24)-1)
#define MAX_U32             (0xffffffff)

//*****************************************************************************
//
//! @brief Start the SYSTICK.
//!
//! This function starts the systick timer.
//!
//! @note This timer does not run in deep-sleep mode as it runs from the core
//! clock, which is gated in deep-sleep. If a timer is needed in deep-sleep use
//! one of the ctimers instead. Also to note is this timer will consume higher
//! power than the ctimers.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_systick_start(void)
{
    //
    // Start the systick timer.
    //
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

//*****************************************************************************
//
//! @brief Stop the SYSTICK.
//!
//! This function stops the systick timer.
//!
//! @note This timer does not run in deep-sleep mode as it runs from the core
//! clock, which is gated in deep-sleep. If a timer is needed in deep-sleep use
//! one of the ctimers instead. Also to note is this timer will consume higher
//! power than the ctimers.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_systick_stop(void)
{
    //
    // Stop the systick timer.
    //
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}

//*****************************************************************************
//
//! @brief Enable the interrupt in the SYSTICK.
//!
//! This function enables the interupt in the systick timer.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_systick_int_enable(void)
{
    //
    // Enable the systick timer interrupt.
    //
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

//*****************************************************************************
//
//! @brief Disable the interrupt in the SYSTICK.
//!
//! This function disables the interupt in the systick timer.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_systick_int_disable(void)
{
    //
    // Disable the systick timer interrupt.
    //
    SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

//*****************************************************************************
//
//! @brief Reads the interrupt status.
//!
//! This function reads the interrupt status in the systick timer.
//!
//! @return the interrupt status.
//
//*****************************************************************************
uint32_t
am_hal_systick_int_status_get(void)
{
    //
    // Return the systick timer interrupt status.
    //
    return SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk;
}

//*****************************************************************************
//
//! @brief Reset the interrupt in the SYSTICK.
//!
//! This function resets the systick timer by clearing out the configuration
//! register.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_systick_reset(void)
{
    //
    // Reset the systick timer interrupt.
    //
    SysTick->CTRL = 0x0;
}

//*****************************************************************************
//
//! @brief Load the value into the SYSTICK.
//!
//! @param ui32LoadVal the desired load value for the systick. Maximum value is
//! 0x00FF.FFFF.
//!
//! This function loads the desired value into the systick timer.
//!
//! @return None.
//
//*****************************************************************************
void
am_hal_systick_load(uint32_t ui32LoadVal)
{
    //
    //  The proper SysTick initialization sequence is: (p 4-36 of the M4 UG).
    //      1. Program reload value
    //      2. Clear current value
    //      3. Program CSR
    // Write the given value to the reload register.
    // Write the Current Value Register to clear it to 0.
    //
    SysTick->LOAD = ui32LoadVal;
    SysTick->VAL = 0;
}

//*****************************************************************************
//
//! @brief Get the current count value in the SYSTICK.
//!
//! This function gets the current count value in the systick timer.
//!
//! @return Current count value.
//
//*****************************************************************************
uint32_t
am_hal_systick_count(void)
{
    //
    // Return the current systick timer count value.
    //
    return SysTick->VAL;
}

//*****************************************************************************
//
//! @brief Wait the specified number of ticks.
//!
//! This function delays for the given number of SysTick ticks.
//!
//! @note If the SysTick timer is being used elsewhere, it will be corrupted
//! by calling this function.
//!
//! @return 0 if successful.
//
//*****************************************************************************
uint32_t
am_hal_systick_wait_ticks(uint32_t ui32Ticks)
{

    if ( ui32Ticks == 0 )
    {
        ui32Ticks++;                // Make sure we get the COUNTFLAG
    }

    //
    //  The proper SysTick initialization sequence is: (p 4-36 of the M4 UG).
    //      1. Program reload value
    //      2. Clear current value
    //      3. Program CSR
    //
    // Set the reload value to the required number of ticks.
    //
    SysTick->LOAD = ui32Ticks;

    //
    // Clear the current count.
    //
    SysTick->VAL = 0x0;

    //
    // Set to use the processor clock, but don't cause an exception (we'll poll).
    //
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;

    //
    // Poll till done
    //
    while ( !(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) );

    //
    // And disable systick before exiting.
    //
    SysTick->CTRL = 0x0;

    return 0;
}

//*****************************************************************************
//
//! @brief Delay the specified number of microseconds.
//!
//! This function will use the SysTick timer to delay until the specified
//!  number of microseconds have elapsed.  It uses the processor clocks and
//!  takes into account the current CORESEL setting.
//!
//! @note If the SysTick timer is being used elsewhere, it will be corrupted
//! by calling this function.
//!
//! @return Total number of SysTick ticks delayed.
//
//*****************************************************************************
uint32_t
am_hal_systick_delay_us(uint32_t ui32NumUs)
{
    uint32_t ui32nLoops, ui32Ticks, uRet;
    uint32_t ui32ClkFreq, ui32TicksPerMHz;
    uint32_t ui32CoreSel = CLKGEN->CCTRL_b.CORESEL;

    ui32nLoops = 0;
    if ( (ui32CoreSel <= AM_HAL_CLKGEN_CORESEL_MAXDIV)  &&  (ui32NumUs >= 2) )
    {
        //
        // Determine clock freq, then whether we need more than 1 iteration.
        //
        ui32ClkFreq = AM_HAL_CLKGEN_FREQ_MAX_MHZ >> ui32CoreSel;
        ui32ClkFreq <<= (am_hal_burst_mode_status() == AM_HAL_BURST_MODE)? 1 : 0;

        ui32TicksPerMHz = SYSTICK_MAX_TICKS / ui32ClkFreq;
        if ( ui32NumUs > ui32TicksPerMHz )
        {
            //
            // Get number of required loops, as well as additional ticks.
            //
            ui32nLoops = ui32NumUs / ui32TicksPerMHz;
            ui32NumUs  = ui32NumUs % ui32TicksPerMHz;
        }

        //
        // Compute the number of ticks required.
        // Allow for about 2us of call overhead.
        //
        ui32Ticks = (ui32NumUs - 2) * ui32ClkFreq;
    }
    else
    {
        ui32Ticks = 1;
    }

    uRet = (ui32nLoops * SYSTICK_MAX_TICKS) + ui32Ticks;
    while ( ui32nLoops )
    {
        am_hal_systick_wait_ticks(SYSTICK_MAX_TICKS);
        ui32nLoops--;
    }
    am_hal_systick_wait_ticks(ui32Ticks);

    return uRet;
}

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
