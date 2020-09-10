//*****************************************************************************
//
//! @file am_util_delay.c
//!
//! @brief A few useful delay functions.
//!
//! Functions for fixed delays.
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
#include "am_util_delay.h"

//*****************************************************************************
//
//! @brief Delays for a desired amount of loops.
//!
//! @param ui32CycleLoops - Desired number of cycle loops to delay for.
//!
//! This function will delay for a number of cycle loops.
//!
//! @note - the number of cycles each loops takes to execute is approximately 3.
//! Therefore the actual number of cycles executed will be ~3x ui32CycleLoops.
//!
//! For example, a ui32CycleLoops value of 100 will delay for 300 cycles.
//!
//! @returns None
//
//*****************************************************************************
void
am_util_delay_cycles(uint32_t ui32Iterations)
{
    //
    // Call the BOOTROM cycle delay function
    //
    am_hal_flash_delay(ui32Iterations);
}

//*****************************************************************************
//
//! @brief Delays for a desired amount of milliseconds.
//!
//! @param ui32MilliSeconds - number of milliseconds to delay for.
//!
//! This function will delay for a number of milliseconds.
//!
//! @returns None
//
//*****************************************************************************
void
am_util_delay_ms(uint32_t ui32MilliSeconds)
{
    uint32_t ui32Loops, ui32HFRC;
#if AM_APOLLO3_CLKGEN
    am_hal_clkgen_status_t sClkgenStatus;
    am_hal_clkgen_status_get(&sClkgenStatus);
    ui32HFRC = sClkgenStatus.ui32SysclkFreq;
#else // AM_APOLLO3_CLKGEN
    ui32HFRC = am_hal_clkgen_sysclk_get();
#endif // AM_APOLLO3_CLKGEN
    ui32Loops = ui32MilliSeconds * (ui32HFRC / 3000);

    //
    // Call the BOOTROM cycle delay function
    //
    am_hal_flash_delay(ui32Loops);
}

//*****************************************************************************
//
//! @brief Delays for a desired amount of microseconds.
//!
//! @param ui32MicroSeconds - number of microseconds to delay for.
//!
//! This function will delay for a number of microseconds.
//!
//! @returns None
//
//*****************************************************************************
void
am_util_delay_us(uint32_t ui32MicroSeconds)
{
    uint32_t ui32Loops, ui32HFRC;

#if AM_APOLLO3_CLKGEN
    am_hal_clkgen_status_t sClkgenStatus;
    am_hal_clkgen_status_get(&sClkgenStatus);
    ui32HFRC = sClkgenStatus.ui32SysclkFreq;
#else // AM_APOLLO3_CLKGEN
    ui32HFRC = am_hal_clkgen_sysclk_get();
#endif // AM_APOLLO3_CLKGEN
    ui32Loops = ui32MicroSeconds * (ui32HFRC / 3000000);

    //
    // Call the BOOTROM cycle delay function
    //
    am_hal_flash_delay(ui32Loops);
}
