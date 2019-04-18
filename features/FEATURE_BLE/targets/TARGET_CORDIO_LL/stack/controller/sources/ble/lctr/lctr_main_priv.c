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
 * \brief Link layer controller privacy implementation file.
 */
/*************************************************************************************************/

#include "lctr_int.h"
#include "lctr_int_priv.h"
#include "lctr_api_priv.h"
#include "bb_ble_api_reslist.h"
#include "sch_api.h"
#include "bb_ble_api.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "wsf_cs.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/**************************************************************************************************
  Globals
**************************************************************************************************/

/*! \brief      Privacy context. */
lctrPrivCtx_t lctrPriv;

/*************************************************************************************************/
/*!
 *  \brief      Restart resolvable private address timer.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrRestartResPrivAddrTimer(void)
{
  lctrPriv.tmrResPrivAddrTimeout.handlerId = lmgrPersistCb.handlerId;
  WsfTimerStartSec(&lctrPriv.tmrResPrivAddrTimeout, lmgrPrivCb.resPrivAddrTimeout);
}

/*************************************************************************************************/
/*!
 *  \brief      Privacy message dispatcher.
 *
 *  \param      pMsg    Pointer to message buffer.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrPrivDisp(LctrPrivMsg_t *pMsg)
{
  WSF_CS_INIT(cs);

  switch (pMsg->hdr.event)
  {
    case LCTR_PRIV_MSG_RES_PRIV_ADDR_TIMEOUT:
    {
      BbBleResListHandleTimeout();
      lctrRestartResPrivAddrTimer();
      break;
    }

    case LCTR_PRIV_MSG_ADDR_RES_NEEDED:
    {
      lctrAddrResNeededMsg_t *pAddrResNeededMsg = (lctrAddrResNeededMsg_t *)pMsg;

      if (pAddrResNeededMsg->peer)
      {
        (void) BbBleResListResolvePeer(pAddrResNeededMsg->rpa, &pAddrResNeededMsg->peerAddrType,
                                       &pAddrResNeededMsg->peerIdentityAddr);
      }
      else
      {
        (void) BbBleResListResolveLocal(pAddrResNeededMsg->rpa, &pAddrResNeededMsg->peerAddrType,
                                        &pAddrResNeededMsg->peerIdentityAddr);
      }

      WSF_CS_ENTER(cs);
      lmgrPrivCb.numPendingAddrRes--;
      WSF_CS_EXIT(cs);
      break;
    }
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Pend address resolution.
 *
 *  \param      rpa   Resolvable private address.
 *  \param      peer  TRUE if this is a peer RPA.
 *  \param      peerAddrType       Peer identity address type.
 *  \param      peerIdentityAddr   Peer identity address.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrPrivPendAddrRes(uint64_t rpa, bool_t peer, uint8_t peerAddrType, uint64_t peerIdentityAddr)
{
  WSF_CS_INIT(cs);

  const uint8_t maxNumPendingAddrRes = 4;
  uint8_t numPendingAddrRes;

  /* Check whether we can resolve another address now. */
  WSF_CS_ENTER(cs);
  numPendingAddrRes = lmgrPrivCb.numPendingAddrRes;
  WSF_CS_EXIT(cs);
  if (numPendingAddrRes >= maxNumPendingAddrRes)
  {
    return;
  }

  /* Send message to resolve the address. */
  lctrAddrResNeededMsg_t *pMsg;

  if ((pMsg = (lctrAddrResNeededMsg_t *)WsfMsgAlloc(sizeof(*pMsg))) != NULL)
  {
    pMsg->hdr.dispId = LCTR_DISP_PRIV;
    pMsg->hdr.event = LCTR_PRIV_MSG_ADDR_RES_NEEDED;

    pMsg->rpa  = rpa;
    pMsg->peer = peer;
    pMsg->peerAddrType = peerAddrType;
    pMsg->peerIdentityAddr = peerIdentityAddr;
    WsfMsgSend(lmgrPersistCb.handlerId, pMsg);

    WSF_CS_ENTER(cs);
    lmgrPrivCb.numPendingAddrRes++;
    WSF_CS_EXIT(cs);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Set resolvable private address timeout.
 *
 *  \param      timeout   Timeout in seconds.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LctrPrivSetResPrivAddrTimeout(uint32_t timeout)
{
  lmgrPrivCb.resPrivAddrTimeout = timeout;
  lctrRestartResPrivAddrTimer();
}

/*************************************************************************************************/
/*!
 *  \brief      Initialize link layer controller resources for privacy.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void LctrPrivInit(void)
{
  lctrMsgDispTbl[LCTR_DISP_PRIV] = (LctrMsgDisp_t)lctrPrivDisp;

  memset(&lctrPriv, 0, sizeof(lctrPriv));

  if (pLctrRtCfg->btVer >= LL_VER_BT_CORE_SPEC_4_2)
  {
    lmgrPersistCb.featuresDefault |=
        LL_FEAT_PRIVACY;
  }

  /* Set callback for task-level address resolution. */
  BbBleResListSetAddrResNeededCback(lctrPrivPendAddrRes);

  /* Set up timer for address resolution timeout. */
  lctrMsgHdr_t *pMsg = (lctrMsgHdr_t *)&lctrPriv.tmrResPrivAddrTimeout.msg;
  /* pMsg->handle = 0;  */ /* not used */
  pMsg->dispId = LCTR_DISP_PRIV;
  pMsg->event = LCTR_PRIV_MSG_RES_PRIV_ADDR_TIMEOUT;
}
