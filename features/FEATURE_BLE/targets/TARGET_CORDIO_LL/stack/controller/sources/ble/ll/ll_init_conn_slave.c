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

#include "lctr_api_adv_slave.h"
#include "lctr_api_conn.h"
#include "lmgr_api_adv_slave.h"
#include "lmgr_api_conn.h"
#include "wsf_assert.h"

/*************************************************************************************************/
/*!
 *  \brief      Initialize memory for connections.
 *
 *  \param      pFreeMem        Pointer to free memory.
 *  \param      freeMemSize     Size of pFreeMem.
 *
 *  \return     Amount of free memory consumed.
 *
 *  This function allocates memory for connections.
 *
 *  \note       This routine must be called after LlInitRunTimeCfg() but only once before any
 *              other initialization routines.
 */
/*************************************************************************************************/
uint16_t LlInitConnMem(uint8_t *pFreeMem, uint32_t freeMemSize)
{
  WSF_ASSERT(pLctrRtCfg);
  WSF_ASSERT(pFreeMem);

  uint16_t bytesUsed = 0;

#if (LL_MAX_CONN > 0)       /* connections capable */
  bytesUsed = LctrInitConnMem(pFreeMem, freeMemSize);
#endif

  return bytesUsed;
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize LL subsystem for operation as a connectable slave.
 *
 *  \return     None.
 *
 *  This function initializes the LL subsystem for use as an advertising and connectable slave.
 */
/*************************************************************************************************/
void LlConnSlaveInit(void)
{
  WSF_ASSERT(pLctrRtCfg);     /* Runtime configuration must be available. */

  LmgrConnInit();
  LctrSlvConnInit();
}

/*************************************************************************************************/
/*!
 *  \brief      Register ACL handler.
 *
 *  \param      sendCompCback   Client ACL send complete callback function.
 *  \param      recvPendCback   Client ACL receive pending callback function.
 *
 *  \return     None.
 *
 *  This function is called by a client to register for ACL data.
 */
/*************************************************************************************************/
void LlAclRegister(llAclCback_t sendCompCback, llAclCback_t recvPendCback)
{
  lmgrPersistCb.sendCompCback = sendCompCback;
  lmgrPersistCb.recvPendCback = recvPendCback;
}
