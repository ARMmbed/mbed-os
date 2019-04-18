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

  /* Accommodate peer PreferredPeriodicity. */
  uint32_t commonPrefPerUsec = SchRmCalcCommonPeriodicityUsec(LCTR_CONN_IND_US(pCtx->connParam.prefPeriod));

  if (!SchRmStartUpdate(LCTR_GET_CONN_HANDLE(pCtx), interMinUsec, interMaxUsec, commonPrefPerUsec, durUsec, &connIntervalUsec))
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
