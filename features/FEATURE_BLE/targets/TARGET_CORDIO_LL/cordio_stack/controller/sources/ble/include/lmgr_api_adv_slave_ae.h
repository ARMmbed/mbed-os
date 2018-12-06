/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      Link layer manager advertising extension interface file.
 *
 *  Copyright (c) 2009-2018 ARM Ltd., all rights reserved.
 *  SPDX-License-Identifier: LicenseRef-PBL
 *
 *  This file and the related binary are licensed under the
 *  Permissive Binary License, Version 1.0 (the "License");
 *  you may not use these files except in compliance with the License.
 *
 *  You may obtain a copy of the License here:
 *  LICENSE-permissive-binary-license-1.0.txt and at
 *  https://www.mbed.com/licenses/PBL-1.0
 *
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
/*************************************************************************************************/

#ifndef LMGR_API_ADV_SLAVE_AE_H
#define LMGR_API_ADV_SLAVE_AE_H

#include "lmgr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Events */
void LmgrSendExtAdvEnableCnf(uint8_t handle, uint8_t status);
void LmgrSendScanReqReceivedInd(uint8_t handle, uint8_t scanAddrType, uint64_t scanAddr);
void LmgrSendAdvSetTermInd(uint8_t handle, uint8_t status, uint16_t connHandle, uint8_t numEvents);
void LmgrSendPeriodicAdvEnableCnf(uint8_t handle, uint8_t status);

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_ADV_SLAVE_AE_H */
