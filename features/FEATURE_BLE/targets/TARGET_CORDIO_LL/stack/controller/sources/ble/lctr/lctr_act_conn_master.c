/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief  Link layer controller master connection state machine action routines.
 *
 *  Copyright (c) 2013-2017 ARM Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#include "lctr_int_conn_master.h"
#include "lctr_pdu_adv.h"
#include "lctr_int_adv_master.h"
#include "sch_api.h"
#include "sch_api_ble.h"
#include "wsf_trace.h"
#include "util/bstream.h"
#include <string.h>

/*************************************************************************************************/
/*!
 *  \brief      Send connection update request PDU to peer.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrSendConnUpdateInd(lctrConnCtx_t *pCtx)
{
  pCtx->connUpd.txWinSize = LL_MIN_TX_WIN_SIZE;
  pCtx->connUpd.latency = pCtx->connUpdSpec.connLatency;
  pCtx->connUpd.timeout = pCtx->connUpdSpec.supTimeout;

#if (LL_ENABLE_TESTER)
  if (llTesterCb.connUpdIndEnabled)
  {
    pCtx->connUpd = llTesterCb.connUpdInd;

    llTesterCb.connUpdIndEnabled = FALSE;
    pCtx->data.mst.sendConnUpdInd = TRUE;

    return;
  }
#endif

  uint32_t interMinUsec = LCTR_CONN_IND_US(pCtx->connUpdSpec.connIntervalMin);
  uint32_t interMaxUsec = LCTR_CONN_IND_US(pCtx->connUpdSpec.connIntervalMax);
  uint32_t durUsec = pCtx->localConnDurUsec;
  uint32_t connIntervalUsec;

  /* TODO: accommodate pCtx->connParam.prefPeriod. */
  if (!SchRmStartUpdate(LCTR_GET_CONN_HANDLE(pCtx), interMinUsec, interMaxUsec, durUsec, &connIntervalUsec))
  {
    LL_TRACE_WARN1("Could not update connection, handle=%u", LCTR_GET_CONN_HANDLE(pCtx));
    lctrSendConnMsg(pCtx, LCTR_CONN_LLCP_REJECT_CONN_UPD);
    return;
  }

  pCtx->connUpd.interval = LCTR_US_TO_CONN_IND(connIntervalUsec);

  /* Delay delivery until txWindowOffset can be computed. */
  pCtx->data.mst.sendConnUpdInd = TRUE;
}

/*************************************************************************************************/
/*!
 *  \brief      Reload an empty BOD with a data PDU.
 *
 *  \param      pCtx    Connection context.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrMstReloadDataPdu(lctrConnCtx_t *pCtx)
{
  /* Modify the next CE. Possible changes include empty CE or modification of MD. */
  SchReload(&pCtx->connBod);
}
