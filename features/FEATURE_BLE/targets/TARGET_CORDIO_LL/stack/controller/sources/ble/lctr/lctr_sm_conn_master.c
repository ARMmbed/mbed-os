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
 * \brief Link layer controller master connection state machine implementation file.
 */
/*************************************************************************************************/

#include "lctr_int_conn.h"
#include "lctr_int_conn_master.h"
#include "lctr_int_conn_slave.h"
#include "lctr_int_adv_master.h"
#include "lmgr_api_conn.h"
#include "sch_api_ble.h"
#include "wsf_assert.h"
#include "wsf_trace.h"

/*************************************************************************************************/
/*!
 *  \brief      Execute master connection state machine.
 *
 *  \param      pCtx    Connection context.
 *  \param      event   State machine event.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void lctrMstConnExecuteSm(lctrConnCtx_t *pCtx, uint8_t event)
{
  switch (pCtx->state)
  {
    case LCTR_CONN_STATE_ESTABLISHED_READY:
      switch (event)
      {
        case LCTR_CONN_DATA_PENDING:
          lctrMstReloadDataPdu(pCtx);
          break;
        case LCTR_CONN_MST_ESTABLISH:
        case LCTR_CONN_TERM_SUP_TIMEOUT:
        case LCTR_CONN_TERM_MIC_FAILED:
          /* Data path related events. */
          break;
        default:
          /* Invoke LLCP child state machine */
          lctrMstLlcpExecuteSm(pCtx, event);
          break;
      }
      break;

    case LCTR_CONN_STATE_INITIALIZED:
      switch (event)
      {
        case LCTR_CONN_MST_ESTABLISH:
          LL_TRACE_INFO2("lctrMstConnExecuteSm: handle=%u, state=%u, event=MST_ESTABLISH", LCTR_GET_CONN_HANDLE(pCtx), pCtx->state);

          lctrMstSetEstablishConn(pCtx);
          lctrStoreConnFailEstablishTerminateReason(pCtx);        /* default termination reason */

          if (lctrGetConnOpFlag(pCtx, LL_OP_MODE_FLAG_ENA_VER_LLCP_STARTUP))
          {
            /* Queue version exchange. */
            lctrLlcpExecuteCommonSm(pCtx, LCTR_CONN_LLCP_VERSION_EXCH);
          }

          if (lctrGetConnOpFlag(pCtx, LL_OP_MODE_FLAG_ENA_FEAT_LLCP_STARTUP))
          {
            /* Queue feature exchange. */
            lctrLlcpExecuteCommonSm(pCtx, LCTR_CONN_LLCP_FEATURE_EXCH);
          }

          if (lctrGetConnOpFlag(pCtx, LL_OP_MODE_FLAG_ENA_LEN_LLCP_STARTUP))
          {
            /* Only send initial length feature if the default is an extended length. */
            if ((pCtx->localDataPdu.maxTxLen  > LL_MAX_DATA_LEN_MIN)  ||
                (pCtx->localDataPdu.maxTxTime > LL_MAX_DATA_TIME_MIN))
            {
              lctrLlcpExecuteCommonSm(pCtx, LCTR_CONN_LLCP_LENGTH_EXCH);
            }
          }

          lctrNotifyHostConnectInd(LCTR_GET_CONN_HANDLE(pCtx), LL_ROLE_MASTER,
              &pLctrConnMsg->connEstablish.connInd, pLctrConnMsg->connEstablish.peerIdAddrType,
              pLctrConnMsg->connEstablish.peerIdAddr, pLctrConnMsg->connEstablish.peerRpa,
              pLctrConnMsg->connEstablish.localRpa, LL_SUCCESS, pCtx->usedChSel);

          pCtx->state = LCTR_CONN_STATE_ESTABLISHED_READY;
          break;

        default:
          break;
      }
      break;

    case LCTR_CONN_STATE_TERMINATING:
      switch (event)
      {
        case LCTR_CONN_TMR_LLCP_RSP_EXP:
          pCtx->llcpNotifyMask &= ~(1 << LCTR_PROC_CMN_TERM);
          lctrStoreLlcpTimeoutTerminateReason(pCtx);
          pCtx->llcpState = LCTR_LLCP_STATE_IDLE;  /* Stop waiting to send TERMINATE_IND. */
          break;
        default:
          break;
      }
      break;

    default:
      break;
  }

  /* State-less events. */
  switch (event)
  {
    case LCTR_CONN_TERMINATED:
      LL_TRACE_INFO2("lctrMstConnExecuteSm: handle=%u, state=%u, event=TERMINATED", LCTR_GET_CONN_HANDLE(pCtx), pCtx->state);
      SchRmRemove(LCTR_GET_CONN_HANDLE(pCtx));
      lctrNotifyHostDisconnectInd(pCtx);
      lctrFreeConnCtx(pCtx);
      break;
    default:
      lctrConnStatelessEventHandler(pCtx, event);
      break;
  }
}
