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
 * \brief Link layer controller master connection state machine action routines.
 */
/*************************************************************************************************/

#include "lctr_int_init_master.h"
#include "sch_api.h"
#include "sch_api_ble.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief      Start initiate connection scan.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrInitActInitiate(void)
{
  bool_t result = TRUE;
  lctrConnCtx_t *pCtx;
  uint32_t connInterUsec;
  lctrInitiateMsg_t *pInitMsg = (lctrInitiateMsg_t *)pLctrMsg;

  do
  {
    if ((pCtx = lctrAllocConnCtx()) == NULL)
    {
      result = FALSE;
      lctrScanNotifyHostInitiateError(LL_ERROR_CODE_CONN_LIMIT_EXCEEDED, pInitMsg->peerAddrType,
                                      pInitMsg->peerAddr);
      break;
    }

    uint32_t interMinUsec = LCTR_CONN_IND_US(pInitMsg->connSpec.connIntervalMin);
    uint32_t interMaxUsec = LCTR_CONN_IND_US(pInitMsg->connSpec.connIntervalMax);
    uint32_t durUsec = pCtx->localConnDurUsec;

    if (!SchRmAdd(LCTR_GET_CONN_HANDLE(pCtx), SCH_RM_PREF_PERFORMANCE, interMinUsec, interMaxUsec, durUsec, &connInterUsec, lctrGetConnRefTime))
    {
      lctrFreeConnCtx(pCtx);
      result = FALSE;
      lctrScanNotifyHostInitiateError(LL_ERROR_CODE_CONN_REJ_LIMITED_RESOURCES, pInitMsg->peerAddrType, pInitMsg->peerAddr);
      break;
    }
  } while (FALSE);

  if (!result)
  {
    lctrMsgHdr_t *pTermMsg;

    /* Send SM a terminate event. */
    if ((pTermMsg = (lctrMsgHdr_t *)WsfMsgAlloc(sizeof(*pTermMsg))) != NULL)
    {
      pTermMsg->dispId = LCTR_DISP_INIT;
      pTermMsg->event = LCTR_INIT_MSG_TERMINATE;

      WsfMsgSend(lmgrPersistCb.handlerId, pTermMsg);
    }

    return;
  }

  lctrMstInit.data.init.connHandle = LCTR_GET_CONN_HANDLE(pCtx);
  lctrMstInit.data.init.connInterval = LCTR_US_TO_CONN_IND(connInterUsec);
  lctrMstInit.data.init.connBodLoaded = FALSE;
  lctrMstInit.scanParam = pInitMsg->scanParam;

  BbStart(BB_PROT_BLE);

  LmgrIncResetRefCount();
  lmgrCb.numInitEnabled = 1;
  switch (lctrMstInit.scanParam.scanFiltPolicy)
  {
    case LL_SCAN_FILTER_WL_BIT:
    case LL_SCAN_FILTER_WL_OR_RES_INIT:
      LmgrIncWhitelistRefCount();
      break;
    default:
      break;
  }
  lctrMstInit.backoffCount = 1;
  lctrMstInit.upperLimit = 1;

  lctrMstInitiateBuildOp(&pInitMsg->connSpec, pInitMsg->peerAddrType, pInitMsg->peerAddr);
  lctrMstConnBuildOp(pCtx, &lctrMstInit.data.init.connInd);

  lctrMstInitiateOpCommit();

  lmgrCb.scanMode = LMGR_SCAN_MODE_INITIATE;
}

/*************************************************************************************************/
/*!
 *  \brief      Establish connection.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrInitActConnect(void)
{
  if (lmgrCb.numInitEnabled)
  {
    BbBleData_t * const pBle = &lctrMstInit.bleData;
    BbBleMstAdvEvent_t * const pScan = &pBle->op.mstAdv;

    lctrConnEstablish_t *pMsg;
    lctrConnCtx_t *pCtx;

    if ((pMsg = (lctrConnEstablish_t *)WsfMsgAlloc(sizeof(*pMsg))) != NULL)
    {
      pCtx = LCTR_GET_CONN_CTX(lctrMstInit.data.init.connHandle);

      pCtx->role = LL_ROLE_MASTER;

      pMsg->hdr.handle = LCTR_GET_CONN_HANDLE(pCtx);
      pMsg->hdr.dispId = LCTR_DISP_CONN;
      pMsg->hdr.event = LCTR_CONN_MST_ESTABLISH;

      pMsg->connInd = lctrMstInit.data.init.connInd;
      /* Not used by master. */
      /* pMsg->connIndEndTs = 0; */

      BbBlePduFiltResultsGetPeerIdAddr(&pScan->filtResults, &pMsg->peerIdAddr, &pMsg->peerIdAddrType);
      BbBlePduFiltResultsGetPeerRpa(&pScan->filtResults, &pMsg->peerRpa);

      pMsg->localRpa = lctrMstInit.data.init.localRpa;

      pMsg->usedChSel = lctrMstInit.data.init.usedChSel;
      pMsg->phy = LL_PHY_LE_1M;
      /* Not used by master. */
      /* pMsg->sendAdvSetTerm = FALSE; */
      /* pMsg->numExtAdvEvents = 0; */

      WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
    }

    /* Terminate scan. */
    BbStop(BB_PROT_BLE);
    lctrScanCleanup(&lctrMstInit);
  }
  else
  {
    /* This occurs when an initiate start fails. */
    lctrInitActShutdown();
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Shutdown active initiation operation.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrInitActShutdown(void)
{
  if (lmgrCb.numInitEnabled)
  {
    lctrMstInit.shutdown = TRUE;
    SchRemove(&lctrMstInit.scanBod);

    /* Shutdown completes with events generated in BOD end callback. */
  }
  else
  {
    /* TODO Can this occur if a reset or cancel is received after a initiate start fails? */
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Terminated scan after host initiate disable.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrInitActScanTerm(void)
{
  BbStop(BB_PROT_BLE);

  lctrScanCleanup(&lctrMstInit);

  SchRmRemove(lctrMstInit.data.init.connHandle);
  lctrFreeConnCtx(LCTR_GET_CONN_CTX(lctrMstInit.data.init.connHandle));

  LlCreateConnCancelCnf_t evt =
  {
      .hdr =
      {
          .event  = LL_CREATE_CONN_CANCEL_CNF,
          .status = LL_SUCCESS
      },

      .status = LL_SUCCESS,
  };

  LL_TRACE_INFO0("### LlEvent ###  LL_CREATE_CONN_CANCEL_CNF, status=LL_SUCCESS");

  LmgrSendEvent((LlEvt_t *)&evt);
  /* Send connection complete event after response to create connection cancel command. */
  BbBleData_t * const pBle = &lctrMstInit.bleData;
  lctrScanNotifyHostInitiateError(LL_ERROR_CODE_UNKNOWN_CONN_ID,
                                  BB_BLE_PDU_FILT_FLAG_IS_SET(&pBle->pduFilt, PEER_ADDR_MATCH_RAND),
                                  pBle->pduFilt.peerAddrMatch);
}

/*************************************************************************************************/
/*!
 *  \brief      Terminated scan after host reset.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrInitActResetTerm(void)
{
  BbStop(BB_PROT_BLE);
  lctrScanCleanup(&lctrMstInit);
}

/*************************************************************************************************/
/*!
 *  \brief      Notify host disallowing initiate.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrInitActDisallowInitiate(void)
{
  lctrInitiateMsg_t *pInitMsg = (lctrInitiateMsg_t *)pLctrMsg;

  lctrScanNotifyHostInitiateError(LL_ERROR_CODE_CMD_DISALLOWED, pInitMsg->peerAddrType, pInitMsg->peerAddr);
}

/*************************************************************************************************/
/*!
 *  \brief      Notify host disallowing initiate cancel.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrInitActDisallowCancel(void)
{
  LlCreateConnCancelCnf_t evt =
  {
    .hdr =
    {
      .event  = LL_CREATE_CONN_CANCEL_CNF,
      .status = LL_ERROR_CODE_CMD_DISALLOWED
    },

    .status = LL_ERROR_CODE_CMD_DISALLOWED,
  };

  LL_TRACE_INFO0("### LlEvent ###  LL_CREATE_CONN_CANCEL_CNF, status=LL_ERROR_CODE_CMD_DISALLOWED");

  LmgrSendEvent((LlEvt_t *)&evt);
}
