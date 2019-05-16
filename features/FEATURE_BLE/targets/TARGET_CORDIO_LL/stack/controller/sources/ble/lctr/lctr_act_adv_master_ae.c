/* Copyright (c) 2009-2019 Arm Limited
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
 *  \brief Link layer controller master scan action routines.
 */
/*************************************************************************************************/

#include "lctr_int_adv_master_ae.h"
#include "lctr_int_adv_master.h"
#include "lctr_api_adv_master_ae.h"
#include "lmgr_api_adv_master_ae.h"
#include "sch_api.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "wsf_trace.h"

#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief      Setup periodic scanning context.
 *
 *  \param      pPerCreateSync  Create sync control block.
 *  \param      pMsg            Create sync messgae.
 *
 *  \return     TRUE if successful, FALSE otherwise.
 */
/*************************************************************************************************/
static uint8_t lctrPerScanSetup(lctrPerCreateSyncCtrlBlk_t *pPerCreateSync, lctrPerCreateSyncMsg_t *pMsg)
{
  lctrPerScanCtx_t *pPerScanCtx;

  if ((pPerScanCtx = lctrAllocPerScanCtx()) == NULL)
  {
    return LL_ERROR_CODE_CONN_LIMIT_EXCEEDED;
  }

  pPerCreateSync->pPerScanCtx = pPerScanCtx;
  pPerScanCtx->filtParam.filterPolicy = pMsg->filterPolicy;
  pPerScanCtx->filtParam.advSID = pMsg->advSID;
  pPerScanCtx->filtParam.advAddrType = pMsg->advAddrType;
  pPerScanCtx->filtParam.advAddr = pMsg->advAddr;
  pPerScanCtx->skip = pMsg->skip;
  pPerScanCtx->syncTimeOutMs = LCTR_PER_SYNC_TIMEOUT_TO_MS(pMsg->syncTimeOut);

  return LL_SUCCESS;
}

/*************************************************************************************************/
/*!
 *  \brief      Pack periodic advertising sync established report.
 *
 *  \param      pPerScanCtx Periodic scan context.
 *  \param      pRpt        Periodic advertising report.
 *
 *  \return     TRUE if pack report successful, FALSE otherwise.
 */
/*************************************************************************************************/
static bool_t lctrPerAdvSyncEstRptPack(lctrPerScanCtx_t *pPerScanCtx, lmgrPerAdvSyncEstdInd_t *pRpt)
{
  memset(pRpt, 0, sizeof(lmgrPerAdvSyncEstdInd_t));

  pRpt->advSID = pPerScanCtx->advSID;
  Bda64ToBstream(pRpt->addr, pPerScanCtx->advAddr);
  pRpt->addrType = pPerScanCtx->advAddrType;
  pRpt->advPhy = pPerScanCtx->rxPhys;
  pRpt->advInterval = LCTR_PER_INTER_TO_MS(BB_TICKS_TO_US(pPerScanCtx->perInter));
  pRpt->advClkAccuracy = pPerScanCtx->sca;

  return TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Common periodic scan resource cleanup.
 *
 *  \param      pPerScanCtx     Periodic scan context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrMstPerScanCleanupOp(lctrPerScanCtx_t *pPerScanCtx)
{
  BbStop(BB_PROT_BLE);

  if (pPerScanCtx->filtParam.filterPolicy)
  {
    LmgrDecPeriodiclistRefCount();
  }

  LmgrDecResetRefCount();

  pPerScanCtx->enabled = FALSE;
}

/*************************************************************************************************/
/*!
 *  \brief      Common scan resource cleanup.
 *
 *  \param      pExtScanCtx     Extended scan context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
static void lctrMstExtScanCleanupOp(lctrExtScanCtx_t *pExtScanCtx)
{
  BbStop(BB_PROT_BLE);

  switch (pExtScanCtx->scanParam.scanFiltPolicy)
  {
    case LL_SCAN_FILTER_WL_BIT:
    case LL_SCAN_FILTER_WL_OR_RES_INIT:
      LmgrDecWhitelistRefCount();
      break;
    default:
      break;
  }

  BbBleMstAdvEvent_t * const pScan = &pExtScanCtx->scanBleData.op.mstAdv;
  if (pScan->pRxAdvBuf)
  {
    WsfMsgFree(pScan->pRxAdvBuf);
    pScan->pRxAdvBuf = NULL;
  }
  if (pScan->pRxRspBuf)
  {
    WsfMsgFree(pScan->pRxRspBuf);
    pScan->pRxRspBuf = NULL;
  }

  WSF_ASSERT(lmgrCb.numScanEnabled > 0);
  lmgrCb.numScanEnabled--;
  LmgrDecResetRefCount();
}

/*************************************************************************************************/
/*!
 *  \brief      Start scan discovery.
 *
 *  \param      pExtScanCtx     Extended scan context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtScanActDiscover(lctrExtScanCtx_t *pExtScanCtx)
{
  BbStart(BB_PROT_BLE);

  switch (pExtScanCtx->scanParam.scanFiltPolicy)
  {
    case LL_SCAN_FILTER_WL_BIT:
    case LL_SCAN_FILTER_WL_OR_RES_INIT:
      LmgrIncWhitelistRefCount();
      break;
    default:
      break;
  }

  /* Reset state. */
  pExtScanCtx->selfTerm = FALSE;
  pExtScanCtx->shutdown = FALSE;
  pExtScanCtx->auxOpPending = FALSE;
  pExtScanCtx->bodTermCnt = 0;
  pExtScanCtx->data.scan.advRptState = LCTR_RPT_STATE_IDLE;

  uint8_t status;

  if (((status = lctrMstExtDiscoverBuildOp(pExtScanCtx)) != LL_SUCCESS) ||
      ((status = lctrMstAuxDiscoverBuildOp(pExtScanCtx)) != LL_SUCCESS))
  {
    LmgrSendExtScanEnableCnf(status);
    lctrSendExtScanMsg(pExtScanCtx, LCTR_EXT_SCAN_MSG_TERMINATE);
    return;
  }

  lmgrCb.numScanEnabled++;
  LmgrIncResetRefCount();

  pExtScanCtx->data.scan.backoffCount = 1;
  pExtScanCtx->data.scan.upperLimit = 1;

  LmgrSendExtScanEnableCnf(LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      Update scan discovery.
 *
 *  \param      pExtScanCtx     Extended scan context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtScanActUpdateDiscover(lctrExtScanCtx_t *pExtScanCtx)
{
  LmgrSendExtScanEnableCnf(LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      Shutdown active scan operation.
 *
 *  \param      pExtScanCtx     Extended scan context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtScanActShutdown(lctrExtScanCtx_t *pExtScanCtx)
{
  pExtScanCtx->shutdown = TRUE;

  if (!pExtScanCtx->auxOpPending)
  {
    SchRemove(&pExtScanCtx->scanBod);
  }
  else
  {
    SchRemove(&pExtScanCtx->auxScanBod);
  }

  /* Shutdown completes with events generated in BOD end callback. */
}

/*************************************************************************************************/
/*!
 *  \brief      Send scan operation confirm.
 *
 *  \param      pExtScanCtx     Extended scan context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtScanActScanCnf(lctrExtScanCtx_t *pExtScanCtx)
{
  LmgrSendExtScanEnableCnf(LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      Send disallow scan host notification.
 *
 *  \param      pExtScanCtx     Extended scan context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtScanActDisallowScan(lctrExtScanCtx_t *pExtScanCtx)
{
  if (lctrMstExtScan.scanTermByHost)
  {
    LmgrSendExtScanEnableCnf(LL_ERROR_CODE_CMD_DISALLOWED);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Terminated scan after host scan disable.
 *
 *  \param      pExtScanCtx     Extended scan context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtScanActScanTerm(lctrExtScanCtx_t *pExtScanCtx)
{
  lctrMstExtScanCleanupOp(pExtScanCtx);

  if (lctrMstExtScan.scanTermByHost)
  {
    LmgrSendExtScanEnableCnf(LL_SUCCESS);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Terminated scan after internal scan disable.
 *
 *  \param      pExtScanCtx     Extended scan context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrExtScanActSelfTerm(lctrExtScanCtx_t *pExtScanCtx)
{
  lctrMstExtScanCleanupOp(pExtScanCtx);
}

/*************************************************************************************************/
/*!
 *  \brief      Create sync action function.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrCreateSyncActCreate(void)
{
  lctrPerCreateSyncMsg_t *pMsg  = (lctrPerCreateSyncMsg_t *)pLctrMstPerScanMsg;

  BbStart(BB_PROT_BLE);

  if (pMsg->filterPolicy)
  {
    LmgrIncPeriodiclistRefCount();
  }

  lctrPerCreateSync.filtParam.filterPolicy = pMsg->filterPolicy;
  lctrPerCreateSync.filtParam.advAddr = pMsg->advAddr;
  lctrPerCreateSync.filtParam.advAddrType = pMsg->advAddrType;
  lctrPerCreateSync.filtParam.advSID = pMsg->advSID;

  lctrPerScanSetup(&lctrPerCreateSync, pMsg);
  lctrMstPerScanBuildOp(lctrPerCreateSync.pPerScanCtx, pMsg);

  LmgrIncResetRefCount();
}

/*************************************************************************************************/
/*!
 *  \brief      Create sync done action function.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrCreateSyncActDone(void)
{
  if (lctrPerCreateSync.createSyncPending == TRUE)
  {
    lctrPerCreateSync.createSyncPending = FALSE;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Create sync cancel action function.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrCreateSyncActCancel(void)
{
  if (lctrPerCreateSync.createSyncPending == TRUE)
  {
    lctrPerCreateSync.pPerScanCtx->cancelCreateSync = TRUE;
    SchRemove(&lctrPerCreateSync.pPerScanCtx->bod);
  }

  lctrSendCreateSyncMsg(LCTR_CREATE_SYNC_MSG_TERMINATE);
}

/*************************************************************************************************/
/*!
 *  \brief      Create sync cancel action function.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrCreateSyncActTerminate(void)
{
  if (lctrPerCreateSync.pPerScanCtx->cancelCreateSync == TRUE)
  {
    lctrPerCreateSync.pPerScanCtx->cancelCreateSync = FALSE;
  }

  lctrMstPerScanCleanupOp(lctrPerCreateSync.pPerScanCtx);

  lmgrPerAdvSyncEstdInd_t rpt;
  memset(&rpt, 0, sizeof(lmgrPerAdvSyncEstdInd_t));
  LmgrSendSyncEstInd(LL_ERROR_CODE_OP_CANCELLED_BY_HOST, LCTR_GET_PER_SCAN_HANDLE(lctrPerCreateSync.pPerScanCtx), &rpt);
}

/*************************************************************************************************/
/*!
 *  \brief      Periodic scanning sync established action function.
 *
 *  \param      pPerScanCtx     Periodic scan context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrPerScanActSyncEstd(lctrPerScanCtx_t *pPerScanCtx)
{
  /* Pack and notify the host about sync established event. */
  lmgrPerAdvSyncEstdInd_t rpt;

  lctrPerAdvSyncEstRptPack(pPerScanCtx, &rpt);
  LmgrSendSyncEstInd(LL_SUCCESS, LCTR_GET_PER_SCAN_HANDLE(pPerScanCtx), &rpt);
}

/*************************************************************************************************/
/*!
 *  \brief      Periodic scanning sync terminate action function.
 *
 *  \param      pPerScanCtx     Periodic scan context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrPerScanActSyncTerminate(lctrPerScanCtx_t *pPerScanCtx)
{
  pPerScanCtx->shutdown = TRUE;

  SchRemove(&pPerScanCtx->bod);

  /* Shutdown completes with events generated in BOD end callback. */
}

/*************************************************************************************************/
/*!
 *  \brief      Periodic scanning sync terminate done action function.
 *
 *  \param      pPerScanCtx     Periodic scan context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrPerScanActSyncTerminateDone(lctrPerScanCtx_t *pPerScanCtx)
{
  lctrMstPerScanCleanupOp(pPerScanCtx);
}

/*************************************************************************************************/
/*!
 *  \brief      Periodic scanning sync terminate action function.
 *
 *  \param      pPerScanCtx     Periodic scan context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrPerScanActSyncTimeout(lctrPerScanCtx_t *pPerScanCtx)
{
  pPerScanCtx->shutdown = TRUE;

  SchRemove(&pPerScanCtx->bod);

  WsfTimerStop(&pPerScanCtx->tmrSupTimeout);

  LmgrSendSyncLostInd(LCTR_GET_PER_SCAN_HANDLE(pPerScanCtx));

  /* Shutdown completes with events generated in BOD end callback. */
}
