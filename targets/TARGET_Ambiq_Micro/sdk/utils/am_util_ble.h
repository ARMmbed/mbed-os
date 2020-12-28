//*****************************************************************************
//
//! @file am_util_apollo3_ble.h
//!
//! @brief Useful BLE functions not covered by the HAL.
//!
//! This file contains functions for interacting with the Apollo3 BLE hardware
//! that are not already covered by the HAL. Most of these commands either
//! adjust RF settings or facilitate RF testing operations.
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

#ifndef AM_UTIL_BLE_H
#define AM_UTIL_BLE_H

//*****************************************************************************
//
// External function declarations.
//
//*****************************************************************************
#ifdef __cplusplus
extern "C"
{
#endif

extern uint32_t am_util_ble_set_constant_transmission(void *pHandle, bool enable);
extern uint32_t am_util_ble_transmitter_control(void *pHandle, uint8_t ui8TxCtrl);
extern uint32_t am_util_ble_init_rf_channel(void *pHandle);
extern uint32_t am_util_ble_BQB_test_init(void *pHandle, bool enable);
extern uint32_t am_util_ble_crystal_trim_set(void *pHandle, uint32_t ui32TrimValue);
extern uint32_t am_util_ble_hci_reset(void *pHandle);
extern uint32_t am_util_ble_trasmitter_test_ex(void *pHandle, uint8_t channel, uint8_t pattern);
extern uint32_t am_util_ble_receiver_test_ex(void *pHandle, uint8_t channel, uint32_t *recvpackets);
extern uint32_t am_util_ble_set_carrier_wave_ex(void *pHandle, uint8_t channel);
extern uint32_t am_util_ble_transmitter_control_ex(void *pHandle, uint8_t ui8TxChannel);
extern uint32_t am_util_ble_set_constant_transmission_ex(void *pHandle, uint8_t channel);
extern uint32_t am_util_ble_read_modex_value(void *pHandle);

#ifdef __cplusplus
}
#endif

#endif // AM_UTIL_BLE_H
