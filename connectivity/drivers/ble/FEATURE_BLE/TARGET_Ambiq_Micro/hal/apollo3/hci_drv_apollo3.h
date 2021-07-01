//*****************************************************************************
//
//! @file hci_drv_apollo3.h
//!
//! @brief Support functions for the Nationz BTLE radio in Apollo3.
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2019, Ambiq Micro
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
// This is part of revision 2.3.2 of the AmbiqSuite Development Package.
//
//*****************************************************************************
// SPDX-License-Identifier: BSD-3-Clause
#ifndef HCI_DRV_APOLLO3_H
#define HCI_DRV_APOLLO3_H
#include "wsf_os.h"
#ifdef __cplusplus
extern "C"
{
#endif
  //*****************************************************************************
  //
  // NATIONZ vendor specific events
  //
  //*****************************************************************************

  // Tx power level in dBm.
  typedef enum
  {
    TX_POWER_LEVEL_MINUS_10P0_dBm = 0x3,
    TX_POWER_LEVEL_0P0_dBm = 0x8,
    TX_POWER_LEVEL_PLUS_3P0_dBm = 0xF,
    TX_POWER_LEVEL_INVALID = 0x10,
  } txPowerLevel_t;

#define HCI_DRV_SPECIFIC_ERROR_START 0x09000000
  typedef enum
  {
    HCI_DRV_TRANSMIT_QUEUE_FULL = HCI_DRV_SPECIFIC_ERROR_START,
    HCI_DRV_TX_PACKET_TOO_LARGE,
    HCI_DRV_RX_PACKET_TOO_LARGE,
    HCI_DRV_BLE_STACK_UNABLE_TO_ACCEPT_PACKET,
    HCI_DRV_PACKET_TRANSMIT_FAILED,
    HCI_DRV_IRQ_STUCK_HIGH,
    HCI_DRV_TOO_MANY_PACKETS,
  } hci_drv_error_t;

  typedef void (*hci_drv_error_handler_t)(uint32_t ui32Error);

  bool_t HciVsA3_SetRfPowerLevelEx(txPowerLevel_t txPowerlevel);
  void HciVsA3_ConstantTransmission(uint8_t txchannel);
  void HciVsA3_CarrierWaveMode(uint8_t txchannel);

  //*****************************************************************************
  //
  // Hci driver functions unique to Apollo3
  //
  //*****************************************************************************
  extern void HciDrvHandler(wsfEventMask_t event, wsfMsgHdr_t *pMsg);
  extern void HciDrvHandlerInit(wsfHandlerId_t handlerId);
  extern void HciDrvIntService(void);

  uint16_t ap3_hciDrvWrite(uint8_t type, uint16_t len, uint8_t *pData);

  extern void HciDrvRadioBoot(bool bColdBoot);
  extern void HciDrvRadioShutdown(void);

#ifdef __cplusplus
};
#endif

#endif // HCI_DRV_APOLLO3_H
