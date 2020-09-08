//*****************************************************************************
//
//! @file am_util_id.h
//!
//! @brief Identification of the Ambiq Micro device.
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
#ifndef AM_UTIL_ID_H
#define AM_UTIL_ID_H

#include "am_mcu_apollo.h"

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
//! ID structure
//
//*****************************************************************************
typedef struct
{
    //
    //! Contains the HAL hardware information about the device.
    //
    am_hal_mcuctrl_device_t sMcuCtrlDevice;

    //
    //! Device type (derived value, not a hardware value)
    //
    uint32_t ui32Device;

    //
    //! Vendor name from the MCUCTRL VENDORID register and stringized here.
    //
    const uint8_t *pui8VendorName;

    //
    //! Device name (derived value, not a hardware value)
    //
    const uint8_t *pui8DeviceName;

    //
    // Major chip revision (e.g. char 'A' or 'B')
    //
    uint8_t ui8ChipRevMaj;

    //
    // Minor chip revision (e.g. char '0', '1', ' ')
    //
    uint8_t ui8ChipRevMin;
}
am_util_id_t;

//*****************************************************************************
//
// Macros for MCUCTRL CHIP_INFO field.
// Note - these macros are derived from the Apollo2 auto-generated register
// definitions.
//
//*****************************************************************************
#define AM_UTIL_MCUCTRL_CHIP_INFO_PARTNUM_APOLLO3P    0x07000000
#define AM_UTIL_MCUCTRL_CHIP_INFO_PARTNUM_APOLLO3     0x06000000
#define AM_UTIL_MCUCTRL_CHIP_INFO_PARTNUM_APOLLOBL    0x05000000
#define AM_UTIL_MCUCTRL_CHIP_INFO_PARTNUM_APOLLO2     0x03000000
#define AM_UTIL_MCUCTRL_CHIP_INFO_PARTNUM_APOLLO      0x01000000
#define AM_UTIL_MCUCTRL_CHIP_INFO_PARTNUM_PN_M        0xFF000000

//*****************************************************************************
//
// Macros for silicon identification
//
//*****************************************************************************
#define AM_UTIL_ID_UNKNOWN      0
#define AM_UTIL_ID_APOLLO       0x0001
#define AM_UTIL_ID_APOLLO2      0x0002
#define AM_UTIL_ID_APOLLO3      0x0003      // Apollo3 Blue
#define AM_UTIL_ID_APOLLO3P     0x0103      // Apollo3 Blue Plus

//*****************************************************************************
//
// External function definitions
//
//*****************************************************************************
extern uint32_t am_util_id_device(am_util_id_t *psIDDevice);

#ifdef __cplusplus
}
#endif

#endif // AM_UTIL_ID_H

