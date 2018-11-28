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

#ifndef LMGR_API_ADV_MASTER_AE_H
#define LMGR_API_ADV_MASTER_AE_H

#include "lmgr_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************************************
  Data Types
**************************************************************************************************/

/*! \brief      Periodic advertising sync established event */
typedef struct
{
  uint8_t       advSID;         /*!< Advertising SID. */
  uint8_t       addrType;       /*!< Address type. */
  bdAddr_t      addr;           /*!< Address. */
  uint8_t       advPhy;         /*!< Advertising PHY. */
  uint16_t      advInterval;    /*!< Advertising interval. */
  uint8_t       advClkAccuracy; /*!< Advertising clock accuracy. */
} lmgrPerAdvSyncEstdInd_t;

/**************************************************************************************************
  Function Declarations
**************************************************************************************************/

/* Events */
void LmgrSendExtScanEnableCnf(uint8_t status);
void LmgrSendScanTimeoutInd(void);
void LmgrSendExtAdvRptInd(LlExtAdvReportInd_t *pEvt);
void LmgrSendPerAdvRptInd(LlPerAdvReportInd_t *pEvt);
void LmgrSendSyncEstInd(uint8_t status, uint16_t handle, lmgrPerAdvSyncEstdInd_t *pEvt);
void LmgrSendSyncLostInd(uint16_t handle);

#ifdef __cplusplus
};
#endif

#endif /* LMGR_API_ADV_MASTER_AE_H */
