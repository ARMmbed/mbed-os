//*****************************************************************************
//
//  am_hal_global.h
//! @file
//!
//! @brief Locate all HAL global variables here.
//!
//! This module contains global variables that are used throughout the HAL,
//! but not necessarily those designated as const (which typically end up in
//! flash). Consolidating globals here will make it easier to manage them.
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
#ifndef AM_HAL_GLOBAL_H
#define AM_HAL_GLOBAL_H

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Include the SDK global version information.
//
//*****************************************************************************
#include "../../am_sdk_version.h"

//*****************************************************************************
//
// Device definitions
//
//*****************************************************************************
#define AM_HAL_DEVICE_NAME      "Apollo3 Blue"

//*****************************************************************************
//
// Macro definitions
//
//*****************************************************************************
// Utility for compile time assertions
// Will cause divide by 0 error at build time
#define _AM_ASSERT_CONCAT_(a, b) a##b
#define _AM_ASSERT_CONCAT(a, b) _AM_ASSERT_CONCAT_(a, b)
#define am_ct_assert(e) enum { _AM_ASSERT_CONCAT(assert_line_, __LINE__) = 1/(!!(e)) }

//*****************************************************************************
//
// Macros to determine compiler version information
//
//*****************************************************************************
//
// Since the stringize operator itself does not first expand macros, two levels
//  of indirection are required in order to fully resolve the pre-defined
//  compiler (integer) macros.  The 1st level expands the macro, and the 2nd
//  level actually stringizes it.
// This method will also work even if the argument is not a macro. However, if
//  the argument is already a string, the string will end up with inserted quote
//   marks.
//
#define STRINGIZE_VAL(n)                    STRINGIZE_VAL2(n)
#define STRINGIZE_VAL2(n)                   #n

#ifdef __GNUC__
#define COMPILER_VERSION                    ("GCC " __VERSION__)
#elif defined(__ARMCC_VERSION)
#define COMPILER_VERSION                    ("ARMCC " STRINGIZE_VAL(__ARMCC_VERSION))
#elif defined(__KEIL__)
#define COMPILER_VERSION                    "KEIL_CARM " STRINGIZE_VAL(__CA__)
#elif defined(__IAR_SYSTEMS_ICC__)
#define COMPILER_VERSION                    __VERSION__
#else
#define COMPILER_VERSION                    "Compiler unknown"
#endif

//*****************************************************************************
//
// Utility Macros
//
//*****************************************************************************
// As long as the two values are not apart by more that 2^31, this should give
// correct result, taking care of wraparound
#define AM_HAL_U32_GREATER(val1, val2)     ((int32_t)((int32_t)(val1) - (int32_t)(val2)) > 0)
#define AM_HAL_U32_SMALLER(val1, val2)     ((int32_t)((int32_t)(val1) - (int32_t)(val2)) < 0)

//******************************************************************************
//
// Global typedefs
//
//******************************************************************************
typedef union
{
    uint32_t    u32;
    struct
    {
        uint32_t    resvd       : 7;    // [6:0]
        uint32_t    bAMREGS     : 1;    // [7]
        uint32_t    Revision    : 8;    // [15:8]
        uint32_t    Minor       : 8;    // [23:16]
        uint32_t    Major       : 8;    // [31:24]
    } s;
} am_hal_version_t;

typedef union
{
    uint32_t    u32;
    struct
    {
        uint32_t    magic   : 24;
        uint32_t    bInit   : 1;
        uint32_t    bEnable : 1;
        uint32_t    resv    : 6;
    } s;
} am_hal_handle_prefix_t;

//*****************************************************************************
//
// Global Variables extern declarations.
//
//*****************************************************************************
extern volatile uint32_t g_ui32HALflags;
extern const    uint8_t  g_ui8HALcompiler[];
extern const    am_hal_version_t g_ui32HALversion;

#ifdef __cplusplus
}
#endif

#endif // AM_HAL_GLOBAL_H
