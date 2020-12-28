//*****************************************************************************
//
//! @file system_apollo3.c
//!
//! @brief Ambiq Micro Apollo3 MCU specific functions.
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
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause

#include <stdint.h>
#include "system_apollo3.h"
#include "apollo3.h"

//*****************************************************************************
//
// Defines
//
//*****************************************************************************

//
// Clocks
//
#define __HSI             (6000000UL)
#define __XTAL            (32768UL)         // Crystal Oscillator frequency
#define __SYS_OSC_CLK     (48000000)        // Main oscillator frequency
#define __SYSTEM_CLOCK    (1*__SYS_OSC_CLK)

//
// Initialize SystemCoreClock with the system core clock frequency value
// achieved after system intitialization.
// This means system core clock frequency after call to SystemInit()
//
uint32_t SystemCoreClock = __SYSTEM_CLOCK;  // System Clock Frequency (Core Clock)

//*****************************************************************************
//
//! @brief Set the global clock frequncy.
//!
//! This function sets the global clock frequency.
//!
//! @return None.
//
//*****************************************************************************
void
SystemCoreClockUpdate(void)
{
    //
    // Calculate the system frequency based upon the current register settings.
    // This function can be used to retrieve the system core clock frequeny
    // after user changed register sittings.
    //
    SystemCoreClock = __SYS_OSC_CLK / (CLKGEN->CCTRL_b.CORESEL + 1);
}

//*****************************************************************************
//
//! @brief Initialize the system.
//!
//! This function sets up the microcontroller system.
//!
//! @return None.
//
//*****************************************************************************
void
SystemInit(void)
{
    //
    // Initialize the system
    // Do not use global variables because this function is called before
    // reaching pre-main. RW section maybe overwritten afterwards.
    //
    SystemCoreClock = __SYSTEM_CLOCK;

    CLKGEN->CLKKEY = 0x47;              // Enable write to CCTRL
    CLKGEN->CCTRL_b.CORESEL = 0;        // Div by 1 for 48MHz
    CLKGEN->CLKKEY = 0;                 // Disable write to CCTRL
}

