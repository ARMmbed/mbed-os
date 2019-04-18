/* Copyright (c) 2019 Arm Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*************************************************************************************************/
/*!
 * \file
 * \brief Link layer manager advertising extension interface file.
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
