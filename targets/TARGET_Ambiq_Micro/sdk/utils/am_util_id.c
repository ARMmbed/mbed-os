//*****************************************************************************
//
//! @file am_util_id.c
//!
//! @brief Identification of the Ambiq Micro device.
//!
//! This module contains functions for run time identification of Ambiq Micro
//! devices.
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
#include "am_util_id.h"


//*****************************************************************************
//
// Globals.
//
//*****************************************************************************
//
// Strings for use with pui8VendorName.
//
static const uint8_t g_DeviceNameApollo[]     = "Apollo";
static const uint8_t g_DeviceNameApollo2[]    = "Apollo2";
static const uint8_t g_DeviceNameApollo3[]    = "Apollo3 Blue";
static const uint8_t g_DeviceNameApollo3p[]   = "Apollo3 Blue Plus";
static const uint8_t g_ui8VendorNameAmbq[]    = "AMBQ";
static const uint8_t g_ui8VendorNameUnknown[] = "????";
static const uint8_t g_ui8DeviceNameUnknown[] = "Unknown device";

//*****************************************************************************
// Return the major version of the chip rev.
// Returns: 'A', 'B', 'C', ...
//*****************************************************************************
static uint32_t
revmaj_get(uint32_t ui32ChipRev)
{
    uint32_t ui32ret;

#ifdef _FLD2VAL
    ui32ret = _FLD2VAL(MCUCTRL_CHIPREV_REVMAJ, ui32ChipRev);
#else
    ui32ret = (ui32ChipRev & 0xF0) >> 4;
#endif

    //
    // Major revision is 1=A, 2=B, 3=C, ...
    // Convert to the expected return value.
    //
    return ui32ret + 'A' - 1;

} // revmaj_get()

//*****************************************************************************
// Update the ID structure with the appropriate ChipRev letter.
// ui32minrevbase should be 0 for Apollo or Apollo2, 1 for Apollo3.
//*****************************************************************************
static void
chiprev_set(am_util_id_t *psIDDevice, uint32_t ui32minrevbase)
{
    uint32_t ui32maj, ui32min;

    ui32maj = ((psIDDevice->sMcuCtrlDevice.ui32ChipRev & 0xF0) >> 4);
    psIDDevice->ui8ChipRevMaj  = (uint8_t)('A' - 1 + ui32maj);

    //
    // For Apollo and Apollo2:  rev0=0, rev1=1, ... (0-based)
    // For Apollo3:             rev0=1, rev1=2, ... (1-based)
    //
    ui32min = ((psIDDevice->sMcuCtrlDevice.ui32ChipRev & 0x0F) >> 0);
    psIDDevice->ui8ChipRevMin = (uint8_t)('0' + ui32min - ui32minrevbase);

} // chiprev_set()

//*****************************************************************************
//
//! @brief Device identification.
//!
//! @param psIDDevice - ptr to a device ID structure (am_util_id_t*) to be
//! filled in by the function.
//!
//! This function provides additional information about the currently running
//! Ambiq Micro MCU device.
//!
//! @returns The ui32Device value, which is a value corresponding to the
//! device type.
//
//*****************************************************************************
uint32_t
am_util_id_device(am_util_id_t *psIDDevice)
{
    uint32_t ui32PN, ui32ChipRev;

    //
    // Go get all the device (hardware) info from the HAL
    //
#if AM_APOLLO3_MCUCTRL
    am_hal_mcuctrl_info_get(AM_HAL_MCUCTRL_INFO_DEVICEID, &psIDDevice->sMcuCtrlDevice);
#else // AM_APOLLO3_MCUCTRL
    am_hal_mcuctrl_device_info_get(&psIDDevice->sMcuCtrlDevice);
#endif // AM_APOLLO3_MCUCTRL

    //
    // Device identification
    //
    ui32PN = psIDDevice->sMcuCtrlDevice.ui32ChipPN  &
             AM_UTIL_MCUCTRL_CHIP_INFO_PARTNUM_PN_M;
    ui32ChipRev = psIDDevice->sMcuCtrlDevice.ui32ChipRev;

    if ( (psIDDevice->sMcuCtrlDevice.ui32JedecCID   == 0xB105100D)          &&
         (psIDDevice->sMcuCtrlDevice.ui32JedecJEPID == 0x0000009B)          &&
         ((psIDDevice->sMcuCtrlDevice.ui32JedecPN & 0xF00) != 0xE00) )
    {
        //
        // It's Ambiq Micro, set up the VENDORID.
        //
        psIDDevice->pui8VendorName = g_ui8VendorNameAmbq;
    }
    else
    {
        //
        // For now, set it as unknown vendor, but we may change it later.
        //
        psIDDevice->pui8VendorName = g_ui8VendorNameUnknown;
    }

    if ( psIDDevice->sMcuCtrlDevice.ui32VendorID ==
         (('A' << 24) | ('M' << 16) | ('B' << 8) | ('Q' << 0)) )
    {
        //
        // VENDORID is AMBQ, so set the string pointer.
        //
        psIDDevice->pui8VendorName = g_ui8VendorNameAmbq;
    }

    if ( ( ui32PN == AM_UTIL_MCUCTRL_CHIP_INFO_PARTNUM_APOLLO )             &&
         ((psIDDevice->sMcuCtrlDevice.ui32JedecPN & 0x0F0) == 0x0E0) )
    {
        psIDDevice->ui32Device = AM_UTIL_ID_APOLLO;
        psIDDevice->pui8DeviceName = g_DeviceNameApollo;
        chiprev_set(psIDDevice, 0);

        //
        // Force the vendor name for Apollo, which did not support VENDORID.
        //
        psIDDevice->pui8VendorName = g_ui8VendorNameAmbq;
    }
    else if ( ( ui32PN == AM_UTIL_MCUCTRL_CHIP_INFO_PARTNUM_APOLLO2 )       &&
              ((psIDDevice->sMcuCtrlDevice.ui32JedecPN & 0x0F0) == 0x0D0) )
    {
        psIDDevice->ui32Device = AM_UTIL_ID_APOLLO2;
        psIDDevice->pui8DeviceName = g_DeviceNameApollo2;
        chiprev_set(psIDDevice, 0);
    }
    else if ( ( ui32PN == AM_UTIL_MCUCTRL_CHIP_INFO_PARTNUM_APOLLO3 )       &&
              ((psIDDevice->sMcuCtrlDevice.ui32JedecPN & 0x0F0) == 0x0C0)   &&
              ( revmaj_get(ui32ChipRev) <= 'B' ) )
    {
        psIDDevice->ui32Device = AM_UTIL_ID_APOLLO3;
        psIDDevice->pui8DeviceName = g_DeviceNameApollo3;
        chiprev_set(psIDDevice, 1);
    }
    else if ( ( ui32PN == AM_UTIL_MCUCTRL_CHIP_INFO_PARTNUM_APOLLO3P)       &&
              ((psIDDevice->sMcuCtrlDevice.ui32JedecPN & 0x0F0) == 0x0C0)   &&
              ( revmaj_get(ui32ChipRev) == 'C' ) )
    {
        psIDDevice->ui32Device = AM_UTIL_ID_APOLLO3P;
        psIDDevice->pui8DeviceName = g_DeviceNameApollo3p;
        chiprev_set(psIDDevice, 1);
    }
    else
    {
        psIDDevice->ui32Device = AM_UTIL_ID_UNKNOWN;
        psIDDevice->pui8DeviceName = g_ui8DeviceNameUnknown;
        psIDDevice->ui8ChipRevMaj = (uint8_t)'?';
        psIDDevice->ui8ChipRevMin = (uint8_t)' ';
    }

    return psIDDevice->ui32Device;
}
