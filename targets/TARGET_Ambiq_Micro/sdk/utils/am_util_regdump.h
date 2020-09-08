//*****************************************************************************
//
//! @file am_util_regdump.h
//!
//! @brief Dump specified registers for debug purposes.
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
#ifndef AM_UTIL_REGDUMP_H
#define AM_UTIL_REGDUMP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "am_mcu_apollo.h"

//
// Apollo peripherals
//
#define AM_UTIL_REGDUMP_ADC             (1 << 0)
#define AM_UTIL_REGDUMP_CLKGEN          (1 << 1)
#define AM_UTIL_REGDUMP_CTIMER          (1 << 2)
#define AM_UTIL_REGDUMP_GPIO            (1 << 3)
#define AM_UTIL_REGDUMP_IOM             (1 << 4)
#define AM_UTIL_REGDUMP_IOS             (1 << 5)
#define AM_UTIL_REGDUMP_MCUCTRL         (1 << 6)
#define AM_UTIL_REGDUMP_RSTGEN          (1 << 7)
#define AM_UTIL_REGDUMP_RTC             (1 << 8)
#define AM_UTIL_REGDUMP_UART            (1 << 9)
#define AM_UTIL_REGDUMP_VCOMP           (1 << 10)
#define AM_UTIL_REGDUMP_WDT             (1 << 11)

//
// Apollo2 new peripherals
//
#define AM_UTIL_REGDUMP_CACHE           (1 << 12)
#define AM_UTIL_REGDUMP_PDM             (1 << 13)
#define AM_UTIL_REGDUMP_PWRCTRL         (1 << 14)

//
// Apollo3 new peripherals
//
#define AM_UTIL_REGDUMP_BLE             (1 << 15)
#define AM_UTIL_REGDUMP_MSPI            (1 << 16)


#define AM_UTIL_REGDUMP_INFO0           (1 << 24)

//
// ARM Core blocks
//
#define AM_UTIL_REGDUMP_ITM             (1 << 25)
#define AM_UTIL_REGDUMP_NVIC            (1 << 26)
#define AM_UTIL_REGDUMP_SYSCTRL         (1 << 27)
#define AM_UTIL_REGDUMP_SYSTICK         (1 << 28)
#define AM_UTIL_REGDUMP_TPIU            (1 << 29)



//*****************************************************************************
//
// Module mask definitions
//
//*****************************************************************************
#define AM_UTIL_REGDUMP_APOLLO                  \
        (   AM_UTIL_REGDUMP_ADC     |           \
            AM_UTIL_REGDUMP_CLKGEN  |           \
            AM_UTIL_REGDUMP_CTIMER  |           \
            AM_UTIL_REGDUMP_GPIO    |           \
            AM_UTIL_REGDUMP_IOM     |           \
            AM_UTIL_REGDUMP_IOS     |           \
            AM_UTIL_REGDUMP_MCUCTRL |           \
            AM_UTIL_REGDUMP_RSTGEN  |           \
            AM_UTIL_REGDUMP_RTC     |           \
            AM_UTIL_REGDUMP_UART    |           \
            AM_UTIL_REGDUMP_VCOMP   |           \
            AM_UTIL_REGDUMP_WDT )

#define AM_UTIL_REGDUMP_APOLLO2                 \
        (   AM_UTIL_REGDUMP_CACHE   |           \
            AM_UTIL_REGDUMP_PDM     |           \
            AM_UTIL_REGDUMP_PWRCTRL )

#define AM_UTIL_REGDUMP_CORE                    \
        (   AM_UTIL_REGDUMP_ITM     |           \
            AM_UTIL_REGDUMP_NVIC    |           \
            AM_UTIL_REGDUMP_SYSCTRL |           \
            AM_UTIL_REGDUMP_SYSTICK |           \
            AM_UTIL_REGDUMP_TPIU )

//
// Get a register dump of ALL modules in a block.
//
#ifdef AM_PART_APOLLO
#define AM_UTIL_REGDUMP_ALL                     \
        (   AM_UTIL_REGDUMP_APOLLO  |           \
            AM_UTIL_REGDUMP_CORE )
#endif // PART_APOLLO

#if defined(AM_PART_APOLLO2) || defined(AM_PART_APOLLO3) || defined(AM_PART_APOLLO3P)
#define AM_UTIL_REGDUMP_ALL                     \
        (   AM_UTIL_REGDUMP_APOLLO  |           \
            AM_UTIL_REGDUMP_APOLLO2 |           \
            AM_UTIL_REGDUMP_CORE )
#endif // PART_APOLLO

//
// Get a register dump of ALL modules in a block.
//
#define AM_UTIL_REGDUMP_MOD_ALL             0xFFFFFFFF

//
// This macro determines a mask given the first and last modules desired. e.g.
//  REGDUMP_MOD_MASK(2,4)       // Dump regs for modules 2, 3, and 4
//
#define REGDUMP_MOD_MASK(modfirst, modlast)     \
        (((1 << (modlast - modfirst + 1)) - 1) << modfirst)

//
// These macros determine a single module.  e.g.
//  REGDUMP_MOD2 | REGDUMP_MOD4    // Dump regs for modules 2 and 4 (skip 3)
//
#define REGDUMP_MOD(n)                  (1 << n)
#define REGDUMP_MOD0                    (REGDUMP_MOD(0))
#define REGDUMP_MOD1                    (REGDUMP_MOD(1))
#define REGDUMP_MOD2                    (REGDUMP_MOD(2))
#define REGDUMP_MOD3                    (REGDUMP_MOD(3))
#define REGDUMP_MOD4                    (REGDUMP_MOD(4))
#define REGDUMP_MOD5                    (REGDUMP_MOD(5))
#define REGDUMP_MOD6                    (REGDUMP_MOD(6))
#define REGDUMP_MOD7                    (REGDUMP_MOD(7))

//*****************************************************************************
//
// External function definitions
//
//*****************************************************************************
extern void am_util_regdump_print(uint32_t ui32PeriphMask, uint32_t ui32ModuleMask);

#ifdef __cplusplus
}
#endif

#endif // AM_UTIL_REGDUMP_H

