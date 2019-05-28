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
 * \brief Link layer (LL) slave initialization implementation file.
 */
/*************************************************************************************************/

#include "ll_api.h"
#include "lmgr_api_adv_slave.h"
#include "lctr_api_adv_slave_ae.h"
#include "wsf_assert.h"

/*************************************************************************************************/
/*!
 *  \brief      Initialize memory for extended advertising.
 *
 *  \param      pFreeMem        Pointer to free memory.
 *  \param      freeMemSize     Size of pFreeMem.
 *
 *  \return     Amount of free memory consumed.
 *
 *  This function allocates memory for extended advertising.
 *
 *  \note       This routine must be called after LlInitRunTimeCfg() but only once before any
 *              other initialization routines.
 */
/*************************************************************************************************/
uint16_t LlInitExtAdvMem(uint8_t *pFreeMem, uint32_t freeMemSize)
{
  WSF_ASSERT(pLctrRtCfg);
  WSF_ASSERT(pFreeMem);

  uint16_t bytesUsed = 0;

#if (LL_MAX_ADV_SETS > 0)   /* extended advertising capable */
  bytesUsed = LctrInitAdvSetMem(pFreeMem, freeMemSize);
#endif

  return bytesUsed;
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL subsystem for operation for extended advertising slave.
 *
 *  \return     None.
 *
 *  This function initializes the LL subsystem for use as an extended advertising slave.
 */
/*************************************************************************************************/
void LlExtAdvSlaveInit(void)
{
  WSF_ASSERT(pLctrRtCfg);     /* Runtime configuration must be available. */

  LmgrSlvInit();
  LctrSlvExtAdvInit();
  LctrSlvPeriodicAdvInit();
}
