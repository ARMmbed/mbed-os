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

#include "lctr_int_init_master_ae.h"
#include "lctr_int_conn_master.h"
#include "lmgr_api_adv_master_ae.h"
#include "sch_api_ble.h"
#include "wsf_assert.h"
#include "util/bstream.h"

/*************************************************************************************************/
/*!
 *  \brief      Setup connection context and BOD.
 *
 *  \param      pExtInitCtx     Extended initiate context.
 *  \param      pConnSpec       Connection spec.
 *
 *  \return     TRUE if successful, FALSE otherwise.
 */
/*************************************************************************************************/
static uint8_t lctrExtInitSetupConn(lctrExtScanCtx_t *pExtInitCtx, LlConnSpec_t *pConnSpec)
{
  lctrConnCtx_t *pCtx;
  uint32_t connInterUsec;

  /* Allow only one connection between two devices. */
  if ((pCtx = lctrAllocConnCtx()) == NULL)
  {
    return LL_ERROR_CODE_CONN_LIMIT_EXCEEDED;
  }

  uint32_t interMinUsec = LCTR_CONN_IND_US(pConnSpec->connIntervalMin);
  uint32_t interMaxUsec = LCTR_CONN_IND_US(pConnSpec->connIntervalMax);
  uint32_t durUsec = pCtx->localConnDurUsec;

  if (!SchRmAdd(LCTR_GET_CONN_HANDLE(pCtx), SCH_RM_PREF_PERFORMANCE, interMinUsec, interMaxUsec, durUsec, &connInterUsec, lctrGetConnRefTime))
  {
    lctrFreeConnCtx(pCtx);
    return LL_ERROR_CODE_CONN_REJ_LIMITED_RESOURCES;
  }

  pExtInitCtx->data.init.connHandle = LCTR_GET_CONN_HANDLE(pCtx);
  pExtInitCtx->data.init.connInterval = LCTR_US_TO_CONN_IND(connInterUsec);

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Start initiate connection scan.
 *
 *  \param      pExtInitCtx     Extended initiate context.
 *  \param      peerAddrType    Peer address type.
 *  \param      peerAddr        Peer address.
 *  \param      filtPolicy      Initiate filter policy.
 *  \param      ownAddrType     Own address type.
 *
 *  \return     Status code.
 */
/*************************************************************************************************/
uint8_t lctrExtInitSetupInitiate(lctrExtScanCtx_t *pExtInitCtx, uint8_t peerAddrType, uint64_t peerAddr,
                                 uint8_t filtPolicy, uint8_t ownAddrType)
{
  uint8_t status;

  if ((status = lctrExtInitSetupConn(pExtInitCtx, &pExtInitCtx->data.init.connSpec)) != LL_SUCCESS)
  {
    lctrScanNotifyHostInitiateError(status, peerAddrType, peerAddr);
    lctrSendExtInitMsg(pExtInitCtx, LCTR_EXT_INIT_MSG_INITIATE_CANCEL);
    return status;
  }

  /* Reset state. */
  pExtInitCtx->selfTerm = FALSE;
  pExtInitCtx->shutdown = FALSE;
  pExtInitCtx->auxOpPending = FALSE;
  pExtInitCtx->bodTermCnt = 0;
  pExtInitCtx->data.init.filtPolicy = filtPolicy;
  pExtInitCtx->data.init.ownAddrType = ownAddrType;
  pExtInitCtx->data.init.connBodLoaded = FALSE;

  BbStart(BB_PROT_BLE);

  if (((status = lctrMstExtInitiateBuildOp(pExtInitCtx,
                                           &pExtInitCtx->data.init.connSpec,
                                           peerAddr,
                                           peerAddrType)) != LL_SUCCESS) ||
      ((status = lctrMstAuxInitiateBuildOp(pExtInitCtx,
                                           &pExtInitCtx->data.init.connSpec,
                                           peerAddr,
                                           peerAddrType)) != LL_SUCCESS))
  {
    BbStop(BB_PROT_BLE);
    lctrScanNotifyHostInitiateError(status, peerAddrType, peerAddr);
    lctrSendExtInitMsg(pExtInitCtx, LCTR_EXT_INIT_MSG_INITIATE_CANCEL);
    return status;
  }

  switch (pExtInitCtx->data.init.filtPolicy)
  {
    case LL_SCAN_FILTER_WL_BIT:
    case LL_SCAN_FILTER_WL_OR_RES_INIT:
      LmgrIncWhitelistRefCount();
      break;
    default:
      break;
  }

  lmgrCb.numInitEnabled++;
  LmgrIncResetRefCount();

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Establish connection.
 *
 *  \param      pExtInitCtx     Extended initiate context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtInitActConnect(lctrExtScanCtx_t *pExtInitCtx)
{
  lctrConnEstablish_t *pMsg;
  lctrConnCtx_t *pCtx;
  uint64_t peerIdAddr = 0;
  uint8_t peerIdAddrType = 0;
  uint64_t peerRpa = 0;

  if ((pMsg = (lctrConnEstablish_t *)WsfMsgAlloc(sizeof(*pMsg))) != NULL)
  {
    pCtx = LCTR_GET_CONN_CTX(pExtInitCtx->data.init.connHandle);

    pCtx->role = LL_ROLE_MASTER;

    pMsg->hdr.handle = LCTR_GET_CONN_HANDLE(pCtx);
    pMsg->hdr.dispId = LCTR_DISP_CONN;
    pMsg->hdr.event = LCTR_CONN_MST_ESTABLISH;

    pMsg->connInd = pExtInitCtx->data.init.connInd;

    if (pExtInitCtx->data.init.isLegacy == TRUE)
    {
      BbBleMstAdvEvent_t * const pScan = &pExtInitCtx->scanBleData.op.mstAdv;
      BbBlePduFiltResultsGetPeerIdAddr(&pScan->filtResults, &peerIdAddr, &peerIdAddrType);
      BbBlePduFiltResultsGetPeerRpa(&pScan->filtResults, &peerRpa);
    }
    else
    {
      BbBleMstAuxAdvEvent_t * const pAuxScan = &pExtInitCtx->auxBleData.op.mstAuxAdv;
      BbBlePduFiltResultsGetPeerIdAddr(&pAuxScan->filtResults, &peerIdAddr, &peerIdAddrType);
      BbBlePduFiltResultsGetPeerRpa(&pAuxScan->filtResults, &peerRpa);
    }

    pMsg->peerIdAddrType = peerIdAddrType;
    pMsg->peerIdAddr = peerIdAddr;
    pMsg->peerRpa = peerRpa;
    pMsg->localRpa = pExtInitCtx->data.init.localRpa;

    pMsg->usedChSel = pExtInitCtx->data.init.usedChSel;
    pMsg->phy = pExtInitCtx->data.init.phy;

    WsfMsgSend(lmgrPersistCb.handlerId, pMsg);
  }

  /* Shutdown scanner. */
  lctrMstExtInitCleanupOp(pExtInitCtx);
  lctrMstExtInit.estConnPhys = 1 << LCTR_GET_EXT_INIT_HANDLE(pExtInitCtx);
}

/*************************************************************************************************/
/*!
 *  \brief      Common initiate resource cleanup.
 *
 *  \param      pExtInitCtx     Extended initiate context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrMstExtInitCleanupOp(lctrExtScanCtx_t *pExtInitCtx)
{
  BbStop(BB_PROT_BLE);

  switch (pExtInitCtx->data.init.filtPolicy)
  {
    case LL_SCAN_FILTER_WL_BIT:
    case LL_SCAN_FILTER_WL_OR_RES_INIT:
      LmgrDecWhitelistRefCount();
      break;
    default:
      break;
  }

  BbBleMstAdvEvent_t * const pScan = &pExtInitCtx->scanBleData.op.mstAdv;
  if (pScan->pRxAdvBuf)
  {
    WsfMsgFree(pScan->pRxAdvBuf);
    pScan->pRxAdvBuf = NULL;
  }
  WSF_ASSERT(pScan->pRxRspBuf == NULL);

  WSF_ASSERT(lmgrCb.numInitEnabled > 0);
  lmgrCb.numInitEnabled--;
  LmgrDecResetRefCount();
}
