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
 * \brief Link layer controller master scan action routines.
 */
/*************************************************************************************************/

#include "lctr_int_adv_master.h"
#include "sch_api.h"
#include "util/bstream.h"
#include "wsf_assert.h"
#include "wsf_msg.h"
#include "wsf_trace.h"
#include <string.h>

/*! \brief      Assert table entries do not exceed addressable 32-bits in advFiltValidMask. */
WSF_CT_ASSERT((LL_NUM_ADV_FILT <= 32));

/*************************************************************************************************/
/*!
 *  \brief      Common scan resource cleanup.
 *
 *  \param      pCtx    Scan context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrScanCleanup(lctrMstScanCtx_t *pCtx)
{
  LmgrDecResetRefCount();
  if (pCtx == &lctrMstScan)
  {
    lmgrCb.numScanEnabled = 0;
  }
  else
  {
    lmgrCb.numInitEnabled = 0;
  }
  switch (pCtx->scanParam.scanFiltPolicy)
  {
    case LL_SCAN_FILTER_WL_BIT:
    case LL_SCAN_FILTER_WL_OR_RES_INIT:
      LmgrDecWhitelistRefCount();
      break;
    default:
      break;
  }
  lctrMstScanCleanupOp(pCtx);

  uint8_t *pBuf;
  wsfHandlerId_t handlerId;

  while ((pBuf = WsfMsgDeq(&pCtx->rxAdvbQ, &handlerId)) != NULL)
  {
    /* Drop pending AdvB PDUs. */
    WsfMsgFree(pBuf);
  }
  while ((pBuf = WsfMsgDeq(&pCtx->rxDirectAdvbQ, &handlerId)) != NULL)
  {
    /* Drop pending direct AdvB PDUs. */
    WsfMsgFree(pBuf);
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Start scan discovery.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrScanActDiscover(void)
{
  lctrScanEnableMsg_t *pMsg = (lctrScanEnableMsg_t *)pLctrMsg;

  lctrMstScan.scanParam = lmgrMstScanCb.scanParam;

  lctrAdvRptEnable(&lctrMstScan.data.disc.advFilt, pMsg->filtDup);

  BbStart(BB_PROT_BLE);

  LmgrIncResetRefCount();
  lmgrCb.numScanEnabled = 1;
  switch (lctrMstScan.scanParam.scanFiltPolicy)
  {
    case LL_SCAN_FILTER_WL_BIT:
    case LL_SCAN_FILTER_WL_OR_RES_INIT:
      LmgrIncWhitelistRefCount();
      break;
    default:
      break;
  }
  lctrMstDiscoverBuildOp();
  lmgrCb.scanMode = LMGR_SCAN_MODE_DISCOVER;
  lctrMstScan.backoffCount = 1;
  lctrMstScan.upperLimit = 1;

  LmgrSendScanEnableCnf(LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      Shutdown active scan operation.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrScanActShutdown(void)
{
  if (lmgrCb.numScanEnabled)
  {
    lctrMstScan.shutdown = TRUE;
    SchRemove(&lctrMstScan.scanBod);

    /* Shutdown completes with events generated in BOD end callback. */
  }
  else
  {
    /* TODO Can this occur if a reset or cancel is received after a initiate start fails? */
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Send scan operation confirm.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrScanActScanCnf(void)
{
  LmgrSendScanEnableCnf(LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      Send disallow scan host notification.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrScanActDisallowScan(void)
{
  LmgrSendScanEnableCnf(LL_ERROR_CODE_CMD_DISALLOWED);
}

/*************************************************************************************************/
/*!
 *  \brief      Operation self terminated (e.g. on connection indication).
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrScanActSelfTerm(void)
{
  BbStop(BB_PROT_BLE);
  lctrScanCleanup(&lctrMstScan);
}

/*************************************************************************************************/
/*!
 *  \brief      Terminated scan after host scan disable.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrScanActScanTerm(void)
{
  BbStop(BB_PROT_BLE);

  lctrScanCleanup(&lctrMstScan);
  LmgrSendScanEnableCnf(LL_SUCCESS);
}

/*************************************************************************************************/
/*!
 *  \brief      Terminated scan after host reset.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrScanActResetTerm(void)
{
  BbStop(BB_PROT_BLE);
  lctrScanCleanup(&lctrMstScan);
}

/*************************************************************************************************/
/*!
 *  \brief      Update scan parameters.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrScanActUpdateScanParam(void)
{
  lctrScanParamMsg_t *pMsg = (lctrScanParamMsg_t *)pLctrMsg;

  lmgrMstScanCb.scanParam = pMsg->param;
}

/*************************************************************************************************/
/*!
 *  \brief      Update scan filter.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrScanActUpdateScanFilt(void)
{
  lctrScanEnableMsg_t *pMsg = (lctrScanEnableMsg_t *)pLctrMsg;
  if (lctrMstScan.data.disc.advFilt.enable != pMsg->filtDup)
  {
    lctrAdvRptEnable(&lctrMstScan.data.disc.advFilt, pMsg->filtDup);
  }

  LmgrSendScanEnableCnf(LL_SUCCESS);
}

