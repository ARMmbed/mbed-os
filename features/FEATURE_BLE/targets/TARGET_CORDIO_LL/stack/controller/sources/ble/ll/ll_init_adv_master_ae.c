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
 * \brief Link layer (LL) master initialization implementation file.
 */
/*************************************************************************************************/

#include "ll_api.h"
#include "lmgr_api_adv_master.h"
#include "lctr_api_adv_master_ae.h"
#include "wsf_assert.h"

/*************************************************************************************************/
/*!
 *  \brief      Initialize memory for extended scanning.
 *
 *  \param      pFreeMem        Pointer to free memory.
 *  \param      freeMemSize     Size of pFreeMem.
 *
 *  \return     Amount of free memory consumed.
 *
 *  This function allocates memory for extended scanning.
 *
 *  \note       This routine must be called after LlInitRunTimeCfg() but only once before any
 *              other initialization routines.
 */
/*************************************************************************************************/
uint16_t LlInitExtScanMem(uint8_t *pFreeMem, uint32_t freeMemSize)
{
  WSF_ASSERT(pLctrRtCfg);
  WSF_ASSERT(pFreeMem);

  return LctrInitExtScanMem(pFreeMem, freeMemSize);
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL subsystem for operation as an scanning master.
 *
 *  \return     None.
 *
 *  This function initializes the LL subsystem for use as an scanning master.
 */
/*************************************************************************************************/
void LlExtScanMasterInit(void)
{
  WSF_ASSERT(pLctrRtCfg);     /* Runtime configuration must be available. */

  LmgrMstInit();
  LctrMstExtScanInit();
  LctrMstPerCreateSyncInit();
  LctrMstPerTransferSyncInit();
  LctrMstPerScanInit();
}
