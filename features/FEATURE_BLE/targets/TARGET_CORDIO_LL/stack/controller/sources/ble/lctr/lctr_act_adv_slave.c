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
 * \brief Link layer controller slave advertising action routines.
 */
/*************************************************************************************************/

#include "lctr_int_adv_slave.h"
#include "sch_api.h"
#include "bb_ble_api_reslist.h"
#include "lctr_pdu_adv.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include "util/bstream.h"

/*************************************************************************************************/
/*!
 *  \brief      Notify host of direct connect timeout failure.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrNotifyHostDirectConnectTimeout(void)
{
  LlConnInd_t evt =
  {
    .hdr =
    {
      .param        = 0,
      .event        = LL_CONN_IND,
      .status       = LL_ERROR_CODE_ADV_TIMEOUT
    },

    .status         = LL_ERROR_CODE_ADV_TIMEOUT,
    .handle         = 0,
    .role           = LL_ROLE_SLAVE,
    .addrType       = lmgrSlvAdvCb.advParam.peerAddrType,
    .connInterval   = 0,
    .connLatency    = 0,
    .supTimeout     = 0,
    .clockAccuracy  = 0
  };

  uint8_t *pBuf = (uint8_t *)&evt.peerAddr;
  BDA64_TO_BSTREAM(pBuf, lmgrSlvAdvCb.advParam.peerAddr);

  LL_TRACE_INFO0("### LlEvent ###  LL_CONN_IND, status=LL_ERROR_CODE_ADV_TIMEOUT");

  LmgrSendEvent((LlEvt_t *)&evt);
}

/*************************************************************************************************/
/*!
 *  \brief      Common advertise resource cleanup.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrAdvCleanup(void)
{
  LmgrDecResetRefCount();
  lmgrCb.advEnabled = FALSE;
  if (lmgrSlvAdvCb.advParam.advFiltPolicy)
  {
    LmgrDecWhitelistRefCount();
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Start advertising.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrAdvActStart(void)
{
  BbStart(BB_PROT_BLE);

  LmgrIncResetRefCount();
  lmgrCb.advEnabled = TRUE;
  if (lmgrSlvAdvCb.advParam.advFiltPolicy)
  {
    LmgrIncWhitelistRefCount();
  }
  lctrSlvAdvBuildOp();

  LmgrSendAdvEnableCnf(LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      Start advertising.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrAdvActSelfStart(void)
{
  BbStart(BB_PROT_BLE);

  LmgrIncResetRefCount();
  lmgrCb.advEnabled = TRUE;
  if (lmgrSlvAdvCb.advParam.advFiltPolicy)
  {
    LmgrIncWhitelistRefCount();
  }
  lctrSlvAdvBuildOp();
}

/*************************************************************************************************/
/*!
 *  \brief      Shutdown active advertising operation.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrAdvActShutdown(void)
{
  if (lmgrCb.advEnabled)
  {
    lctrSlvAdv.shutdown = TRUE;
    SchRemove(&lctrSlvAdv.advBod);

    /* Shutdown completes with events generated in BOD end callback. */
  }
  else
  {
      /* TODO Can this occur if a disable or cancel is received after a advertising start fails? */
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Send advertising operation confirm.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrAdvActAdvCnf(void)
{
  LmgrSendAdvEnableCnf(LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      Send advertising operation command disallowed.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrAdvActDisallowAdvCnf(void)
{
  LmgrSendAdvEnableCnf(LL_ERROR_CODE_CMD_DISALLOWED);
}

/*************************************************************************************************/
/*!
 *  \brief      Operation self terminated (e.g. on connection indication).
 *
 *  \return     None.
 *
 *  Slave self-termination is a result of the reception of a CONN_IND.
 */
/*************************************************************************************************/
void lctrAdvActSelfTerm(void)
{
  if (lmgrCb.advEnabled)
  {
    if (lctrSlvAdv.connIndRcvd)
    {
      BbBleData_t * const pBle = &lctrSlvAdv.bleData;
      BbBleSlvAdvEvent_t * const pAdv = &pBle->op.slvAdv;
      uint8_t *pBuf = pAdv->pRxReqBuf;

      if (pBuf)
      {
        bool_t restartAdv = FALSE;

        /* If peer address was not resolved, attempt to resolve it now. */
        if (!BbBlePduFiltCheck(pBuf, &pBle->pduFilt, TRUE, &pAdv->filtResults))
        {
          /* Restart advertising if address resolution fails or PDU is still not allowed. */
          restartAdv = TRUE;
        }

        lctrConnEstablish_t *pMsg;
        if ((pMsg = (lctrConnEstablish_t *)WsfMsgAlloc(sizeof(*pMsg))) != NULL)
        {
          lctrUnpackConnIndPdu(&pMsg->connInd, pBuf + LL_ADV_HDR_LEN);

          if (!lctrValidateConnIndPdu(&pMsg->connInd))
          {
            restartAdv = TRUE;
          }
        }

        if (restartAdv)
        {
          if (pMsg != NULL)
          {
            WsfMsgFree(pMsg);
          }

          /* Reuse message. */
          lctrMsgHdr_t *pResMsg = (lctrMsgHdr_t *)pBuf - 1;
          pResMsg->dispId = LCTR_DISP_ADV;
          pResMsg->event = LCTR_ADV_MSG_INT_START;
          WsfMsgSend(lmgrPersistCb.handlerId, pResMsg);

          BbStop(BB_PROT_BLE);
        }
        else
        {
          if (pMsg != NULL)
          {
            /* pMsg->hdr.handle = 0; */
            pMsg->hdr.dispId = LCTR_DISP_CONN_IND;
            /* pMsg->hdr.event = 0; */

            pMsg->connIndEndTs = lctrSlvAdv.reqEndTs;

            BbBlePduFiltResultsGetPeerIdAddr(&pAdv->filtResults, &pMsg->peerIdAddr, &pMsg->peerIdAddrType);
            BbBlePduFiltResultsGetPeerRpa(&pAdv->filtResults, &pMsg->peerRpa);

            pMsg->localRpa = lmgrSlvAdvCb.localRpa;

            pMsg->usedChSel = lctrSlvAdv.usedChSel;
            pMsg->phy = LL_PHY_LE_1M;
            pMsg->sendAdvSetTerm = FALSE;
            pMsg->isAuxConnReq = FALSE;
            /* pMsg->numExtAdvEvents = 0; */        /* Not used when sendAdvSetTerm = FALSE. */

            WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
          }

          WsfMsgFree((lctrMsgHdr_t *)pBuf - 1);
        }
      }
    }
    else
    {
      lctrSlvAdvCleanupOp();

      if (lmgrSlvAdvCb.advParam.advType == LL_ADV_CONN_DIRECT_HIGH_DUTY)
      {
        lctrNotifyHostDirectConnectTimeout();
      }

      BbStop(BB_PROT_BLE);
    }

    lctrAdvCleanup();
  }
  else
  {
    /* This occurs when an advertise enable fails. */
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Terminated advertising after host advertising disable.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrAdvActAdvTerm(void)
{
  BbStop(BB_PROT_BLE);

  lctrAdvCleanup();
  lctrSlvAdvCleanupOp();

  LmgrSendAdvEnableCnf(LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      Terminated advertising after host reset.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrAdvActResetTerm(void)
{
  BbStop(BB_PROT_BLE);
  lctrAdvCleanup();
  lctrSlvAdvCleanupOp();
}

/*************************************************************************************************/
/*!
 *  \brief      Update advertising parameters.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrAdvActUpdateAdvParam(void)
{
  lctrAdvParamMsg_t *pMsg = (lctrAdvParamMsg_t *)pLctrMsg;

  lmgrSlvAdvCb.advParam = pMsg->param;
}
