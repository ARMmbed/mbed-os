//*****************************************************************************
//
//! @file am_hal_ble_patch_b0.h
//!
//! @brief This is a binary patch for the BLE core.
//!
//! @addtogroup
//! @ingroup
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

#ifndef AM_HAL_BLE_PATCH_B0_H
#define AM_HAL_BLE_PATCH_B0_H

#ifdef __cplusplus
extern "C"
{
#endif

//*****************************************************************************
//
// Patch array pointer.
//
//*****************************************************************************
extern am_hal_ble_patch_t **am_hal_ble_default_patches_b0;
extern am_hal_ble_patch_t **am_hal_ble_default_copy_patches_b0;
extern const uint32_t am_hal_ble_num_default_patches_b0;

//*****************************************************************************
//
// Pointers for specific patches.
//
//*****************************************************************************
extern am_hal_ble_patch_t am_ble_performance_patch_b0;
extern am_hal_ble_patch_t am_ble_nvds_patch_b0;

//*****************************************************************************
//
// Default patch structure.
//
//*****************************************************************************
extern am_hal_ble_patch_t g_AMBLEDefaultPatchB0;

//*****************************************************************************
//
// Macros for accessing specific NVDS parameters.
//
//*****************************************************************************
#define AM_HAL_BLE_NVDS_CLOCKDRIFT_OFFSET          30
#define AM_HAL_BLE_NVDS_SLEEPCLOCKDRIFT_OFFSET     35
#define AM_HAL_BLE_NVDS_CLOCKSOURCE_OFFSET         44
#define AM_HAL_BLE_NVDS_SLEEPENABLE_OFFSET         85
#define AM_HAL_BLE_NVDS_AGC_OFFSET                 125

#ifdef __cplusplus
}
#endif

#endif // AM_HAL_BLE_PATCH_B0_H

//*****************************************************************************
//
// End Doxygen group.
//! @}
//
//*****************************************************************************
